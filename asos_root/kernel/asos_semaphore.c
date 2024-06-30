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
  
#include "asos_semaphore.h"


#define OBJ_CHECK(xme)       if(xme == NULL){ /** TODO add print **/ return ASOS_SEMAPHORE_FAIL; }

#define OS_MS_TIMEOUT_MAX    (unsigned int)( ASOS_TIMER_US_MAX_COUNT/1000 )  /** OS->asos_timer_us Cleared value **/

#define US_TO_MS(tm)         (unsigned int)(tm/1000)


 

asos_semaphore_fout_e
asos_samaphore_create(asos_semaphore_t* me , 
                      asos_object_t*    asos ,
                      unsigned int      load_count , 
                      unsigned int      load_limit)
{
  OBJ_CHECK(me);
  OBJ_CHECK(asos);
  
  me->count = load_count;
  me->limit = load_limit;
  me->os = asos;
  
  return ASOS_SEMAPHORE_SUCCESS;
}

asos_semaphore_fout_e
asos_samaphore_delete(asos_semaphore_t* me )
{
  OBJ_CHECK(me);
  return asos_samaphore_create(me , me->os , 0 , 0 );
}


 

/** 
if timeout 0 then there is no timeout !! like->SEMAPHORE_FOREVER_TIMEOUT
CHECK OUT: @ASOS_SEMAPHORE_TAKE_DONE and @ASOS_SEMAPHORE_TIMEOUT_ERROR  
**/
asos_semaphore_fout_e
asos_samaphore_wait(asos_semaphore_t* me, 
                    unsigned int timeout)
{
  OBJ_CHECK(me); 
  OBJ_CHECK(me->os); 
  
  unsigned int _time = 0;
  
  if( me->count ){
    me->count--; 
    me->timeout_record = 0;
    return ASOS_SEMAPHORE_TAKE_DONE;  
  }
  
  if(me->timeout_record == 0){
     me->timeout_record = me->os->asos_timer_us;
     return ASOS_SEMAPHORE_SUCCESS;
  } 
   
  if( US_TO_MS(me->os->asos_timer_us)  < me->timeout_record )
  {
     _time = US_TO_MS(me->os->asos_timer_us) + OS_MS_TIMEOUT_MAX; 
  } 
  else{
     _time = US_TO_MS(me->os->asos_timer_us);
  }
  
  /** timeout check **/
  if( (timeout != SEMAPHORE_FOREVER_TIMEOUT ) && 
      (( _time - me->timeout_record ) > timeout ) )
  { 
    /** @timeout **/ 
     return ASOS_SEMAPHORE_TIMEOUT_ERROR;
  }

  
  
  return ASOS_SEMAPHORE_SUCCESS;
}


 
 
 
/** CHECK OUT: @ASOS_SEMAPHORE_GIVE_DONE  **/
asos_semaphore_fout_e
asos_samaphore_post(asos_semaphore_t* me)
{
  OBJ_CHECK(me); 
  OBJ_CHECK(me->os); 
  
  if( me->count >= me->limit){
    me->count++; 
    return ASOS_SEMAPHORE_GIVE_DONE;  
  }
  
  return ASOS_SEMAPHORE_SUCCESS;
}







asos_semaphore_fout_e
asos_samaphore_reset(asos_semaphore_t* me)
{
  OBJ_CHECK(me); 
  return asos_samaphore_delete(me );
}




