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
#ifndef ASOS_DEVICE_TREE_H
#define ASOS_DEVICE_TREE_H

#ifdef __cplusplus
extern "C"
{
#endif
	 

  
/***

 @description
   - hardware functions and interrupts must be handle in Middeleware of related modul driver, and this topic is differant from our "device tree"
   - our device tree is just an @option for literally to separate harware software and app software
   - USER can direct access from "asos_device_tree"
**/

  

/******************************/
/** ASOS INTERRUPT VECTOR USER TABLE *********/
/** @related by -> asos_config -> ASOS_CALLBACKS_OBSERVERS_MAX_COUNT **/
typedef enum {
  
  USER_IN_1_INT = 0,// ASOS_DEV_INT_1    // CHANGED
  USER_UART1_INT, // ASOS_DEV_INT_2   // CHANGED
  
  ASOS_DEV_INT_3, // ASOS_DEV_INT_3
  ASOS_DEV_INT_4, // ASOS_DEV_INT_4
  ASOS_DEV_INT_5, // ASOS_DEV_INT_5
  ASOS_DEV_INT_6, // ASOS_DEV_INT_6
  ASOS_DEV_INT_7, // ASOS_DEV_INT_7
  ASOS_DEV_INT_8, // ASOS_DEV_INT_8
  ASOS_DEV_INT_9, // ASOS_DEV_INT_9
  ASOS_DEV_INT_10, // ASOS_DEV_INT_10
  ASOS_DEV_INT_11, // ASOS_DEV_INT_11
  ASOS_DEV_INT_12, // ASOS_DEV_INT_12
  ASOS_DEV_INT_13, // ASOS_DEV_INT_13
  /** changeable and addable by @user */
}asos_device_tree_interrupt_vector_e;/** using index for asos->callbacks , -> @ASOS_CALLBACKS_MAX_COUNT    max value***/
 














/***************************************************************************/








/** ASOS FUNCTION CALL USER TABLE *********/

typedef enum { 
  
  USER_LED_1_ON = 0,// ASOS_FCALL_1     // CHANGED
  USER_LED_1_OFF,// ASOS_FCALL_2    // CHANGED
  UART_1_SENDER,  // ASOS_FCALL_3   // CHANGED
  
  ASOS_FCALL_4,  // ASOS_FCALL_4
  ASOS_FCALL_5,  // ASOS_FCALL_5
  ASOS_FCALL_6,  // ASOS_FCALL_6
  ASOS_FCALL_7,  // ASOS_FCALL_7
  ASOS_FCALL_8,  // ASOS_FCALL_8
  ASOS_FCALL_9,  // ASOS_FCALL_9
  ASOS_FCALL_10, // ASOS_FCALL_10
  ASOS_FCALL_11, // ASOS_FCALL_11
  ASOS_FCALL_12, // ASOS_FCALL_12
  ASOS_FCALL_13, // ASOS_FCALL_13
  /** changeable and addable by @user */
}asos_device_tree_function_call_e;   /** -> @ASOS_FUNCTION_CALL_MAX_COUNT    max value ****/



/** changeable and addable by @user */

typedef void (*asos_dt_fp_t_1)( void);
typedef void (*asos_dt_fp_t_2)( void*me );
typedef int  (*asos_dt_fp_t_3)( int data );
typedef void (*asos_dt_fp_t_4)( const char* data , unsigned short size );
  
/**
  asos_device_tree_function_call_u: table for function call, name is important
   @defineFP     |   @retrunStade  |  @inputStade                           
   @importandNote: this using methot in "asos.h" to "asos_device_tree_function_call_u" is suitable for "COMMAND DESIGN PATTERN" ****/
typedef union{
    asos_dt_fp_t_1   call_empty_return_and_input;
    asos_dt_fp_t_2   call_empty_return_void_pointer_input; 
    asos_dt_fp_t_3   call_int_return_int_input;
    asos_dt_fp_t_4   call_std_sender;
    /** add your types **/
}asos_device_tree_function_call_u;


/******************************/
/******************************/
















/***************************************************************************/








/** ASOS MAILBOX NAME *********/

typedef enum { 
  
  USB_TO_BLINKY_MAILBOX = 0,// ASOS_MAILBOX_1     // CHANGED
  
  ASOS_MAILBOX_2,   // ASOS_MAILBOX_2   
  ASOS_MAILBOX_3,  // ASOS_MAILBOX_3
  ASOS_MAILBOX_4,  // ASOS_MAILBOX_4
  ASOS_MAILBOX_5,  // ASOS_MAILBOX_5
  ASOS_MAILBOX_6,  // ASOS_MAILBOX_6
  ASOS_MAILBOX_7,  // ASOS_MAILBOX_7
  ASOS_MAILBOX_8,  // ASOS_MAILBOX_8
  ASOS_MAILBOX_9,  // ASOS_MAILBOX_9
  ASOS_MAILBOX_10, // ASOS_MAILBOX_10
  ASOS_MAILBOX_11, // ASOS_MAILBOX_11
  ASOS_MAILBOX_12, // ASOS_MAILBOX_12
  ASOS_MAILBOX_13, // ASOS_MAILBOX_13
  /** changeable and addable by @user */
}asos_device_tree_mailbox_call_e;     /** -> indexes of list @ASOS_MAILBOXES_MAX_COUNT    max value ****/
















/** ASOS LISTS NAME *****************/

typedef enum { 
  
  TEST_APP_LIST = 0,// ASOS_LIST_1     // CHANGED
  
  ASOS_LIST_2,  // ASOS_LIST_2   
  ASOS_LIST_3,  // ASOS_LIST_3
  ASOS_LIST_4,  // ASOS_LIST_4
  ASOS_LIST_5,  // ASOS_LIST_5
  ASOS_LIST_6,  // ASOS_LIST_6
  ASOS_LIST_7,  // ASOS_LIST_7
  ASOS_LIST_8,  // ASOS_LIST_8
  ASOS_LIST_9,  // ASOS_LIST_9
  ASOS_LIST_10, // ASOS_LIST_10
  ASOS_LIST_11, // ASOS_LIST_11
  ASOS_LIST_12, // ASOS_LIST_12
  ASOS_LIST_13  // ASOS_LIST_13
  /** changeable and addable by @user */
}asos_device_tree_list_call_e;     /** -> @ASOS_LISTS_MAX_COUNT    max value ****/



#ifdef __cplusplus
	}
#endif

#endif
