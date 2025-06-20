/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "logger.h"
#include "uart.h"
#include "console.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticSemaphore_t osStaticMutexDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

COMP_HandleTypeDef hcomp2;
COMP_HandleTypeDef hcomp3;
COMP_HandleTypeDef hcomp4;
COMP_HandleTypeDef hcomp6;

DAC_HandleTypeDef hdac1;
DAC_HandleTypeDef hdac2;
DAC_HandleTypeDef hdac3;

HRTIM_HandleTypeDef hhrtim1;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_FS;

/* Definitions for task0 */
osThreadId_t task0Handle;
uint32_t task0Buffer[128];
osStaticThreadDef_t task0ControlBlock;
const osThreadAttr_t task0_attributes = {
	.name = "task0",
	.cb_mem = &task0ControlBlock,
	.cb_size = sizeof(task0ControlBlock),
	.stack_mem = &task0Buffer[0],
	.stack_size = sizeof(task0Buffer),
	.priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for task1 */
osThreadId_t task1Handle;
uint32_t task1Buffer[128];
osStaticThreadDef_t task1ControlBlock;
const osThreadAttr_t task1_attributes = {
	.name = "task1",
	.cb_mem = &task1ControlBlock,
	.cb_size = sizeof(task1ControlBlock),
	.stack_mem = &task1Buffer[0],
	.stack_size = sizeof(task1Buffer),
	.priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for task2 */
osThreadId_t task2Handle;
uint32_t task2Buffer[128];
osStaticThreadDef_t task2ControlBlock;
const osThreadAttr_t task2_attributes = {
	.name = "task2",
	.cb_mem = &task2ControlBlock,
	.cb_size = sizeof(task2ControlBlock),
	.stack_mem = &task2Buffer[0],
	.stack_size = sizeof(task2Buffer),
	.priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for task3 */
osThreadId_t task3Handle;
uint32_t task3Buffer[128];
osStaticThreadDef_t task3ControlBlock;
const osThreadAttr_t task3_attributes = {
	.name = "task3",
	.cb_mem = &task3ControlBlock,
	.cb_size = sizeof(task3ControlBlock),
	.stack_mem = &task3Buffer[0],
	.stack_size = sizeof(task3Buffer),
	.priority = (osPriority_t)osPriorityBelowNormal,
};
/* Definitions for loggerTask */
osThreadId_t loggerTaskHandle;
uint32_t loggerTaskBuffer[256];
osStaticThreadDef_t loggerTaskControlBlock;
const osThreadAttr_t loggerTask_attributes = {
	.name = "loggerTask",
	.cb_mem = &loggerTaskControlBlock,
	.cb_size = sizeof(loggerTaskControlBlock),
	.stack_mem = &loggerTaskBuffer[0],
	.stack_size = sizeof(loggerTaskBuffer),
	.priority = (osPriority_t)osPriorityLow,
};
/* Definitions for consoleTask */
osThreadId_t consoleTaskHandle;
uint32_t consoleTaskBuffer[128];
osStaticThreadDef_t consoleTaskControlBlock;
const osThreadAttr_t consoleTask_attributes = {
	.name = "consoleTask",
	.cb_mem = &consoleTaskControlBlock,
	.cb_size = sizeof(consoleTaskControlBlock),
	.stack_mem = &consoleTaskBuffer[0],
	.stack_size = sizeof(consoleTaskBuffer),
	.priority = (osPriority_t)osPriorityLow,
};
/* Definitions for queueLogger */
osMessageQueueId_t queueLoggerHandle;
uint8_t queueLoggerBuffer[32 * sizeof(LogMessage)];
osStaticMessageQDef_t queueLoggerControlBlock;
const osMessageQueueAttr_t queueLogger_attributes = {
	.name = "queueLogger",
	.cb_mem = &queueLoggerControlBlock,
	.cb_size = sizeof(queueLoggerControlBlock),
	.mq_mem = &queueLoggerBuffer,
	.mq_size = sizeof(queueLoggerBuffer)};
/* Definitions for queueUart */
osMessageQueueId_t queueUartHandle;
uint8_t queueUartBuffer[64 * sizeof(uint8_t)];
osStaticMessageQDef_t queueUartControlBlock;
const osMessageQueueAttr_t queueUart_attributes = {
	.name = "queueUart",
	.cb_mem = &queueUartControlBlock,
	.cb_size = sizeof(queueUartControlBlock),
	.mq_mem = &queueUartBuffer,
	.mq_size = sizeof(queueUartBuffer)};
/* Definitions for mutexUart */
osMutexId_t mutexUartHandle;
osStaticMutexDef_t mutexUartControlBlock;
const osMutexAttr_t mutexUart_attributes = {
	.name = "mutexUart",
	.cb_mem = &mutexUartControlBlock,
	.cb_size = sizeof(mutexUartControlBlock),
};
/* Definitions for semUart */
osSemaphoreId_t semUartHandle;
osStaticSemaphoreDef_t semUartControlBlock;
const osSemaphoreAttr_t semUart_attributes = {
	.name = "semUart",
	.cb_mem = &semUartControlBlock,
	.cb_size = sizeof(semUartControlBlock),
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_COMP2_Init(void);
static void MX_COMP3_Init(void);
static void MX_COMP4_Init(void);
static void MX_COMP6_Init(void);
static void MX_DAC1_Init(void);
static void MX_DAC2_Init(void);
static void MX_DAC3_Init(void);
static void MX_HRTIM1_Init(void);
static void MX_UCPD1_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_PCD_Init(void);
void startTask0(void *argument);
void startTask1(void *argument);
void startTask2(void *argument);
void startTask3(void *argument);
void startLoggerTask(void *argument);
void startConsoleTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

Uart uart(&huart3);
Console console(uart);

extern "C" int _write(int file, char *ptr, int len)
{
	(void)file;
	uart.transmit(ptr, len);
	return len;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_ADC1_Init();
	MX_COMP2_Init();
	MX_COMP3_Init();
	MX_COMP4_Init();
	MX_COMP6_Init();
	MX_DAC1_Init();
	MX_DAC2_Init();
	MX_DAC3_Init();
	MX_HRTIM1_Init();
	MX_UCPD1_Init();
	MX_USART3_UART_Init();
	MX_USB_PCD_Init();
	/* USER CODE BEGIN 2 */

	LOG_init(&queueLoggerHandle);
	uart.setHandleMutexTx(&mutexUartHandle);
	uart.setHandleQueueRx(&queueUartHandle);
	uart.setHandleSemTx(&semUartHandle);
	uart.init();

	/* USER CODE END 2 */

	/* Init scheduler */
	osKernelInitialize();
	/* Create the mutex(es) */
	/* creation of mutexUart */
	mutexUartHandle = osMutexNew(&mutexUart_attributes);

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	semUartHandle = osSemaphoreNew(1, 0, &semUart_attributes);

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* Create the queue(s) */
	/* creation of queueLogger */
	queueLoggerHandle = osMessageQueueNew(32, sizeof(LogMessage), &queueLogger_attributes);

	queueUartHandle = osMessageQueueNew(64, sizeof(uint8_t), &queueUart_attributes);

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of task0 */
	task0Handle = osThreadNew(startTask0, NULL, &task0_attributes);

	/* creation of task1 */
	task1Handle = osThreadNew(startTask1, NULL, &task1_attributes);

	/* creation of task2 */
	task2Handle = osThreadNew(startTask2, NULL, &task2_attributes);

	/* creation of task3 */
	task3Handle = osThreadNew(startTask3, NULL, &task3_attributes);

	/* creation of loggerTask */
	loggerTaskHandle = osThreadNew(startLoggerTask, NULL, &loggerTask_attributes);

	consoleTaskHandle = osThreadNew(startConsoleTask, NULL, &consoleTask_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
	RCC_OscInitStruct.PLL.PLLN = 85;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV8;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV8;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void)
{

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_MultiModeTypeDef multimode = {0};
	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */

	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.GainCompensation = 0;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc1.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure the ADC multi-mode
	 */
	multimode.Mode = ADC_MODE_INDEPENDENT;
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */
}

/**
 * @brief COMP2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_COMP2_Init(void)
{

	/* USER CODE BEGIN COMP2_Init 0 */

	/* USER CODE END COMP2_Init 0 */

	/* USER CODE BEGIN COMP2_Init 1 */

	/* USER CODE END COMP2_Init 1 */
	hcomp2.Instance = COMP2;
	hcomp2.Init.InputPlus = COMP_INPUT_PLUS_IO1;
	hcomp2.Init.InputMinus = COMP_INPUT_MINUS_DAC3_CH2;
	hcomp2.Init.OutputPol = COMP_OUTPUTPOL_NONINVERTED;
	hcomp2.Init.Hysteresis = COMP_HYSTERESIS_NONE;
	hcomp2.Init.BlankingSrce = COMP_BLANKINGSRC_NONE;
	hcomp2.Init.TriggerMode = COMP_TRIGGERMODE_NONE;
	if (HAL_COMP_Init(&hcomp2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN COMP2_Init 2 */

	/* USER CODE END COMP2_Init 2 */
}

/**
 * @brief COMP3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_COMP3_Init(void)
{

	/* USER CODE BEGIN COMP3_Init 0 */

	/* USER CODE END COMP3_Init 0 */

	/* USER CODE BEGIN COMP3_Init 1 */

	/* USER CODE END COMP3_Init 1 */
	hcomp3.Instance = COMP3;
	hcomp3.Init.InputPlus = COMP_INPUT_PLUS_IO1;
	hcomp3.Init.InputMinus = COMP_INPUT_MINUS_DAC3_CH1;
	hcomp3.Init.OutputPol = COMP_OUTPUTPOL_NONINVERTED;
	hcomp3.Init.Hysteresis = COMP_HYSTERESIS_NONE;
	hcomp3.Init.BlankingSrce = COMP_BLANKINGSRC_NONE;
	hcomp3.Init.TriggerMode = COMP_TRIGGERMODE_NONE;
	if (HAL_COMP_Init(&hcomp3) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN COMP3_Init 2 */

	/* USER CODE END COMP3_Init 2 */
}

/**
 * @brief COMP4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_COMP4_Init(void)
{

	/* USER CODE BEGIN COMP4_Init 0 */

	/* USER CODE END COMP4_Init 0 */

	/* USER CODE BEGIN COMP4_Init 1 */

	/* USER CODE END COMP4_Init 1 */
	hcomp4.Instance = COMP4;
	hcomp4.Init.InputPlus = COMP_INPUT_PLUS_IO1;
	hcomp4.Init.InputMinus = COMP_INPUT_MINUS_DAC1_CH1;
	hcomp4.Init.OutputPol = COMP_OUTPUTPOL_NONINVERTED;
	hcomp4.Init.Hysteresis = COMP_HYSTERESIS_NONE;
	hcomp4.Init.BlankingSrce = COMP_BLANKINGSRC_NONE;
	hcomp4.Init.TriggerMode = COMP_TRIGGERMODE_NONE;
	if (HAL_COMP_Init(&hcomp4) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN COMP4_Init 2 */

	/* USER CODE END COMP4_Init 2 */
}

/**
 * @brief COMP6 Initialization Function
 * @param None
 * @retval None
 */
static void MX_COMP6_Init(void)
{

	/* USER CODE BEGIN COMP6_Init 0 */

	/* USER CODE END COMP6_Init 0 */

	/* USER CODE BEGIN COMP6_Init 1 */

	/* USER CODE END COMP6_Init 1 */
	hcomp6.Instance = COMP6;
	hcomp6.Init.InputPlus = COMP_INPUT_PLUS_IO1;
	hcomp6.Init.InputMinus = COMP_INPUT_MINUS_DAC2_CH1;
	hcomp6.Init.OutputPol = COMP_OUTPUTPOL_NONINVERTED;
	hcomp6.Init.Hysteresis = COMP_HYSTERESIS_NONE;
	hcomp6.Init.BlankingSrce = COMP_BLANKINGSRC_NONE;
	hcomp6.Init.TriggerMode = COMP_TRIGGERMODE_NONE;
	if (HAL_COMP_Init(&hcomp6) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN COMP6_Init 2 */

	/* USER CODE END COMP6_Init 2 */
}

/**
 * @brief DAC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_DAC1_Init(void)
{

	/* USER CODE BEGIN DAC1_Init 0 */

	/* USER CODE END DAC1_Init 0 */

	DAC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN DAC1_Init 1 */

	/* USER CODE END DAC1_Init 1 */

	/** DAC Initialization
	 */
	hdac1.Instance = DAC1;
	if (HAL_DAC_Init(&hdac1) != HAL_OK)
	{
		Error_Handler();
	}

	/** DAC channel OUT1 config
	 */
	sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
	sConfig.DAC_DMADoubleDataMode = DISABLE;
	sConfig.DAC_SignedFormat = DISABLE;
	sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_Trigger2 = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
	sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_INTERNAL;
	sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
	if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN DAC1_Init 2 */

	/* USER CODE END DAC1_Init 2 */
}

/**
 * @brief DAC2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_DAC2_Init(void)
{

	/* USER CODE BEGIN DAC2_Init 0 */

	/* USER CODE END DAC2_Init 0 */

	DAC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN DAC2_Init 1 */

	/* USER CODE END DAC2_Init 1 */

	/** DAC Initialization
	 */
	hdac2.Instance = DAC2;
	if (HAL_DAC_Init(&hdac2) != HAL_OK)
	{
		Error_Handler();
	}

	/** DAC channel OUT1 config
	 */
	sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
	sConfig.DAC_DMADoubleDataMode = DISABLE;
	sConfig.DAC_SignedFormat = DISABLE;
	sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_Trigger2 = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
	sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_INTERNAL;
	sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
	if (HAL_DAC_ConfigChannel(&hdac2, &sConfig, DAC_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN DAC2_Init 2 */

	/* USER CODE END DAC2_Init 2 */
}

/**
 * @brief DAC3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_DAC3_Init(void)
{

	/* USER CODE BEGIN DAC3_Init 0 */

	/* USER CODE END DAC3_Init 0 */

	DAC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN DAC3_Init 1 */

	/* USER CODE END DAC3_Init 1 */

	/** DAC Initialization
	 */
	hdac3.Instance = DAC3;
	if (HAL_DAC_Init(&hdac3) != HAL_OK)
	{
		Error_Handler();
	}

	/** DAC channel OUT1 config
	 */
	sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
	sConfig.DAC_DMADoubleDataMode = DISABLE;
	sConfig.DAC_SignedFormat = DISABLE;
	sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_Trigger2 = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
	sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_INTERNAL;
	sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
	if (HAL_DAC_ConfigChannel(&hdac3, &sConfig, DAC_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	/** DAC channel OUT2 config
	 */
	if (HAL_DAC_ConfigChannel(&hdac3, &sConfig, DAC_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN DAC3_Init 2 */

	/* USER CODE END DAC3_Init 2 */
}

/**
 * @brief HRTIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_HRTIM1_Init(void)
{

	/* USER CODE BEGIN HRTIM1_Init 0 */

	/* USER CODE END HRTIM1_Init 0 */

	HRTIM_TimeBaseCfgTypeDef pTimeBaseCfg = {0};
	HRTIM_TimerCtlTypeDef pTimerCtl = {0};
	HRTIM_TimerCfgTypeDef pTimerCfg = {0};
	HRTIM_OutputCfgTypeDef pOutputCfg = {0};

	/* USER CODE BEGIN HRTIM1_Init 1 */

	/* USER CODE END HRTIM1_Init 1 */
	hhrtim1.Instance = HRTIM1;
	hhrtim1.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
	hhrtim1.Init.SyncOptions = HRTIM_SYNCOPTION_NONE;
	if (HAL_HRTIM_Init(&hhrtim1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_DLLCalibrationStart(&hhrtim1, HRTIM_CALIBRATIONRATE_3) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_PollForDLLCalibration(&hhrtim1, 10) != HAL_OK)
	{
		Error_Handler();
	}
	pTimeBaseCfg.Period = 0xFFDF;
	pTimeBaseCfg.RepetitionCounter = 0x00;
	pTimeBaseCfg.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
	pTimeBaseCfg.Mode = HRTIM_MODE_CONTINUOUS;
	if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, &pTimeBaseCfg) != HAL_OK)
	{
		Error_Handler();
	}
	pTimerCtl.UpDownMode = HRTIM_TIMERUPDOWNMODE_UP;
	pTimerCtl.DualChannelDacEnable = HRTIM_TIMER_DCDE_DISABLED;
	if (HAL_HRTIM_WaveformTimerControl(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, &pTimerCtl) != HAL_OK)
	{
		Error_Handler();
	}
	pTimerCfg.InterruptRequests = HRTIM_TIM_IT_NONE;
	pTimerCfg.DMARequests = HRTIM_TIM_DMA_NONE;
	pTimerCfg.DMASrcAddress = 0x0000;
	pTimerCfg.DMADstAddress = 0x0000;
	pTimerCfg.DMASize = 0x1;
	pTimerCfg.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
	pTimerCfg.InterleavedMode = HRTIM_INTERLEAVED_MODE_DISABLED;
	pTimerCfg.StartOnSync = HRTIM_SYNCSTART_DISABLED;
	pTimerCfg.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
	pTimerCfg.DACSynchro = HRTIM_DACSYNC_NONE;
	pTimerCfg.PreloadEnable = HRTIM_PRELOAD_DISABLED;
	pTimerCfg.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
	pTimerCfg.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
	pTimerCfg.RepetitionUpdate = HRTIM_UPDATEONREPETITION_DISABLED;
	pTimerCfg.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
	pTimerCfg.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;
	pTimerCfg.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
	pTimerCfg.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
	pTimerCfg.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
	pTimerCfg.UpdateTrigger = HRTIM_TIMUPDATETRIGGER_NONE;
	pTimerCfg.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
	pTimerCfg.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
	pTimerCfg.ReSyncUpdate = HRTIM_TIMERESYNC_UPDATE_UNCONDITIONAL;
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, &pTimerCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_C, &pTimerCfg) != HAL_OK)
	{
		Error_Handler();
	}
	pTimerCfg.DelayedProtectionMode = HRTIM_TIMER_D_E_DELAYEDPROTECTION_DISABLED;
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, &pTimerCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_E, &pTimerCfg) != HAL_OK)
	{
		Error_Handler();
	}
	pTimerCfg.DelayedProtectionMode = HRTIM_TIMER_F_DELAYEDPROTECTION_DISABLED;
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F, &pTimerCfg) != HAL_OK)
	{
		Error_Handler();
	}
	pOutputCfg.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
	pOutputCfg.SetSource = HRTIM_OUTPUTSET_NONE;
	pOutputCfg.ResetSource = HRTIM_OUTPUTRESET_NONE;
	pOutputCfg.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
	pOutputCfg.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
	pOutputCfg.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_NONE;
	pOutputCfg.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
	pOutputCfg.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA1, &pOutputCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_C, HRTIM_OUTPUT_TC1, &pOutputCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD1, &pOutputCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_E, HRTIM_OUTPUT_TE1, &pOutputCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F, HRTIM_OUTPUT_TF1, &pOutputCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_C, &pTimeBaseCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformTimerControl(&hhrtim1, HRTIM_TIMERINDEX_TIMER_C, &pTimerCtl) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_C, HRTIM_OUTPUT_TC2, &pOutputCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD2, &pOutputCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F, HRTIM_OUTPUT_TF2, &pOutputCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, &pTimeBaseCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformTimerControl(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, &pTimerCtl) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_E, &pTimeBaseCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformTimerControl(&hhrtim1, HRTIM_TIMERINDEX_TIMER_E, &pTimerCtl) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F, &pTimeBaseCfg) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HRTIM_WaveformTimerControl(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F, &pTimerCtl) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN HRTIM1_Init 2 */

	/* USER CODE END HRTIM1_Init 2 */
	HAL_HRTIM_MspPostInit(&hhrtim1);
}

/**
 * @brief UCPD1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_UCPD1_Init(void)
{

	/* USER CODE BEGIN UCPD1_Init 0 */

	/* USER CODE END UCPD1_Init 0 */

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Peripheral clock enable */
	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_UCPD1);

	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
	/**UCPD1 GPIO Configuration
	PB4   ------> UCPD1_CC2
	PB6   ------> UCPD1_CC1
	*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USER CODE BEGIN UCPD1_Init 1 */

	/* USER CODE END UCPD1_Init 1 */
	/* USER CODE BEGIN UCPD1_Init 2 */

	/* USER CODE END UCPD1_Init 2 */
}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART3_UART_Init(void)
{

	/* USER CODE BEGIN USART3_Init 0 */

	/* USER CODE END USART3_Init 0 */

	/* USER CODE BEGIN USART3_Init 1 */

	/* USER CODE END USART3_Init 1 */
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart3.Init.ClockPrescaler = UART_PRESCALER_DIV4;
	huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart3) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART3_Init 2 */

	/* USER CODE END USART3_Init 2 */
}

