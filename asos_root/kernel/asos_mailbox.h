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
#ifndef ASOS_MAILBOX_H
#define ASOS_MAILBOX_H

#include "asos_queue.h"

#include "../configs/asos_config.h"

#ifdef __cplusplus
extern "C"
{
#endif
 

/** ASOS_MAILBOX_LOW_ALLOCATION_SELECT: @DEVELOPER SELECT CONFIG FOR MEMORY **/
    
#define ASOS_MAILBOX_LOW_ALLOCATION_SELECT  /** or COMMENT **/
   
#ifdef ASOS_MAILBOX_LOW_ALLOCATION_SELECT
  #define MAILBOX_QUEUE_OBJ_TYPE                        asos_queue_str 
  #define MAILBOX_QUEUE_DATA_TYPE                       asos_q_type 
#else
  #define MAILBOX_QUEUE_OBJ_TYPE                        asos_l_queue_str  
  #define MAILBOX_QUEUE_DATA_TYPE                       asos_l_q_type 
#endif   
  
  


typedef enum {
  ASOS_MAILBOX_FREE_STYLE = 0, /** there is no protochol **/
/**
  DESC @ASOS_MAILBOX_ASOS_PROTOCHOL:
  header   : '> '
  end byte : '\n'
    ex;
        > MSG .. int ..\n          ( it is for rx and tx both )
***/
  ASOS_MAILBOX_ASOS_PROTOCHOL,
  // ADD MORE
}asos_mailbox_protochol_e;
         

  
/** @superObj  **/
typedef struct{ 
   
 MAILBOX_QUEUE_OBJ_TYPE             way_1;
 
 MAILBOX_QUEUE_OBJ_TYPE             way_2;
  
 asos_mailbox_protochol_e   type;
   
 asos_mbool                 is_init;
 
}asos_mailbox_t;


 
   
typedef enum {
  ASOS_MAILBOX_SUCCESS = 0,
  ASOS_MAILBOX_FAIL,
  ASOS_MAILBOX_TIMEOUT,
  ASOS_MAILBOX_WAIT,
  ASOS_MAILBOX_DATA_READY
}asos_mailbox_fout_e;



typedef enum {
  ASOS_MAILBOX_WAY_1_SELECT,
  ASOS_MAILBOX_WAY_2_SELECT
}asos_mailbox_way_e;



/** @proFunctions **/


/***** main class constructors functions ****/

asos_mailbox_fout_e
asos_xmailbox_create(asos_mailbox_t* me , asos_mailbox_protochol_e type);

asos_mailbox_fout_e
asos_xmailbox_delete(asos_mailbox_t* me );



/***** process functions  ****/
 

asos_mailbox_fout_e
asos_xmailbox_write(asos_mailbox_t* me , asos_mailbox_way_e way , MAILBOX_QUEUE_DATA_TYPE* data , unsigned short data_size);

asos_mailbox_fout_e
asos_xmailbox_read(asos_mailbox_t* me , asos_mailbox_way_e way , MAILBOX_QUEUE_DATA_TYPE* out_buf , unsigned short out_buf_size);

asos_mailbox_fout_e
asos_xmailbox_fresh(asos_mailbox_t* me , asos_mailbox_way_e way  );




#ifdef __cplusplus
}
#endif

#endif
