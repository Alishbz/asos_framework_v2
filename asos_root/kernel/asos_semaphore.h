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
#ifndef ASOS_SEMAPHORE_H
#define ASOS_SEMAPHORE_H

#include "../asos.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define SEMAPHORE_FOREVER_TIMEOUT       (99991) 

    
typedef enum {
  ASOS_SEMAPHORE_SUCCESS=0,
  ASOS_SEMAPHORE_FAIL,
  ASOS_SEMAPHORE_TIMEOUT_ERROR,
  ASOS_SEMAPHORE_GIVE_DONE,
  ASOS_SEMAPHORE_TAKE_DONE
}asos_semaphore_fout_e;
  

typedef struct {

  asos_object_t         *os;            /** using for find -> os->asos_timer_us **/
  
  unsigned int          count;
  
  unsigned int          limit;

  unsigned int          timeout_record;   /** MUTEX_FOREVER_TIMEOUT for ignore **/
  
}asos_semaphore_t;



 /** @proConstructorsFunctions **/


asos_semaphore_fout_e
asos_samaphore_create(asos_semaphore_t* me , 
                      asos_object_t*    asos ,
                      unsigned int      load_count , 
                      unsigned int      load_limit);

asos_semaphore_fout_e
asos_samaphore_delete(asos_semaphore_t* me );





/** @userFunctions **/


asos_semaphore_fout_e
asos_samaphore_wait(asos_semaphore_t* me, 
                    unsigned int timeout);


asos_semaphore_fout_e
asos_samaphore_post(asos_semaphore_t* me);


asos_semaphore_fout_e
asos_samaphore_reset(asos_semaphore_t* me);




#ifdef __cplusplus
}
#endif

#endif
