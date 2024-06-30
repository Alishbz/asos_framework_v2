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
#ifndef ASOS_TASK_H
#define ASOS_TASK_H


#include "../configs/asos_config.h"

#ifdef __cplusplus
	extern "C"
	{
#endif
           
typedef enum {
  ASOS_USER_TASK_SUCCESS = 0,
  ASOS_USER_TASK_FAIL,
  ASOS_USER_TASK_TIMEOUT,
  ASOS_USER_TASK_KILL_ME,
  ASOS_USER_TASK_WAIT
}ASOS_USER_FOUT;
          
/** high prio is high priorty **/
typedef enum { 
  ASOS_TASK_PRIO_EMPTY = 0,
  ASOS_TASK_PRIO_1 = 1,
  ASOS_TASK_PRIO_2,
  ASOS_TASK_PRIO_3,
  ASOS_TASK_PRIO_4,
  ASOS_TASK_PRIO_5,
  ASOS_TASK_PRIO_6,
  ASOS_TASK_PRIO_7,
  ASOS_TASK_PRIO_8,
  ASOS_TASK_PRIO_9,
  ASOS_TASK_PRIO_10 
}asos_task_priority_t;

typedef struct _asos_task_t asos_task_t;
          
typedef ASOS_USER_FOUT (*asos_app_init_fp_t)(    void*me ,asos_task_t * os);
typedef ASOS_USER_FOUT (*asos_app_delete_fp_t)(  void*me );
typedef ASOS_USER_FOUT (*asos_app_process_fp_t)( void*me , asos_task_t * os ); 
typedef ASOS_USER_FOUT (*asos_idle_sleep_fp_t)(  asos_task_t * os );
 
/** @idleTask&SleepObject **/
typedef struct {  
  asos_idle_sleep_fp_t  task;
  asos_mbool            is_sleep_active;
  asos_idle_sleep_fp_t  sleep_on;
  asos_idle_sleep_fp_t  sleep_off; 
}asos_idle_task_t;
 


/** @taskConfigObject **/
typedef struct { 
  asos_task_priority_t prioty;
}asos_task_config_t;

/** @loadObject **/
typedef struct { 
  asos_task_config_t    config;
  /** app object **/
  void*app_obj;
  /** app function pointers **/
  asos_app_init_fp_t    app_init_fp;
  asos_app_delete_fp_t  app_delete_fp;
  asos_app_process_fp_t app_process_fp; 
  
  /** private **/
  unsigned int          app_wait_ms_counter;
  unsigned int          app_wait_ms_set_value;
#ifdef IS_ROUND_ROBIN_ACTIVE
   int          quantum_time;
#endif
}asos_task_load_t;
 
/** @processObject **/
struct _asos_task_t{ 
  
   asos_task_load_t tasks[ASOS_TASK_SUPPORT_MAX_COUNT];
   unsigned short   task_loaded_number;
   unsigned short   task_run_index;
    
   unsigned int     context_switch_ms_counter;
   unsigned int     context_switch_ms_set_value;   
#ifdef IS_ROUND_ROBIN_ACTIVE
   unsigned int     current_quantum_time; 
#endif
 
   
   asos_idle_task_t idle;
   asos_mbool       is_sleep_on;
};

typedef enum {
  ASOS_TASK_SUCCESS = 0,
  ASOS_TASK_FAIL,
  ASOS_TASK_TIMEOUT,
  ASOS_TASK_WAIT
}asos_task_fout_t;
          
/** @proFunctions **/




/***** main task class constructors ****/

asos_task_fout_t
asos_xtask_create(asos_task_t* me ,  unsigned int tasks_context_switch_ms_value );

asos_task_fout_t
asos_xtask_delete(asos_task_t* me);






/***** task app creator constructors  ****/

asos_task_fout_t
asos_xtask_app_create(asos_task_t* me , asos_task_load_t* new_app);

asos_task_fout_t
asos_xtask_app_delete(asos_task_t* me , asos_task_load_t* app_obj);







/***** task app process  ****/

asos_task_fout_t
asos_xtask_run_next_task(asos_task_t* me);

asos_task_fout_t
asos_xtask_init_all_tasks(asos_task_t* me);


asos_task_fout_t
asos_xtask_synchronous_wait_load(asos_task_t* me, void*app_obj , unsigned int wait_ms);






/******* idle & sleep API *******/

/** if sleep functions not NULL then sleep mode auto active **/
asos_task_fout_t
asos_xtask_idle_assign(asos_task_t* me , 
                       asos_idle_sleep_fp_t   task_f,
                       asos_idle_sleep_fp_t   sleep_on_f,
                       asos_idle_sleep_fp_t   sleep_off_f);

asos_task_fout_t
asos_xtask_idle_sleep_active(asos_task_t* me );

asos_task_fout_t
asos_xtask_idle_sleep_pasive(asos_task_t* me );




#ifdef __cplusplus
}
#endif

#endif
