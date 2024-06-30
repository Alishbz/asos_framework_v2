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
 
#include "asos_stade_machine.h"


#define ASOS_SM_CHECK(obj)                 if(obj == NULL){ return ASOS_SM_FAIL; }
#define ASOS_SM_IS_RUNING_CHECK(obj)       if(obj->is_runing == ASOS_FALSE){ return ASOS_SM_FAIL; }

#define ASOS_SM_LOG_PRINT(obj , str)       \
do{\
  if(obj->os){ \
     asos_debug_print(obj->os ,(const char * )str );\
  }\
}while(0);


static void _1_ms_timer( void*me , asos_timer_t * os );


asos_sm_fout_e
asos_stade_machine_run( asos_stade_machine_t *     const me)
{
  ASOS_SM_CHECK(me) ;
  ASOS_SM_IS_RUNING_CHECK(me);

  if(me->timeout_value){
  
    if(me->timer_ms > me->timeout_value){
    
      me->timeout_value = 0;
      me->timer_ms = 0;
    }
    else{
       return ASOS_SM_WAITING;
    }
  }
  
  asos_sm_stade_id_type stade_id = me->next_stade->task(me);
   
  asos_list_node_t *node = asos_xlist_get_node_with_data(&me->stades_list , (list_data_type)stade_id);

  if(node){
     
    if(stade_id == ASOS_SM_IDLE_STADE_CASE_ID){
      
       me->next_stade = &me->idle_stade;
    }
    else{
      asos_stade_t *stade = ASOS_CONTAINER_OF ( node , asos_stade_t , node );     
       
      me->next_stade  = stade;   
    }
  }
  else{
    /** error goto idle stade **/ 
  
    ASOS_SM_LOG_PRINT(me , "SM ; NODE ERROR");
    
    me->next_stade = &me->idle_stade;
  }
   
  return ASOS_SM_SUCCESS;
}
 



 

asos_sm_fout_e
asos_stade_machine_create( asos_stade_machine_t *     const me , 
                           asos_object_t        *     os,
                           asos_sm_task_fp_t          idle_task)
{
  ASOS_SM_CHECK(me) ;
  ASOS_SM_CHECK(os) ;
  ASOS_SM_CHECK(idle_task) ;
   
  me->os = os; 
   
  asos_xlist_create(&me->stades_list , 
                    ASOS_LIST_MODE_REV_SORTED); 
  
      
  me->idle_stade.stade_id = ASOS_SM_IDLE_STADE_CASE_ID;
  me->idle_stade.task = idle_task;

  asos_xlist_node_insert(&me->stades_list, 
                         &me->idle_node, 
                         (void*)&me->idle_stade,
                         (list_data_type)ASOS_SM_IDLE_STADE_CASE_ID);
  
  me->next_stade = &me->idle_stade;

  me->timer_ms = 0; 
  
  me->timeout_value = 0;
  
  me->is_runing = ASOS_TRUE;  
  
  me->is_log_active = ASOS_TRUE;

  ASOS_SM_LOG_PRINT(me , "SM CREATED");
    
  asos_timer_create( os ,  me,  &_1_ms_timer, 1);
  
  return ASOS_SM_SUCCESS;
}
 


asos_sm_fout_e
asos_stade_machine_delete( asos_stade_machine_t *     const me)
{
  ASOS_SM_CHECK(me) ;
  
  asos_sm_fout_e out =
  asos_stade_machine_create( me, 
                             me->os,
                             me->idle_stade.task);
  asos_stade_machine_stop(  me);
  asos_stade_machine_loging_stop( me);
  return out;
}


 
asos_sm_fout_e
asos_stade_machine_add_stade( asos_stade_machine_t *     const me ,
                              asos_stade_t  *            const stade ,
                              asos_sm_stade_id_type      stade_id)
{
  ASOS_SM_CHECK(me) ;
  ASOS_SM_CHECK(stade) ;
  if(stade_id == ASOS_SM_IDLE_STADE_CASE_ID){ return  ASOS_SM_FAIL ; } 
   
  asos_xlist_node_insert(&me->stades_list, 
                         &stade->node, 
                         (void*)stade,
                         (list_data_type)stade->stade_id );
  
 
  return ASOS_SM_SUCCESS;
}


asos_sm_fout_e
asos_stade_machine_delete_stade( asos_stade_machine_t *     const me ,
                                 asos_sm_stade_id_type      stade_id)
{
  ASOS_SM_CHECK(me) ; 
  if(stade_id == ASOS_SM_IDLE_STADE_CASE_ID){ return  ASOS_SM_FAIL ; } 
  
  asos_list_node_t *node = asos_xlist_get_node_with_data(&me->stades_list , (list_data_type)stade_id);

  if(node){
    asos_xlist_node_delete(&me->stades_list , 
                           node);
  }
  
  return ASOS_SM_SUCCESS;
}




 


asos_sm_fout_e
asos_stade_machine_loging_start( asos_stade_machine_t *     const me)
{
  ASOS_SM_CHECK(me);  
  
  me->is_log_active = ASOS_TRUE; 
  
  return ASOS_SM_SUCCESS;
}
 


asos_sm_fout_e
asos_stade_machine_loging_stop( asos_stade_machine_t *     const me)
{
  ASOS_SM_CHECK(me);  
  
  me->is_log_active = ASOS_FALSE; 
  
  return ASOS_SM_SUCCESS;
}
 


asos_sm_fout_e
asos_stade_machine_stop( asos_stade_machine_t *     const me)
{
  ASOS_SM_CHECK(me);  
  ASOS_SM_IS_RUNING_CHECK(me); 
  
  me->is_runing = ASOS_FALSE; 
  
  return ASOS_SM_SUCCESS;
}
 
/**  @REV -> created in OS 
asos_sm_fout_e
asos_stade_machine_timer_ms( asos_stade_machine_t *     const me)
{
  ASOS_SM_CHECK(me);  
  ASOS_SM_IS_RUNING_CHECK(me); 
 
  me->timer_ms++;
  
  return ASOS_SM_SUCCESS;
}*/

/** timer interrupt  ***/
static void _1_ms_timer( void*me , asos_timer_t * os )
{
  asos_stade_machine_t *_me = (asos_stade_machine_t*)(me); 
  
  if(_me && _me->is_runing == ASOS_TRUE){
     _me->timer_ms++;
  } 
}




asos_sm_fout_e
asos_stade_machine_set_delay( asos_stade_machine_t *     const me,
                              unsigned int delay_ms )
{
  ASOS_SM_CHECK(me);  
  ASOS_SM_IS_RUNING_CHECK(me); 
  
  me->timeout_value = delay_ms;
  me->timer_ms = 0;
  
  return ASOS_SM_SUCCESS;
}


 
