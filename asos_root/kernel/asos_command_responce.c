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
 
#include "asos_command_responce.h"

//#define MY_LOG_WATCH_IS_ACTIVE    /** for test log -> do comment **/
 
#ifdef ASOS_COMMAND_RESPONCE_QUEUE_ALLOCATION_SIZE_LOW
  #define QUEUE_CREATE(queue)                   asos_queue_create( (asos_queue_str*) queue)
  #define QUEUE_WRITE(queue , data , size)      asos_queue_data_write( (asos_queue_str*) queue , (asos_q_type*) data , (unsigned int)size)
  #define QUEUE_CLEAR(queue)                    asos_queue_clear( (asos_queue_str*) queue)
  #define QUEUE_IS_EMPTY(queue)                 asos_queue_is_empty( (asos_queue_str*) queue)
  #define QUEUE_READ(queue , out_buf , size)    asos_queue_data_read( (asos_queue_str*) queue , (asos_q_type*)out_buf , (unsigned int)size)
  #define QUEUE_GET_UNREAD_SIZE(queue )         asos_queue_get_unreaden_size( (asos_queue_str*) queue)
  #define QUEUE_SIZE                            ASOS_QUEUE_LOW_SIZE_STATIC_SIZE 
#else
  #define QUEUE_CREATE(queue)                   asos_l_queue_create( (asos_l_queue_str*) queue)
  #define QUEUE_WRITE(queue , data , size)      asos_l_queue_data_write( (asos_l_queue_str*) queue , (asos_l_q_type*) data , (unsigned int)size)
  #define QUEUE_CLEAR(queue)                    asos_l_queue_clear( (asos_l_queue_str*) queue)
  #define QUEUE_IS_EMPTY(queue)                 asos_l_queue_is_empty( (asos_l_queue_str*) queue)
  #define QUEUE_READ(queue , out_buf , size)    asos_l_queue_data_read( (asos_l_queue_str*) queue , (asos_l_q_type*)out_buf , (unsigned int)size)
  #define QUEUE_GET_UNREAD_SIZE(queue )         asos_l_queue_get_unreaden_size( (asos_l_queue_str*) queue)
  #define QUEUE_SIZE                            ASOS_QUEUE_HIGH_SIZE_STATIC_SIZE 
#endif   

#define QUEUE_OBJ_TYPE    CMD_RSP_QUEUE_OBJ_TYPE
#define QUEUE_DATA_TYPE   CMD_RSP_QUEUE_DATA_TYPE 

#define ASOS_OBJ_CHECK(obj)        if(obj == NULL){ return ASOS_CMD_RSP_FAIL_FUNC; }
#define ASOS_INIT_CHECK(obj)       if(obj->is_init != ASOS_TRUE){ return ASOS_CMD_RSP_FAIL_FUNC; }

#define ASOS_LOG_PRINT(obj , str)       \
do{\
  if(obj->os){ \
     asos_debug_print(obj->os ,(const char * )str );\
  }\
}while(0);
 
#define GOTO_TASK(me , task)     me->privates->stade = (asos_cmd_rsp_stade_e)task;  me->privates->timer_ms = 0; 
#define SEND_COMMAND(me , str)   me->sender_fp( (const char *)str , _array_length((const char *)str) );





typedef enum {
  ASOS_CMD_RSP_INIT_STEP = 0, 
  ASOS_CMD_RSP_RECEIVE_STEP,
  ASOS_CMD_RSP_FAIL_STEP
}asos_cmd_rsp_stade_e; 


struct _asos_cmd_rsp_privates_t {
 
  unsigned int      timer_ms;
  
  unsigned short    try_count;
  
  asos_cmd_rsp_stade_e    stade;
    
} ;

/** timer interrupt  ***/
static void _1_ms_timer( void*me , asos_timer_t * os )
{
  asos_command_responce_t *_me = (asos_command_responce_t*)(me); 
  
  if(_me && _me->privates && _me->is_init == ASOS_TRUE){
     _me->privates->timer_ms++; 
  } 
}
   

static   unsigned short _array_length( const char *array){
  if(array ==NULL){return 0;}
  unsigned int i = 0;
  do{
    i++;
    array++;
  }while(*array);
  return i;
}
          
asos_cmd_rsp_fout_e
asos_command_responce_create(asos_command_responce_t * me ,
                             asos_object_t           * os ,
                             asos_cmd_rsp_fp           sender_fp ,
                             CMD_RSP_QUEUE_OBJ_TYPE  * load_queue)
{
   ASOS_OBJ_CHECK(me);
   ASOS_OBJ_CHECK(os);
   ASOS_OBJ_CHECK(load_queue);
   
   /** asos malloc **/
   me->privates = ( struct _asos_cmd_rsp_privates_t * ) asos_malloc( sizeof( struct _asos_cmd_rsp_privates_t ) );
   
   ASOS_OBJ_CHECK(me->privates);
   
   me->os = os; 
   me->sender_fp = sender_fp; 
   me->receive_queue = load_queue; 
   me->privates->stade = ASOS_CMD_RSP_INIT_STEP; 
   me->privates->timer_ms = 0; 
   me->privates->try_count = 0;
   me->is_init = ASOS_TRUE;
    
   asos_timer_create( os ,  me,  &_1_ms_timer, 1);
     
   ASOS_LOG_PRINT(me , "asos command responce created");
     
   return ASOS_CMD_RSP_SUCCESS_FUNC;
}

       
asos_cmd_rsp_fout_e
asos_command_responce_delete(asos_command_responce_t *me )
{
  ASOS_OBJ_CHECK(me);
  ASOS_INIT_CHECK(me) 
  ASOS_OBJ_CHECK(me->privates);
  
  asos_free( me->privates );  /** it is not work **/
  
  me->os = NULL; 
  me->sender_fp = NULL; 
  me->receive_queue = NULL; 
  me->privates->stade = ASOS_CMD_RSP_INIT_STEP; 
  me->privates->timer_ms = 0; 
  me->privates->try_count = 0;
  me->is_init = ASOS_TRUE;
  
  return ASOS_CMD_RSP_SUCCESS_FUNC;
}

          












