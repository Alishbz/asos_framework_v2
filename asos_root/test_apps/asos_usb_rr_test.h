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
#ifndef asos_usb_rr_test_H
#define asos_usb_rr_test_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../asos.h"

typedef struct {
  int id;
  asos_task_t * os;   /** Developer can hold own task object then reach his OS  **/
}usb_t;

ASOS_USER_FOUT
usb_create( void * me , asos_task_t * os );
 
ASOS_USER_FOUT
usb_process( void * me , asos_task_t * os);

#ifdef __cplusplus
}
#endif

#endif
