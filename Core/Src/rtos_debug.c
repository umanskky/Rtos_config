
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

#pragma pack(push, 1)

typedef struct{
	
	uint8_t a;
	uint16_t b;
	uint8_t buff[1];
	
		
}MY_STRUCT;

#pragma pack(pop)

//*****************************************
const osThreadAttr_t Task1_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 1024
};

const osThreadAttr_t Task2_attributes = {
  .name = "defaultTask_2",
  .priority = (osPriority_t) osPriorityNormal, //osPriorityHigh
  .stack_size = 1024
};

//*****************************************

void Task_init(void)
{
  
  Task1 = osThreadNew(StartTask1, NULL, &Task1_attributes);
  Task2 = osThreadNew(StartTask2, NULL, &Task2_attributes);
  tim_periodic = osTimerNew(periodic_Callback, osTimerPeriodic,(void*) 0, NULL );
  sem_tim = osSemaphoreNew(1, 1, NULL);
  que_id = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(MY_STRUCT), NULL);
  
}


//*****************************************

void StartTask1(void *argument)
{
  osTimerStart(tim_periodic, 1000);
	MY_STRUCT msg;
	
  for(;;)
  {
		msg.a = 0x54;
		msg.b = 0xad21;
		msg.buff[0] = 0xdd;
		
		osMessageQueuePut(que_id, &msg, 0U, 0U);
		
    osDelay(500);
  }
  
}


//*****************************************

void StartTask2(void *argument)
{
  osStatus_t status_sem, staus_que;
	MY_STRUCT msg;
	
  osSemaphoreAcquire(sem_tim, 0);
 
  for(;;)
  {
    status_sem = osSemaphoreAcquire(sem_tim, 0);
    staus_que = osMessageQueueGet(que_id, &msg, 0U, 0U);
		
    if(status_sem == osOK){
      
      char *path = pvPortMalloc(100*sizeof (char));
      
      sprintf(path, "Hallo Alex\r\n");
      size = sizeof(char);      
      HAL_UART_Transmit(&huart2, (uint8_t*) path, strlen(path), 0xffff);
      HAL_GPIO_TogglePin(GPIOA,  GPIO_PIN_5 );
      vPortFree(path);
      
    }
		else if(staus_que == osOK){
			HAL_UART_Transmit(&huart2, (uint8_t*) &msg, sizeof(msg), 0xffff);
		}
    
		osDelay(500);
    //osThreadYield(); 
  }
  
}

//*****************************************

void periodic_Callback(void *argument)
{
  osSemaphoreRelease(sem_tim);
}

//*****************************************