/**
  @howToUse:
     if you wanna break "asos_command_responce_process"  when it is runing
     you must  "asos_command_responce_process_clear" for breaking 
**/ 
asos_cmd_rsp_fout_e          
asos_command_responce_process_clear(asos_command_responce_t * me)
{
  ASOS_OBJ_CHECK(me);
  ASOS_OBJ_CHECK(me->privates);
  ASOS_INIT_CHECK(me); 
  
  me->privates->stade = ASOS_CMD_RSP_INIT_STEP; 
  me->privates->timer_ms = 0; 
  me->privates->try_count = 0;

  return ASOS_CMD_RSP_SUCCESS_FUNC;
}






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
                                  unsigned int              try_again_timeout_ms)
{
   ASOS_OBJ_CHECK(me);
   ASOS_OBJ_CHECK(me->privates);
   ASOS_INIT_CHECK(me);
   ASOS_OBJ_CHECK(send_command); 
 
   asos_cmd_rsp_fout_e f_out = ASOS_CMD_RSP_WAIT_FUNC;
    
   switch (me->privates->stade){ 
     
     case ASOS_CMD_RSP_INIT_STEP:{ 
       
       #ifdef MY_LOG_WATCH_IS_ACTIVE
       ASOS_LOG_PRINT(me , "asos cmd rsp init for send ->");
       ASOS_LOG_PRINT(me , send_command);
       #endif
       
       GOTO_TASK(me , ASOS_CMD_RSP_RECEIVE_STEP);
         
       me->privates->try_count++;
       
       SEND_COMMAND(me ,  send_command); 
       
       break;
     }
     
     case ASOS_CMD_RSP_RECEIVE_STEP:{  
       
       CMD_RSP_QUEUE_DATA_TYPE hold_q[ASOS_COMMAND_RESPONCE_BUF_HOLD_STACK_SIZE];
         
       if( QUEUE_IS_EMPTY( me->receive_queue ) && 
           expected_receive_responce != NULL &&
           out_buffer != NULL){
             
           unsigned short read_lenght = QUEUE_GET_UNREAD_SIZE( me->receive_queue );
             
           QUEUE_READ(me->receive_queue , hold_q , ASOS_COMMAND_RESPONCE_BUF_HOLD_STACK_SIZE);
         
           if(strstr((char*)hold_q,expected_receive_responce)) 
           {
             if(out_buffer_size){
             
               unsigned short i = 0 ; 
               
               do{
                 out_buffer[i] = (char)hold_q[i];
                 i++;
                 if(i>(out_buffer_size-2)){ break;}
               }while(i<read_lenght);
               
               out_buffer[i] = 0;
             }
             
             asos_command_responce_process_clear( me );
             
             f_out = ASOS_CMD_RSP_SUCCESS_FUNC; 
             
             #ifdef MY_LOG_WATCH_IS_ACTIVE
             ASOS_LOG_PRINT(me , "asos cmd rsp SUCCESS"); 
             #endif
             
             break;
           }
       }
       else if(me->privates->timer_ms > try_again_timeout_ms){

          #ifdef MY_LOG_WATCH_IS_ACTIVE
          ASOS_LOG_PRINT(me , "asos cmd rsp err timeout"); 
          #endif

          f_out = ASOS_CMD_RSP_TIMEOUT_FUNC;

          GOTO_TASK(me , ASOS_CMD_RSP_FAIL_STEP);
       }
         
       break;
     } 
     
     case ASOS_CMD_RSP_FAIL_STEP:{ 

       if(me->privates->try_count < try_count){
       
         GOTO_TASK(me , ASOS_CMD_RSP_INIT_STEP);
         
         #ifdef MY_LOG_WATCH_IS_ACTIVE
         ASOS_LOG_PRINT(me , "asos cmd rsp rty again"); 
         #endif
          
       }
       else{
       
         asos_command_responce_process_clear( me );
         
         f_out = ASOS_CMD_RSP_FAIL_FUNC;
         
         #ifdef MY_LOG_WATCH_IS_ACTIVE
         ASOS_LOG_PRINT(me , "asos cmd rsp FAIL"); 
         #endif 
       }
       
       break;
     }
     
     default:{ 
       
        asos_command_responce_process_clear( me );
        
        #ifdef MY_LOG_WATCH_IS_ACTIVE
        ASOS_LOG_PRINT(me , "asos cmd rsp err default"); 
        #endif
        
        break;
     }
   }
    
   return f_out;
}
          
          

                             

          

  

















          