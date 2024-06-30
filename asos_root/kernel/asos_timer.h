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
#ifndef ASOS_TIMER_H
#define ASOS_TIMER_H

#include "../configs/asos_config.h"

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct _asos_timer_t asos_timer_t;

typedef void (*asos_timer_callback_fp_t)( void*me , asos_timer_t * os );
  


typedef struct{ 
  
   void*app_obj;
  
   asos_timer_callback_fp_t callback;
   
   unsigned int             config_ms;
    
   float                    process_counter;
   
}asos_timer_unit_t;




struct _asos_timer_t{ 
  
   asos_timer_unit_t timers[ASOS_TIMER_SUPPORT_MAX_COUNT];
   unsigned short    timer_loaded_number;
   unsigned short    timer_run_index;
   
   
   unsigned int      timer_process_us_val; 
};










typedef enum {
  ASOS_TIMER_SUCCESS = 0,
  ASOS_TIMER_FAIL,
  ASOS_TIMER_TIMEOUT,
  ASOS_TIMER_WAIT
}asos_timer_fout_t;
         



/** @proFunctions **/


/***** main timer class constructors ****/

asos_timer_fout_t
asos_xtimer_create(asos_timer_t* me , unsigned int timer_us_val);

asos_timer_fout_t
asos_xtimer_delete(asos_timer_t* me);



/***** timer creator constructors  ****/

asos_timer_fout_t
asos_xtimer_unit_create(asos_timer_t* me , asos_timer_unit_t* timer_obj);

asos_timer_fout_t
asos_xtimer_unit_delete(asos_timer_t* me , asos_timer_unit_t* timer_obj);



/***** timer process  ****/

asos_timer_fout_t
asos_xtimer_process_run(asos_timer_t* me );



#ifdef __cplusplus
}
#endif

#endif
