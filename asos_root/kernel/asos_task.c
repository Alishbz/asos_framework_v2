/****************************************************************************************
* ALI SAHBAZ
*
*
* ASOS - "Ali Sahbaz Operating System" Framework
*
* Date          : 06.07.2023
* By            : Ali Sahbaz
* e-mail        : ali_sahbaz@outlook.com
*/

#include "../asos.h"

#define    ASOS_DEBUG_SEND(os,str)   asos_debug_print(os,(const char*)str)

#ifdef IS_ROUND_ROBIN_ACTIVE

/*** @RoundRobin Config  ***/
  #define RR_TASK_QUANTUM_TIME    3

  typedef enum { 
    RR_EMPTY,
    RR_GOTO_NEXT_TASK,
    RR_TASK_RUN,
    RR_GOTO_NEXT_TASK_AND_RUN_TASK 
  }_rr_f_out_t; 

  static _rr_f_out_t _round_robin_algorithm(asos_task_load_t* me, unsigned int current_time); 

#endif



#define ASOS_TASK_OBJ_CHECK(obj)  if(obj == NULL){ return ASOS_TASK_FAIL; }

static inline asos_task_t* _task_obj_transform(void* me){ return (asos_task_t*)me;}

static void
_task_config_clear(asos_task_config_t* me)
{
  /** add other configs **/
  me->prioty = ASOS_TASK_PRIO_EMPTY;
}

static void
_task_config_load(asos_task_config_t* me,asos_task_config_t* loader)
{
  /** add other configs **/
  me->prioty = loader->prioty;;
}



static void _all_tasks_wait_time_counter( asos_task_t * me )
{  
  if( me == NULL){return;}
  
  unsigned short i = 0;
  
  do{ 
    if( me->tasks[i].app_wait_ms_set_value ){
      me->tasks[i].app_wait_ms_counter++;
    } 
    i++;
    
  }while(i < me->task_loaded_number);
}

static void _task_1_ms_callback( void*me , asos_timer_t * os )
{
  asos_task_t *_me = _task_obj_transform(me);  
  
  _me->context_switch_ms_counter++;
  
  _all_tasks_wait_time_counter(_me);
}

asos_task_fout_t
asos_xtask_create(asos_task_t* me,  unsigned int tasks_context_switch_ms_value)
{
  ASOS_TASK_OBJ_CHECK(me);
   
  unsigned short i = 0;
  me->task_loaded_number = 0;
  me->task_run_index = 0;
  for(i = 0 ; i < ASOS_TASK_SUPPORT_MAX_COUNT ; i++){
    _task_config_clear(&me->tasks[i].config);
    me->tasks[i].app_obj = NULL;
    me->tasks[i].app_init_fp = NULL;
    me->tasks[i].app_delete_fp = NULL;
    me->tasks[i].app_process_fp = NULL; 
    me->tasks[i].app_wait_ms_counter = 0 ;
    me->tasks[i].app_wait_ms_set_value = 0; 
#ifdef IS_ROUND_ROBIN_ACTIVE
    me->tasks[i].quantum_time = 0;
#endif
  }  
  me->context_switch_ms_counter = 0;
#ifdef IS_ROUND_ROBIN_ACTIVE
   me->current_quantum_time = 0;
#endif
  me->context_switch_ms_set_value = tasks_context_switch_ms_value;
  asos_object_t *osx =  ASOS_TASK_TO_OS(me);
  
  /** 1ms int create **/
  asos_timer_create( osx , 
                     me, 
                     &_task_1_ms_callback,
                     1);
 
   me->idle.task = NULL;
   me->idle.sleep_off = NULL;
   me->idle.sleep_on = NULL;
   me->idle.is_sleep_active = ASOS_FALSE; 
   me->is_sleep_on= ASOS_FALSE; 
   
   ASOS_DEBUG_SEND(osx,"xtask created");  // test
   
  return ASOS_TASK_SUCCESS;
}

asos_task_fout_t
asos_xtask_delete(asos_task_t* me)
{
  ASOS_TASK_OBJ_CHECK(me);
   
  asos_xtask_create(me,0);
  
  return ASOS_TASK_SUCCESS;
}

asos_task_fout_t
asos_xtask_app_create(asos_task_t* me , asos_task_load_t* app_obj)
{
  ASOS_TASK_OBJ_CHECK(me);
  
  if(me->task_loaded_number + 1 > ASOS_TASK_SUPPORT_MAX_COUNT - 1){
    assert( "ASOS_TASK_SUPPORT_MAX_COUNT is not enough" );
    return ASOS_TASK_FAIL; 
  }
  
  _task_config_load(&me->tasks[ me->task_loaded_number].config, &app_obj->config);
  me->tasks[ me->task_loaded_number].app_obj = app_obj->app_obj;
  me->tasks[ me->task_loaded_number].app_init_fp = app_obj->app_init_fp;
  me->tasks[ me->task_loaded_number].app_delete_fp = app_obj->app_delete_fp;
  me->tasks[ me->task_loaded_number].app_process_fp = app_obj->app_process_fp; 
   
  me->task_loaded_number++;
   
  return ASOS_TASK_SUCCESS;
}

