
#include "rtos_debug.h"

extern osThreadId_t defaultTaskHandle;

uint16_t size;
uint8_t flag_error = 0;

uint8_t rxBuffer[1024];

//*** defines****

#define  MSGQUEUE_OBJECTS 16  

//*** Task *****
osThreadId_t Task1;
osThreadId_t Task2;

//*** Timer *****
osTimerId_t tim_periodic;

//*** Semafore ******
osSemaphoreId_t sem_tim;

//*** Memory *****
/*
 char *path = pvPortMalloc(20*sizeof (char));
 vPortFree(path);
*/

//*** Queue ******

osMessageQueueId_t que_id;

//****************

//*****************************************
const osThreadAttr_t Task1_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,  //osPriorityHigh
  .stack_size = 128
};

const osThreadAttr_t Task2_attributes = {
  .name = "defaultTask_2",
  .priority = (osPriority_t) osPriorityNormal, //osPriorityHigh
  .stack_size = 128
};

//*****************************************

MY_STRUCT *str_tr;



void Task_init(void)
{
  
  Task1 = osThreadNew(StartTask1, NULL, &Task1_attributes);
  Task2 = osThreadNew(StartTask2, NULL, &Task2_attributes);
  tim_periodic = osTimerNew(periodic_Callback, osTimerPeriodic,(void*) 0, NULL );
  sem_tim = osSemaphoreNew(1, 1, NULL);
  que_id = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(uint32_t), NULL);
  
}


//*****************************************

void USER_Usart2_Init(void)
{
  if(HAL_UART_Receive_DMA(&huart2, (uint8_t *)rxBuffer, sizeof(rxBuffer)) == HAL_OK)
  {
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    //printf("STM32F207 Ready!\r\n");
  }
  else
  {
    //printf("Err: USER_Usart3_Init\r\n");
    //FAILEDProc();
  }
  
}

//*****************************************
    
void StartTask1(void *argument)
{
  MY_STRUCT *str;
  uint32_t flags; 
  
  for(;;)
  {
    str = pvPortMalloc(sizeof(MY_STRUCT));    
    if(str!=NULL){
      str->buff[0] = 0x54;
      str->buff_5[240] = 0xad;
      osMessageQueuePut(que_id, str, 0U, osWaitForever);
      vPortFree(str);
      //osSemaphoreRelease(sem_tim); 
      flags = osThreadFlagsSet(defaultTaskHandle, 0x0002U);      
    }
    else{
      flag_error = 1;    //exec();      
    } 

    //osSemaphoreRelease(sem_tim);  
    
    osDelay(500);
  }  
}


//*****************************************

void StartTask2(void *argument)
{
//  osStatus_t  staus_que;
//  MY_STRUCT *str_2;
  
  for(;;)
  {
//    str_2 = pvPortMalloc(sizeof(MY_STRUCT));  
//    
//		if(str_2!=NULL){
//      staus_que = osMessageQueueGet(que_id, str_2, 0U, osWaitForever);
//      if(staus_que == osOK){
//        HAL_UART_Transmit(&huart2, (uint8_t*)&str_2->buff[0], sizeof(str_2->buff[0]), 0xffff);
//        HAL_UART_Transmit(&huart2, (uint8_t*)&str_2->buff_5[240], sizeof(str_2->buff_5[240]), 0xffff);
//        vPortFree(str_2);     
//      }
//      else{
//        flag_error = 1;          
//      }      
//    }
		osDelay(1);   
  } 
}

//void StartTask2(void *argument)
//{
//  osStatus_t  staus_que;
//  MY_STRUCT *str_recv;
//  
//  for(;;)
//  {
//    __nop();
//    
//    str_recv = pvPortMalloc(sizeof(MY_STRUCT));  
//    //str_tr = malloc(sizeof(MY_STRUCT)); 
//    
//		if(str_tr!=NULL){
//      staus_que = osMessageQueueGet(que_id, str_recv, 0U, osWaitForever);
//      if(staus_que == osOK){
//        HAL_UART_Transmit(&huart2, (uint8_t*)&str_recv->buff, strlen((char*)&str_recv->buff), 0xffff);
//        vPortFree(str_recv);  
//        //free(str_tr);
//      }
//      else{
//        flag_error = 1;          
//      }      
//    }
//		osDelay(500);   
//  } 
//}

//*****************************************

void periodic_Callback(void *argument)
{
  //osSemaphoreRelease(sem_tim);
}

//*****************************************





