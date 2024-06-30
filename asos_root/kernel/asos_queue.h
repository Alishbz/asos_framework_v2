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
#pragma once    /** related header include just one time **/
#ifndef ASOS_QUEUE_H
#define ASOS_QUEUE_H

#include "../configs/asos_config.h"
#include "asos_heap.h"

#ifdef __cplusplus
extern "C"
{
#endif

/***
                                                       DEEP EXPLANATION OF QUEUE DRIVER
                                                    **************************************

 @asos_queue_str    ->   ASOS_QUEUE_STATIC_SIZE(255)  , asos_q_type (unsigned char)
    :   @Prototypes                                                                                          @Descriptions
   - asos_queue_create(asos_queue_str* me)                                                                   : init, clear all queue datas
   - asos_queue_data_write( asos_queue_str * me , asos_q_type* data , unsigned int data_size)                : write q , "data" pointer of writer array
   - asos_queue_clear(asos_queue_str * me)                                                                   : clear all
   - asos_queue_is_empty(asos_queue_str * me)                                                                : if return 1 -> queue not empty , 0 -> queue empty
   - asos_queue_data_read( asos_queue_str * me , asos_q_type* out_buffer, unsigned int out_buffer_size )     : read q , "out_buffer" pointer of reader array
   - asos_queue_get_unreaden_size(asos_queue_str * me)                                                       : return ( unreaden datas size )



 @asos_l_queue_str    ->   ASOS_L_QUEUE_STATIC_SIZE(1023)  , asos_l_q_type (unsigned char)
   : @Prototypes                                                                                                 : @Descriptions
   - asos_l_queue_create(asos_l_queue_str* me)                                                                   : init, clear all queue datas
   - asos_l_queue_data_write( asos_l_queue_str * me , asos_l_q_type* data , unsigned int data_size)              : write q , "data" pointer of writer array
   - asos_l_queue_clear(asos_l_queue_str * me)                                                                   : clear all
   - asos_l_queue_is_empty(asos_l_queue_str * me)                                                                : if return 1 -> queue not empty , 0 -> queue empty
   - asos_l_queue_data_read( asos_l_queue_str * me , asos_l_q_type* out_buffer, unsigned int out_buffer_size )   : read q , "out_buffer" pointer of reader array
   - asos_l_queue_get_unreaden_size(asos_l_queue_str * me)                                                       : return ( unreaden datas size )


 @X_QUEUE    ->   size dinamic  , type dinamic
   : @Prototypes                                                                                                 : @Descriptions
   - X_QUEUE(q_name , q_size , q_type)                                                                           : define queue trick necessary veriables
   - x_queue_init(q_name , q_size)                                                                               : clear all queue datas
   - x_queue_data_write(q_name , q_size ,  q_data )                                                              : write q , "q_data" is veriable , NOT array or pointer
   - x_queue_data_read(q_name , q_size , q_out_data )                                                            : read q , "q_out_data" is veriable , NOT array or pointer
   @note: example of x_queue is at the bottom


**/ 
         
  
/*****************************************************************************************/
/*** ASOS NORMAL SIZE QUEUE (255)  **************************************************************************************/	   
/*****************************************************************************************/
/*****************************************************************************************/  
  
  /** USER CONFIGS ***/
  #define ASOS_QUEUE_STATIC_SIZE              ASOS_QUEUE_LOW_SIZE_STATIC_SIZE //(0xFF)   
  #define ASOS_QUEUE_DATA_READ_ALL_SIGNAL    (ASOS_QUEUE_STATIC_SIZE+13)  
  typedef unsigned char                       asos_q_type; 
  
  
  typedef struct{ 
    unsigned int read_ptr; 
    unsigned int write_ptr; 
    asos_q_type  data_buffer[ASOS_QUEUE_STATIC_SIZE];   
  }asos_queue_str;
  
  inline static void asos_queue_create(asos_queue_str* me)
  {
    if(me == NULL){return;}
    me->read_ptr  = 0; 
    me->write_ptr = 0; 
    unsigned int i = 0; 
    for(i = 0 ; i< ASOS_QUEUE_STATIC_SIZE ; i++){
       me->data_buffer[i] = 0;
    }
  }
  
  inline static void asos_queue_data_write( asos_queue_str * me , asos_q_type* data , unsigned int data_size)
  { 
    if(data_size < 1 ||data == NULL || me == NULL){return;} 
    unsigned int index_counter = 0; 
    do{
      me->data_buffer[ me->write_ptr++ ] = data[index_counter];   
      if(me->write_ptr > (ASOS_QUEUE_STATIC_SIZE-1)){
        me->write_ptr = 0;
      } 
      index_counter++; 
    }while( index_counter < data_size);  
  }
  
  inline static void asos_queue_clear(asos_queue_str * me){
    asos_queue_create(me);
  }
  
  inline static char asos_queue_is_empty(asos_queue_str * me){
    if( me->read_ptr == me->write_ptr ){
       return 0;
    }
    return 1;
  }

  inline static void asos_queue_data_read( asos_queue_str * me , asos_q_type* out_buffer, unsigned int out_buffer_size )
  { 
     if(out_buffer_size < 1 ||out_buffer == NULL || me == NULL){return;} 
     if(out_buffer_size > (ASOS_QUEUE_STATIC_SIZE-1) && (out_buffer_size!=ASOS_QUEUE_DATA_READ_ALL_SIGNAL)){return;}
      unsigned int index_counter = 0;    
      do{  
          if(asos_queue_is_empty(me)){
            *(out_buffer + index_counter ) =  me->data_buffer[ me->read_ptr++ ] ;   
          }
          else{ break; }
           if(me->read_ptr > (ASOS_QUEUE_STATIC_SIZE-1)){
             me->read_ptr = 0;
          }
          index_counter++;
          if(index_counter > (ASOS_QUEUE_STATIC_SIZE-1)){break;}
      }while( index_counter < out_buffer_size);   
    }

   inline static unsigned short asos_queue_get_unreaden_size(asos_queue_str * me){
     short hold = me->write_ptr - me->read_ptr;  
      if(hold < 0){ 
            hold = (short)(me->write_ptr)   +   (short)((ASOS_QUEUE_STATIC_SIZE-1) - me->read_ptr);
      }
      if(hold < 0 || me->read_ptr == me->write_ptr ){ return 0; }
      return (unsigned short)hold;
    }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
/*****************************************************************************************/
/*** ASOS LARGE SIZE QUEUE (1023)  **************************************************************************************/	   
/*****************************************************************************************/
/*****************************************************************************************/  
  
  /** USER CONFIGS ***/
  #define ASOS_L_QUEUE_STATIC_SIZE              ASOS_QUEUE_HIGH_SIZE_STATIC_SIZE // (0x3FF)   
  #define ASOS_L_QUEUE_DATA_READ_ALL_SIGNAL    (ASOS_L_QUEUE_STATIC_SIZE+13)  
  typedef unsigned char                         asos_l_q_type; 
  
  
  typedef struct{ 
    unsigned int   read_ptr; 
    unsigned int   write_ptr; 
    asos_l_q_type  data_buffer[ASOS_L_QUEUE_STATIC_SIZE];   
  }asos_l_queue_str;
  
  inline static void asos_l_queue_create(asos_l_queue_str* me)
  {
    if(me == NULL){return;}
    me->read_ptr  = 0; 
    me->write_ptr = 0; 
    unsigned int i = 0; 
    for(i = 0 ; i< ASOS_L_QUEUE_STATIC_SIZE ; i++){
       me->data_buffer[i] = 0;
    }
  }
  
  inline static void asos_l_queue_data_write( asos_l_queue_str * me , asos_l_q_type* data , unsigned int data_size)
  { 
    if(data_size < 1 ||data == NULL || me == NULL){return;} 
    unsigned int index_counter = 0; 
    do{
      me->data_buffer[ me->write_ptr++ ] = data[index_counter];   
      if(me->write_ptr > (ASOS_L_QUEUE_STATIC_SIZE-1)){
        me->write_ptr = 0;
      } 
      index_counter++; 
    }while( index_counter < data_size);  
  }
  
  inline static void asos_l_queue_clear(asos_l_queue_str * me){
    asos_l_queue_create(me);
  }
  
  inline static char asos_l_queue_is_empty(asos_l_queue_str * me){
    if( me->read_ptr == me->write_ptr ){
       return 0;
    }
    return 1;
  }

  inline static void asos_l_queue_data_read( asos_l_queue_str * me , asos_l_q_type* out_buffer, unsigned int out_buffer_size )
  { 
     if(out_buffer_size < 1 ||out_buffer == NULL || me == NULL){return;} 
     if(out_buffer_size > (ASOS_L_QUEUE_STATIC_SIZE-1) && (out_buffer_size!=ASOS_L_QUEUE_DATA_READ_ALL_SIGNAL)){return;}
      unsigned int index_counter = 0;    
      do{  
          if(asos_l_queue_is_empty(me)){
            *(out_buffer + index_counter ) =  me->data_buffer[ me->read_ptr++ ] ;   
          }
          else{ break; }
           if(me->read_ptr > (ASOS_L_QUEUE_STATIC_SIZE-1)){
             me->read_ptr = 0;
          }
          index_counter++;
          if(index_counter > (ASOS_L_QUEUE_STATIC_SIZE-1)){break;}
      }while( index_counter < out_buffer_size);   
    }

   inline static unsigned short asos_l_queue_get_unreaden_size(asos_l_queue_str * me){
     short hold = me->write_ptr - me->read_ptr;  
      if(hold < 0){ 
            hold = (short)(me->write_ptr)   +   (short)((ASOS_L_QUEUE_STATIC_SIZE-1) - me->read_ptr);
      }
      if(hold < 0 || me->read_ptr == me->write_ptr ){ return 0; }
      return (unsigned short)hold;
    }
  




  
  
  
  
  
  
  
  
  
  
  
  
  
/*****************************************************************************************/
/*** X_QUEUE   **************************************************************************************/	   
/*****************************************************************************************/
/*****************************************************************************************/  
  
  #define  X_QUEUE(q_name , q_size , q_type)  \
            unsigned int q_name##_read_ptr; \
            unsigned int q_name##_write_ptr; \
            q_type q_name##_data_buffer[q_size];  \
              
              
  #define  x_queue_init(q_name , q_size)  \
            do{\
               q_name##_read_ptr = 0;\
               q_name##_write_ptr = 0;\
               unsigned int i = 0;\
               for(i = 0 ; i< q_size ; i++){\
                   q_name##_data_buffer[i] = 0;\
               } \
            }while(0);
     
              
  #define  x_queue_data_write(q_name , q_size ,  q_data )   \
            do{\
                q_name##_data_buffer[q_name##_write_ptr++] = q_data;  \
                if(q_name##_write_ptr > (q_size-1)){\
                    q_name##_write_ptr = 0;\
                }\
            }while(0);
             
  #define  x_queue_data_read(q_name , q_size , q_out_data )   \
            do{\
              if(q_name##_read_ptr != q_name##_write_ptr ){\
                  q_out_data = q_name##_data_buffer[q_name##_read_ptr++];\
                  if(q_name##_read_ptr > (q_size-1)){\
                      q_name##_write_ptr = 0;\
                  }\
              }\
            }while(0);
              
            
 
     
/***

x_queue EXAMPLE:


#define WIFI_MODUL_RX_QUEUE_SIZE                50          
#define WIFI_MODUL_RX_QUEUE_UNREACHABLE_VALUE   254  

// abstract trig queue interface 
#define wifi_modul_queue_init(name)                 x_queue_init(name , WIFI_MODUL_RX_QUEUE_SIZE);
#define wifi_modul_queue_data_write(name,data)      x_queue_data_write(name , WIFI_MODUL_RX_QUEUE_SIZE , data);
#define wifi_modul_queue_data_read(name,read_data)  x_queue_data_read(name , WIFI_MODUL_RX_QUEUE_SIZE , read_data);

typedef struct {
    
    int a ;
    
    X_QUEUE(rx_queue , WIFI_MODUL_RX_QUEUE_SIZE , unsigned char);
    
}wifi_modul_t;          
 
void wifi_modul_create(wifi_modul_t * me){
    
    me->a = 5;
    
    x_queue_init(me->rx_queue , WIFI_MODUL_RX_QUEUE_SIZE);

    wifi_modul_queue_data_write(me->rx_queue , 0x09);//x_queue_data_write(me->rx_queue , WIFI_MODUL_RX_QUEUE_SIZE , 0x09);
    wifi_modul_queue_data_write(me->rx_queue , 0x08);//x_queue_data_write(me->rx_queue , WIFI_MODUL_RX_QUEUE_SIZE , 0x08);
    wifi_modul_queue_data_write(me->rx_queue , 0x07);//x_queue_data_write(me->rx_queue , WIFI_MODUL_RX_QUEUE_SIZE , 0x07);
}  

void wifi_modul_work(wifi_modul_t * me){
    
    // This value must not write in queue
    unsigned char q_read = WIFI_MODUL_RX_QUEUE_UNREACHABLE_VALUE;  

    
    wifi_modul_queue_data_read(me->rx_queue  , q_read);//x_queue_data_read(me->rx_queue , WIFI_MODUL_RX_QUEUE_SIZE , q_read);
    
    if(q_read != WIFI_MODUL_RX_QUEUE_UNREACHABLE_VALUE)
    {
        do{
            printf("q_read: %x\n" , q_read);

            q_read = WIFI_MODUL_RX_QUEUE_UNREACHABLE_VALUE;

            wifi_modul_queue_data_read(me->rx_queue  , q_read);// x_queue_data_read(me->rx_queue , WIFI_MODUL_RX_QUEUE_SIZE , q_read);    

            if(q_read == WIFI_MODUL_RX_QUEUE_UNREACHABLE_VALUE){break;}
        }while(1) ;
    } 
}  
            
int main() { 
    printf("TEST x_queue \n\n");
    wifi_modul_t wifi;
    wifi_modul_create(&wifi); 
    wifi_modul_work(&wifi);
    return 0;
}


**/           
            
  
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
  /** ASOS DINAMIC QUEUE (dx) USER CONFIGS ***/   
  #define ASOS_QUEUE_DX_DATA_READ_ALL_SIGNAL    (9999)  
  typedef unsigned char                       asos_q_dx_type; 
  
  
  typedef struct{ 
    unsigned int     read_ptr; 
    unsigned int     write_ptr; 
    unsigned short   size;
    asos_q_dx_type  *data_buffer;   
  }asos_queue_dx_str;
  
  inline static void asos_queue_dx_create(asos_queue_dx_str* me , unsigned short q_size)
  {
    if(me == NULL || q_size < 1  || me->data_buffer == NULL ){return;}
    if((q_size-1) == ASOS_QUEUE_DX_DATA_READ_ALL_SIGNAL || 
       (q_size-1) > (unsigned short)asos_get_free_heap_size() ){
       assert( "queue heap allocation error" );
       return;
    }
    me->read_ptr  = 0; 
    me->write_ptr = 0;   
    me->size = q_size;   
    me->data_buffer = (asos_q_dx_type*)asos_malloc(  sizeof(asos_q_dx_type)*q_size);
  }
  
  inline /* static */ void asos_queue_dx_data_write(asos_queue_dx_str * me, asos_q_dx_type * data, unsigned int data_size)
  { 
    if(data_size < 1 ||data == NULL || me == NULL || me->data_buffer == NULL){return;} 
    unsigned int index_counter = 0; 
    do{
      *(me->data_buffer + me->write_ptr ) = data[index_counter];   
      me->write_ptr++;
      if(me->write_ptr > (unsigned int)(me->size-1)){
        me->write_ptr = 0;
      } 
      index_counter++; 
    }while( index_counter < data_size);  
  }
  
  inline/* static */void asos_queue_dx_delete(asos_queue_dx_str * me){
    asos_queue_dx_create(me,1);
    me->size = 0;   
    asos_free(me->data_buffer);
  }
  
  inline /* static */ void asos_queue_dx_clear(asos_queue_dx_str * me){
    asos_queue_dx_create(me,me->size); 
  }
  
  inline /* static*/ char asos_queue_dx_is_empty(asos_queue_dx_str * me){
    if( me->read_ptr == me->write_ptr ){
       return 0;
    }
    return 1;
  }

  inline /*static*/ void asos_queue_dx_data_read( asos_queue_dx_str * me , asos_q_dx_type* out_buffer, unsigned int out_buffer_size )
  { 
     if(out_buffer_size < 1 ||out_buffer == NULL || me == NULL || me->data_buffer == NULL){return;} 
     if(out_buffer_size > (unsigned int)(me->size-1) && (out_buffer_size!=ASOS_QUEUE_DX_DATA_READ_ALL_SIGNAL)){return;}
      unsigned int index_counter = 0;    
      do{  
          if(asos_queue_dx_is_empty(me)){
            *(out_buffer + index_counter ) =  me->data_buffer[ me->read_ptr ] ; 
            me->read_ptr++ ;
          }
          else{ break; }
           if(me->read_ptr > (unsigned int)(me->size-1)){
             me->read_ptr = 0;
          }
          index_counter++;
          if(index_counter > (unsigned int)(me->size-1)){break;}
      }while( index_counter < out_buffer_size);   
    }

   inline /*static*/ unsigned short asos_queue_dx_get_unreaden_size(asos_queue_dx_str * me){
     short hold = me->write_ptr - me->read_ptr;  
      if(hold < 0){ 
            hold = (short)(me->write_ptr)   +   (short)((me->size-1) - me->read_ptr);
      }
      if(hold < 0 || me->read_ptr == me->write_ptr ){ return 0; }
      return (unsigned short)hold;
    }
  
  /**  queue_dx TEST EX:

  asos_queue_dx_str  q_test ;
  asos_q_dx_type     data = 0 , rx_data = 0;
  
  asos_queue_dx_create(&q_test , 10);
  
  data = 'a';
  asos_queue_dx_data_write(&q_test , &data , 1);
  data = 'b';
  asos_queue_dx_data_write(&q_test , &data , 1);
  data = 'c';
  asos_queue_dx_data_write(&q_test , &data , 1);
  data = 'd';
  asos_queue_dx_data_write(&q_test , &data , 1);
  
  unsigned short unread_size = asos_queue_dx_get_unreaden_size(&q_test);
    
  char printer[100]; memset(printer,0,100);
  snprintf(printer, 100 , "q unread data size: %d " ,unread_size);
  
  asos_print(&ASOS , (const char*)printer); 
  
  if(asos_queue_dx_is_empty(&q_test)){
        
    do{
        asos_queue_dx_data_read(&q_test  , &rx_data , 1);
        
        memset(printer,0,100);
        
        snprintf(printer, 100 ,  "q read data  : %c" , rx_data); 
        
        asos_print(&ASOS , (const char*)printer);
        
     }while(asos_queue_dx_is_empty(&q_test));
  
  }
***/
  
#ifdef __cplusplus
}
#endif

#endif