asos_task_fout_t
asos_xtask_app_delete(asos_task_t* me , asos_task_load_t* app_obj)
{
  // todo
  return ASOS_TASK_SUCCESS;
}


static void _sleep_off_check(asos_task_t* me){
  if(me->idle.is_sleep_active == ASOS_TRUE &&
     me->is_sleep_on == ASOS_TRUE ){
       me->is_sleep_on = ASOS_FALSE;
       me->idle.sleep_off(me);
       
   }
}

static void _idle_task_call(asos_task_t* me){
  if(me->idle.task != NULL   ){
       me->idle.task(me);
   }
}

static void _sleep_on_check(asos_task_t* me){
  if(me->idle.is_sleep_active == ASOS_TRUE &&
     me->is_sleep_on == ASOS_FALSE ){
       me->is_sleep_on = ASOS_TRUE;
       me->idle.sleep_on(me);
   }
}

/** CHECK all task delay wait time, 
    if all tasks wanna wait and there is intersecting set then return -> ASOS_TRUE **/
static asos_mbool _all_task_wait_ms_check(asos_task_t* me){
   
  unsigned short task_index = 0;
  
  do{ 
    
    if( !me->tasks[ task_index ].app_wait_ms_set_value || 
        (me->tasks[ me->task_run_index ].app_wait_ms_counter > ( me->tasks[ me->task_run_index ].app_wait_ms_set_value -1 ) ) ) 
    {
       return ASOS_FALSE;
    }
  
    task_index++;
    
  }while(task_index < me->task_loaded_number);
  
  return ASOS_TRUE;
}

#ifdef IS_ROUND_ROBIN_ACTIVE 
static void
_load_raund_robin_quantum_times_for_all_tasks(asos_task_t* me)
{ 
  unsigned short i = 0;
  
  do{
     

  /** super quatum time for round robin **/
   me->tasks[i].quantum_time = me->tasks[i].config.prioty; 

   
    i++;
    
  }while(i < me->task_loaded_number);
   
}

static void
_raund_robin_index_finder(asos_task_t* me)
{ 
  me->task_run_index++;
  me->current_quantum_time = 0;
  if(me->task_run_index > me->task_loaded_number -1 ){ 
     me->task_run_index = 0;
     /** round robin start critical assignment "prio to quantum_time"  **/
     _load_raund_robin_quantum_times_for_all_tasks( me );
  } 
}
  
#endif


asos_task_fout_t
asos_xtask_run_next_task(asos_task_t* me)
{
  ASOS_TASK_OBJ_CHECK(me);

  if(!me->task_loaded_number){  
    
    _sleep_on_check(me); 
    
    _idle_task_call(me);
    
    /** there is no task **/
    return ASOS_TASK_FAIL; 
  }


  if( ((me->context_switch_ms_set_value) && 
       (me->context_switch_ms_counter < me->context_switch_ms_set_value) ) ||
       (ASOS_TRUE == _all_task_wait_ms_check(me) )  ){  
        
        _sleep_on_check(me); 
        
        _idle_task_call(me);
         
        return ASOS_TASK_WAIT;
  }
   
  _sleep_off_check(me);  

  me->context_switch_ms_counter=0;
 
  if(!me->tasks[ me->task_run_index ].app_wait_ms_set_value || 
      me->tasks[ me->task_run_index ].app_wait_ms_counter > (me->tasks[ me->task_run_index ].app_wait_ms_set_value -1 ) ) 
  {
      me->tasks[ me->task_run_index ].app_wait_ms_counter = 0;
      
      me->tasks[ me->task_run_index ].app_wait_ms_set_value = 0; 
      
#ifdef IS_ROUND_ROBIN_ACTIVE
    _rr_f_out_t f_out = RR_EMPTY;
    
    f_out =  _round_robin_algorithm(&me->tasks[ me->task_run_index ] , 
                                    me->current_quantum_time);
      
    if(f_out == RR_TASK_RUN){
       me->tasks[ me->task_run_index ].app_process_fp( me->tasks[me->task_run_index].app_obj ,
                                                          me) ;
    }
    else if(f_out == RR_GOTO_NEXT_TASK || f_out == RR_GOTO_NEXT_TASK_AND_RUN_TASK){
      
        if(f_out == RR_GOTO_NEXT_TASK_AND_RUN_TASK){
           me->tasks[ me->task_run_index ].app_process_fp( me->tasks[me->task_run_index].app_obj ,
                                                           me) ;
        }
        
       _raund_robin_index_finder(me); 
    }
#else
    /** @taskRunDirect ***/
    me->tasks[ me->task_run_index ].app_process_fp( me->tasks[me->task_run_index].app_obj ,
                                                        me) ;
#endif 
  }
  
#ifdef IS_ROUND_ROBIN_ACTIVE  
  else if(me->tasks[ me->task_run_index ].app_wait_ms_set_value ){
     _raund_robin_index_finder(me); 
  }
#endif 
  
#ifdef IS_ROUND_ROBIN_ACTIVE 
  /** super quatum time for round robin **/
  me->current_quantum_time++;
#else
  me->task_run_index++; 
  
  if(me->task_run_index > me->task_loaded_number -1 ){ 
   me->task_run_index = 0;
  }
#endif
   
  return ASOS_TASK_SUCCESS;
}

