cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(TEST C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
 
add_executable(asos main.cpp)
 
add_library(asos_lib
  asos_root/asos.c asos_root/asos.h
  asos_root/kernel/asos_bridge.h asos_root/kernel/asos_command_responce.c asos_root/kernel/asos_command_responce.h 
  asos_root/kernel/asos_heap.c asos_root/kernel/asos_heap.h 
  asos_root/kernel/asos_list.c asos_root/kernel/asos_list.h 
  asos_root/kernel/asos_mailbox.c asos_root/kernel/asos_mailbox.h 
  asos_root/kernel/asos_mutex.c asos_root/kernel/asos_mutex.h 
  asos_root/kernel/asos_observer.c asos_root/kernel/asos_observer.h 
  asos_root/kernel/asos_queue.h 
  asos_root/kernel/asos_semaphore.c asos_root/kernel/asos_semaphore.h 
  asos_root/kernel/asos_stade_machine.c asos_root/kernel/asos_stade_machine.h 
  asos_root/kernel/asos_task.c asos_root/kernel/asos_task.h 
  asos_root/kernel/asos_tester.h 
  asos_root/kernel/asos_timer.c asos_root/kernel/asos_timer.h
  asos_root/configs/asos_config.h asos_root/configs/asos_device_tree.h
  asos_root/test_apps/asos_blinky_test.c asos_root/test_apps/asos_blinky_test.h 
  asos_root/test_apps/asos_cmd_rsp_test.c asos_root/test_apps/asos_cmd_rsp_test.h 
  asos_root/test_apps/asos_sm_test.c asos_root/test_apps/asos_sm_test.h 
  asos_root/test_apps/asos_usb_rr_test.c asos_root/test_apps/asos_usb_rr_test.h
)

target_link_libraries(asos PRIVATE asos_lib)