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
 
#include "asos_mailbox.h"


#ifdef ASOS_MAILBOX_LOW_ALLOCATION_SELECT
  #define QUEUE_CREATE(queue)                   asos_queue_create( (asos_queue_str*) queue)
  #define QUEUE_WRITE(queue , data , size)      asos_queue_data_write( (asos_queue_str*) queue , (asos_q_type*) data , (unsigned int)size)
  #define QUEUE_CLEAR(queue)                    asos_queue_clear( (asos_queue_str*) queue)
  #define QUEUE_IS_EMPTY(queue)                 asos_queue_is_empty( (asos_queue_str*) queue)
  #define QUEUE_READ(queue , out_buf , size)    asos_queue_data_read( (asos_queue_str*) queue , (asos_q_type*)out_buf , (unsigned int)size)
  #define QUEUE_GET_UNREAD_SIZE(queue )         asos_queue_get_unreaden_size( (asos_queue_str*) queue)
  #define QUEUE_SIZE                            ASOS_QUEUE_LOW_SIZE_STATIC_SIZE 
#else
  #define QUEUE_CREATE(queue)                   asos_l_queue_create( (asos_l_queue_str*) queue)
  #define QUEUE_WRITE(queue , data , size)      asos_l_queue_data_write( (asos_l_queue_str*) queue , (asos_l_q_type*) data , (unsigned int)size)
  #define QUEUE_CLEAR(queue)                    asos_l_queue_clear( (asos_l_queue_str*) queue)
  #define QUEUE_IS_EMPTY(queue)                 asos_l_queue_is_empty( (asos_l_queue_str*) queue)
  #define QUEUE_READ(queue , out_buf , size)    asos_l_queue_data_read( (asos_l_queue_str*) queue , (asos_l_q_type*)out_buf , (unsigned int)size)
  #define QUEUE_GET_UNREAD_SIZE(queue )         asos_l_queue_get_unreaden_size( (asos_l_queue_str*) queue)
  #define QUEUE_SIZE                            ASOS_QUEUE_HIGH_SIZE_STATIC_SIZE 
#endif   
  
  

#define ASOS_MAILBOX_OBJ_CHECK(obj)        if(obj == NULL){ return ASOS_MAILBOX_FAIL; }
#define ASOS_MAILBOX_OBJ_BOOL_CHECK(obj)   if(obj == NULL){ return ASOS_FALSE; } 
 
#define P_STACK_SIZE                       (QUEUE_SIZE/2)

#define FIND_STRING(all_string,look_string)        strstr((char*)(all_string ), (const char*)look_string)

 

asos_mailbox_fout_e
asos_xmailbox_create(asos_mailbox_t* me , asos_mailbox_protochol_e type)
{
  ASOS_MAILBOX_OBJ_CHECK(me);
  QUEUE_CREATE(&me->way_1);
  QUEUE_CREATE(&me->way_2);
  me->type = type;
  me->is_init = ASOS_TRUE; 
  return ASOS_MAILBOX_SUCCESS;
}

asos_mailbox_fout_e
asos_xmailbox_delete(asos_mailbox_t* me )
{
  ASOS_MAILBOX_OBJ_CHECK(me);
  return asos_xmailbox_create(me,ASOS_MAILBOX_FREE_STYLE);
}



static void _writer(MAILBOX_QUEUE_OBJ_TYPE *queue , asos_mailbox_protochol_e   type , MAILBOX_QUEUE_DATA_TYPE* data , unsigned short data_size){
  switch (type){
    case ASOS_MAILBOX_FREE_STYLE: { 
      QUEUE_WRITE(queue, data , data_size) ;
      break;
    }
    case ASOS_MAILBOX_ASOS_PROTOCHOL: {
      QUEUE_WRITE(queue , ">" , 1) ;
      QUEUE_WRITE(queue , " " , 1) ;
      QUEUE_WRITE(queue , data , data_size) ;
      QUEUE_WRITE(queue , "\n" , 1) ;
      break;
    }  
    // TODO: add your other protochol Writers
  }  
} 


/** OUT check: ASOS_MAILBOX_SUCCESS or ASOS_MAILBOX_FAIL **/
asos_mailbox_fout_e
asos_xmailbox_write(asos_mailbox_t* me , asos_mailbox_way_e way , MAILBOX_QUEUE_DATA_TYPE* data , unsigned short data_size)
{
  ASOS_MAILBOX_OBJ_CHECK(me); 
  ASOS_MAILBOX_OBJ_CHECK(data); 
  if(!data_size){return ASOS_MAILBOX_FAIL;}
  
  switch (way){
    case ASOS_MAILBOX_WAY_1_SELECT: {
      _writer(&me->way_1 , me->type , data , data_size); 
      break;
    }
    case ASOS_MAILBOX_WAY_2_SELECT: {
      _writer(&me->way_2 , me->type , data , data_size); 
      break;
    } 
    // TODO: add your other protochol parsers 
    default:
      return ASOS_MAILBOX_FAIL;
      break;
  }
  
  return ASOS_MAILBOX_SUCCESS;
}


 

