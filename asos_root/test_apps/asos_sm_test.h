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
#ifndef asos_sm_test_H
#define asos_sm_test_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "../asos.h"

  
#define STADES_MAX_COUNT   3
  
  
  
  
typedef struct {
  
  asos_task_t * os;   /** Developer can hold own task object then reach his OS  **/
  
  asos_stade_machine_t  sm; 
  
  asos_stade_t stades[STADES_MAX_COUNT];
  
  asos_device_tree_function_call_u * led_on , * led_off;
  
}sm_t;




ASOS_USER_FOUT
sm_create( void * me , asos_task_t * os );
 
ASOS_USER_FOUT
sm_process( void * me , asos_task_t * os);






#ifdef __cplusplus
}
#endif

#endif
