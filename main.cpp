/****************************************************************************************
* ALI SAHBAZ
*
*
* ASOS - "Ali Sahbaz Operating System" Framework
*
*
* Date          : 06.07.2023
* By            : Ali Sahbaz
* e-mail        : ali_sahbaz@outlook.com
*/
  
#include "asos_root/asos.h"

#include "asos_root/test_apps/asos_blinky_test.h"
#include "asos_root/test_apps/asos_cmd_rsp_test.h"
#include "asos_root/test_apps/asos_sm_test.h"
#include "asos_root/test_apps/asos_usb_rr_test.h"

/** OS HANDLER **/
static asos_object_t ASOS;

static blinky_obj_t  blink_app;
static cmd_rsp_t     cmd_app;
static sm_t          sm_app;
static usb_t         usb_app;


static void systic_callback_mocking()  // 1 ms
{
  asos_timer_process_run( &ASOS );
} 
 
int main(int argc, char *argv[])
{
    /*
    QCoreApplication a(argc, argv);

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, &systic_callback_mocking);
    timer.start(1);
    */

    asos_create(&ASOS,
                5,       /**  CONTEXT SWITCH MS **/
                1000);   /**  SYSTICK US        **/

    // You can load your hardware RTE layer and connect ASOS here

    asos_app_create(&ASOS ,
                    &blink_app,
                    &blinky_create,
                    &blinky_delete,
                    &blinky_task,
                    ASOS_TASK_PRIO_4);

    asos_app_create(&ASOS ,
                    &cmd_app,
                    &cmd_rsp_create,
                    NULL,
                    &cmd_rsp_process,
                    ASOS_TASK_PRIO_3);


    asos_app_create(&ASOS ,
                    &sm_app,
                    &sm_create,
                    NULL,
                    &sm_process,
                    ASOS_TASK_PRIO_2);

    asos_app_create(&ASOS ,
                    &usb_app,
                    &usb_create,
                    NULL,
                    &usb_process,
                    ASOS_TASK_PRIO_1);

    asos_debug_mode_on(&ASOS);

    asos_app_process_run( &ASOS );

    return 0;
}
