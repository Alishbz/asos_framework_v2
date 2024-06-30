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
#ifndef ASOS_MUTEX_H
#define ASOS_MUTEX_H

#include "../asos.h"

#ifdef __cplusplus
extern "C"
{
#endif
  
#define MUTEX_FOREVER_TIMEOUT       (99991) 
  
typedef enum {
  ASOS_MUTEX_SUCCESS=0,
  ASOS_MUTEX_FAIL  ,
  ASOS_MUTEX_TIMEOUT_ERROR ,
  ASOS_MUTEX_LOCKED_DONE,
  ASOS_MUTEX_UNLOCKED_DONE
}asos_mutex_fout_e;
  
typedef enum {
  ASOS_UNDEF_LOCK=0,
  ASOS_UNLOCK,
  ASOS_LOCK
}asos_mutex_lock_e;
  

typedef struct {

  asos_object_t     *os;             /** using for find -> os->asos_timer_us **/
  
  asos_mutex_lock_e  lock_stade;
    
  unsigned int       lock_count;

  unsigned int       timeout_record;   /** MUTEX_FOREVER_TIMEOUT for ignore **/
  
}asos_mutex_t;
  



 /** @proConstructorsFunctions **/


asos_mutex_fout_e
asos_mutex_create(asos_mutex_t* me, asos_object_t* asos);

asos_mutex_fout_e
asos_mutex_delete(asos_mutex_t* me );





/** @userFunctions **/


asos_mutex_fout_e
asos_mutex_lock(asos_mutex_t* me, unsigned int timeout);

asos_mutex_fout_e
asos_mutex_unlock(asos_mutex_t* me );




 
#ifdef __cplusplus
}
#endif

#endif
