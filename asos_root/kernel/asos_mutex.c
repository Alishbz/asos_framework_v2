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
 
#include "asos_mutex.h"


#define OBJ_CHECK(xme)       if(xme == NULL){ /** TODO add print **/ return ASOS_MUTEX_FAIL; }

#define OS_MS_TIMEOUT_MAX    (unsigned int)( ASOS_TIMER_US_MAX_COUNT/1000 )  /** OS->asos_timer_us Cleared value **/

#define US_TO_MS(tm)         (unsigned int)(tm/1000)


asos_mutex_fout_e
asos_mutex_create(asos_mutex_t* me, asos_object_t* asos)
{
  OBJ_CHECK(me);
  OBJ_CHECK(asos);
  
  me->os = asos;
  me->lock_stade = ASOS_UNDEF_LOCK;
  me->lock_count = 0;
  me->timeout_record = 0;
  
  return ASOS_MUTEX_SUCCESS;
}

asos_mutex_fout_e
asos_mutex_delete(asos_mutex_t* me )
{
  return asos_mutex_create(me , me->os);
}


/** CHECK OUT : @ASOS_MUTEX_LOCKED_DONE and @ASOS_MUTEX_TIMEOUT_ERROR **/
asos_mutex_fout_e
asos_mutex_lock(asos_mutex_t* me, unsigned int timeout)
{
  OBJ_CHECK(me); 
  OBJ_CHECK(me->os); 
   
  asos_mutex_fout_e out = ASOS_MUTEX_FAIL;
  
  unsigned int _time = 0;
  
  if(me->lock_stade == ASOS_UNLOCK){
    
    me->timeout_record = US_TO_MS(me->os->asos_timer_us);  
    me->lock_stade = ASOS_LOCK;
    me->lock_count++;
    out = ASOS_MUTEX_LOCKED_DONE;
  }
  else if(me->lock_stade == ASOS_LOCK){  
    
    out = ASOS_MUTEX_SUCCESS;
    
    if( US_TO_MS(me->os->asos_timer_us)  < me->timeout_record )
    {
       _time = US_TO_MS(me->os->asos_timer_us) + OS_MS_TIMEOUT_MAX; 
    } 
    else{
       _time = US_TO_MS(me->os->asos_timer_us);
    }
    
    /** timeout check **/
    if( (timeout != MUTEX_FOREVER_TIMEOUT ) && 
        (( _time - me->timeout_record ) > timeout ) )
    { 
      /** @timeout **/
       asos_mutex_unlock( me );
       
       out = ASOS_MUTEX_TIMEOUT_ERROR;
    }
  }
  
  return out;

}


/** CHECK OUT : @ASOS_MUTEX_UNLOCKED_DONE **/
asos_mutex_fout_e
asos_mutex_unlock(asos_mutex_t* me )
{
  OBJ_CHECK(me); 
  OBJ_CHECK(me->os); 
  
  if( (me->lock_stade == ASOS_LOCK) &&
      (me->lock_count) ){ 
    
    me->lock_count--;
    
    if(me->lock_count == 0){
      
      me->timeout_record = 0;  
      
      me->lock_stade = ASOS_UNLOCK;
      
      return ASOS_MUTEX_UNLOCKED_DONE;
    }
  }
  
  return ASOS_MUTEX_FAIL;
}

