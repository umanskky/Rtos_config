#ifndef __RTOS_DEBUG_H
#define __RTOS_DEBUG_H

/* #ifdef __cplusplus
extern "C" {
#endif */

//*******************************************


#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"


extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;

//void StartTask1(void *argument);
void StartTask2(void *argument);

void periodic_Callback(void *argument);

void Task_init(void);
void USER_Usart2_Init(void);



#pragma pack(push, 1)

typedef struct{
	
  uint8_t buff[1024];
  //uint8_t buff_2[1024];
  //uint8_t buff_3[1024];
  //uint8_t buff_4[1024];
  //uint8_t buff_5[1024];
		
}MY_STRUCT;

#pragma pack(pop)



//*******************************************
/* #ifdef __cplusplus
} */


#endif

