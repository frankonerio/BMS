/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// #include "delay.h"
#include "spi.h"
#include "modbus.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "socket.h"
#include "wizchip_conf.h"
#include "FreeRTOS.h"
#include "task.h"
#include "BQ769x2Header.h"
#include "utils.h"
#include "bq_common.h"
#include "soc_soh.h"
#include "semphr.h"
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
  Bit_RESET = 0,
  Bit_SET
} BitAction;

typedef enum {
    STATE_WAIT_FOR_ETHERNET,
    STATE_INIT_SOCKET,
    STATE_WAIT_FOR_CLIENT,
    STATE_COMMUNICATE_WITH_CLIENT
} modbus_state_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE BEGIN PD */

// #define PORT_TCPS 5000
#define DATA_BUF_SIZE 2048
uint8_t gDATABUF[DATA_BUF_SIZE];
#define SEPARATOR "=============================================\r\n"
#define WELCOME_MSG "Welcome to STM32Nucleo Ethernet configuration\r\n"
#define NETWORK_MSG "Network configuration:\r\n"
#define IP_MSG "  IP ADDRESS:  %d.%d.%d.%d\r\n"
#define NETMASK_MSG "  NETMASK:     %d.%d.%d.%d\r\n"
#define GW_MSG "  GATEWAY:     %d.%d.%d.%d\r\n"
#define MAC_MSG "  MAC ADDRESS: %x:%x:%x:%x:%x:%x\r\n"
#define GREETING_MSG "Well done guys! Welcome to the IoT world. Bye!\r\n"
#define CONN_ESTABLISHED_MSG "Connection established with remote IP: %d.%d.%d.%d:%d\r\n"
#define SENT_MESSAGE_MSG "Sent a message. Let's close the socket!\r\n"
#define WRONG_RETVAL_MSG "Something went wrong; return value: %d\r\n"
#define WRONG_STATUS_MSG "Something went wrong; STATUS: %d\r\n"
#define LISTEN_ERR_MSG "LISTEN Error!\r\n"

#define PRINT_STR(msg)                                            \
  do                                                              \
  {                                                               \
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 100); \
  } while (0)

#define PRINT_HEADER()                                                            \
  do                                                                              \
  {                                                                               \
    HAL_UART_Transmit(&huart2, (uint8_t *)SEPARATOR, strlen(SEPARATOR), 100);     \
    HAL_UART_Transmit(&huart2, (uint8_t *)WELCOME_MSG, strlen(WELCOME_MSG), 100); \
    HAL_UART_Transmit(&huart2, (uint8_t *)SEPARATOR, strlen(SEPARATOR), 100);     \
  } while (0)

#define PRINT_NETINFO(netInfo)                                                                                             \
  do                                                                                                                       \
  {                                                                                                                        \
    HAL_UART_Transmit(&huart2, (uint8_t *)NETWORK_MSG, strlen(NETWORK_MSG), 100);                                          \
    sprintf(msg, MAC_MSG, netInfo.mac[0], netInfo.mac[1], netInfo.mac[2], netInfo.mac[3], netInfo.mac[4], netInfo.mac[5]); \
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 100);                                                          \
    sprintf(msg, IP_MSG, netInfo.ip[0], netInfo.ip[1], netInfo.ip[2], netInfo.ip[3]);                                      \
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 100);                                                          \
    sprintf(msg, NETMASK_MSG, netInfo.sn[0], netInfo.sn[1], netInfo.sn[2], netInfo.sn[3]);                                 \
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 100);                                                          \
    sprintf(msg, GW_MSG, netInfo.gw[0], netInfo.gw[1], netInfo.gw[2], netInfo.gw[3]);                                      \
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 100);                                                          \
  } while (0)

char msg[60];

#define LISTEN_PORT 502
#define RECEIVE_BUFF_SIZE 128

#define EEPROM_I2C_ADDRESS 0xA0
#define EEPROM_MEM_ADDRESS_SIZE I2C_MEMADD_SIZE_16BIT

