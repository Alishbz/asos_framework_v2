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
 
#include "asos.h"


#define ASOS_OBJ_CHECK(xme)  if(xme == NULL){ /** TODO add print **/ return ASOS_FAIL; }
#define ASOS_STADE_CHECK(condition)  if(condition){ /** TODO add print **/ return ASOS_FAIL; }


/** if app_obj == NULL then ASOS can not wait **/
asos_fout_t
asos_app_process_wait_ms(asos_object_t* me ,  
                         void* app_obj,
                         unsigned int wait_ms)  
{
   ASOS_OBJ_CHECK(me); 
   ASOS_OBJ_CHECK(app_obj); 
     
   asos_xtask_synchronous_wait_load(&me->tasks_manager , app_obj , wait_ms);
 
   return ASOS_SUCCESS;
}



void
asos_timer_process_run(asos_object_t* me  )
{
  if(me->process_working == ASOS_TRUE){
    
    asos_xtimer_process_run(&me->timer_manager  ); 
    
    me->asos_timer_us += (me->timer_manager.timer_process_us_val); 
     
    if(me->asos_timer_us > ASOS_TIMER_US_MAX_COUNT){ me->asos_timer_us = 0; }
  } 
  
  if( me->is_delay_active == ASOS_TRUE){
    
     me->delay_ms_counter += (unsigned int)((me->timer_manager.timer_process_us_val)/1000);
  }
}

asos_fout_t
asos_create(asos_object_t* me,
            unsigned int   tasks_context_switch_ms_value,
            unsigned int   timer_process_us_value)
{
  ASOS_OBJ_CHECK(me);
    
  asos_heap_init();
            
  unsigned short index = 0;
  
  me->process_working = ASOS_FALSE;
  
  me->asos_timer_us = 0; 
  
  me->process_working = ASOS_TRUE;
  
  me->print_fp = NULL; 
  
  me->debug_print = NULL; 
  
  me->is_delay_active = ASOS_FALSE;
  
  me->delay_ms_counter = 0; 
    
  me->is_debug_mode_on = ASOS_FALSE;
  
  me->function_calls_loaded_number = 0;
    
  me->mailbox_init_counter = 0;
  
  me->list_init_counter = 0;
  
  // todo ; list , mailboxes and mailboxes_keys DO INIT
    
  for(index = 0 ; index < ASOS_CALLBACKS_MAX_COUNT ; index++){
    subject_create( &me->callbacks[index] , NULL); 
  } 
    
  asos_xtimer_create(&me->timer_manager, timer_process_us_value); 
  
  asos_xtask_create(&me->tasks_manager , tasks_context_switch_ms_value);
  

   
  return ASOS_SUCCESS;
}

asos_fout_t 
asos_app_create(asos_object_t*        me , 
                void*                 app_obj, 
                asos_app_init_fp_t    app_init_fp,
                asos_app_delete_fp_t  app_delete_fp,
                asos_app_process_fp_t app_process_fp,
                asos_task_priority_t  app_priorty)
{
  ASOS_OBJ_CHECK(me);
  
  if(app_init_fp == NULL || app_process_fp == NULL) {
    assert( "PROCESS and INIT functions are MUST" ); 
    return ASOS_FAIL;
  }
  
  asos_task_load_t loader;
  
  /** add manuely ?**/
  loader.config.prioty = app_priorty;
  
  loader.app_obj = app_obj;
  loader.app_init_fp = app_init_fp;
  loader.app_delete_fp = app_delete_fp;
  loader.app_process_fp = app_process_fp;
  
  asos_xtask_app_create(&me->tasks_manager , &loader ); 
 
  return ASOS_SUCCESS;
}

