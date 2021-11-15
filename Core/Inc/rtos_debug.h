#ifndef __RTOS_DEBUG_H
#define __RTOS_DEBUG_H

/* #ifdef __cplusplus
extern "C" {
#endif */

//*******************************************


#include "stm32l4xx_hal.h"
//#include "cmsis_os2.h"
#include "cmsis_os.h"
#include "string.h"
#include "stdio.h"


extern UART_HandleTypeDef huart2;

void StartTask1(void *argument);
void StartTask2(void *argument);

void periodic_Callback(void *argument);

void Task_init(void);


//*******************************************
/* #ifdef __cplusplus
} */


#endif

