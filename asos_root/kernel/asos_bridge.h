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
#ifndef ASOS_BRIDGE_H
#define ASOS_BRIDGE_H

#include "../configs/asos_config.h"

#ifdef __cplusplus
extern "C"
{
#endif
	 

  
typedef struct {
  char way; // can be add queue
}asos_bridge_pure_t;

/*** client can access to server, server not access to client ***/
typedef asos_bridge_pure_t*  asos_bridge_server_way;
typedef asos_bridge_pure_t** asos_bridge_client_way;

typedef enum {
  ASOS_BRIDGE_FIRST_PILE,
  ASOS_BRIDGE_SECOND_PILE
}asos_bridge_type_t;


#define BRIDGE_OBJ_CREATE(bridge_shared_object_number)             \
         asos_bridge_pure_t asos_shared_area_a_##bridge_shared_object_number; \
         asos_bridge_pure_t asos_shared_area_b_##bridge_shared_object_number

#define BRIDGE_FIRST_PILE_CREATE(bridge_object_number)    \
         asos_bridge_server_way asos_tx_way_##bridge_object_number;  \
         asos_bridge_client_way asos_rx_way_##bridge_object_number;  \
         asos_bridge_type_t     asos_bridge_pile##bridge_object_number

#define BRIDGE_SECOND_PILE_CREATE(bridge_object_number)    \
         asos_bridge_server_way asos_rx_way_##bridge_object_number;   \
         asos_bridge_client_way asos_tx_way_##bridge_object_number;   \
         asos_bridge_type_t     asos_bridge_pile##bridge_object_number

#define BRIDGE_FIRST_PILE_INIT(bridge_object_number , my_obj_pointer)    \
        my_obj_pointer->asos_tx_way_##bridge_object_number = NULL;  \
        my_obj_pointer->asos_rx_way_##bridge_object_number = NULL;  \
        my_obj_pointer->asos_bridge_pile##bridge_object_number = ASOS_BRIDGE_FIRST_PILE

#define BRIDGE_SECOND_PILE_INIT(bridge_object_number , my_obj_pointer)    \
        my_obj_pointer->asos_tx_way_##bridge_object_number = NULL;  \
        my_obj_pointer->asos_rx_way_##bridge_object_number = NULL;  \
        my_obj_pointer->asos_bridge_pile##bridge_object_number = ASOS_BRIDGE_SECOND_PILE


#define BRIDGE_CONNECTION_ASSIGN(bridge_shared_object_number ,bridge_object_number  , first_obj , second_obj)             \
        {\
          first_obj.asos_tx_way_##bridge_object_number  = &asos_shared_area_a_##bridge_shared_object_number;     \
          second_obj.asos_rx_way_##bridge_object_number = &asos_shared_area_b_##bridge_shared_object_number;     \
          first_obj.asos_rx_way_##bridge_object_number =  &second_obj.asos_rx_way_##bridge_object_number; \
          second_obj.asos_tx_way_##bridge_object_number = &first_obj.asos_tx_way_##bridge_object_number;  \
        }while (0)


/** 
* "BRIDGE_GOTO_CROSS"
* 
Fcross Pointer Logic Creator : Ali Sahbaz 
ref:
CONTAINER_OF(ptr, type, member)    ((type *)((uintptr_t)(ptr) - offsetof(type, member)))

Can be manage bridge piles and it will find its own way, then reach the cross object
**/
#define BRIDGE_GOTO_CROSS(bridge_object_number , my_obj_pointer , cross_obj_type)  \
        ((cross_obj_type*)(((uintptr_t)(my_obj_pointer->asos_rx_way_##bridge_object_number) - offsetof(cross_obj_type, asos_rx_way_##bridge_object_number))* (my_obj_pointer->asos_bridge_pile##bridge_object_number == ASOS_BRIDGE_FIRST_PILE) +\
        ((uintptr_t)(my_obj_pointer->asos_tx_way_##bridge_object_number) - offsetof(cross_obj_type, asos_tx_way_##bridge_object_number)) * (my_obj_pointer->asos_bridge_pile##bridge_object_number == ASOS_BRIDGE_SECOND_PILE)));
  
  
  
#ifdef __cplusplus
}
#endif

#endif