asos_task_fout_t
asos_xtask_init_all_tasks(asos_task_t* me)
{
  ASOS_TASK_OBJ_CHECK(me);
  
  if(!me->task_loaded_number){ 
    /** there is no task **/
    return ASOS_TASK_FAIL; 
  }
  
  unsigned short i = 0;
  
  do{
    ASOS_USER_FOUT out_holder = ASOS_USER_TASK_FAIL;
    
    out_holder = me->tasks[i].app_init_fp( me->tasks[i].app_obj ,
                                           me);
    
    
#ifdef IS_ROUND_ROBIN_ACTIVE 
  /** super quatum time for round robin **/
   me->tasks[i].quantum_time = me->tasks[i].config.prioty; 
#endif
  
    if( ASOS_USER_TASK_SUCCESS != out_holder )
    {
      // todo add -> ASOS_USER_TASK_KILL_ME
      // todo , add printf warning
    }
    
    i++;
    
  }while(i < me->task_loaded_number);
  
  return ASOS_TASK_SUCCESS;
}

asos_task_fout_t
asos_xtask_synchronous_wait_load(asos_task_t* me, void*app_obj , unsigned int wait_ms)
{
  ASOS_TASK_OBJ_CHECK(me);
  ASOS_TASK_OBJ_CHECK(app_obj);
  
  unsigned short i = 0 ;
  
  do{ 
    if( me->tasks[i].app_obj == app_obj ){
      
      me->tasks[i].app_wait_ms_set_value = wait_ms;
      
      return ASOS_TASK_SUCCESS;
    } 
    i++; 
  }while(i < me->task_loaded_number);
  
  return ASOS_TASK_FAIL;
}




 

asos_task_fout_t
asos_xtask_idle_assign(asos_task_t* me , 
                       asos_idle_sleep_fp_t  task_f,
                       asos_idle_sleep_fp_t  sleep_on_f,
                       asos_idle_sleep_fp_t  sleep_off_f)
{
  ASOS_TASK_OBJ_CHECK(me);
  ASOS_TASK_OBJ_CHECK(task_f);
  
  me->idle.task = task_f;
  
  if(sleep_on_f == NULL && sleep_off_f == NULL){
    me->idle.is_sleep_active = ASOS_FALSE;
    return ASOS_TASK_SUCCESS;
  }
  else if(sleep_on_f != NULL && sleep_off_f != NULL){
  
    me->idle.is_sleep_active = ASOS_TRUE;
    me->idle.sleep_off = sleep_off_f;
    me->idle.sleep_on = sleep_on_f;
  }
  return ASOS_TASK_SUCCESS;
}




asos_task_fout_t
asos_xtask_idle_sleep_active(asos_task_t* me )
{
  ASOS_TASK_OBJ_CHECK(me);
  
  if( me->idle.sleep_off != NULL && me->idle.sleep_on != NULL){
     me->idle.is_sleep_active = ASOS_TRUE;
  }
  
  return ASOS_TASK_SUCCESS;
}

asos_task_fout_t
asos_xtask_idle_sleep_pasive(asos_task_t* me )
{
  ASOS_TASK_OBJ_CHECK(me);
  
  me->idle.is_sleep_active = ASOS_FALSE;

  return ASOS_TASK_SUCCESS;
}


 
#ifdef IS_ROUND_ROBIN_ACTIVE

/** 
  @roundRobinProcess 
     out :
       0 -> do nothing
       1 -> goto next task
       2 -> TASK_RUN
**/  
static _rr_f_out_t _round_robin_algorithm(asos_task_load_t* me, unsigned int current_time   ) 
{
	char _q_process_step = 1 , task_run_stade = 0;
        _rr_f_out_t out = RR_EMPTY;

	if (me->config.prioty || me->quantum_time > 0 ) {
		 
		if (me->quantum_time+1 < RR_TASK_QUANTUM_TIME) {
			_q_process_step = 0;
		}
 
		me->quantum_time--;
                
                task_run_stade = 1; 

	}
	else { 
		return RR_GOTO_NEXT_TASK;
	}
	 
	
	if (current_time ) {
		if (_q_process_step == 0) {

			if (me->quantum_time < 1 || 
				(current_time % (me->quantum_time % RR_TASK_QUANTUM_TIME)) == 0)
			{  
                                if(task_run_stade == 1){
                                  return RR_GOTO_NEXT_TASK_AND_RUN_TASK;
                                }
				return RR_GOTO_NEXT_TASK;
			}
		}
		else if ((current_time % RR_TASK_QUANTUM_TIME) == 0)
		{  
                        if(task_run_stade == 1){
                          return RR_GOTO_NEXT_TASK_AND_RUN_TASK;
                        }

			return RR_GOTO_NEXT_TASK;
		}
	}
        
        if(task_run_stade == 1){
            return RR_TASK_RUN;
        }

	return out;
}

#endif





















