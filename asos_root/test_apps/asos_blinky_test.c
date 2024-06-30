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

#include "asos_blinky_test.h"

#define NULL_CHECK_RETURN(ptr , out)     if(ptr == NULL){ return out; }


static asos_device_tree_function_call_u * _f_led_on , * _f_led_off;



static void blinky_test_1000ms_timer( void*me , asos_timer_t * os )
{
  blinky_obj_t *_me = blinky_obj_transform(me); 
  
  _me->time1++;
  
  if(_me->time1 > 100){ _me->time1 = 0; }
}
   
static void blinky_test_500ms_timer( void*me , asos_timer_t * os )
{
  blinky_obj_t *_me = blinky_obj_transform(me); 
  
  _me->time2++;
  
  if(_me->time2 > 100){ _me->time2 = 0; }
}
  



ASOS_USER_FOUT 
blinky_task( blinky_obj_p  me , asos_task_t * os)
{
  NULL_CHECK_RETURN(me , ASOS_USER_TASK_FAIL);
  NULL_CHECK_RETURN(os , ASOS_USER_TASK_FAIL);

  blinky_obj_t *_me = blinky_obj_transform(me); 
  asos_object_t *_os = ASOS_TASK_TO_OS(os); 
   
  // APP
  
  asos_app_process_wait_ms( _os , me , 1000 );
 
  return ASOS_USER_TASK_SUCCESS; 
}


 
static void _uart1_interrupt(void*me ,
                             observer_event_types event,
                             OBS_DATA data) 
{ 
  blinky_obj_t *_me = blinky_obj_transform(me); 
  
  if(data == 'a'){ 
    if( _f_led_on != NULL ){
      /** CALL mcall **/
      _f_led_on->call_empty_return_and_input();
    } 
   
  }
  else  if(data == 'b'){ 
    if( _f_led_off != NULL ){
      /** CALL mcall **/
      _f_led_off->call_empty_return_and_input();
    } 
  
  }
}
 


ASOS_USER_FOUT 
blinky_create( blinky_obj_p  me , asos_task_t * os)
{ 
  NULL_CHECK_RETURN(me , ASOS_USER_TASK_FAIL);
  NULL_CHECK_RETURN(os , ASOS_USER_TASK_FAIL);
  
  blinky_obj_t *_me = blinky_obj_transform(me); 
  
  asos_object_t *_os =  ASOS_TASK_TO_OS(os);
  
  asos_timer_create( _os , 
                     me, 
                     &blinky_test_1000ms_timer,
                     1000);

  asos_timer_create( _os , 
                     me, 
                     &blinky_test_500ms_timer,
                     500); 
   
  asos_callback_assign(_os , 
                       me,
                       USER_UART1_INT,  
                       &_uart1_interrupt);  
  
  _me->time1 = 50;
  
  _me->time2 = 20;
   
  _f_led_on  =  asos_fcall_get(_os,USER_LED_1_ON); 

  _f_led_off =  asos_fcall_get(_os,USER_LED_1_OFF); 
   
  
  return ASOS_USER_TASK_SUCCESS;
}

  
ASOS_USER_FOUT 
blinky_delete( blinky_obj_p  me )
{
  NULL_CHECK_RETURN(me , ASOS_USER_TASK_FAIL);

  return ASOS_USER_TASK_SUCCESS;
}





