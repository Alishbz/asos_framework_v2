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

#include "asos_sm_test.h"

typedef enum{
  IDLE_TASK = ASOS_SM_IDLE_STADE_CASE_ID,
  TEST_STEP_1,
  TEST_STEP_2,
  TEST_STEP_3
}_sm_stade_ids;
  

static asos_sm_stade_id_type _idle_task_step(  asos_stade_machine_t * const sm )
{
  sm_t *me = ASOS_CONTAINER_OF(sm, sm_t, sm)   ;
  
  if( me->led_on ){
    me->led_on->call_empty_return_and_input();
  }
  
  asos_stade_machine_set_delay( sm, 1000 );
  
  return TEST_STEP_1;
}

static asos_sm_stade_id_type _task_step_1(  asos_stade_machine_t * const sm )
{
  sm_t *me = ASOS_CONTAINER_OF(sm, sm_t, sm)   ;
  
  if( me->led_on ){
    me->led_off->call_empty_return_and_input();
  }
  
  asos_stade_machine_set_delay( sm, 1000 );
 
  return TEST_STEP_2;
}

static asos_sm_stade_id_type _task_step_2(  asos_stade_machine_t * const sm )
{
  sm_t *me = ASOS_CONTAINER_OF(sm, sm_t, sm)   ;
  
  if( me->led_on ){
    me->led_on->call_empty_return_and_input();
  }
  
  asos_stade_machine_set_delay( sm, 3000 );
    
  return TEST_STEP_3;
}

static asos_sm_stade_id_type _task_step_3(  asos_stade_machine_t * const sm )
{
  sm_t *me = ASOS_CONTAINER_OF(sm, sm_t, sm)   ;
  
  if( me->led_on ){
    me->led_off->call_empty_return_and_input();
  }
  
  asos_stade_machine_set_delay( sm, 3000 );
 
  return IDLE_TASK;
}
          



ASOS_USER_FOUT
sm_create( void * me , asos_task_t * os)
{
  sm_t * _me = (sm_t*)me;
  
  asos_object_t *_os = ASOS_TASK_TO_OS(os);
   
  _me->os = os;
    
  _me->led_on  =  asos_fcall_get(_os,USER_LED_1_ON); 

  _me->led_off =  asos_fcall_get(_os,USER_LED_1_OFF); 

  
  /*** stade machine load *****/
  
  asos_stade_machine_create( &_me->sm,  _os,  &_idle_task_step);
  
  ASOS_STADE_MACHINE_TABLE_ADD(&_me->sm , &_me->stades[0] , TEST_STEP_1 , &_task_step_1 );
  ASOS_STADE_MACHINE_TABLE_ADD(&_me->sm , &_me->stades[1] , TEST_STEP_2 , &_task_step_2 );
  ASOS_STADE_MACHINE_TABLE_ADD(&_me->sm , &_me->stades[2] , TEST_STEP_3 , &_task_step_3 );
 
  return ASOS_USER_TASK_SUCCESS;
}

ASOS_USER_FOUT
sm_process( void * me , asos_task_t * os)
{
  sm_t * _me = (sm_t*)me;
  
  asos_object_t *_os = ASOS_TASK_TO_OS(os); 
    
  asos_stade_machine_run( &_me->sm );
   
  asos_app_process_wait_ms( _os , me , 1 );  
  
  return ASOS_USER_TASK_SUCCESS;
}


 






















/**

Note:

  ASOS_STADE_MACHINE_TABLE_ADD(&_me->sm , &_me->stades[0] , TEST_STEP_1 , &_task_step_1 );
  ASOS_STADE_MACHINE_TABLE_ADD(&_me->sm , &_me->stades[1] , TEST_STEP_2 , &_task_step_2 );
  ASOS_STADE_MACHINE_TABLE_ADD(&_me->sm , &_me->stades[2] , TEST_STEP_3 , &_task_step_3 );

equel to >

  _me->stades[0].stade_id = TEST_STEP_1;    
  _me->stades[0].task = &_task_step_1; 
  asos_stade_machine_add_stade( &_me->sm ,
                                &_me->stades[0],
                                TEST_STEP_1);
  
  
  _me->stades[1].stade_id = TEST_STEP_2;
  _me->stades[1].task = &_task_step_2; 
  asos_stade_machine_add_stade( &_me->sm ,
                                &_me->stades[1],
                                TEST_STEP_2);


  _me->stades[2].stade_id = TEST_STEP_3;
  _me->stades[2].task = &_task_step_3; 
  asos_stade_machine_add_stade( &_me->sm ,
                                &_me->stades[2],
                                TEST_STEP_3);
**/
