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

  Note1: in our "asos" object we used static memory allocation for all -> ex: callbacks[ASOS_CALLBACKS_MAX_COUNT] or mailboxes[ASOS_MAILBOXES_MAX_COUNT];  .....
         in the feature we could changed these design pattern to our list -> "asos_list" but in that way the user must define these objects in data area to RAM, 
         but now user can create these in heap area , readable is much more now I thing

*/


#pragma once    /** related header include just one time **/
#ifndef ASOS_H
#define ASOS_H 

#ifdef __cplusplus
extern "C"
{
#endif
  
	 
  
/** @mainProcessObject **/
typedef struct _asos_object_t     asos_object_t;


/** kernel / APP configs  **/
//#include "configs/asos_config.h"
#include "configs/asos_device_tree.h"

/** kernel API's  **/
#include "kernel/asos_timer.h"
#include "kernel/asos_task.h"
#include "kernel/asos_queue.h"
#include "kernel/asos_tester.h"
#include "kernel/asos_mutex.h"
#include "kernel/asos_semaphore.h"
#include "kernel/asos_mailbox.h"
#include "kernel/asos_heap.h"
#include "kernel/asos_list.h"

/** design patterns **/
#include "kernel/asos_observer.h"
#include "kernel/asos_stade_machine.h"
#include "kernel/asos_command_responce.h"

/** @functionsOutTypes **/
typedef enum {
  ASOS_SUCCESS = 0,
  ASOS_FAIL,
  ASOS_TIMEOUT,
  ASOS_WAIT
}asos_fout_t;



typedef void (*asos_print_fp_t)( const char *str , unsigned short lenght );


/** private developer config **/
#define ASOS_TIMER_US_MAX_COUNT     10000
  
        

struct _asos_object_t{

  asos_mbool process_working;
  
  unsigned int asos_timer_us;   /** max val: ASOS_TIMER_US_MAX_COUNT **/
  
  
  /**  process  API's objects **/
  
  asos_timer_t timer_manager;
    
  asos_task_t  tasks_manager;
  
  
  /** callback manager API object **/
  
  subject_obj    callbacks[ASOS_CALLBACKS_MAX_COUNT];
   
  observer_obj   observers[ ASOS_CALLBACKS_MAX_COUNT*ASOS_CALLBACKS_OBSERVERS_MAX_COUNT ];
  
  
  
  /** function call API object **/
  
  asos_device_tree_function_call_u  function_calls[ASOS_FUNCTION_CALL_MAX_COUNT];
    
  unsigned short function_calls_loaded_number;
  
  
  
  
  
  /** user printer **/
  
  asos_print_fp_t print_fp;
  
  
  
  /** asos own debug printer **/
  
  asos_print_fp_t debug_print;
  
  asos_mbool      is_debug_mode_on;
    
  
  
  
  
  /** delay process **/
  
  asos_mbool      is_delay_active;
  
  unsigned int    delay_ms_counter;
  
  
  
   /** mailbox process : for threads to talk to each other **/
  
  asos_mailbox_t    mailboxes[ASOS_MAILBOXES_MAX_COUNT];
  
  unsigned char     mailboxes_keys[ASOS_MAILBOXES_MAX_COUNT][2]; /**  it is for reach related mailbox for rx or tx !  **/
  
  unsigned short    mailbox_init_counter;
  
  
  
  /** List Process ***/

  asos_list_t list[ASOS_LISTS_MAX_COUNT];   /** INDEX -> asos_device_tree_list_call_e **/
  
  unsigned short    list_init_counter;
    
} ;














 /** @proConstructorsFunctions **/

asos_fout_t
asos_create(asos_object_t* me ,
            unsigned int   tasks_context_switch_ms_value,
            unsigned int   timer_process_us_value);

asos_fout_t
asos_delete(asos_object_t* me);


/** all processes @STOP **/
asos_fout_t
asos_delay(asos_object_t* me , 
           unsigned int   delay_ms);












 /** @proProcessFunction **/

asos_fout_t
asos_app_process_run(asos_object_t* me);




 /** @synchronousWaitFunction  **/

asos_fout_t
asos_app_process_wait_ms(asos_object_t* me , 
                         void* app_obj,
                         unsigned int wait_ms);




/** @appConstructorFunction  **/

asos_fout_t
asos_app_create(asos_object_t*        me , 
                void*                 app_obj, 
                asos_app_init_fp_t    app_init_fp,
                asos_app_delete_fp_t  app_delete_fp,
                asos_app_process_fp_t app_process_fp ,
                asos_task_priority_t  app_priorty);
  














/** @timerConstructorFunction  **/

asos_fout_t
asos_timer_create(asos_object_t*           me , 
                  void*                    app_obj, 
                  asos_timer_callback_fp_t timer_callback,
                  unsigned int             timer_trig_ms_value);
  





/** @mustCallFunctions : Interrupt handlers **/

void
asos_timer_process_run(asos_object_t* me );














/** @callbacksManager API **/

/** asos init must create **/
asos_fout_t
asos_callback_create(asos_object_t* me ,
                     asos_device_tree_interrupt_vector_e  callback_index,
                     SBJ_DATA *                           callback_data);


/** MID api can call -> default "asos_callback_start" all observers **/
asos_fout_t
asos_callback_assign(asos_object_t* me ,
                     void*          app_obj, 
                     asos_device_tree_interrupt_vector_e  callback_index,
                     observer_event_callback_fp           callback_fp);

/** in interrupt or wanna in callback process stade function if you wish  **/
asos_fout_t
asos_callback_signal(asos_object_t* me ,
                     asos_device_tree_interrupt_vector_e  callback_index);


asos_fout_t
asos_callback_start(asos_object_t* me ,
                   asos_device_tree_interrupt_vector_e  callback_index);


asos_fout_t
asos_callback_stop(asos_object_t* me ,
                   asos_device_tree_interrupt_vector_e  callback_index);













/** @functionCall API **/

asos_fout_t
asos_fcall_create( asos_object_t * me ,
                   asos_device_tree_function_call_e   fcall_index,
                   asos_device_tree_function_call_u * fcall);


asos_device_tree_function_call_u *
asos_fcall_get(asos_object_t * me ,
               asos_device_tree_function_call_e   fcall_index );














 /** @printfManager , rules asos_print(x)   ->    "> ASOS USER: ...x.... \n " **/

asos_fout_t
asos_print_create(asos_object_t* me ,
                  asos_print_fp_t _printf_fp   );
 
asos_fout_t
asos_print(asos_object_t* me ,
           const char * string );












/** @asosOwnDebugCreation**/

asos_fout_t
asos_debug_create(asos_object_t* me ,
                  asos_print_fp_t _printf_fp   );

asos_fout_t
asos_debug_mode_on(asos_object_t* me    );

asos_fout_t
asos_debug_mode_off(asos_object_t* me    );

/** could be private for user (if there is no important stutation  ) **/
asos_fout_t
asos_debug_print(asos_object_t* me ,
                 const char * string );












/** @idleTaskCreator**/
/** 
  @idleSleepDescription :
    - if you wanna activate "idle_task" then 
    you must call the -> "asos_app_process_wait_ms" in your app thread task
    or context_switch delay can jump to idle_task and sleep activate
    - ASOS can manage sleep modes, before going to "idle_task"
    ASOS can call the "sleep_mode_on" and end of the "idle_task" 
    ASOS can call the "sleep_mode_off" 
**/

/** if sleep functions not NULL the sleep auto active **/
asos_fout_t
asos_idle_task_create(asos_object_t* me ,
                      asos_idle_sleep_fp_t  idle_fp,
                      asos_idle_sleep_fp_t  sleep_on_fp,
                      asos_idle_sleep_fp_t  sleep_off_fp);
  

asos_fout_t
asos_idle_sleep_active(asos_object_t* me );

asos_fout_t
asos_idle_sleep_pasive(asos_object_t* me );
  






















 /** @mailboxConstructorAndProcessFunctions **/

asos_fout_t
asos_mailbox_create(asos_object_t* me ,
                    asos_device_tree_mailbox_call_e   mailbox_index_name,  
                    unsigned char key_way_1,  /** record to asos->mailboxes_keys **/
                    unsigned char key_way_2);

asos_fout_t
asos_mailbox_delete(asos_object_t* me , 
                    asos_device_tree_mailbox_call_e   mailbox_index_name );
 



 /** @mailboxProcessFunctions **/

asos_fout_t
asos_mailbox_write (asos_object_t * me ,
                    asos_device_tree_mailbox_call_e   mailbox_index_name,  
                    unsigned char key_way ,       /*** select which line writing ***/
                    const MAILBOX_QUEUE_DATA_TYPE* message);


asos_fout_t
asos_mailbox_read  (asos_object_t * me ,
                    asos_device_tree_mailbox_call_e   mailbox_index_name,  
                    unsigned char key_way ,       /*** select which line reading ***/
                    MAILBOX_QUEUE_DATA_TYPE* out_data , 
                    unsigned short out_data_size);

asos_fout_t
asos_mailbox_fresh (asos_object_t * me ,
                    asos_device_tree_mailbox_call_e   mailbox_index_name,  
                    unsigned char key_way );





















/** @superContainerOFF ***/
#define ASOS_TASK_TO_OS(ptr)       ASOS_CONTAINER_OF(ptr,asos_object_t,tasks_manager)
  

#ifdef __cplusplus
}
#endif

#endif
