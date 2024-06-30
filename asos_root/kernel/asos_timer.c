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
 
#include "asos_timer.h"

#define ASOS_TIMER_OBJ_CHECK(obj)        if(obj == NULL){ return ASOS_TIMER_FAIL; }
#define ASOS_TIMER_OBJ_BOOL_CHECK(obj)   if(obj == NULL){ return ASOS_FALSE; }
#define ASOS_TIMER_US_TO_MS_FLOAT(val)   (float)(val/1000)


asos_timer_fout_t
asos_xtimer_create(asos_timer_t* me , unsigned int timer_us_val)
{
  ASOS_TIMER_OBJ_CHECK(me);
  
  if( !timer_us_val ){
    
     return ASOS_TIMER_FAIL;
  }
  
  unsigned short i = 0;
  me->timer_loaded_number = 0;
  me->timer_run_index = 0;
  for(i = 0 ; i < ASOS_TASK_SUPPORT_MAX_COUNT ; i++){ 
    me->timers[i].app_obj = NULL;
    me->timers[i].callback = NULL;
    me->timers[i].config_ms = 0;  
    me->timers[i].process_counter = (float)0; 
  }
  me->timer_process_us_val = timer_us_val;
  
  return ASOS_TIMER_SUCCESS;
}

asos_timer_fout_t
asos_xtimer_delete(asos_timer_t* me)
{
  ASOS_TIMER_OBJ_CHECK(me);
  asos_xtimer_create(me,0);
  return ASOS_TIMER_SUCCESS;
}



asos_timer_fout_t
asos_xtimer_unit_create(asos_timer_t* me , asos_timer_unit_t* timer_obj)
{
  ASOS_TIMER_OBJ_CHECK(me);
  ASOS_TIMER_OBJ_CHECK(timer_obj);
  ASOS_TIMER_OBJ_CHECK(timer_obj->callback);
  
  if(me->timer_loaded_number + 1 > ASOS_TIMER_SUPPORT_MAX_COUNT - 1){
    assert( "ASOS_TIMER_SUPPORT_MAX_COUNT is not enough" );
    return ASOS_TIMER_FAIL; 
  }
   
  me->timers[ me->timer_loaded_number].app_obj = timer_obj->app_obj;
  me->timers[ me->timer_loaded_number].callback = timer_obj->callback;
  me->timers[ me->timer_loaded_number].config_ms = timer_obj->config_ms; 
   
  me->timer_loaded_number++;
   
  return ASOS_TIMER_SUCCESS;
}

asos_timer_fout_t
asos_timer_unit_delete(asos_timer_t* me , asos_timer_unit_t* timer_obj)
{
  // TODO
  return ASOS_TIMER_SUCCESS;
}



static asos_mbool
_timer_process(asos_timer_unit_t* me , float p_time) 
{
  ASOS_TIMER_OBJ_BOOL_CHECK(me);
  
  me->process_counter += p_time;
  
  if(me->process_counter >= me->config_ms){
    me->process_counter = 0;
    return ASOS_TRUE;
  }
  
  return ASOS_FALSE;
}



asos_timer_fout_t
asos_xtimer_process_run(asos_timer_t* me)
{
  ASOS_TIMER_OBJ_CHECK(me);
  
  if(!me->timer_loaded_number){ 
    /** there is no timer **/
    return ASOS_TIMER_FAIL; 
  }
  
  unsigned short i = 0;
   
  do{ 
    if( ASOS_TRUE == _timer_process(&me->timers[i] , 
                                    ASOS_TIMER_US_TO_MS_FLOAT(me->timer_process_us_val)) ){
      me->timers[i].callback( me->timers[i].app_obj , me );
    }
    i++;
  }while(i < me->timer_loaded_number);
  
  return ASOS_TIMER_SUCCESS;
}









