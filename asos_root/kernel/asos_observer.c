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
 
#include "asos_observer.h"


/************************************************/
/************** OBSERVER CLASS **********************************/
/************************************************/


observer_func_out_types
observer_create(observer_obj* me, void* obj ,observer_event_callback_fp my_callback)
{
	if (me == NULL || my_callback == NULL) {
		return OBSERVER_FUNC_FAIL;
	}
	me->_e_calback = my_callback;

        me->_obj = obj;
        
	me->_is_observation_active = 1;

	me->_last_event = OBSERVER_EVENT_EMPTY;

#ifdef OBSERVER_SPACIAL_OBJECT_NOTIFYS_ACTIVE
	/** first call for notify load success **/
    observer_update(me, OBSERVER_EVENT_FIRST_LOAD, 0);
#endif

	/** @ATTANTION, AFTER CREATION YOU MUST CALL -> observer_start_watch ***/
	observer_stop_watch(me);

	return OBSERVER_FUNC_SUCCESS;
}



observer_func_out_types
observer_update(observer_obj* me, observer_event_types event, OBS_DATA data)
{
	if (me == NULL || event == OBSERVER_EVENT_FAIL || me->_is_observation_active == 0) {
		return OBSERVER_FUNC_FAIL;
	}

	/*** @callback for MASTER App  ***/
	me->_e_calback(me->_obj , event, data); 

	me->_last_event = event;

	return OBSERVER_FUNC_SUCCESS;
}

observer_func_out_types
observer_stop_watch(observer_obj* me)
{
	if (me == NULL) {
		return OBSERVER_FUNC_FAIL;
	}

	me->_is_observation_active = 0;

	return OBSERVER_FUNC_SUCCESS;
}

observer_func_out_types
observer_start_watch(observer_obj* me)
{
	if (me == NULL) {
		return OBSERVER_FUNC_FAIL;
	}

	me->_is_observation_active = 1;

	return OBSERVER_FUNC_SUCCESS;
}

observer_func_out_types
observer_delete(observer_obj* me)
{
	if (me == NULL) {
		return OBSERVER_FUNC_FAIL;
	}

#ifdef OBSERVER_SPACIAL_OBJECT_NOTIFYS_ACTIVE  
	/** first call for notify load success **/
    observer_update(me, OBSERVER_EVENT_DELETED, 0);
#endif

	observer_stop_watch(me);

	me->_e_calback = NULL;
        
        me->_obj = NULL;

	me->_last_event = OBSERVER_EVENT_FAIL;

	return OBSERVER_FUNC_SUCCESS;
}










/************************************************/
/************** SUBJECT CLASS **********************************/
/************************************************/


subject_func_out_types
subject_create(subject_obj* me, SBJ_DATA* subject_data)
{
	if (me == NULL) {
		return SUBJECT_FUNC_FAIL;
	}

	me->data = subject_data;

	int i = 0;

	for (i = 0; i < SUBJECT_S_MAX_OBSERVER_COUNT; i++) {
		me->observers[i] = NULL;
	}

	me->_is_created = 1;

	me->observer_count = 0;

	return SUBJECT_FUNC_SUCCESS;
}

subject_func_out_types
subject_add_observer(subject_obj* me, observer_obj* observer)
{
    if (me == NULL || observer == NULL || !me->_is_created || me->observer_count > (SUBJECT_S_MAX_OBSERVER_COUNT - 2)) { /** ; can 2->1 ? **/
		return SUBJECT_FUNC_FAIL;
	}

	me->observers[me->observer_count] = observer;

#ifdef OBSERVER_SPACIAL_OBJECT_NOTIFYS_ACTIVE
    if(me->data){
        unsigned char hold = me->data[0];
        /** first call for notify load success **/
        observer_update(me->observers[me->observer_count], OBSERVER_EVENT_SUBJECT_ADDED,  hold);
    }
#endif	  

	me->observer_count++;

	return SUBJECT_FUNC_SUCCESS;
}

subject_func_out_types
subject_remove_observer(subject_obj* me, observer_obj* observer)
{
	if (me == NULL || observer == NULL || !me->_is_created) {
		return SUBJECT_FUNC_FAIL;
	}

	int i = 0, _is_delete_success = 0;

	for (i = 0; i < SUBJECT_S_MAX_OBSERVER_COUNT; i++) {
		if (_is_delete_success && i > 0) {
			if (me->observers[i] == NULL) {
				break;
			}

			/** slide observers array **/
			me->observers[i - 1] = me->observers[i];
			me->observers[i] = NULL;
		}
		else if (me->observers[i] == observer) {

#ifdef OBSERVER_SPACIAL_OBJECT_NOTIFYS_ACTIVE
			/** first call for notify load success **/
			observer_update(me->observers[i], OBSERVER_EVENT_SUBJECT_DELETED, *me->data);
#endif	

			me->observers[i] = NULL;

			me->observer_count--;

			_is_delete_success = 1;
		}
	}

	if (_is_delete_success) {
		return SUBJECT_FUNC_SUCCESS;
	}

	/** observer not found **/
	return SUBJECT_FUNC_FAIL;
}




subject_func_out_types
subject_update_observers(subject_obj* me)
{
	if (me == NULL || !me->_is_created) {
		return SUBJECT_FUNC_FAIL;
	}

	int i = 0;

	for (i = 0; i < me->observer_count; i++) {
		if (me->observers[i]) {
			observer_update(me->observers[i], OBSERVER_EVENT_UPDATE, *me->data);
		}
	}

	return SUBJECT_FUNC_SUCCESS;
}


subject_func_out_types
subject_delete(subject_obj* me)
{
	if (me == NULL) {
		return SUBJECT_FUNC_FAIL;
	}

	me->data = NULL;

	int i = 0;

	for (i = 0; i < SUBJECT_S_MAX_OBSERVER_COUNT; i++) {
		me->observers[i] = NULL;
	}

	me->_is_created = 0;

	me->observer_count = 0;

	return SUBJECT_FUNC_SUCCESS;
}
