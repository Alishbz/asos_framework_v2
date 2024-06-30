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
 
#include "asos_list.h"
 

#define ASOS_LIST_OBJ_CHECK(obj)        if(obj == NULL){ return ASOS_LIST_FAIL; }
#define ASOS_LIST_OBJ_BOOL_CHECK(obj)   if(obj == NULL){ return ASOS_FALSE; } 
 

/******************************/
#define GOTO_LAST_NODE(xwalker) \
do{ \
  if(xwalker == NULL){\
    break;\
  }\
  else if(xwalker->next == NULL){\
    break;\
  }\
  xwalker = xwalker->next;\
}while(1);


#define FIND_NODE(xwalker , node) \
do{ \
  if(xwalker == NULL){\
    break;\
  }\
  else if(xwalker->next == NULL){\
    break;\
  }\
  else if(xwalker->next == node){\
    break;\
  }\
  xwalker = xwalker->next;\
}while(1);
 
#define GOTO_HAS_BIGGEST_DATA_NODE(xwalker , xdata) \
do{ \
  if(xwalker == NULL){\
    break;\
  }\
  else if(xwalker->next == NULL){\
    break;\
  }\
  else if(xwalker->next->data >= xdata){\
    break;\
  }\
  xwalker = xwalker->next;\
}while(1);


#define GOTO_HAS_SMALLEST_DATA_NODE(xwalker , xdata) \
do{ \
  if(xwalker == NULL){\
    break;\
  }\
  else if(xwalker->next == NULL){\
    break;\
  }\
  else if(xwalker->next->data <= xdata){\
    break;\
  }\
  xwalker = xwalker->next;\
}while(1);


 
/************************************************/ 


static void _node_create(asos_list_t * me , asos_list_node_t * node, void * node_object, list_data_type node_data);
static void _node_insert_to_first_node(asos_list_t * me , asos_list_node_t * node );
static void _node_insert_to_end(asos_list_node_t * me , asos_list_node_t * node );
static void _node_insert_to_between(asos_list_node_t * me , asos_list_node_t * node );
static void _node_delete_to_first_node(asos_list_t      * me , asos_list_node_t * node );
static void _node_delete_to_end(asos_list_node_t * me , asos_list_node_t * node );
static void _node_delete_to_between(asos_list_node_t * me , asos_list_node_t * node );
static void _node_insert_to_first_between( asos_list_t      * me  , asos_list_node_t * node_new,asos_list_node_t * node);

 

asos_list_fout_e
asos_xlist_create(asos_list_t     * const me, 
                  asos_list_mode_e  mode_select)
{
  ASOS_LIST_OBJ_CHECK(me);
  me->stade = ASOS_LIST_STADE_CREATED;
  me->number_of_items = 0; 
  me->mode = mode_select;
  me->head = NULL; 
  return ASOS_LIST_SUCCESS;
}

asos_list_fout_e
asos_xlist_delete(asos_list_t * const me)
{
  ASOS_LIST_OBJ_CHECK(me);  
  asos_list_fout_e out = asos_xlist_create(me , ASOS_LIST_MODE_QUEUE);
  me->stade = ASOS_LIST_STADE_DELETE;
  return out;  
}
 


asos_list_fout_e
asos_xlist_node_insert(asos_list_t      * const me , 
                       asos_list_node_t * const node, 
                       void             * node_object,
                       list_data_type     node_data)
{
  ASOS_LIST_OBJ_CHECK(me);
  ASOS_LIST_OBJ_CHECK(node);
  
  asos_list_node_t *walker = me->head;
    
  /** node create **/
  _node_create(me,node,node_object,node_data);
  
  if( walker == NULL)
  {
      _node_insert_to_first_node(me, node);
  }
  else
  { 
    switch (me->mode){
      
        case ASOS_LIST_MODE_QUEUE:{
          
          GOTO_LAST_NODE(walker);
          
           /** always add to end in that mode **/
           _node_insert_to_end( walker , node ); 
           
          break;
        }
        case ASOS_LIST_MODE_SORTED:{
          
          GOTO_HAS_BIGGEST_DATA_NODE( walker , node->data ) ;
           
          if(walker->data >= node->data ){
            _node_insert_to_first_between( me , node , walker);
          }  
          else if(walker->next == NULL){
            _node_insert_to_end( walker , node ); 
          }
          else{
            _node_insert_to_between( walker , node );
          }
          
          break;
        }
        case ASOS_LIST_MODE_REV_SORTED:{
          
          GOTO_HAS_SMALLEST_DATA_NODE( walker , node->data ) ;
           
          if(walker->data <= node->data ){
            _node_insert_to_first_between( me , node , walker);
          }  
          else if(walker->next == NULL){
            _node_insert_to_end( walker , node ); 
          }
          else{
            _node_insert_to_between( walker , node );
          }
          
          break;
        }
        
        default:{
          node->conteiner_off = NULL;
          node->stade = ASOS_LIST_STADE_DELETE;
          return ASOS_LIST_FAIL;
          break;
        }
    }  
  }
  
  if(node->stade == ASOS_LIST_STADE_INSERTED ){
       
    me->number_of_items++;
    
    return ASOS_LIST_SUCCESS;
  } 
  
  return ASOS_LIST_FAIL;
}
 
