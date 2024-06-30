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
#ifndef ASOS_TESTER_H
#define ASOS_TESTER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../configs/asos_config.h"
       
       
#ifdef ASOS_ASOS_TEST_TDD_MODE_ACTIVE 

  
  
  #define ASOS_TEST_PRINT_SIZE  100 
  
  
  #define ASOS_TEST_START( group_name , test_name )     \
  snprintf((char*)out1,ASOS_TEST_PRINT_SIZE, "%s" , (char*)#group_name ); \
  snprintf((char*)out2,ASOS_TEST_PRINT_SIZE, "%s" , (char*)#test_name);  \
  int test_out = 0;
  
  #define ASOS_TEST_HARNESS(xgroup_name , xtest_name)      \
  static int ASOS_TEST_##xgroup_name##_##xtest_name##_test(char*out1 , char*out2) \
  {ASOS_TEST_START(xgroup_name , xtest_name);
    
  #define ASOS_TEST_STOP( )    }return test_out; 

  #define ASOS_TEST_CHECK( data1 , data2 )    if(data1 != data2){test_out = 1;}
   
  /***  USER PRINT  ***/
  #define ASOS_TEST_RUN(osxx ,group_name , test_name)      \
  do{\
    char holder_1[ASOS_TEST_PRINT_SIZE]; \
    char holder_2[ASOS_TEST_PRINT_SIZE]; \
    char holder_3[ASOS_TEST_PRINT_SIZE]; \
    int out = ASOS_TEST_##group_name##_##test_name##_test((char*)holder_1 , (char*)holder_2) ; \
    if(out == 1){  \
      snprintf((char*)holder_3,ASOS_TEST_PRINT_SIZE, "TDD %s , %s : FAIL (%s , %d)" , (char*)holder_1 , (char*)holder_2 , __FILE__ , __LINE__ );\
    }else{ \
      snprintf((char*)holder_3,ASOS_TEST_PRINT_SIZE, "TDD %s , %s : PASS" ,  (char*)holder_1 , (char*)holder_2  );\
    } \
    asos_print(osxx,(const char*)holder_3);\
  }while(0)

    
    
    
    
    
    
#else
    
  #define ASOS_TEST_PRINT_SIZE 
    
  #define ASOS_TEST_HARNESS(group_name , test_name)  \
  static int ASOS_TEST_##group_name##_##test_name##_test(char*out1 , char*out2) 
    
  #define ASOS_TEST_START( group_name , test_name ) 
    
  #define ASOS_TEST_STOP( )  
    
  #define ASOS_TEST_CHECK( data1 , data2 )   
    
  #define ASOS_TEST_RUN(osxx ,group_name , test_name)       
    
#endif

  
    
/********

HOW TO USE ?

ex:
 
 
#ifdef ASOS_ASOS_TEST_TDD_MODE_ACTIVE 

  ASOS_TEST_HARNESS(asos, asos_create1) 
  {
    ASOS_TEST_CHECK(6 , 6); 
   
    ASOS_TEST_STOP();  
  }
  
  ASOS_TEST_HARNESS(asos, asos_create2) 
  {
    ASOS_TEST_CHECK(5 , 6); 
   
    ASOS_TEST_STOP();   
  }
#endif




AND in the INIT -> 

load

  ASOS_TEST_RUN(&ASOS, asos, asos_create1);
  ASOS_TEST_RUN(&ASOS, asos, asos_create2);







********/
    
    
#ifdef __cplusplus
	}
#endif

#endif