asos_fout_t
asos_timer_create(asos_object_t*           me , 
                  void*                    app_obj, 
                  asos_timer_callback_fp_t timer_callback,
                  unsigned int             timer_trig_ms_value)
{
  ASOS_OBJ_CHECK(me);

  if(timer_callback == NULL || !timer_trig_ms_value ) { 
    return ASOS_FAIL;
  }

  asos_timer_unit_t loader;

  loader.app_obj = app_obj;
  loader.callback = timer_callback;
  loader.config_ms = timer_trig_ms_value;

  asos_xtimer_unit_create( &me->timer_manager , &loader );

  return ASOS_SUCCESS;
}

asos_fout_t
asos_app_process_run(asos_object_t* me)
{
  ASOS_OBJ_CHECK(me);
  
  asos_xtask_init_all_tasks(&me->tasks_manager);
  
  while( me->process_working ){ 
    
    asos_xtask_run_next_task(&me->tasks_manager); 
    
  }
  
  return ASOS_SUCCESS;
}



asos_fout_t
asos_delete(asos_object_t* me)
{
  ASOS_OBJ_CHECK(me);
  
  me->process_working = ASOS_FALSE;
  
  asos_create(me,0,0);
  
  return ASOS_SUCCESS;
}













/** asos init must create **/
asos_fout_t
asos_callback_create(asos_object_t* me ,
                     asos_device_tree_interrupt_vector_e  callback_index,
                     SBJ_DATA *                           callback_data)
{
  ASOS_OBJ_CHECK(me);
  
  unsigned short _callback_index = (unsigned short)callback_index;
  
  if(_callback_index  > ASOS_CALLBACKS_MAX_COUNT-1){
    assert( "ASOS_CALLBACKS_MAX_COUNT is not enough" ); 
    return ASOS_FAIL;
  }
  
  subject_create( &me->callbacks[_callback_index] , callback_data); 
  
  return ASOS_SUCCESS;
}


/** MID api can call **/
asos_fout_t
asos_callback_assign(asos_object_t* me ,
                     void*          app_obj, 
                     asos_device_tree_interrupt_vector_e  callback_index,
                     observer_event_callback_fp           callback_fp)
{
  ASOS_OBJ_CHECK(me); 
  ASOS_OBJ_CHECK(callback_fp);

  unsigned short _callback_index = (unsigned short)callback_index;
  
  if(_callback_index  > ASOS_CALLBACKS_MAX_COUNT-1){ 
    assert( "ASOS_CALLBACKS_MAX_COUNT is not enough" ); 
    return ASOS_FAIL;
  }
   
  unsigned short observer_index = (me->callbacks[_callback_index].observer_count + 1) *
                                  ASOS_CALLBACKS_MAX_COUNT;
  
  if(observer_index > ((ASOS_CALLBACKS_MAX_COUNT*ASOS_CALLBACKS_OBSERVERS_MAX_COUNT)-1)){
    assert( "OBSERVERS index error" ); 
    return ASOS_FAIL;
  }
  
  observer_create( &me->observers[observer_index]  , app_obj , callback_fp );  
  
  observer_start_watch(&me->observers[observer_index]);
  
  subject_add_observer(&me->callbacks[_callback_index]  , &me->observers[observer_index] );
  
  return ASOS_SUCCESS;
}


/** in interrupt or wanna in callback process stade function if you wish  **/
asos_fout_t
asos_callback_signal(asos_object_t* me ,
                     asos_device_tree_interrupt_vector_e  callback_index)
{
  ASOS_OBJ_CHECK(me);
  
  unsigned short _callback_index = (unsigned short)callback_index;
  
  if(_callback_index > ASOS_CALLBACKS_MAX_COUNT-1){
    assert( "ASOS_CALLBACKS_MAX_COUNT is not enough" ); 
   return ASOS_FAIL;
  }
  
  subject_update_observers( &me->callbacks[_callback_index] );
  
  return ASOS_SUCCESS;
}




