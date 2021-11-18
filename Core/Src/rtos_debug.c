
#include "rtos_debug.h"


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

uint16_t size;

uint8_t flag_error = 0;

#pragma pack(push, 1)

typedef struct{
	
  uint8_t buff[1024];
  uint8_t buff_2[1024];
  uint8_t buff_3[1024];
  uint8_t buff_4[1024];
  uint8_t buff_5[1024];
		
}MY_STRUCT;

#pragma pack(pop)

MY_STRUCT msg;

//*****************************************
const osThreadAttr_t Task1_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 128
};

const osThreadAttr_t Task2_attributes = {
  .name = "defaultTask_2",
  .priority = (osPriority_t) osPriorityNormal, //osPriorityHigh
  .stack_size = 128
};

//*****************************************

void Task_init(void)
{
  
  Task1 = osThreadNew(StartTask1, NULL, &Task1_attributes);
  Task2 = osThreadNew(StartTask2, NULL, &Task2_attributes);
  tim_periodic = osTimerNew(periodic_Callback, osTimerPeriodic,(void*) 0, NULL );
  sem_tim = osSemaphoreNew(1, 1, NULL);
  que_id = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(uint32_t), NULL);
  
}


//*****************************************

void StartTask1(void *argument)
{
  MY_STRUCT *str;
  
  for(;;)
  {
    str = pvPortMalloc(sizeof(MY_STRUCT));    
    if(str!=NULL){
      str->buff[0] = 0x54;
      str->buff_5[240] = 0xad;
      osMessageQueuePut(que_id, str, 0U, osWaitForever);
      vPortFree(str);  
    }
    else{
      flag_error = 1;    //exec();      
    }         
    osDelay(500);
  }  
}


//*****************************************

void StartTask2(void *argument)
{
  osStatus_t  staus_que;
  MY_STRUCT *str_2;
  
  for(;;)
  {
    str_2 = pvPortMalloc(sizeof(MY_STRUCT));  
    
		if(str_2!=NULL){
      staus_que = osMessageQueueGet(que_id, str_2, 0U, osWaitForever);
      if(staus_que == osOK){
        HAL_UART_Transmit(&huart2, (uint8_t*)&str_2->buff[0], sizeof(str_2->buff[0]), 0xffff);
        HAL_UART_Transmit(&huart2, (uint8_t*)&str_2->buff_5[240], sizeof(str_2->buff_5[240]), 0xffff);
        vPortFree(str_2);     
      }
      else{
        flag_error = 1;          
      }      
    }
		osDelay(500);   
  } 
}

//*****************************************

void periodic_Callback(void *argument)
{
  osSemaphoreRelease(sem_tim);
}

//*****************************************



