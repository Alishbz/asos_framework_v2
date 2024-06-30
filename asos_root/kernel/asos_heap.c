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
 

#include "asos_heap.h"

static unsigned char _HEAP[ASOS_HEAP_SIZE];

static size_t _heap_free_byte_size = ( size_t ) 0;


/** @NOTE: sizex  MUST BE SOLID OF 4 ***/                       
void *asos_malloc( size_t sizex )
{
  void *out = NULL; 
  
  unsigned short size = sizex;
  
  if(sizex%4){
    size += (sizex%4);
  }
  
  if(( _heap_free_byte_size + size ) > (ASOS_HEAP_SIZE - 4) ){
     assert( "ASOS_HEAP_SIZE is not enough" );
     return out;
  }
  
  
  if( ( ( _heap_free_byte_size + size ) < ASOS_HEAP_SIZE ) &&
      ( ( _heap_free_byte_size + size ) > _heap_free_byte_size )) 
  { 
      out = &( _HEAP[ _heap_free_byte_size ] );
      _heap_free_byte_size += (4+size);			
  }	
   
  return out;
}



void asos_free( void * me )
{
  (void) me;
}






void asos_heap_init( void )
{
  //memset(_HEAP , 0 , ASOS_HEAP_SIZE );
  _heap_free_byte_size = ( size_t )0;
}


size_t asos_get_free_heap_size( void )
{
  return (size_t)(ASOS_HEAP_SIZE - _heap_free_byte_size);
}


