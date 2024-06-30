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
#ifndef asos_blinky_test_H
#define asos_blinky_test_H

#ifdef __cplusplus
extern "C"
{
#endif


  
  
#include "../asos.h"

  
  
/** MAIN object declaration **/
typedef struct  blinky_obj blinky_obj_t;

/** TASK object must be "void*" for reach ASOS **/
typedef void* blinky_obj_p;

/** TRANSFORM void* to own object **/
static inline blinky_obj_t* blinky_obj_transform(blinky_obj_p me){ return (blinky_obj_t*)me;} 




/** @superAPPObject **/
struct blinky_obj{

  unsigned int time1;
  unsigned int time2;
  
};





/** @publicFunctions **/
 
ASOS_USER_FOUT 
blinky_create( blinky_obj_p  me , asos_task_t * os);

ASOS_USER_FOUT 
blinky_delete( blinky_obj_p  me );

ASOS_USER_FOUT 
blinky_task( blinky_obj_p    me , asos_task_t * os);



#ifdef __cplusplus
}
#endif

#endif
