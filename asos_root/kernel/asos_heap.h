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
#ifndef ASOS_HEAP_H
#define ASOS_HEAP_H

#ifdef __cplusplus
	extern "C"
	{
#endif

#include "../configs/asos_config.h"

/**
  @howToUse ex:

  typedef struct{ int x; }test; 
  test * out = ( test * ) asos_malloc( sizeof( test ) );
 
**/

          
          
/** @proDriverFunction ***/
          
void asos_heap_init( void );    

void *asos_malloc( size_t sizex );

void asos_free( void * me );

size_t asos_get_free_heap_size( void );




#ifdef __cplusplus
	}
#endif

#endif