asos_list_fout_e
asos_xlist_node_delete(asos_list_t      * const me , 
                       asos_list_node_t * const node)
{
  ASOS_LIST_OBJ_CHECK(me); 
  ASOS_LIST_OBJ_CHECK(node);
  
  asos_list_node_t *walker = me->head;
  
  if(walker != NULL)
  { 
    FIND_NODE(walker , node);
      
    if(walker->previous == me->head){
        _node_delete_to_first_node( me , node );
    }
    else if(walker->next->next == NULL){
      _node_delete_to_end( walker , node );
    }
    else{
       _node_delete_to_between( walker , node );
    }
  }
  
  if(node->stade == ASOS_LIST_STADE_DELETE && 
     me->number_of_items ){
       
    me->number_of_items--;
  } 
  
  return ASOS_LIST_SUCCESS;
}
 








/***  NODE WAY'S MANAGER INTERFACE  ***/
 

static void _node_create(asos_list_t      * me , 
                         asos_list_node_t * node, 
                         void             * node_object,
                         list_data_type     node_data)
{
  node->object = node_object;
  node->data = node_data;
  node->next = NULL;
  node->previous = NULL;
  node->conteiner_off = me;
  node->stade = ASOS_LIST_STADE_CREATED;
}


static void _node_insert_to_first_between( asos_list_t      * me , 
                                           asos_list_node_t * node_new,
                                           asos_list_node_t * node)
{ 
  node->previous =node_new; 
  node_new->next = node;
  node_new->previous = me->head; 
  me->head = node_new; 
  node_new->stade = ASOS_LIST_STADE_INSERTED;
}

static void _node_insert_to_first_node(asos_list_t      * me , 
                                       asos_list_node_t * node )
{
  node->next = NULL;
  node->previous = me->head;
  me->head = node;
  node->stade = ASOS_LIST_STADE_INSERTED;
}

static void _node_insert_to_end(asos_list_node_t * me , 
                                asos_list_node_t * node )
{
  if(me->next != NULL){return;}
  node->next = NULL;
  node->previous = me;
  me->next = node;
  node->stade = ASOS_LIST_STADE_INSERTED;
}

static void _node_insert_to_between(asos_list_node_t * me , 
                                    asos_list_node_t * node )
{
  if(me->next == NULL){return;} 
  node->next = me->next;
  node->previous = me;
  me->next->previous = node; 
  me->next = node; 
  node->stade = ASOS_LIST_STADE_INSERTED;
}


static void _node_delete_to_first_node(asos_list_t      * me , 
                                       asos_list_node_t * node )
{
  node->next = NULL;
  node->previous = NULL; 
  me->head = NULL;
  node->stade = ASOS_LIST_STADE_DELETE;
}

static void _node_delete_to_end(asos_list_node_t * me , 
                                asos_list_node_t * node )
{
  if(me->next != NULL){return;}
  node->next = NULL;
  node->previous = NULL;
  me->next = NULL;
  node->stade = ASOS_LIST_STADE_DELETE;
}


static void _node_delete_to_between(asos_list_node_t * me , 
                                    asos_list_node_t * node )
{
  if(me->next == NULL || me->next->previous == NULL){return;}  
  me->next->next->previous = node; 
  me->next = me->next->next; 
  node->next =NULL;
  node->previous = NULL; 
  node->stade = ASOS_LIST_STADE_DELETE;
}