// Enable cycle count for STM32
#define DWT_CTRL (*(volatile uint32_t *)0xE0001000)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx;
DMA_HandleTypeDef hdma_i2c1_tx;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim10;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
int16_t statusClient;
uint8_t statusSlave;
uint8_t tcp_error = 0;
uint8_t rtu_error = 0;
uint8_t TX_buffer[128];
uint8_t send_size;
uint8_t RX_buffer[128];
uint8_t recv_size;
uint16_t cntSend = 0;

uint8_t FrameNrBytes = 0;
bool soc_ready = false;

bms_context bms1;
CellSelectRegister selected_cells = {.value = 0x801F}; // Assuming 6 cells are selected
//CellSelectRegister selected_cells = {.value = 0xFFFF};
Soc_coulomb_counting soc_coulomb_count_init;

// Set I.P address of slave

wiz_NetInfo gWIZNETINFO = {
    .mac = {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},
    .ip = {192, 168, 137, 14},
    .sn = {255, 255, 255, 0},
    .gw = {192, 168, 3, 1},
    .dns = {8, 8, 8, 8},
    .dhcp = NETINFO_STATIC};
uint8_t RX_buffer[RX_buffer_length];

uint16_t holding_register[64] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

float ocv_simple[11] = {4.16, 4.07, 3.99, 3.90, 3.82, 3.72, 3.61, 3.53, 3.38, 3.20, 2.85};
float soc_simple[11] = {100.0, 90.0, 80.0, 70.0, 60.0, 50.0, 40.0, 30.0, 20.0, 10.0, 0.0};


TaskHandle_t modbus_task_handle = NULL;
TaskHandle_t soc_task_handle = NULL;
TaskHandle_t read_voltage_task_handle = NULL;
TaskHandle_t bms_state_machine_handle = NULL;
BaseType_t status;

uint8_t sr = 0x00;

SemaphoreHandle_t data_ready_semaphore;
SemaphoreHandle_t soc_ready_semaphore;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM10_Init(void);
/* USER CODE BEGIN PFP */

void wizchip_select(void);
void wizchip_deselect(void);
void wizchip_write(uint8_t wb);
uint8_t wizchip_read();

// Initialize Network information and display it
void network_init(void);
ErrorState PHYStatusCheck(void);
void PrintPHYConf(void);
void DelayIncCnt(void);

static void modbus_comm_task(void *parameters);
static void soc_task(void *parameters);
static void read_voltage_task(void *parameters);
static void bms_state_machine_task(void *parameters);

