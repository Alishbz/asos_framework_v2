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
#pragma once
#ifndef ASOS_COMMAND_RESPONCE_H
#define ASOS_COMMAND_RESPONCE_H

/*** asos_cmd_rsp -> asos_command_responce ***/

#ifdef __cplusplus
	extern "C"
	{
#endif

#include "../asos.h"
//#include "../../asos.h"

/** @user config **/
#define ASOS_COMMAND_RESPONCE_QUEUE_ALLOCATION_SIZE_LOW    1
#define ASOS_COMMAND_RESPONCE_BUF_HOLD_STACK_SIZE          100 /** importand for parseing stack size **/
          
          
#ifdef ASOS_COMMAND_RESPONCE_QUEUE_ALLOCATION_SIZE_LOW
  #define CMD_RSP_QUEUE_OBJ_TYPE                        asos_queue_str 
  #define CMD_RSP_QUEUE_DATA_TYPE                       asos_q_type 
#else
  #define CMD_RSP_QUEUE_OBJ_TYPE                        asos_l_queue_str  
  #define CMD_RSP_QUEUE_DATA_TYPE                       asos_l_q_type 
#endif   
  
          
typedef enum {
  ASOS_CMD_RSP_FAIL_FUNC,
  ASOS_CMD_RSP_TIMEOUT_FUNC,
  ASOS_CMD_RSP_WAIT_FUNC,
  ASOS_CMD_RSP_SUCCESS_FUNC
}asos_cmd_rsp_fout_e;



typedef struct  _asos_cmd_rsp_privates_t     asos_cmd_rsp_privates_t;

typedef void    (*asos_cmd_rsp_fp)( const char *str , unsigned short lenght );          
          
          
typedef struct {

  asos_object_t             * os;              /** inheritance to asos os **/
  
  asos_cmd_rsp_fp             sender_fp;
  
  CMD_RSP_QUEUE_OBJ_TYPE    * receive_queue;   /** YOU MUST USE ASOS QUEUE **/
  
  /** @hardPrivate with @AsosMalloc **/
  asos_cmd_rsp_privates_t   * privates;
  
  asos_mbool                  is_init;
   
} asos_command_responce_t;
          
          
          
asos_cmd_rsp_fout_e
asos_command_responce_create(asos_command_responce_t * me ,
                             asos_object_t           * os ,
                             asos_cmd_rsp_fp           sender_fp ,
                             CMD_RSP_QUEUE_OBJ_TYPE  * load_queue);     

       
asos_cmd_rsp_fout_e
asos_command_responce_delete(asos_command_responce_t *me );     


/**
  @howToUse:
     asos_cmd_rsp_fout_e out = asos_command_responce_process_run(... out_buffer ....) 
     if( out ==  ASOS_CMD_RSP_SUCCESS_FUNC){
         - TASK is over and SUCCESS you can parse your own buffer,
          if(out_buffer == "TESTX: 1234"){  .... }
     }
     else if( out ==  ASOS_CMD_RSP_FAIL_FUNC){
        - Hard FAIL , you shold not try again
     }
     else if( out ==  ASOS_CMD_RSP_TIMEOUT_FUNC){
        - trying again to send and responce check
     }
     else if( out ==  ASOS_CMD_RSP_WAIT_FUNC){
        - do nohing
     }

..........

    if you wanna stop the process -> 
       ->  "asos_command_responce_process_clear"

**/
asos_cmd_rsp_fout_e
asos_command_responce_process_run(asos_command_responce_t * me ,
                                  const char              * send_command , 
                                  const char              * expected_receive_responce ,
                                  char                    * out_buffer,
                                  unsigned short            out_buffer_size,
                                  unsigned short            try_count ,
                                  unsigned int              try_again_timeout_ms);             
     

/**
  @howToUse:
     if you wanna break "asos_command_responce_process"  when it is runing
     you must  "asos_command_responce_process_clear" for breaking 
**/
asos_cmd_rsp_fout_e          
asos_command_responce_process_clear(asos_command_responce_t * me);        
          



#ifdef __cplusplus
	}
#endif

#endif
