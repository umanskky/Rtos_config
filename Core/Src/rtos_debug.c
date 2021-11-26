
#include "rtos_debug.h"

extern osThreadId_t defaultTaskHandle;

uint16_t size;
uint8_t flag_error = 0;

uint8_t rxBuffer[1024];
char str_5[100];

//*** defines****

#define  MSGQUEUE_OBJECTS     16 
#define  TIME_DELAY           50U   //ms

//*** Task *****
osThreadId_t TaskSender1;
osThreadId_t TaskSender2;
osThreadId_t TaskReciver;
osThreadId_t TaskTimer;
osThreadId_t TaskButton;

//*** Timer *****
osTimerId_t tim_periodic;

//*** Semafore ******
osSemaphoreId_t sem_tim;
osSemaphoreId_t sem_clb;

//*** Memory *****
/*
 char *path = pvPortMalloc(20*sizeof (char));
 vPortFree(path);
*/

osMemoryPoolId_t mem_id;

//*** Queue ******

osMessageQueueId_t que_id;

//****************

//*****************************************
const osThreadAttr_t TaskSender1_attributes = {
  .name = "Sender_1",
  .priority = (osPriority_t) osPriorityNormal,  //osPriorityHigh
  .stack_size = 128
};

const osThreadAttr_t TaskSender2_attributes = {
  .name = "Sender_2",
  .priority = (osPriority_t) osPriorityNormal, //osPriorityHigh
  .stack_size = 128
};

const osThreadAttr_t TaskReciver_attributes = {
  .name = "Reciver",
  .priority = (osPriority_t) osPriorityNormal, //osPriorityHigh
  .stack_size = 128
};

const osThreadAttr_t TaskTimer_attributes = {
  .name = "TaskTimer",
  .priority = (osPriority_t) osPriorityNormal, //osPriorityHigh
  .stack_size = 128
};

const osThreadAttr_t TaskButton_attributes = {
  .name = "TaskButton",
  .priority = (osPriority_t) osPriorityNormal, //osPriorityHigh
  .stack_size = 128
};
//*****************************************

MY_STRUCT str;

void Task_init(void)
{
  
  TaskSender1 = osThreadNew(Sender1, NULL, &TaskSender1_attributes);
  TaskSender2 = osThreadNew(Sender2, NULL, &TaskSender2_attributes);
  TaskReciver = osThreadNew(Reciver, NULL, &TaskReciver_attributes);
  TaskTimer   = osThreadNew(Timer,   NULL, &TaskTimer_attributes  );
  TaskButton  = osThreadNew(Button,  NULL, &TaskButton_attributes );
  
  tim_periodic = osTimerNew(periodic_Callback, osTimerPeriodic,(void*) 0, NULL );
  
  sem_tim = osSemaphoreNew(1, 1, NULL);
  sem_clb = osSemaphoreNew(1, 1, NULL);
  
  que_id = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(uint32_t), NULL);
  
  mem_id = osMemoryPoolNew(MSGQUEUE_OBJECTS, sizeof(uint32_t), NULL);
  
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
    __nop(); // error!
  }
  
}

//*****************************************
    
void Sender1(void *argument)
{
  MY_SENDER *str2;
  
  for(;;)
  {
    str2 = pvPortMalloc(sizeof(MY_SENDER));
    //str = osMemoryPoolAlloc(mem_id, 0U);  
    if(str2!=NULL)
    {
      sprintf(str2->send_1, "Work sender 1\n\r");
      sprintf(str2->send_2, "Work sender 2\n\r");
      osMessageQueuePut(que_id, &str2, 0U, osWaitForever);
      __nop();
    }     
    
    osDelay(500);
  }  
}


//*****************************************

void Sender2(void *argument)
{
//  osStatus_t status;
//  osSemaphoreAcquire(sem_clb, 0U);
  
  for(;;)
  {
//    status = osSemaphoreAcquire(sem_clb, 0U);
//    if(status == osOK)
//    {
//      //HAL_UART_Transmit(&huart2, (uint8_t*)&str.buff, strlen(str.buff), 0xffff);
//      osMessageQueuePut(que_id, &str, 0U, osWaitForever);
//    }

		osDelay(1);   
  } 
  
}


//*****************************************

void Reciver(void *argument)
{
	MY_SENDER* strp;
  osStatus_t status_que, status_sem;
  
  osSemaphoreAcquire(sem_clb, 0U);
 
  for(;;)
  {

    status_que = osMessageQueueGet(que_id, &strp, 0U, osWaitForever);
    status_sem = osSemaphoreAcquire(sem_clb, 0U); 
    
    if(status_que == osOK)
    {
      HAL_UART_Transmit(&huart2, (uint8_t*)strp->send_1, strlen(strp->send_1), 0xffff);
      HAL_UART_Transmit(&huart2, (uint8_t*)strp->send_2, strlen(strp->send_2), 0xffff);
        
      vPortFree(strp);
    }
    
    
    if(status_sem == osOK)
    {
      HAL_UART_Transmit(&huart2, (uint8_t*)str.buff, strlen(str.buff), 0xffff);
    }
     
    //osThreadYield();
		osDelay(1);   
  } 
}

//*****************************************

void Timer(void *argument)
{
  osStatus_t status;
  osTimerStart(tim_periodic, TIME_DELAY);
  osSemaphoreAcquire(sem_tim, 0U);
  
  for(;;)
  {
    status = osSemaphoreAcquire(sem_tim, 0U);
    if(status == osOK)
    {
      //HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
    
    
    osDelay(1);
  }  
}

//*****************************************

void Button(void *argument)
{
  uint8_t k;
  
  for(;;)
  {
    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_RESET)
    {
      while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_RESET){vTaskDelay(50);}
      k=1;
      if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_SET && k)
      {
        __nop();
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
      }
    }

    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_SET)
    {
      while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_SET){vTaskDelay(50);}
      k=0;
    } 
    
    osDelay(20);
  }  
}

//*****************************************

void periodic_Callback(void *argument)
{
  osSemaphoreRelease(sem_tim);
}

//*****************************************

/*
HAL_UART_Transmit(&huart2, (uint8_t*)&strp->buff[0], sizeof(strp->buff[0]), 0xffff);
HAL_UART_Transmit(&huart2, (uint8_t*)&strp->buff_5[240], sizeof(strp->buff_5[240]), 0xffff);
*/

//*****************************************