asos_fout_t
asos_callback_start(asos_object_t* me ,
                   asos_device_tree_interrupt_vector_e  callback_index)
{
  ASOS_OBJ_CHECK(me);
  
  unsigned short _callback_index = (unsigned short)callback_index , index = 0;
  
  if(_callback_index > ASOS_CALLBACKS_MAX_COUNT-1){
    assert( "ASOS_CALLBACKS_MAX_COUNT is not enough" ); 
    return ASOS_FAIL;
  }
  
  unsigned short observer_index = 0;
  
  if(!me->callbacks[_callback_index].observer_count ){
   return ASOS_FAIL;
  }  
  
  for(index = 0 ; index < me->callbacks[_callback_index].observer_count; index++)
  { 
     observer_index = index * ASOS_CALLBACKS_MAX_COUNT;
     
     observer_start_watch(&me->observers[observer_index]);
  }
   
  return ASOS_SUCCESS;
}


asos_fout_t
asos_callback_stop(asos_object_t* me ,
                   asos_device_tree_interrupt_vector_e  callback_index)
{
  ASOS_OBJ_CHECK(me);
  
  unsigned short _callback_index = (unsigned short)callback_index , index = 0;
  
  if(_callback_index > ASOS_CALLBACKS_MAX_COUNT-1){
    assert( "ASOS_CALLBACKS_MAX_COUNT is not enough" ); 
   return ASOS_FAIL;
  }
  
  unsigned short observer_index = 0;
  
  if(!me->callbacks[_callback_index].observer_count ){
   return ASOS_FAIL;
  }  
  
  for(index = 0 ; index < me->callbacks[_callback_index].observer_count; index++)
  { 
     observer_index = index * ASOS_CALLBACKS_MAX_COUNT;
     
     observer_stop_watch(&me->observers[observer_index]);
  }
   
  return ASOS_SUCCESS;
}



    
asos_fout_t
asos_fcall_create( asos_object_t * me ,
                   asos_device_tree_function_call_e   fcall_index,
                   asos_device_tree_function_call_u * fcall)
{
  ASOS_OBJ_CHECK(me);
  ASOS_OBJ_CHECK(fcall);
  
  unsigned short _fcall_index = (unsigned short)fcall_index ;
  
  if( _fcall_index > (ASOS_FUNCTION_CALL_MAX_COUNT-1 )){
    assert( "ASOS_FUNCTION_CALL_MAX_COUNT is not enough" ); 
    return ASOS_FAIL;
  }
  
  if( me->function_calls_loaded_number > (ASOS_FUNCTION_CALL_MAX_COUNT-1 )){
    assert( "ASOS_FUNCTION_CALL_MAX_COUNT is not enough" ); 
    return ASOS_FAIL;
  }  
  
  me->function_calls[_fcall_index] = *fcall;
  
  me->function_calls_loaded_number++;
    
  return ASOS_SUCCESS;
}



