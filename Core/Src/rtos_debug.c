
#include "rtos_debug.h"


//*** defines****

#define  MEMPOOL_OBJECTS 16  

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

uint16_t size;

//*****************************************
const osThreadAttr_t Task1_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024
};

const osThreadAttr_t Task2_attributes = {
  .name = "defaultTask_2",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024
};

//*****************************************

void Task_init(void)
{
  
  Task1 = osThreadNew(StartTask1, NULL, &Task1_attributes);
  Task2 = osThreadNew(StartTask2, NULL, &Task2_attributes);
  tim_periodic = osTimerNew(periodic_Callback, osTimerPeriodic,(void*) 0, NULL );
  sem_tim = osSemaphoreNew(1, 1, NULL);
  
  
}


//*****************************************

void StartTask1(void *argument)
{
  osTimerStart(tim_periodic, 1000);
  
  //char *path = pvPortMalloc(20*sizeof (char));
  
  for(;;)
  {
    
    osDelay(1);
  }
  
}


//*****************************************

void StartTask2(void *argument)
{
  osStatus_t status;
  osSemaphoreAcquire(sem_tim, osWaitForever);
 
  for(;;)
  {
    status = osSemaphoreAcquire(sem_tim, osWaitForever);
    
    if(status == osOK){
      
      char *path = pvPortMalloc(100*sizeof (char));
      
      sprintf(path, "Hallo Alex\r\n");
      size = sizeof(char);      
      HAL_UART_Transmit(&huart2, (uint8_t*) path, strlen(path), 0xffff);
      HAL_GPIO_TogglePin(GPIOA,  GPIO_PIN_5 );
      vPortFree(path);
      
    }
    
    osThreadYield(); 
  }
  
}

//*****************************************

void periodic_Callback(void *argument)
{
  osSemaphoreRelease(sem_tim);
}

//*****************************************



