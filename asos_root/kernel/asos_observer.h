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

#ifndef ASOS_OBSERVER_H
#define ASOS_OBSERVER_H

#ifdef __cplusplus
	extern "C"
	{
#endif
	  
     #include "../configs/asos_config.h"

      /************************************************/
      /************** OBSERVER CLASS **********************************/
      /************************************************/

      /** @config : share data type declerate  **/
      /**typedef unsigned char observer_data_type_1;
      typedef unsigned int  observer_data_type_2;
      typedef float         observer_data_type_3;
      #define OBS_DATA  observer_data_type_1 **/
      /** CAN BE ADD MORE **/

      /** @config : share data define **/
      #define OBS_DATA  CALLBACK_DATA_TYPE  /** now in config file , observer_data_type_1**/

      /** @config : importand for log and debug **/
      #define OBSERVER_SPACIAL_OBJECT_NOTIFYS_ACTIVE  1





      /** @SPECÝAL EVENTS *********/
      typedef enum {
              OBSERVER_EVENT_FAIL,
              OBSERVER_EVENT_FIRST_LOAD,
              OBSERVER_EVENT_SUBJECT_ADDED,
              OBSERVER_EVENT_SUBJECT_DELETED,
              OBSERVER_EVENT_DELETED,
              OBSERVER_EVENT_EMPTY,
              OBSERVER_EVENT_UPDATE,
              OBSERVER_EVENT_TIMEOUT,
              OBSERVER_EVENT_SAME_EVENT
              /*** CAN BE ADD MORE  ***/
      }observer_event_types;






      typedef enum {
              OBSERVER_FUNC_FAIL,
              OBSERVER_FUNC_SUCCESS
      }observer_func_out_types;




      typedef void (*observer_event_callback_fp)( void* obj , observer_event_types event, OBS_DATA data);


      typedef struct {

              /** @private  **/
              char                        _is_observation_active;

              observer_event_types        _last_event;

              observer_event_callback_fp  _e_calback;
              
              void *                      _obj;

      } observer_obj;



      /** API USER FUNCTIONS **/

      observer_func_out_types
              observer_create(observer_obj* me, void* obj ,observer_event_callback_fp my_callback);

      observer_func_out_types
              observer_update(observer_obj* me, observer_event_types event, OBS_DATA data);

      observer_func_out_types
              observer_stop_watch(observer_obj* me);

      observer_func_out_types
              observer_start_watch(observer_obj* me);

      observer_func_out_types
              observer_delete(observer_obj* me);







      /************************************************/
      /************** SUBJECT CLASS **********************************/
      /************************************************/



      /** @config : share data define **/
      #define SBJ_DATA  OBS_DATA 

      /** @config : memory container for observers , must be bigger than 2 **/
      #define SUBJECT_S_MAX_OBSERVER_COUNT      ASOS_CALLBACKS_OBSERVERS_MAX_COUNT




 

        typedef enum {
                SUBJECT_FUNC_FAIL,
                SUBJECT_FUNC_SUCCESS
        }subject_func_out_types;



        typedef struct {

                SBJ_DATA* data;

                observer_obj* observers[SUBJECT_S_MAX_OBSERVER_COUNT];

                unsigned int  observer_count;

                char          _is_created;

        } subject_obj;



        /** API USER FUNCTIONS **/

        subject_func_out_types
                subject_create(subject_obj* me, SBJ_DATA* subject_data);

        subject_func_out_types
                subject_add_observer(subject_obj* me, observer_obj* observer);

        subject_func_out_types
                subject_remove_observer(subject_obj* me, observer_obj* observer);

        subject_func_out_types
                subject_update_observers(subject_obj* me);

        subject_func_out_types
                subject_delete(subject_obj* me);


#ifdef __cplusplus
	}
#endif

#endif
