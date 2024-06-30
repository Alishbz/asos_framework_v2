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
#ifndef ASOS_STADE_MACHINE_H
#define ASOS_STADE_MACHINE_H

#ifdef __cplusplus
	extern "C"
	{
#endif
 
#include "../asos.h"


/** sm -> STADE MACHINE  **/         
          
#define ASOS_SM_IDLE_STADE_CASE_ID     0 
          
typedef struct  _asos_stade_t          asos_stade_t;
typedef struct  _asos_stade_machine_t  asos_stade_machine_t;
typedef unsigned short                 asos_sm_stade_id_type;


typedef enum {
  ASOS_SM_SUCCESS = 0,
  ASOS_SM_WAITING,
  ASOS_SM_FAIL
}asos_sm_fout_e;          
          

 
typedef asos_sm_stade_id_type (*asos_sm_task_fp_t)(  asos_stade_machine_t * const sm );
          


/** @superStadeObject **/
struct _asos_stade_t{
 
  /** @note: all case ids must be unique  "asos_list_t->node->data = stade_id"           **/
  
  asos_sm_stade_id_type   stade_id;      /** it must not be (ASOS_SM_IDLE_STADE_CASE_ID) **/

  asos_sm_task_fp_t       task;
  
  asos_list_node_t        node;

};
             
          
          
/** @superStadeMachineObject **/
struct _asos_stade_machine_t{
  
  asos_object_t *      os;            /** interitance of asos_os **/
    
  asos_list_t          stades_list;   /** first stade is idle stade **/
 
  asos_stade_t         idle_stade;
  
  asos_list_node_t     idle_node;
    
  asos_stade_t *       next_stade;
    
  unsigned int         timer_ms;
  
  unsigned int         timeout_value;
   
  asos_mbool           is_runing;
  
  asos_mbool           is_log_active;
};
              












/** @proAPIConstructorsFunctions *********************************/


/** @featuresOF -> asos_stade_machine_create
  - stade machine auto runing active, 
  - first runing stade is "idle stade" ,
  - stade machine loging auto active,  
**/
asos_sm_fout_e
asos_stade_machine_create( asos_stade_machine_t *     const me , 
                           asos_object_t        *     os,
                           asos_sm_task_fp_t          idle_task);


asos_sm_fout_e
asos_stade_machine_delete( asos_stade_machine_t *     const me);

 




/** @proProcessFunction ***************************************/



/** @superLoop **/
asos_sm_fout_e
asos_stade_machine_run( asos_stade_machine_t *     const me);


asos_sm_fout_e
asos_stade_machine_stop( asos_stade_machine_t *     const me);


asos_sm_fout_e
asos_stade_machine_loging_start( asos_stade_machine_t *     const me);

asos_sm_fout_e
asos_stade_machine_loging_stop( asos_stade_machine_t *     const me);



/** @timer_1_ms interrupt **/
/**  @REV -> created in OS 
asos_sm_fout_e
asos_stade_machine_timer_ms( asos_stade_machine_t *     const me);
**/


asos_sm_fout_e
asos_stade_machine_set_delay( asos_stade_machine_t *     const me,
                              unsigned int delay_ms );




asos_sm_fout_e
asos_stade_machine_add_stade( asos_stade_machine_t *     const me ,
                              asos_stade_t  *            const stade ,
                              asos_sm_stade_id_type      stade_id);


asos_sm_fout_e
asos_stade_machine_delete_stade( asos_stade_machine_t *     const me ,
                                 asos_sm_stade_id_type      stade_id);






#define ASOS_STADE_MACHINE_TABLE_ADD(sm_obj , stade_obj , stade_idx , stade_function )\
do{\
  (stade_obj)->stade_id = stade_idx;\
  (stade_obj)->task = stade_function; \
  asos_stade_machine_add_stade( sm_obj ,\
                                stade_obj,\
                                stade_idx);\
}while(0);



#ifdef __cplusplus
	}
#endif

#endif
