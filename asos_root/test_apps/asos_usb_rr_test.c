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

#include "asos_usb_rr_test.h"

/***
#include "usbd_cdc_if.h"         // USB Middleware
****/
static asos_device_tree_function_call_u * _fx_led_on , * _fx_led_off;


static void _uart1x_interrupt( void*me ,
                               observer_event_types event,
                               OBS_DATA data) 
{ 
  /** usb_t * _me = (usb_t*)me; **/
  
  /** asos_object_t *_os = ASOS_TASK_TO_OS(_me->os);  **/
  
  if(data == 'c'){ 
    if( _fx_led_on != NULL ){
      /** CALL mcall **/
      _fx_led_on->call_empty_return_and_input();
    } 
   
  }
  else  if(data == 'd'){ 
    if( _fx_led_off != NULL ){
      /** CALL mcall **/
      _fx_led_off->call_empty_return_and_input();
    } 
  
  }
}
 


ASOS_USER_FOUT
usb_create( void * me , asos_task_t * os)
{
  usb_t * _me = (usb_t*)me;
  
  asos_object_t *_os = ASOS_TASK_TO_OS(os);
  
  if(!_me->id){
    _me->id = 1;
  }
  
    asos_callback_assign(_os , 
                         me,
                         USER_UART1_INT,  
                         &_uart1x_interrupt);  
  
   _fx_led_on  =  asos_fcall_get(_os,USER_LED_1_ON); 

   _fx_led_off =  asos_fcall_get(_os,USER_LED_1_OFF); 
   
   _me->os = os;
  
  return ASOS_USER_TASK_SUCCESS;
}

ASOS_USER_FOUT
usb_process( void * me , asos_task_t * os)
{
  usb_t * _me = (usb_t*)me;
  
  asos_object_t *_os = ASOS_TASK_TO_OS(os); 
  
  unsigned char str[20]; memset(str,0,20);
   
  snprintf((char*)str, 20, "task %d \n", _me->id );
  

  /** direct hardware usb mid layer access AND Transmit datas **/
  /***
  CDC_Transmit_FS((uint8_t*)str, 8);
  ***/


  asos_app_process_wait_ms( _os , me , 500 );
  
  return ASOS_USER_TASK_SUCCESS;
}