/**
 * @brief USB Initialization Function
 * @param None
 * @retval None
 */
static void MX_USB_PCD_Init(void)
{

	/* USER CODE BEGIN USB_Init 0 */

	/* USER CODE END USB_Init 0 */

	/* USER CODE BEGIN USB_Init 1 */

	/* USER CODE END USB_Init 1 */
	hpcd_USB_FS.Instance = USB;
	hpcd_USB_FS.Init.dev_endpoints = 8;
	hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
	hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
	hpcd_USB_FS.Init.Sof_enable = DISABLE;
	hpcd_USB_FS.Init.low_power_enable = DISABLE;
	hpcd_USB_FS.Init.lpm_enable = DISABLE;
	hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
	if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USB_Init 2 */

	/* USER CODE END USB_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, BUCKBOOST_LOAD_1_Pin | BUCKBOOST_LOAD_2_Pin | BUCKBOOST_USBPD_EN_Pin | USBPD_1A_PROTECT_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, LED_ORANGE_Pin | LED_RED_Pin | LED_GREEN_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(USBPD_550mA_PROTECT_GPIO_Port, USBPD_550mA_PROTECT_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : JOYSTICK_SEL_Pin JOYSTICK_LEFT_Pin JOYSTICK_DOWN_Pin */
	GPIO_InitStruct.Pin = JOYSTICK_SEL_Pin | JOYSTICK_LEFT_Pin | JOYSTICK_DOWN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : BUCKBOOST_LOAD_1_Pin BUCKBOOST_LOAD_2_Pin BUCKBOOST_USBPD_EN_Pin USBPD_1A_PROTECT_Pin */
	GPIO_InitStruct.Pin = BUCKBOOST_LOAD_1_Pin | BUCKBOOST_LOAD_2_Pin | BUCKBOOST_USBPD_EN_Pin | USBPD_1A_PROTECT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : LED_ORANGE_Pin LED_RED_Pin LED_GREEN_Pin */
	GPIO_InitStruct.Pin = LED_ORANGE_Pin | LED_RED_Pin | LED_GREEN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : JOYSTICK_RIGHT_Pin JOYSTICK_UP_Pin */
	GPIO_InitStruct.Pin = JOYSTICK_RIGHT_Pin | JOYSTICK_UP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : LED_BLUE_Pin */
	GPIO_InitStruct.Pin = LED_BLUE_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_BLUE_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : USBPD_550mA_PROTECT_Pin */
	GPIO_InitStruct.Pin = USBPD_550mA_PROTECT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(USBPD_550mA_PROTECT_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

	HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	uart.handleTxInterrupt(huart);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uart.handleRxInterrupt(huart);
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_startTask0 */
/**
 * @brief  Function implementing the task0 thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_startTask0 */
void startTask0(void *argument)
{
	/* USER CODE BEGIN 5 */
	osDelay(200);
	/* Infinite loop */
	for (;;)
	{
		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
		// LOG_INFO("blue LED toggled");
		osDelay(500); // Toggle LED every 500 ms
	}
	/* USER CODE END 5 */
}

/* USER CODE BEGIN Header_startTask1 */
/**
 * @brief Function implementing the task1 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startTask1 */
void startTask1(void *argument)
{
	/* USER CODE BEGIN startTask1 */
	osDelay(400);
	/* Infinite loop */
	for (;;)
	{
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		// LOG_INFO("green LED toggled");
		osDelay(500); // Toggle LED every 500 ms
	}
	/* USER CODE END startTask1 */
}

/* USER CODE BEGIN Header_startTask2 */
/**
 * @brief Function implementing the task2 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startTask2 */
void startTask2(void *argument)
{
	/* USER CODE BEGIN startTask2 */
	osDelay(600);
	/* Infinite loop */
	for (;;)
	{
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		// LOG_INFO("red LED toggled");
		osDelay(500); // Toggle LED every 500 ms
	}
	/* USER CODE END startTask2 */
}

/* USER CODE BEGIN Header_startTask3 */
/**
 * @brief Function implementing the task3 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startTask3 */
void startTask3(void *argument)
{
	/* USER CODE BEGIN startTask3 */
	osDelay(800);
	/* Infinite loop */
	for (;;)
	{
		HAL_GPIO_TogglePin(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
		// LOG_INFO("orange LED toggled");
		osDelay(500); // Toggle LED every 500 ms
	}
	/* USER CODE END startTask3 */
}

/* USER CODE BEGIN Header_startLoggerTask */
/**
 * @brief Function implementing the loggerTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startLoggerTask */
void startLoggerTask(void *argument)
{
	/* USER CODE BEGIN startLoggerTask */
	/* Infinite loop */
	for (;;)
	{
		LOG_task();
	}
	/* USER CODE END startLoggerTask */
}



/* USER CODE BEGIN Header_startConsoleTask */

/**
 * @brief Function implementing the consoleTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startConsoleTask */
void startConsoleTask(void *argument)
{
	for (;;)
	{
		console.run();
		osDelay(10); // Allow other tasks to run
	}
	/* USER CODE END startConsoleTask */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM6)
	{
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
