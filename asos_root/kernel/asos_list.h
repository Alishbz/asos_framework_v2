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
#ifndef ASOS_LIST_H
#define ASOS_LIST_H

#include "../configs/asos_config.h"

#ifdef __cplusplus
extern "C"
{
#endif
 
  
/** @userCanChange that type for contain DATA and Sorted  **/
typedef unsigned int         list_data_type;
#define ASOS_LIST_DATA_MAX   (0xFFFFFFFF)   /** when mode: ASOS_LIST_MODE_REV_SORTED, converted auro @zero(0) **/

/**  @example:
typedef unsigned short        list_data_type;  
#define ASOS_LIST_DATA_MAX    (0xFFFF)
**/



typedef enum {
  ASOS_LIST_SUCCESS = 0,
  ASOS_LIST_FAIL
}asos_list_fout_e;


typedef enum {
  ASOS_LIST_STADE_DELETE = 0,
  ASOS_LIST_STADE_CREATED, 
  ASOS_LIST_STADE_INSERTED
}asos_list_stade_e;

typedef enum {
  ASOS_LIST_MODE_QUEUE = 0, /** in that mode, list morks like a @queue **/
  ASOS_LIST_MODE_SORTED,    /** in that mode using for sorted this :  "list_data_type" -> data  - @smaller to bigger**/  
  ASOS_LIST_MODE_REV_SORTED /** in that mode using for sorted this :  "list_data_type" -> data  - @bigger  to smaller**/  
}asos_list_mode_e;
  

typedef struct _asos_list_t  asos_list_t;

typedef struct _asos_node    asos_list_node_t;
   


struct _asos_node {
  
    void                * object;  /** this hold inserted USER's object , it can be NULL ( @ref: * me) **/  
    
    list_data_type      data;      /** real list data, can be sorted (optional) -> "asos_list_mode_e" -> @mode **/
 
    asos_list_node_t    * next;
    
    asos_list_node_t    * previous;
    
    asos_list_t         * conteiner_off;  /** for reach head object -> "asos_list_t" -> @head  **/
     
    asos_list_stade_e   stade;  
};
   


/** @superListObject **/
struct _asos_list_t{
  
  asos_list_node_t   * head;  
  
  asos_list_mode_e   mode;
   
  unsigned short     number_of_items;
  
  asos_list_stade_e  stade;
     
};
   








/** @proAPIfunctions *****/



asos_list_fout_e
asos_xlist_create(asos_list_t *     const me , 
                  asos_list_mode_e  mode_select);

asos_list_fout_e
asos_xlist_delete(asos_list_t * const me);

asos_list_fout_e
asos_xlist_node_insert(asos_list_t      * const me , 
                       asos_list_node_t * const node, 
                       void             * node_object,
                       list_data_type     node_data);
 
asos_list_fout_e
asos_xlist_node_delete(asos_list_t      * const me , 
                       asos_list_node_t * const node);
 
 


/** @listUserManager *****/

static inline asos_list_t * 
asos_xlist_goto_root_list(const asos_list_node_t * const me)
{
  if( me->previous == NULL || me == NULL || me->stade != ASOS_LIST_STADE_INSERTED )
  {
     return NULL;
  }
  return me->conteiner_off;
}



static inline asos_list_node_t * 
asos_xlist_get_node_with_index(const asos_list_t   * const me,
                               unsigned short  node_index)  /*** like array  **/
{
  if(me->stade != ASOS_LIST_STADE_CREATED || me->head == NULL){ return NULL;}
  unsigned short walk_index = 0;
  asos_list_node_t *walker = me->head;
  do{  
    if(walker == NULL){ 
      break; 
    } 
    else if(walk_index == node_index){ 
      return walker; /** success out **/
    } 
    else if(walker->next == NULL){ 
      break; 
    } 
    walker = walker->next; 
    walk_index++;
  }while(1); 
  return NULL;
}



static inline asos_list_node_t * 
asos_xlist_get_node_with_data(const asos_list_t   * const me,
                              list_data_type  data)   
{
  if(me->stade != ASOS_LIST_STADE_CREATED || me->head == NULL){ return NULL;}
  unsigned short walk_index = 0;
  asos_list_node_t *walker = me->head;
  do{  
    if(walker == NULL){ 
      break; 
    } 
    else if(data == walker->data){ 
      return walker; /** success out **/
    } 
    else if(walker->next == NULL){ 
      break; 
    } 
    walker = walker->next; 
    walk_index++;
  }while(1); 
  return NULL;
}






#ifdef __cplusplus
}
#endif

#endif
