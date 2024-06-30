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
#ifndef ASOS_CONFIG_H
#define ASOS_CONFIG_H

#ifdef __cplusplus
	extern "C"
	{
#endif

#include <stdio.h> 
#include <stdlib.h> 
#include <stdio.h>  
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>       

          
#define ASOS_VERSION    "version_1.0.2" 
          
          
          
          

/******************************/          
/******************************/
/** ASOS PUBLIC USER CONFIGS *********/  

#define ASOS_TIMER_SUPPORT_MAX_COUNT              10  // +1 Asos_Task USED

#define ASOS_TASK_SUPPORT_MAX_COUNT               10  // +1 idle task?

#define ASOS_CALLBACKS_MAX_COUNT                  8   // config for max subject size

#define ASOS_CALLBACKS_OBSERVERS_MAX_COUNT        8   // config for every subject's has observer count size

#define ASOS_FUNCTION_CALL_MAX_COUNT              10
          
#define ASOS_MAILBOXES_MAX_COUNT                  6
          
#define ASOS_HEAP_SIZE                            (4*1000) // MUST BE SOLID OF 4
          
#define ASOS_LISTS_MAX_COUNT                      3
          
#define ASOS_OS_DEBUG_MODE                           // OR COMMENT LINE

#define ASOS_USER_PRINT_MODE                         // OR COMMENT LINE

#define ASOS_ASOS_TEST_TDD_MODE_ACTIVE               // OR COMMENT LINE

/******************************/
/******************************/








/******************************/
/******************************/
/** ASOS PRIVATE DEVELOPER CONFIGS *********/



/** round robin active pasive , if pasive then priory of task is meaningless , asos_task.h/.c**/
#define IS_ROUND_ROBIN_ACTIVE     1



/** share data TYPE for callbacks **/
    typedef unsigned char observer_data_type_1;
    typedef unsigned int  observer_data_type_2;
    typedef float         observer_data_type_3;
#define  CALLBACK_DATA_TYPE    observer_data_type_1

    
    
    
#define    ASOS_QUEUE_LOW_SIZE_STATIC_SIZE    0xFF
    
#define    ASOS_QUEUE_HIGH_SIZE_STATIC_SIZE   0x3FF
         
    
    
    
    
    
/******************************/
/******************************/

    
    
    
    
    
    
    
    
    
/** SOME DEVELOPER GLOBAL DEFINES **/   
              
typedef enum {
  ASOS_FALSE = 0,
  ASOS_TRUE,
}asos_mbool;  






/** @superParentStructCaller **/
#define ASOS_CONTAINER_OF(ptr, type, member)    ((type *)((uintptr_t)(ptr) - offsetof(type, member)))


#ifdef __cplusplus
	}
#endif

#endif