asos_device_tree_function_call_u *
asos_fcall_get(asos_object_t * me ,
               asos_device_tree_function_call_e   fcall_index )
{
  if(me == NULL){ return NULL;}

  unsigned short _fcall_index = (unsigned short)fcall_index ;
  
  if( _fcall_index > (ASOS_FUNCTION_CALL_MAX_COUNT-1 )){
    assert( "ASOS_FUNCTION_CALL_MAX_COUNT is not enough" ); 
    return NULL;
  }
  
  
  return &me->function_calls[_fcall_index];
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


asos_fout_t
asos_print_create(asos_object_t* me ,
                  asos_print_fp_t _printf_fp   )
{
  ASOS_OBJ_CHECK(me);
  ASOS_OBJ_CHECK(_printf_fp);
  
  me->print_fp = _printf_fp;
  
  return ASOS_SUCCESS;
}
 
#define MAX_PRINT_SIZE   200



asos_fout_t
asos_print(asos_object_t* me ,
           const char * string )
{
  ASOS_OBJ_CHECK(me);
  ASOS_OBJ_CHECK(string);
  ASOS_OBJ_CHECK(me->print_fp);
  
#ifdef ASOS_USER_PRINT_MODE
  char sender_data[MAX_PRINT_SIZE]; 
  
  memset(sender_data , 0 , MAX_PRINT_SIZE);
  
  snprintf((char*)sender_data,MAX_PRINT_SIZE, "> ASOS USER: %s \n" , string );
  
  unsigned short sender_lenght = _array_length( (const char*)sender_data );
  
  me->print_fp( (const char*)sender_data , sender_lenght ); 
#endif
  
  return ASOS_SUCCESS;
}
 



asos_fout_t
asos_debug_print(asos_object_t* me ,
                 const char * string )
{
  ASOS_OBJ_CHECK(me);
  ASOS_OBJ_CHECK(string);
  ASOS_OBJ_CHECK(me->debug_print);
  
  if(me->is_debug_mode_on == ASOS_TRUE){
    
#ifdef ASOS_OS_DEBUG_MODE
      char sender_data[MAX_PRINT_SIZE]; 
      
      memset(sender_data , 0 , MAX_PRINT_SIZE);
      
      snprintf((char*)sender_data,MAX_PRINT_SIZE, "> ASOS DEBUG: %s \n" , string );
      
      unsigned short sender_lenght = _array_length( (const char*)sender_data );
      
      me->debug_print( (const char*)sender_data , sender_lenght ); 
#endif
  }
  return ASOS_SUCCESS;
}












/** @asosOwnDebugCreation**/

asos_fout_t
asos_debug_create(asos_object_t* me ,
                  asos_print_fp_t _printf_fp   )
{
  ASOS_OBJ_CHECK(me);
  ASOS_OBJ_CHECK(_printf_fp);
  
  me->debug_print = _printf_fp;
  me->is_debug_mode_on = ASOS_TRUE;
  
  return ASOS_SUCCESS;
}

asos_fout_t
asos_debug_mode_on(asos_object_t* me    )
{
  ASOS_OBJ_CHECK(me);
  ASOS_OBJ_CHECK(me->debug_print);
  
  me->is_debug_mode_on = ASOS_TRUE;
  
  return ASOS_SUCCESS;
}

asos_fout_t
asos_debug_mode_off(asos_object_t* me    )
{ 
  ASOS_OBJ_CHECK(me);
  me->is_debug_mode_on = ASOS_FALSE;
  
  return ASOS_SUCCESS;
}



#define ASOS_DELAY_PROTECTION_MS     (100000)

asos_fout_t
asos_delay(asos_object_t* me , 
           unsigned int   delay_ms)
{
  ASOS_OBJ_CHECK(me);
  
  if(delay_ms > 0){
    me->is_delay_active = ASOS_TRUE;
    me->delay_ms_counter = 0;
    
    do{
      if(me->delay_ms_counter > delay_ms){
        break;
      }
      
    }while(me->delay_ms_counter < ASOS_DELAY_PROTECTION_MS);
    
    me->delay_ms_counter = 0;
    me->is_delay_active = ASOS_FALSE;
  }

  return ASOS_SUCCESS;
}













asos_fout_t
asos_idle_task_create(asos_object_t* me ,
                      asos_idle_sleep_fp_t  idle_fp,
                      asos_idle_sleep_fp_t  sleep_on_fp,
                      asos_idle_sleep_fp_t  sleep_off_fp)
{
  ASOS_OBJ_CHECK(me);
  ASOS_OBJ_CHECK(idle_fp); 
  
  asos_xtask_idle_assign(&me->tasks_manager , 
                        idle_fp,
                        sleep_on_fp,
                        sleep_off_fp);
 
  return ASOS_SUCCESS;
}
  

asos_fout_t
asos_idle_sleep_active(asos_object_t* me )
{
  ASOS_OBJ_CHECK(me); 
  
  asos_xtask_idle_sleep_active(&me->tasks_manager );
 
  return ASOS_SUCCESS;
}
  

asos_fout_t
asos_idle_sleep_pasive(asos_object_t* me )
{
  ASOS_OBJ_CHECK(me); 
  
  asos_xtask_idle_sleep_pasive(&me->tasks_manager );
 
  return ASOS_SUCCESS;
}

















                  






                  





                  






 

asos_fout_t
asos_mailbox_create(asos_object_t* me ,
                    asos_device_tree_mailbox_call_e   mailbox_index_name,  
                    unsigned char key_way_1,  /** record to asos->mailboxes_keys **/
                    unsigned char key_way_2)
{
  ASOS_OBJ_CHECK(me); 
  ASOS_STADE_CHECK( ( (int)(mailbox_index_name) > (int)(ASOS_MAILBOXES_MAX_COUNT-1) ) );
  ASOS_STADE_CHECK( ( me->mailbox_init_counter > (int)(ASOS_MAILBOXES_MAX_COUNT-1) ) );
  
  if(ASOS_MAILBOX_FAIL == asos_xmailbox_create( &me->mailboxes[ (int)mailbox_index_name ] , 
                                                ASOS_MAILBOX_ASOS_PROTOCHOL))
  {
     return ASOS_FAIL;
  }

  me->mailboxes_keys[mailbox_index_name][0] = key_way_1;
  me->mailboxes_keys[mailbox_index_name][1] = key_way_2;
  
  me->mailbox_init_counter++;
  
  return ASOS_SUCCESS;
}



asos_fout_t
asos_mailbox_delete(asos_object_t* me , 
                    asos_device_tree_mailbox_call_e   mailbox_index_name )
{
  ASOS_OBJ_CHECK(me); 
  ASOS_STADE_CHECK( ( (int)(mailbox_index_name) > (int)(ASOS_MAILBOXES_MAX_COUNT-1) ) );
  ASOS_STADE_CHECK( ( me->mailbox_init_counter > (int)(ASOS_MAILBOXES_MAX_COUNT-1) ) );
  ASOS_STADE_CHECK(  me->mailbox_init_counter < 1  );
  
  if(ASOS_MAILBOX_FAIL == asos_xmailbox_delete( &me->mailboxes[ (int)mailbox_index_name ] ))
  {
     return ASOS_FAIL;
  }
  
  me->mailboxes_keys[mailbox_index_name][0] = 0;
  me->mailboxes_keys[mailbox_index_name][1] = 0;
  
  me->mailbox_init_counter--;
  
  return ASOS_SUCCESS;
}

 
 
static unsigned short _mailbox_message_size(MAILBOX_QUEUE_DATA_TYPE*array){

  unsigned short lenght = 0;
  while(*(array+lenght)){
  lenght++;
  }
  return  lenght;
} 

 

asos_fout_t
asos_mailbox_write (asos_object_t * me ,
                    asos_device_tree_mailbox_call_e   mailbox_index_name,  
                    unsigned char key_way ,       /*** select which line writing ***/
                    const MAILBOX_QUEUE_DATA_TYPE* message)
{
  ASOS_OBJ_CHECK(me); 
  ASOS_OBJ_CHECK(message); 
  ASOS_STADE_CHECK( ( (int)(mailbox_index_name) > (int)(ASOS_MAILBOXES_MAX_COUNT-1) ) );
  ASOS_STADE_CHECK( ( me->mailbox_init_counter > (int)(ASOS_MAILBOXES_MAX_COUNT-1) ) );
  ASOS_STADE_CHECK(  me->mailbox_init_counter < 1  );
  unsigned short data_size = _mailbox_message_size((MAILBOX_QUEUE_DATA_TYPE*)message);
  ASOS_STADE_CHECK(  data_size < 1  );
   
 
  if(me->mailboxes_keys[mailbox_index_name][0] == key_way){
  
      if(ASOS_MAILBOX_SUCCESS == asos_xmailbox_write(&me->mailboxes[ (int)mailbox_index_name ] , 
                                                      ASOS_MAILBOX_WAY_1_SELECT , 
                                                      (MAILBOX_QUEUE_DATA_TYPE* )message ,
                                                      (unsigned short) data_size) )
      {
        return ASOS_SUCCESS;
      }
  }
  else if(me->mailboxes_keys[mailbox_index_name][1] == key_way){
  
      if(ASOS_MAILBOX_SUCCESS == asos_xmailbox_write(&me->mailboxes[ (int)mailbox_index_name ] , 
                                                      ASOS_MAILBOX_WAY_2_SELECT , 
                                                      (MAILBOX_QUEUE_DATA_TYPE* )message ,
                                                      (unsigned short) data_size) )
      {
        return ASOS_SUCCESS;
      } 
  }
  
  return ASOS_FAIL;
}








asos_fout_t
asos_mailbox_read  (asos_object_t * me ,
                    asos_device_tree_mailbox_call_e   mailbox_index_name,  
                    unsigned char key_way ,       /*** select which line reading ***/
                    MAILBOX_QUEUE_DATA_TYPE* out_data , 
                    unsigned short out_data_size)
{
  ASOS_OBJ_CHECK(me); 
  ASOS_OBJ_CHECK(out_data); 
  ASOS_STADE_CHECK( ( (int)(mailbox_index_name) > (int)(ASOS_MAILBOXES_MAX_COUNT-1) ) );
  ASOS_STADE_CHECK( ( me->mailbox_init_counter > (int)(ASOS_MAILBOXES_MAX_COUNT-1) ) );
  ASOS_STADE_CHECK(  me->mailbox_init_counter < 1  );
  ASOS_STADE_CHECK(  out_data_size < 1  );
   
  if(me->mailboxes_keys[mailbox_index_name][0] == key_way){
  
      if(ASOS_MAILBOX_DATA_READY == asos_xmailbox_read(&me->mailboxes[ (int)mailbox_index_name ] , 
                                                      ASOS_MAILBOX_WAY_1_SELECT , 
                                                      (MAILBOX_QUEUE_DATA_TYPE* )out_data ,
                                                      (unsigned short) out_data_size) )
      {
        return ASOS_SUCCESS;
      }

  
  }
  else if(me->mailboxes_keys[mailbox_index_name][1] == key_way){
  
      if(ASOS_MAILBOX_DATA_READY == asos_xmailbox_read(&me->mailboxes[ (int)mailbox_index_name ] , 
                                                      ASOS_MAILBOX_WAY_2_SELECT , 
                                                      (MAILBOX_QUEUE_DATA_TYPE* )out_data ,
                                                      (unsigned short) out_data_size) )
      {
        return ASOS_SUCCESS;
      } 
  } 
  
  return ASOS_FAIL;
}






asos_fout_t
asos_mailbox_fresh (asos_object_t * me ,
                    asos_device_tree_mailbox_call_e   mailbox_index_name,  
                    unsigned char key_way )
{
  ASOS_OBJ_CHECK(me);   
  ASOS_STADE_CHECK( ( (int)(mailbox_index_name) > (int)(ASOS_MAILBOXES_MAX_COUNT-1) ) );
 
  if(me->mailboxes_keys[mailbox_index_name][0] == key_way){
    
      if(ASOS_MAILBOX_SUCCESS == asos_xmailbox_fresh(&me->mailboxes[ (int)mailbox_index_name ] , 
                                                      ASOS_MAILBOX_WAY_1_SELECT ) )
      {
        return ASOS_SUCCESS;
      } 
  
  }
  else if(me->mailboxes_keys[mailbox_index_name][1] == key_way){
  
      if(ASOS_MAILBOX_SUCCESS == asos_xmailbox_fresh(&me->mailboxes[ (int)mailbox_index_name ] , 
                                                      ASOS_MAILBOX_WAY_2_SELECT ) )
      {
        return ASOS_SUCCESS;
      } 
  }
  
  
  return ASOS_FAIL;
}




  
            