static char _asos_protochol_parser(MAILBOX_QUEUE_DATA_TYPE* in_buf , unsigned short in_buf_size ,
                                   MAILBOX_QUEUE_DATA_TYPE* out_buf , unsigned short out_buf_size)
{ 
  char*  ref_point = FIND_STRING(in_buf, "> "); 
  unsigned short data_size = 0 , index = 0;
  char out = 0;
  
  if(ref_point)
  {
     char*  end_point = FIND_STRING((ref_point+2), "\n");
     
     data_size = (unsigned short)(end_point - ref_point);
     
     if(end_point && (data_size > 2 ) && (data_size < in_buf_size ) ){
     
       out = 1;
       
       do{
         out_buf[index] = *(ref_point + 2 + index);
         index++;
         if(index > (out_buf_size-1) ){
           out = 0; 
           break;
         }
       }while(index < (data_size-2) );
       
     } 
  }
  
  return out;
}

static char _parser(MAILBOX_QUEUE_OBJ_TYPE *queue , asos_mailbox_protochol_e   type , MAILBOX_QUEUE_DATA_TYPE* out_buf , unsigned short out_buf_size){
  switch (type){
    case ASOS_MAILBOX_FREE_STYLE: { 
      if( 1 == QUEUE_IS_EMPTY(queue) ){
        QUEUE_READ(queue , out_buf , out_buf_size);
        return 1;
      }
      break;
    }
    case ASOS_MAILBOX_ASOS_PROTOCHOL: {
      MAILBOX_QUEUE_DATA_TYPE holder[P_STACK_SIZE];  
      if( 1 == QUEUE_IS_EMPTY(queue) ){
        QUEUE_READ(queue , &holder , P_STACK_SIZE ); 
        if (  _asos_protochol_parser((MAILBOX_QUEUE_DATA_TYPE *)holder ,P_STACK_SIZE , out_buf , out_buf_size ) ){
          return 1;
        } 
      } 
      break;
    } 
    
    // TODO: add your other protochol parsers
    
  } 
  
  return 0;
} 



/** OUT check: ASOS_MAILBOX_DATA_READY or ASOS_MAILBOX_TIMEOUT**/
asos_mailbox_fout_e
asos_xmailbox_read(asos_mailbox_t* me , asos_mailbox_way_e way , MAILBOX_QUEUE_DATA_TYPE* out_buf , unsigned short out_buf_size)
{
  ASOS_MAILBOX_OBJ_CHECK(me); 
  ASOS_MAILBOX_OBJ_CHECK(out_buf); 
  if(out_buf_size < 1){return ASOS_MAILBOX_FAIL;}
  
  switch (way){
    case ASOS_MAILBOX_WAY_1_SELECT: {
      if ( _parser(&me->way_1 , me->type , out_buf,out_buf_size) ){
         return ASOS_MAILBOX_DATA_READY;
      }
      break;
    }
    case ASOS_MAILBOX_WAY_2_SELECT: {
      if ( _parser(&me->way_2 , me->type , out_buf,out_buf_size) ){
         return ASOS_MAILBOX_DATA_READY;
      }
      break;
    }
    default:
      return ASOS_MAILBOX_FAIL;
      break;
  }
  
  return ASOS_MAILBOX_TIMEOUT;
}


/** OUT check: ASOS_MAILBOX_SUCCESS or ASOS_MAILBOX_FAIL**/
asos_mailbox_fout_e
asos_xmailbox_fresh(asos_mailbox_t* me , asos_mailbox_way_e way  )
{
  ASOS_MAILBOX_OBJ_CHECK(me); 
   
   switch (way){
    case ASOS_MAILBOX_WAY_1_SELECT: {
      QUEUE_CREATE(&me->way_1);
      return  ASOS_MAILBOX_SUCCESS;
    }
    case ASOS_MAILBOX_WAY_2_SELECT: {
      QUEUE_CREATE(&me->way_2); 
      return  ASOS_MAILBOX_SUCCESS;
    } 
  }
  return ASOS_MAILBOX_FAIL;
}
