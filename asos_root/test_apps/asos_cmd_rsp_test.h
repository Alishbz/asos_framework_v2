/****************************************************************************************
* ALI SAHBAZ
*
*
* ASOS - "Ali Sahbaz Operating System" Framework
*
* Description: it is a GPOS , general purpose operating system. 
*              And kernel hardware manager.
* 
* Date          : 06.07.2023
* By            : Ali Sahbaz
* e-mail        : ali_sahbaz@outlook.com
*/ 
#ifndef asos_cmd_rsp_test_H
#define asos_cmd_rsp_test_H

#ifdef __cplusplus
extern "C"
{
#endif

  
#include "../asos.h"

   
  
  
typedef struct {
  
  asos_task_t * os;   /** Developer can hold own task object then reach his OS  **/
  
  asos_command_responce_t  CMD_RSP; 
   
  asos_device_tree_function_call_u * led_on , * led_off;
   
  asos_device_tree_function_call_u * uart_sender_fp;
  
  asos_queue_str     queue; /*** ASOS_COMMAND_RESPONCE_QUEUE_ALLOCATION_SIZE_LOW because of it  **/
  
}cmd_rsp_t;




ASOS_USER_FOUT
cmd_rsp_create( void * me , asos_task_t * os );
 
ASOS_USER_FOUT
cmd_rsp_process( void * me , asos_task_t * os);






#ifdef __cplusplus
}
#endif

#endif