void delayUS(uint32_t us)
{                                    // Sets the delay in microseconds.
  __HAL_TIM_SET_COUNTER(&htim10, 0); // set the counter value a 0
  while (__HAL_TIM_GET_COUNTER(&htim10) < us)
    ; // wait for the counter to reach the us input in the parameter
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  uint8_t memsize[2][8] = {{2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}};

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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */

  // Enable cycle count for STM32

  DWT_CTRL |= (1 << 0);
  SEGGER_SYSVIEW_Conf();
  SEGGER_SYSVIEW_Start();

  // Reset wiznet module
  HAL_GPIO_WritePin(GPIO_W5500_RESET_GPIO_Port, GPIO_W5500_RESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(GPIO_W5500_RESET_GPIO_Port, GPIO_W5500_RESET_Pin, GPIO_PIN_SET);
  HAL_Delay(1000);

  /* SPI Read & Write callback function */
  reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
  reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);

  ////////////////////////////////////////////////////////////////////////
  /* WIZCHIP SOCKET Buffer initialize */
  if (ctlwizchip(CW_INIT_WIZCHIP, (void *)memsize) == -1)
  {

    while (1)
      ;
  }

  /* Network initialization */
  network_init();
  wizchip_setnetinfo(&gWIZNETINFO);
  // PRINT_NETINFO(gWIZNETINFO);

  ctlnetwork(CN_SET_NETINFO, (void *)&gWIZNETINFO);

  // Configure PHY by software for maximum compatibility
  // so that user can use any model w5500 board
  // else the PINs on the board configure it, which may
  // lead to different configurations in different boards.
  wiz_PhyConf phyconf;

  phyconf.by = PHY_CONFBY_SW;
  phyconf.duplex = PHY_DUPLEX_FULL;
  phyconf.speed = PHY_SPEED_10;     // 10MBps Ethernet link speed
  phyconf.mode = PHY_MODE_AUTONEGO; // best to go with auto-negotiation

  ctlwizchip(CW_SET_PHYCONF, (void *)&phyconf); // PHY Configuration Command
  //*** End Phy Configuration
  // Init Timer
  HAL_TIM_Base_Start(&htim10);
  delayUS(10000);

  // init bms

  bms1.battery_full = 1;
  bms1.charge_enable = 1;
  bms1.discharge_enable = 1;

  // BQ hard and soft reset
  HAL_GPIO_WritePin(BQ_RESET_GPIO_Port, BQ_RESET_Pin, GPIO_PIN_SET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(BQ_RESET_GPIO_Port, BQ_RESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(2000);
  CommandSubcommands(BQ769x2_RESET); // Resets the BQ769x2 registers
  HAL_Delay(1200);
  BQ769x2_Init(); // Configure all of the BQ769x2 register settings
  delayUS(10000);
  CommandSubcommands(SLEEP_DISABLE); // Sleep mode is enabled by default. For this example, Sleep is disabled to
                                     // demonstrate full-speed measurements in Normal mode.

  delayUS(60000);
  delayUS(60000);
  delayUS(60000);
  delayUS(60000);

  uint16_t manuStatus = BQ769x2_ReadRegister(MANUFACTURINGSTATUS);
  uint16_t manuInit = BQ769x2_ReadRegister(MfgStatusInit);
  uint16_t manuData = BQ769x2_ReadRegister(MANU_DATA);
  uint16_t FET_options = BQ769x2_ReadRegister(FETOptions);

  AlarmBits = BQ769x2_ReadAlarmStatus();
  // CommandSubcommands(RESET_PASSQ);
  //CommandSubcommands(ALL_FETS_ON);
  data_ready_semaphore = xSemaphoreCreateBinary();
  data_ready_semaphore = xSemaphoreCreateBinary();

  status = xTaskCreate(modbus_comm_task, "modbus_comm_task", 200, NULL, 2, &modbus_task_handle);
  configASSERT(status == pdPASS);

  status = xTaskCreate(soc_task, "soc_handle", 200, NULL, 3, &soc_task_handle);
  configASSERT(status == pdPASS);

  status = xTaskCreate(read_voltage_task, "read_voltage_task", 200, NULL, 2, &read_voltage_task_handle);
  configASSERT(status == pdPASS);

  status = xTaskCreate(bms_state_machine_task, "bms_state machine_task", 200, (void *)&bms1, 3, &bms_state_machine_handle);
  configASSERT(status == pdPASS);


  vTaskStartScheduler();

  /* USER CODE END 2 */

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 0;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 65535;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_HalfDuplex_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_W5500_RESET_Pin|GPIO_W5500_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_1_Pin|LED_2_Pin|SPI_INT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, BQ_RESET_Pin|BUZZER_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : GPIO_W5500_RESET_Pin GPIO_W5500_CS_Pin */
  GPIO_InitStruct.Pin = GPIO_W5500_RESET_Pin|GPIO_W5500_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_1_Pin LED_2_Pin SPI_INT_Pin */
  GPIO_InitStruct.Pin = LED_1_Pin|LED_2_Pin|SPI_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : BQ_RESET_Pin */
  GPIO_InitStruct.Pin = BQ_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BQ_RESET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BUZZER_Pin */
  GPIO_InitStruct.Pin = BUZZER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BUZZER_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void modbus_comm_task(void *parameters)
{
  bool start_modbus = false;

  PRINT_STR("MODBUS WHILE LOOP\r\n");

  // Keep checking for Ethernet connection before starting Modbus communication
  while (!start_modbus)
  {
    if (PHYStatusCheck() == Ethernet_OK)
    {
      PrintPHYConf();      // Print Ethernet PHY configuration
      start_modbus = true; // Set flag to true to start Modbus communication
    }
    else
    {
      // Wait for a short time before checking again to prevent busy-waiting
      printf("Waiting for Ethernet connection...\r\n");
      vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second before rechecking
    }
  }

  // Now start Modbus communication since Ethernet is connected
  while (start_modbus)
  {
    HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
    printf("\r\nInitializing server socket\r\n");
    vTaskDelay(pdMS_TO_TICKS(500));

    // Parameters in order: socket_id, protocol TCP or UDP, Port number, Flags=0
    // Return value is socket ID on success
    if (socket(1, Sn_MR_TCP, LISTEN_PORT, 0) != 1)
    {
      // Error
      printf("Cannot create Socket!\r\n");
      vTaskDelay(pdMS_TO_TICKS(500)); // Delay before retrying
      continue;                       // Retry initialization
    }

    // Success
    printf("Socket Created Successfully!\r\n");

    uint8_t socket_io_mode = SOCK_IO_BLOCK;
    ctlsocket(1, CS_SET_IOMODE, &socket_io_mode); // Set blocking IO mode

    printf("Start listening on port %d!\r\n", LISTEN_PORT);
    printf("Waiting for a client connection.\r\n");

    // Make it a passive socket (i.e., listen for connection)
    if (listen(1) != SOCK_OK) // Our socket ID is 1 (W5500 has 8 sockets from 0-7)
    {
      // Error
      printf("Cannot listen on port %d", LISTEN_PORT);
      vTaskDelay(pdMS_TO_TICKS(1000)); // Delay before retrying
      continue;                        // Retry initialization
    }

    uint8_t sr = 0x00;                                               // Socket status register
    TickType_t xTimeOut = xTaskGetTickCount() + pdMS_TO_TICKS(5000); // 5-second timeout

    // Wait for a client connection with timeout
    do
    {
      sr = getSn_SR(1);               // Read status reg (SR of socket 1)
      vTaskDelay(pdMS_TO_TICKS(100)); // Allow other tasks to run
    } while (sr != 0x17 && sr != 0x00 && xTaskGetTickCount() < xTimeOut);

    if (sr == 0x00)
    {
      printf("Some error occurred on server socket. Retrying...\r\n");
      vTaskDelay(pdMS_TO_TICKS(1000)); // Delay before retrying
      continue;                        // Retry initialization
    }

    if (sr == 0x17)
    {
      // We come here only when a client has connected.
      // Now we can read data from the socket
      printf("A client connected!\r\n");
      printf("Waiting for Client Data...!\r\n");

      while (1)
      {
        int len = recv(1, RX_buffer, RX_buffer_length);
        if (len == SOCKERR_SOCKSTATUS)
        {
          // Client has disconnected
          printf("Client has disconnected\r\n");
          printf("*** SESSION OVER ***\r\n\r\n");
          break; // Exit client communication loop
        }

        RX_buffer[len] = '\0';
        printf("Received %d bytes from client\r\n", len);
        printf("Data Received: %s", RX_buffer);

        memset(TX_buffer, 0, sizeof(TX_buffer));

        if (xSemaphoreTake(data_ready_semaphore, portMAX_DELAY) == pdTRUE)
        {
        	modbus_receive(RX_buffer, TX_buffer, len, holding_register);
        }

        vTaskDelay(pdMS_TO_TICKS(500));

        printf("\r\nECHO sent back to client\r\n");

        // Look for quit message and quit if received
        if (strcmp((char *)RX_buffer, "QUIT") == 0)
        {
          printf("Received QUIT command from client\r\n");
          printf("Disconnecting...\r\n");
          printf("*** SESSION OVER ***\r\n\r\n");
          disconnect(1); // Disconnect from the client
          break;         // Exit client communication loop
        }

        vTaskDelay(pdMS_TO_TICKS(500)); // Allow other tasks to run
      } // While loop (as long as client is connected)
    } // If block, client connect success
  } // Outer while loop
}

static void soc_task(void *parameters)
{
	bool start_soc = false;
	if (xSemaphoreTake(data_ready_semaphore, portMAX_DELAY) == pdTRUE)
	{
		BQ769x2_ReadAllVoltages(&bms1);
		get_initial_soc();
		CommandSubcommands(RESET_PASSQ);
		start_soc = true;
	}

  while (start_soc)
  {
	soc_ready = false;
    PRINT_STR("TASK2 WHILE LOOP\r\n");
    uint16_t soc = soc_k_update_get(coulumbs);
    holding_register[21] = soc;
    holding_register[22] = coulumbs;
    BQ769x2_ReadPassQ();
    //xSemaphoreGive(soc_ready_semaphore);
    HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
    vTaskDelay(pdMS_TO_TICKS(100));
    // HAL_Delay(500);

    // taskYIELD();
  }
}

static void bms_state_machine_task(void *parameters)

{
  bms_context *bms = (bms_context *)parameters;
  bms->bms_state = BMS_STATE_OFF;
  while (1)
  {
    bms_state_machine(&bms1);
    uint16_t current = BQ769x2_ReadCurrent();
    vTaskDelay(pdMS_TO_TICKS(100));
    // taskYIELD();
  }
}

static void read_voltage_task(void *parameters)
{

	uint16_t counter = 0;
  while (1)
  {

    BQ769x2_ReadAllVoltages(&bms1);
    uint16_t stackVoltage = BQ769x2_ReadVoltage(StackVoltage, &bms1);
    //    holding_register[0] = CellVoltage[0];
    //    holding_register[1] = CellVoltage[1];
    // HAL_Delay(5000);
    // uint16_t alarm;
    // alarm = BQ769x2_ReadAlarmStatus();
    AlarmBits = BQ769x2_ReadAlarmStatus();
        if (AlarmBits & 0x80)
        { // Check if FULLSCAN is complete. If set, new measurements are available
          counter += 1;
        	BQ769x2_ReadAllVoltages(&bms1);
          // float coulombs = BQ769x2_ReadPassQ();
          Pack_Current = BQ769x2_ReadCurrent();
          Temperature[0] = BQ769x2_ReadTemperature(TS1Temperature);
          Temperature[1] = BQ769x2_ReadTemperature(TS3Temperature);

          holding_register[0] = voltage_to_percentage(CellVoltage[0]);
          holding_register[1] = voltage_to_percentage(CellVoltage[1]);
          holding_register[2] = voltage_to_percentage(CellVoltage[2]);
          holding_register[3] = voltage_to_percentage(CellVoltage[3]);
          holding_register[4] = voltage_to_percentage(CellVoltage[4]);
          holding_register[5] = voltage_to_percentage(CellVoltage[5]);
          holding_register[6] = voltage_to_percentage(CellVoltage[6]);
          holding_register[7] = voltage_to_percentage(CellVoltage[7]);
          holding_register[8] = voltage_to_percentage(CellVoltage[15]);



          holding_register[9] = (CellVoltage[0]);
          holding_register[10] = (CellVoltage[1]);
          holding_register[11] = (CellVoltage[2]);
          holding_register[12] = (CellVoltage[3]);
          holding_register[13] = (CellVoltage[4]);
          holding_register[14] = (CellVoltage[5]);
          holding_register[15] = (CellVoltage[6]);
          holding_register[16] = (CellVoltage[7]);
          holding_register[17] = (CellVoltage[15]);

          holding_register[18] = stackVoltage;
          holding_register[19] = BQ769x2_ReadCurrent();
          uint16_t current = BQ769x2_ReadCurrent();
          holding_register[20] = BQ769x2_ReadVoltage(PACKPinVoltage, &bms1);

          DirectCommands(AlarmStatus, 0x0080, W); // Clear the FULLSCAN bit
                                                  //	  			HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);

          //	  				  HAL_Delay(100);

          if(counter > 2){
        	  xSemaphoreGive(data_ready_semaphore);
          }

        }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
void HAL_SYSTICK_Callback()
{
  DelayIncCnt();
}

uint8_t W5500_rxtx(uint8_t data)
{
  uint8_t rxdata;

  HAL_SPI_TransmitReceive(&hspi1, &data, &rxdata, 1, 50);

  return (rxdata);
}

void wizchip_select(void)
{
  W5500_select();
}

void wizchip_deselect(void)
{
  W5500_release();
}

void wizchip_write(uint8_t wb)
{
  W5500_tx(wb);
}

uint8_t wizchip_read()
{
  return W5500_rx();
}

//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// Intialize the network information to be used in WIZCHIP //
/////////////////////////////////////////////////////////////
void network_init(void)
{
  uint8_t tmpstr[6];

  ctlnetwork(CN_SET_NETINFO, (void *)&gWIZNETINFO);

  ctlwizchip(CW_GET_ID, (void *)tmpstr);
}

void UWriteData(const char data)
{
  while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE) == RESET)
    ;

  huart1.Instance->DR = data;
}

int __io_putchar(int ch)
{
  UWriteData(ch);
  return ch;
}

ErrorState PHYStatusCheck(void)
{
  uint8_t tmp;

  do
  {
    printf("\r\nChecking Ethernet Cable Presence ...");
    ctlwizchip(CW_GET_PHYLINK, (void *)&tmp);

    if (tmp == PHY_LINK_OFF)
    {
      printf("NO Cable Connected!");

      HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, 1);
      HAL_Delay(500);
      // HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
      // HAL_Delay(200);
    }
  } while (tmp == PHY_LINK_OFF);

  printf("Good! Cable got connected!");

  return Ethernet_OK;
}

void PrintPHYConf(void)
{
  wiz_PhyConf phyconf;

  ctlwizchip(CW_GET_PHYCONF, (void *)&phyconf);

  if (phyconf.by == PHY_CONFBY_HW)
  {
    printf("\r\nPHY Configured by Hardware Pins");
  }
  else
  {
    printf("\r\nPHY Configured by Registers");
  }

  if (phyconf.mode == PHY_MODE_AUTONEGO)
  {
    printf("\r\nAutonegotiation Enabled");
  }
  else
  {
    printf("\r\nAutonegotiation NOT Enabled");
  }

  if (phyconf.duplex == PHY_DUPLEX_FULL)
  {
    printf("\r\nDuplex Mode: Full");
  }
  else
  {
    printf("\r\nDuplex Mode: Half");
  }

  if (phyconf.speed == PHY_SPEED_10)
  {
    printf("\r\nSpeed: 10Mbps");
  }
  else
  {
    printf("\r\nSpeed: 100Mbps");
  }
}

// Error codes and messages:
// sn   -  Success: The socket number 'sn' passed as a parameter
// 1    -  SOCK_OK
// -1   -  SOCKERR_SOCKNUM  - Invalid socket number
// -5   -  SOCKERR_SOCKMODE - Invalid operation in the socket
// -13  -  SOCKERR_TIMEOUT  - Timeout occurred
// -7   -  SOCKERR_SOCKSTATUS - Invalid socket status for socket operation
// -14  -  SOCKERR_DATALEN    - Zero data length
// -12  -  SOCKERR_IPINVALID   - Wrong server IP address
// -11  -  SOCKERR_PORTZERO    - Server port zero
// -4   -  SOCKERR_SOCKCLOSED  - Socket unexpectedly closed
// -6   -  SOCKERR_SOCKFLAG    - Invalid socket flag
// -1000 -  SOCK_FATAL  - Result is a fatal error about socket process.
// 0    -  SOCK_BUSY         - Socket is busy
// 200  -  SUCCESS_RECEIVE

/* USER CODE END 4 */

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
  if (htim->Instance == TIM6) {
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

#ifdef  USE_FULL_ASSERT
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
