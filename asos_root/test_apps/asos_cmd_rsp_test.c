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

#include "asos_cmd_rsp_test.h"
  

static void _uart_interrupt(void*me ,
                             observer_event_types event,
                             OBS_DATA data) 
{ 
  cmd_rsp_t *_me = (cmd_rsp_t*)(me); 
  asos_q_type datax = (asos_q_type)data;
  asos_queue_data_write(  &_me->queue , &datax , 1);
}


ASOS_USER_FOUT
cmd_rsp_create( void * me , asos_task_t * os)
{
  cmd_rsp_t * _me = (cmd_rsp_t*)me;
  
  asos_object_t *_os = ASOS_TASK_TO_OS(os);
   
  _me->os = os;
   
  
  asos_queue_create( &_me->queue); 
  
  
  asos_callback_assign(_os , 
                       me,
                       USER_UART1_INT,  
                       &_uart_interrupt);  
  
  _me->led_on  =  asos_fcall_get(_os,USER_LED_1_ON); 

  _me->led_off =  asos_fcall_get(_os,USER_LED_1_OFF);     
  
  _me->uart_sender_fp =   asos_fcall_get(_os,UART_1_SENDER); 
     
  /** create test main object -> command responce object **/
  asos_command_responce_create(&_me->CMD_RSP ,
                               _os ,
                               (asos_cmd_rsp_fp)_me->uart_sender_fp->call_std_sender ,
                               (CMD_RSP_QUEUE_OBJ_TYPE  *) &_me->queue);     

   
  return ASOS_USER_TASK_SUCCESS;
}



ASOS_USER_FOUT
cmd_rsp_process( void * me , asos_task_t * os)
{
  char out_buffer[100];
  
  cmd_rsp_t * _me = (cmd_rsp_t*)me;
  
  asos_object_t *_os = ASOS_TASK_TO_OS(os); 
    
  asos_cmd_rsp_fout_e out_f = 
  asos_command_responce_process_run(&_me->CMD_RSP ,
                                     (const char*)"HELLO\n", 
                                     (const char*)"hii\n", 
                                     (char*) out_buffer,
                                     100,
                                     3,
                                     2000);  
  
  asos_app_process_wait_ms( _os , me , 1 );  
   
  if( out_f ==  ASOS_CMD_RSP_SUCCESS_FUNC){
        asos_print(_os , " test SUCCESS ");
        asos_app_process_wait_ms( _os , me , 1000 );  
   }
   else if( out_f ==  ASOS_CMD_RSP_FAIL_FUNC){
        asos_print(_os , " test FAIL ");
        asos_app_process_wait_ms( _os , me , 1000 );  
   }
   else if( out_f ==  ASOS_CMD_RSP_TIMEOUT_FUNC){
        asos_print(_os , " test try again ");
   }
       
  return ASOS_USER_TASK_SUCCESS;
}


 





















 
