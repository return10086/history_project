/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <MyProject.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for task1 */
osThreadId_t task1Handle;
const osThreadAttr_t task1_attributes = {
  .name = "task1",
  .stack_size = 1000 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for task2 */
osThreadId_t task2Handle;
const osThreadAttr_t task2_attributes = {
  .name = "task2",
  .stack_size = 1000 * 4,
  .priority = (osPriority_t) osPriorityLow4,
};
/* Definitions for task3 */
osThreadId_t task3Handle;
const osThreadAttr_t task3_attributes = {
  .name = "task3",
  .stack_size = 1000 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for first_task */
osThreadId_t first_taskHandle;
const osThreadAttr_t first_task_attributes = {
  .name = "first_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh7,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void task1_start(void *argument);
void task2_start(void *argument);
void task3_start(void *argument);
void first_task_begin(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of task1 */
  task1Handle = osThreadNew(task1_start, NULL, &task1_attributes);

  /* creation of task2 */
  task2Handle = osThreadNew(task2_start, NULL, &task2_attributes);

  /* creation of task3 */
  task3Handle = osThreadNew(task3_start, NULL, &task3_attributes);

  /* creation of first_task */
  first_taskHandle = osThreadNew(first_task_begin, NULL, &first_task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */

  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_task1_start */
/**
 * @brief Function implementing the task1 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_task1_start */
void task1_start(void *argument)
{
  /* USER CODE BEGIN task1_start */
  /* Infinite loop */
  for (;;)
  {
    vTaskSuspend(task2Handle);
    task1();
    osDelay(1);
  }
  /* USER CODE END task1_start */
}

/* USER CODE BEGIN Header_task2_start */
/**
 * @brief Function implementing the task2 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_task2_start */
void task2_start(void *argument)
{
  /* USER CODE BEGIN task2_start */
  /* Infinite loop */
  for (;;)
  {
    // printf("任务二解悬\r\n");
    task2();
    osDelay(1);
    // vTaskSuspend(NULL);
  }
  /* USER CODE END task2_start */
}

/* USER CODE BEGIN Header_task3_start */
/**
 * @brief Function implementing the task3 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_task3_start */
void task3_start(void *argument)
{
  /* USER CODE BEGIN task3_start */
  /* Infinite loop */
  for (;;)
  {
    task3();
    osDelay(1);
    vTaskSuspend(NULL);

    // osDelay(1);
  }
  /* USER CODE END task3_start */
}

/* USER CODE BEGIN Header_first_task_begin */
/**
 * @brief Function implementing the first_task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_first_task_begin */
void first_task_begin(void *argument)
{
  /* USER CODE BEGIN first_task_begin */
  /* Infinite loop */
  for (;;)
  {
    init_my_project();
    vTaskSuspend(task1Handle);
    vTaskSuspend(task2Handle);
    vTaskSuspend(task3Handle);
    osDelay(1);
    vTaskSuspend(NULL);
  }
  /* USER CODE END first_task_begin */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

