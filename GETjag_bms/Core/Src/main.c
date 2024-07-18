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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
  Bit_RESET = 0,
  Bit_SET
} BitAction;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE BEGIN PD */
#define HTTP_SOCKET 0
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

// BQ76952 definations
#define EEPROM_ADDRESS 0x57 << 1
#define DEV_ADDR 0x10 // BQ769x2 address is 0x10 including R/W bit or 0x8 as 7-bit address
#define CRC_Mode 0    // 0 for disabled, 1 for enabled
#define MAX_BUFFER_SIZE 10
#define R 0  // Read; Used in DirectCommands and Subcommands functions
#define W 1  // Write; Used in DirectCommands and Subcommands functions
#define W2 2 // Write data with two bytes; Used in Subcommands function
#define NEW_I2C_ADDR 0x12

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
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
    33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
    49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63};

TaskHandle_t modbus_task_handle = NULL;
TaskHandle_t task1_handle = NULL;
TaskHandle_t readVoltage_handle = NULL;
BaseType_t status;

uint8_t sr = 0x00;

// BQ78952 Private Variables
uint8_t RX_data[2] = {0x00, 0x00}; // used in several functions to store data read from BQ769x2
uint8_t RX_32Byte[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// used in Subcommands read function
// Global Variables for cell voltages, temperatures, Stack voltage, PACK Pin voltage, LD Pin voltage, CC2 current
uint16_t CellVoltage[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
float Temperature[3] = {0, 0, 0};
uint16_t Stack_Voltage = 0x00;
uint16_t Pack_Voltage = 0x00;
uint16_t LD_Voltage = 0x00;
uint16_t Pack_Current = 0x00;

uint16_t AlarmBits = 0x00;
uint8_t value_SafetyStatusA; // Safety Status Register A
uint8_t value_SafetyStatusB; // Safety Status Register B
uint8_t value_SafetyStatusC; // Safety Status Register C
uint8_t value_PFStatusA;     // Permanent Fail Status Register A
uint8_t value_PFStatusB;     // Permanent Fail Status Register B
uint8_t value_PFStatusC;     // Permanent Fail Status Register C
uint8_t FET_Status;          // FET Status register contents  - Shows states of FETs
uint16_t CB_ActiveCells;     // Cell Balancing Active Cells

uint8_t UV_Fault = 0;             // under-voltage fault state
uint8_t OV_Fault = 0;             // over-voltage fault state
uint8_t SCD_Fault = 0;            // short-circuit fault state
uint8_t OCD_Fault = 0;            // over-current fault state
uint8_t ProtectionsTriggered = 0; // Set to 1 if any protection triggers

uint8_t LD_ON = 0; // Load Detect status bit
uint8_t DSG = 0;   // discharge FET state
uint8_t CHG = 0;   // charge FET state
uint8_t PCHG = 0;  // pre-charge FET state
uint8_t PDSG = 0;  // pre-discharge FET state

uint32_t AccumulatedCharge_Int;  // in BQ769x2_READPASSQ func
uint32_t AccumulatedCharge_Frac; // in BQ769x2_READPASSQ func
uint32_t AccumulatedCharge_Time; // in BQ769x2_READPASSQ func

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
void PHYStatusCheck(void);
void PrintPHYConf(void);
void DelayIncCnt(void);

static void modbus_comm_task(void *parameters);
static void task1_handler(void *parameters);
static void readVoltage_task(void *parameters);

void delayUS(uint32_t us)
{                                    // Sets the delay in microseconds.
  __HAL_TIM_SET_COUNTER(&htim10, 0); // set the counter value a 0
  while (__HAL_TIM_GET_COUNTER(&htim10) < us)
    ; // wait for the counter to reach the us input in the parameter
}

void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count)
{
  uint8_t copyIndex = 0;
  for (copyIndex = 0; copyIndex < count; copyIndex++)
  {
    dest[copyIndex] = source[copyIndex];
  }
}

unsigned char Checksum(unsigned char *ptr, unsigned char len)
// Calculates the checksum when writing to a RAM register. The checksum is the inverse of the sum of the bytes.
{
  unsigned char i;
  unsigned char checksum = 0;

  for (i = 0; i < len; i++)
    checksum += ptr[i];

  checksum = 0xff & ~checksum;

  return (checksum);
}

unsigned char CRC8(unsigned char *ptr, unsigned char len)
// Calculates CRC8 for passed bytes. Used in i2c read and write functions
{
  unsigned char i;
  unsigned char crc = 0;
  while (len-- != 0)
  {
    for (i = 0x80; i != 0; i /= 2)
    {
      if ((crc & 0x80) != 0)
      {
        crc *= 2;
        crc ^= 0x107;
      }
      else
        crc *= 2;

      if ((*ptr & i) != 0)
        crc ^= 0x107;
    }
    ptr++;
  }
  return (crc);
}

void I2C_WriteReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
  uint8_t TX_Buffer[MAX_BUFFER_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#if CRC_Mode
  {
    uint8_t crc_count = 0;
    crc_count = count * 2;
    uint8_t crc1stByteBuffer[3] = {0x10, reg_addr, reg_data[0]};
    unsigned int j;
    unsigned int i;
    uint8_t temp_crc_buffer[3];

    TX_Buffer[0] = reg_data[0];
    TX_Buffer[1] = CRC8(crc1stByteBuffer, 3);

    j = 2;
    for (i = 1; i < count; i++)
    {
      TX_Buffer[j] = reg_data[i];
      j = j + 1;
      temp_crc_buffer[0] = reg_data[i];
      TX_Buffer[j] = CRC8(temp_crc_buffer, 1);
      j = j + 1;
    }
    HAL_I2C_Mem_Write(&hi2c1, DEV_ADDR, reg_addr, 1, TX_Buffer, crc_count, 1000);
  }
#else
  HAL_I2C_Mem_Write(&hi2c1, DEV_ADDR, reg_addr, 1, reg_data, count, HAL_MAX_DELAY);
#endif
}

int I2C_ReadReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
  unsigned int RX_CRC_Fail = 0; // reset to 0. If in CRC Mode and CRC fails, this will be incremented.
  uint8_t RX_Buffer[MAX_BUFFER_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#if CRC_Mode
  {
    uint8_t crc_count = 0;
    uint8_t ReceiveBuffer[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    crc_count = count * 2;
    unsigned int j;
    unsigned int i;
    unsigned char CRCc = 0;
    uint8_t temp_crc_buffer[3];

    HAL_I2C_Mem_Read(&hi2c1, DEV_ADDR, reg_addr, 1, ReceiveBuffer, crc_count, 1000);
    uint8_t crc1stByteBuffer[4] = {0x10, reg_addr, 0x11, ReceiveBuffer[0]};
    CRCc = CRC8(crc1stByteBuffer, 4);
    if (CRCc != ReceiveBuffer[1])
    {
      RX_CRC_Fail += 1;
    }
    RX_Buffer[0] = ReceiveBuffer[0];

    j = 2;
    for (i = 1; i < count; i++)
    {
      RX_Buffer[i] = ReceiveBuffer[j];
      temp_crc_buffer[0] = ReceiveBuffer[j];
      j = j + 1;
      CRCc = CRC8(temp_crc_buffer, 1);
      if (CRCc != ReceiveBuffer[j])
        RX_CRC_Fail += 1;
      j = j + 1;
    }
    CopyArray(RX_Buffer, reg_data, crc_count);
  }
#else
  HAL_I2C_Mem_Read(&hi2c1, DEV_ADDR, reg_addr, 1, reg_data, count, HAL_MAX_DELAY);
#endif
  return 0;
}

void BQ769x2_SetRegister(uint16_t reg_addr, uint32_t reg_data, uint8_t datalen)
{
  uint8_t TX_Buffer[2] = {0x00, 0x00};
  uint8_t TX_RegData[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  // TX_RegData in little endian format
  TX_RegData[0] = reg_addr & 0xff;
  TX_RegData[1] = (reg_addr >> 8) & 0xff;
  TX_RegData[2] = reg_data & 0xff; // 1st byte of data

  switch (datalen)
  {
  case 1: // 1 byte datalength
    I2C_WriteReg(0x3E, TX_RegData, 3);
    delayUS(2000);
    TX_Buffer[0] = Checksum(TX_RegData, 3);
    TX_Buffer[1] = 0x05;              // combined length of register address and data
    I2C_WriteReg(0x60, TX_Buffer, 2); // Write the checksum and length
    delayUS(2000);
    break;
  case 2: // 2 byte datalength
    TX_RegData[3] = (reg_data >> 8) & 0xff;
    I2C_WriteReg(0x3E, TX_RegData, 4);
    delayUS(2000);
    TX_Buffer[0] = Checksum(TX_RegData, 4);
    TX_Buffer[1] = 0x06;              // combined length of register address and data
    I2C_WriteReg(0x60, TX_Buffer, 2); // Write the checksum and length
    delayUS(2000);
    break;
  case 4: // 4 byte datalength, Only used for CCGain and Capacity Gain
    TX_RegData[3] = (reg_data >> 8) & 0xff;
    TX_RegData[4] = (reg_data >> 16) & 0xff;
    TX_RegData[5] = (reg_data >> 24) & 0xff;
    I2C_WriteReg(0x3E, TX_RegData, 6);
    delayUS(2000);
    TX_Buffer[0] = Checksum(TX_RegData, 6);
    TX_Buffer[1] = 0x08;              // combined length of register address and data
    I2C_WriteReg(0x60, TX_Buffer, 2); // Write the checksum and length
    delayUS(2000);
    break;
  }
}

uint16_t BQ769x2_ReadRegister(uint16_t reg_addr)
{
  uint8_t TX_RegAddr[2] = {0}; // Array to hold the register address
  uint8_t RX_Buffer[2] = {0};  // Array to hold the received data

  // Populate the TX_RegAddr array with the register address
  TX_RegAddr[0] = reg_addr & 0xFF;        // Lower byte of register address
  TX_RegAddr[1] = (reg_addr >> 8) & 0xFF; // Upper byte of register address

  // Write the register address to the 0x3E register
  I2C_WriteReg(0x3E, TX_RegAddr, 2);
  delayUS(2000); // Delay to allow time for the write to complete

  // Poll the 0x3E register until it no longer returns 0xFF
  uint8_t statusLow = 0xFF;
  uint8_t statusHigh = 0xFF;
  do
  {
    I2C_ReadReg(0x3E, &statusLow, 1);
    I2C_ReadReg(0x3F, &statusHigh, 1);
  } while (statusLow == 0xFF && statusHigh == 0xFF);

  // HAL_Delay(200);
  //  Read the data from the buffer starting at 0x40
  I2C_ReadReg(0x40, RX_Buffer, 2);

  // Combine the received bytes into a single 16-bit value
  RX_data[0] = RX_Buffer[0];
  RX_data[1] = RX_Buffer[1];
  uint16_t reg_data = (RX_Buffer[1] << 8) | RX_Buffer[0];

  return reg_data;
}

void I2C_WriteReg(uint8_t reg, uint8_t *data, uint8_t length);

// Function to write a register in BQ769x2
void BQ769x2_WriteRegister(uint16_t reg_addr, uint8_t *reg_data, uint8_t datalen)
{
  uint8_t TX_RegData[3]; // Array to hold data to be written (address + data)

  // Convert register address (16-bit) to 2 bytes (little-endian format)
  TX_RegData[0] = reg_addr & 0xFF;        // Lower byte
  TX_RegData[1] = (reg_addr >> 8) & 0xFF; // Upper byte

  // Copy register data to TX buffer
  for (int i = 0; i < datalen; i++)
  {
    TX_RegData[i + 2] = reg_data[i];
  }

  // Write to the BQ769x2 device
  I2C_WriteReg(0x3E, TX_RegData, datalen + 2);
}

void CommandSubcommands(uint16_t command) // For Command only Subcommands
// See the TRM or the BQ76952 header file for a full list of Command-only subcommands
{ // For DEEPSLEEP/SHUTDOWN subcommand you will need to call this function twice consecutively

  uint8_t TX_Reg[2] = {0x00, 0x00};

  // TX_Reg in little endian format
  TX_Reg[0] = command & 0xff;
  TX_Reg[1] = (command >> 8) & 0xff;

  I2C_WriteReg(0x3E, TX_Reg, 2);
  delayUS(2000);
}

void Subcommands(uint16_t command, uint16_t data, uint8_t type)
// See the TRM or the BQ76952 header file for a full list of Subcommands
{
  // security keys and Manu_data writes dont work with this function (reading these commands works)
  // max readback size is 32 bytes i.e. DASTATUS, CUV/COV snapshot
  uint8_t TX_Reg[4] = {0x00, 0x00, 0x00, 0x00};
  uint8_t TX_Buffer[2] = {0x00, 0x00};
  uint8_t lowByte;
  uint8_t highByte;

  I2C_ReadReg(0x3E, &lowByte, 1);
  I2C_ReadReg(0x3F, &highByte, 1);
  // TX_Reg in little endian format
  TX_Reg[0] = command & 0xff;
  TX_Reg[1] = (command >> 8) & 0xff;

  if (type == R)
  { // read
    I2C_WriteReg(0x3E, TX_Reg, 2);
    do
    {
      I2C_ReadReg(0x3E, &lowByte, 1);
      I2C_ReadReg(0x3F, &highByte, 1);
    } while (lowByte == 0xFF && highByte == 0xFF);

    // delayUS(100000);

    I2C_ReadReg(0x40, RX_32Byte, 32); // RX_32Byte is a global variable
  }
  else if (type == W)
  {
    // FET_Control, REG12_Control
    TX_Reg[2] = data & 0xff;
    I2C_WriteReg(0x3E, TX_Reg, 3);
    delayUS(1000);
    TX_Buffer[0] = Checksum(TX_Reg, 3);
    TX_Buffer[1] = 0x05; // combined length of registers address and data
    I2C_WriteReg(0x60, TX_Buffer, 2);
    delayUS(1000);
  }
  else if (type == W2)
  { // write data with 2 bytes
    // CB_Active_Cells, CB_SET_LVL
    TX_Reg[2] = data & 0xff;
    TX_Reg[3] = (data >> 8) & 0xff;
    I2C_WriteReg(0x3E, TX_Reg, 4);
    delayUS(1000);
    TX_Buffer[0] = Checksum(TX_Reg, 4);
    TX_Buffer[1] = 0x06; // combined length of registers address and data
    I2C_WriteReg(0x60, TX_Buffer, 2);
    delayUS(1000);
  }
}

void DirectCommands(uint8_t command, uint16_t data, uint8_t type)
// See the TRM or the BQ76952 header file for a full list of Direct Commands
{ // type: R = read, W = write
  uint8_t TX_data[2] = {0x00, 0x00};

  // little endian format
  TX_data[0] = data & 0xff;
  TX_data[1] = (data >> 8) & 0xff;

  if (type == R)
  {                                   // Read
    I2C_ReadReg(command, RX_data, 2); // RX_data is a global variable
    delayUS(2000);
  }
  if (type == W)
  { // write
    // Control_status, alarm_status, alarm_enable all 2 bytes long
    I2C_WriteReg(command, TX_data, 2);
    delayUS(2000);
  }
}

void BQ769x2_Init()
{
  // Configures all parameters in device RAM

  // Enter CONFIGUPDATE mode (Subcommand 0x0090) - It is required to be in CONFIG_UPDATE mode to program the device RAM settings
  // See TRM for full description of CONFIG_UPDATE mode
  CommandSubcommands(SET_CFGUPDATE);

  // After entering CONFIG_UPDATE mode, RAM registers can be programmed. When programming RAM, checksum and length must also be
  // programmed for the change to take effect. All of the RAM registers are described in detail in the BQ769x2 TRM.
  // An easier way to find the descriptions is in the BQStudio Data Memory screen. When you move the mouse over the register name,
  // a full description of the register and the bits will pop up on the screen.

  // 'Power Config' - 0x9234 = 0x2D80
  // Setting the DSLP_LDO bit allows the LDOs to remain active when the device goes into Deep Sleep mode
  // Set wake speed bits to 00 for best performance
  BQ769x2_SetRegister(PowerConfig, 0x2D80, 2);

  // 'REG0 Config' - set REG0_EN bit to enable pre-regulator
  BQ769x2_SetRegister(REG0Config, 0x00, 1);

  // 'REG12 Config' - Enable REG1 with 3.3V output (0x0D for 3.3V, 0x0F for 5V)
  // BQ769x2_SetRegister(REG12Config, 0x0D, 1);

  // Set DFETOFF pin to control BOTH CHG and DSG FET - 0x92FB = 0x42 (set to 0x00 to disable)
  // BQ769x2_SetRegister(DFETOFFPinConfig, 0x42, 1);

  // Set up ALERT Pin - 0x92FC = 0x2A
  // This configures the ALERT pin to drive high (REG1 voltage) when enabled.
  // The ALERT pin can be used as an interrupt to the MCU when a protection has triggered or new measurements are available
  BQ769x2_SetRegister(ALERTPinConfig, 0x2A, 1);

  // Set TS1 to measure Cell Temperature - 0x92FD = 0x07
  // BQ769x2_SetRegister(TS1Config, 0x07, 1);

  // Set TS3 to measure FET Temperature - 0x92FF = 0x0F
  // BQ769x2_SetRegister(TS3Config, 0x0F, 1);

  // Set HDQ to measure Cell Temperature - 0x9300 = 0x07
  // BQ769x2_SetRegister(HDQPinConfig, 0x00, 1);  // No thermistor installed on EVM HDQ pin, so set to 0x00

  // 'VCell Mode' - Enable 16 cells - 0x9304 = 0x0000; Writing 0x0000 sets the default of 16 cells
  BQ769x2_SetRegister(VCellMode, 0x801F, 2); // default 0x0000

  // Enable protections in 'Enabled Protections A' 0x9261 = 0xBC
  // Enables SCD (short-circuit), OCD1 (over-current in discharge), OCC (over-current in charge),
  // COV (over-voltage), CUV (under-voltage)
  BQ769x2_SetRegister(EnabledProtectionsA, 0xBC, 1);

  // Enable all protections in 'Enabled Protections B' 0x9262 = 0xF7
  // Enables OTF (over-temperature FET), OTINT (internal over-temperature), OTD (over-temperature in discharge),
  // OTC (over-temperature in charge), UTINT (internal under-temperature), UTD (under-temperature in discharge), UTC (under-temperature in charge)
  BQ769x2_SetRegister(EnabledProtectionsB, 0xF7, 1);

  // 'Default Alarm Mask' - 0x..82 Enables the FullScan and ADScan bits, default value = 0xF800
  BQ769x2_SetRegister(DefaultAlarmMask, 0xF882, 2);

  // Set up Cell Balancing Configuration - 0x9335 = 0x03   -  Automated balancing while in Relax or Charge modes
  // Also see "Cell Balancing with BQ769x2 Battery Monitors" document on ti.com
  BQ769x2_SetRegister(BalancingConfiguration, 0x03, 1);

  // Set up CUV (under-voltage) Threshold - 0x9275 = 0x31 (2479 mV)
  // CUV Threshold is this value multiplied by 50.6mV
  BQ769x2_SetRegister(CUVThreshold, 0x23, 1);

  // Set up COV (over-voltage) Threshold - 0x9278 = 0x55 (4301 mV)
  // COV Threshold is this value multiplied by 50.6mV
  BQ769x2_SetRegister(COVThreshold, 0x55, 1);

  // Set up OCC (over-current in charge) Threshold - 0x9280 = 0x05 (10 mV = 10A across 1mOhm sense resistor) Units in 2mV
  BQ769x2_SetRegister(OCCThreshold, 0x05, 1);

  // Set up OCD1 Threshold - 0x9282 = 0x0A (20 mV = 20A across 1mOhm sense resistor) units of 2mV
  BQ769x2_SetRegister(OCD1Threshold, 0x0A, 1);

  // Set up SCD Threshold - 0x9286 = 0x05 (100 mV = 100A across 1mOhm sense resistor)  0x05=100mV
  BQ769x2_SetRegister(SCDThreshold, 0x05, 1);

  // Set up SCD Delay - 0x9287 = 0x03 (30 us) Enabled with a delay of (value - 1) * 15 µs; min value of 1
  BQ769x2_SetRegister(SCDDelay, 0x03, 1);

  // Set up SCDL Latch Limit to 1 to set SCD recovery only with load removal 0x9295 = 0x01
  // If this is not set, then SCD will recover based on time (SCD Recovery Time parameter).
  BQ769x2_SetRegister(SCDLLatchLimit, 0x01, 1);

  // Exit CONFIGUPDATE mode  - Subcommand 0x0092
  CommandSubcommands(EXIT_CFGUPDATE);
}
//  ********************************* FET Control Commands  ***************************************

void BQ769x2_BOTHOFF()
{
  // Disables all FETs using the DFETOFF (BOTHOFF) pin
  // The DFETOFF pin on the BQ76952EVM should be connected to the MCU board to use this function
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // DFETOFF pin (BOTHOFF) set high
}

void BQ769x2_RESET_BOTHOFF()
{
  // Resets DFETOFF (BOTHOFF) pin
  // The DFETOFF pin on the BQ76952EVM should be connected to the MCU board to use this function
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); // DFETOFF pin (BOTHOFF) set low
}

void BQ769x2_ReadFETStatus()
{
  // Read FET Status to see which FETs are enabled
  DirectCommands(FETStatus, 0x00, R);
  FET_Status = (RX_data[1] * 256 + RX_data[0]);
  DSG = ((0x4 & RX_data[0]) >> 2);  // discharge FET state
  CHG = (0x1 & RX_data[0]);         // charge FET state
  PCHG = ((0x2 & RX_data[0]) >> 1); // pre-charge FET state
  PDSG = ((0x8 & RX_data[0]) >> 3); // pre-discharge FET state
}

// ********************************* End of FET Control Commands *********************************

// ********************************* BQ769x2 Power Commands   *****************************************

void BQ769x2_ShutdownPin()
{
  // Puts the device into SHUTDOWN mode using the RST_SHUT pin
  // The RST_SHUT pin on the BQ76952EVM should be connected to the MCU board to use this function
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // Sets RST_SHUT pin
}

void BQ769x2_ReleaseShutdownPin()
{
  // Releases the RST_SHUT pin
  // The RST_SHUT pin on the BQ76952EVM should be connected to the MCU board to use this function
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); // Resets RST_SHUT pin
}

// ********************************* End of BQ769x2 Power Commands   *****************************************

// ********************************* BQ769x2 Status and Fault Commands   *****************************************

uint16_t BQ769x2_ReadAlarmStatus()
{
  // Read this register to find out why the ALERT pin was asserted
  DirectCommands(AlarmStatus, 0x00, R);
  return (RX_data[1] * 256 + RX_data[0]);
}

void BQ769x2_ReadSafetyStatus()
{ // good example functions
  // Read Safety Status A/B/C and find which bits are set
  // This shows which primary protections have been triggered
  DirectCommands(SafetyStatusA, 0x00, R);
  value_SafetyStatusA = (RX_data[1] * 256 + RX_data[0]);
  // Example Fault Flags
  UV_Fault = ((0x4 & RX_data[0]) >> 2);
  OV_Fault = ((0x8 & RX_data[0]) >> 3);
  SCD_Fault = ((0x8 & RX_data[1]) >> 3);
  OCD_Fault = ((0x2 & RX_data[1]) >> 1);
  DirectCommands(SafetyStatusB, 0x00, R);
  value_SafetyStatusB = (RX_data[1] * 256 + RX_data[0]);
  DirectCommands(SafetyStatusC, 0x00, R);
  value_SafetyStatusC = (RX_data[1] * 256 + RX_data[0]);
  if ((value_SafetyStatusA + value_SafetyStatusB + value_SafetyStatusC) > 1)
  {
    ProtectionsTriggered = 1;
  }
  else
  {
    ProtectionsTriggered = 0;
  }
}

void BQ769x2_ReadPFStatus()
{
  // Read Permanent Fail Status A/B/C and find which bits are set
  // This shows which permanent failures have been triggered
  DirectCommands(PFStatusA, 0x00, R);
  value_PFStatusA = (RX_data[1] * 256 + RX_data[0]);
  DirectCommands(PFStatusB, 0x00, R);
  value_PFStatusB = (RX_data[1] * 256 + RX_data[0]);
  DirectCommands(PFStatusC, 0x00, R);
  value_PFStatusC = (RX_data[1] * 256 + RX_data[0]);
}

// ********************************* End of BQ769x2 Status and Fault Commands   *****************************************

// ********************************* BQ769x2 Measurement Commands   *****************************************

uint16_t BQ769x2_ReadVoltage(uint8_t command)
// This function can be used to read a specific cell voltage or stack / pack / LD voltage
{
  // RX_data is global var
  DirectCommands(command, 0x00, R);
  if (command >= Cell1Voltage && command <= Cell16Voltage)
  {                                         // Cells 1 through 16 (0x14 to 0x32)
    return (RX_data[1] * 256 + RX_data[0]); // voltage is reported in mV
  }
  else
  {                                              // stack, Pack, LD
    return 10 * (RX_data[1] * 256 + RX_data[0]); // voltage is reported in 0.01V units
  }
}
void BQ769x2_ReadAllVoltages()
// Reads all cell voltages, Stack voltage, PACK pin voltage, and LD pin voltage
{
  int cellvoltageholder = Cell1Voltage; // Cell1Voltage is 0x14
  for (int x = 0; x < 16; x++)
  { // Reads all cell voltages
    CellVoltage[x] = BQ769x2_ReadVoltage(cellvoltageholder);
    cellvoltageholder = cellvoltageholder + 2;
  }
  Stack_Voltage = BQ769x2_ReadVoltage(StackVoltage);
  Pack_Voltage = BQ769x2_ReadVoltage(PACKPinVoltage);
  LD_Voltage = BQ769x2_ReadVoltage(LDPinVoltage);
}

uint16_t BQ769x2_ReadCurrent()
// Reads PACK current
{
  DirectCommands(CC2Current, 0x00, R);
  return (RX_data[1] * 256 + RX_data[0]); // current is reported in mA
}

float BQ769x2_ReadTemperature(uint8_t command)
{
  DirectCommands(command, 0x00, R);
  // RX_data is a global var
  return (0.1 * (float)(RX_data[1] * 256 + RX_data[0])) - 273.15; // converts from 0.1K to Celcius
}

void BQ769x2_ReadPassQ()
{ // Read Accumulated Charge and Time from DASTATUS6
  Subcommands(DASTATUS6, 0x00, R);
  AccumulatedCharge_Int = ((RX_32Byte[3] << 24) + (RX_32Byte[2] << 16) + (RX_32Byte[1] << 8) + RX_32Byte[0]);    // Bytes 0-3
  AccumulatedCharge_Frac = ((RX_32Byte[7] << 24) + (RX_32Byte[6] << 16) + (RX_32Byte[5] << 8) + RX_32Byte[4]);   // Bytes 4-7
  AccumulatedCharge_Time = ((RX_32Byte[11] << 24) + (RX_32Byte[10] << 16) + (RX_32Byte[9] << 8) + RX_32Byte[8]); // Bytes 8-11
}

uint8_t EEPROM_ReadByte(uint16_t MemAddress)
{
  uint8_t data;
  HAL_StatusTypeDef status;

  // Send the memory address to the EEPROM
  status = HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDRESS, MemAddress, I2C_MEMADD_SIZE_16BIT, &data, 1, HAL_MAX_DELAY);

  // Check if the read was successful
  if (status != HAL_OK)
  {
    // Handle error
    // You can add your error handling code here
  }

  return data;
}

// ********************************* End of BQ769x2 Measurement Commands   *****************************************

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

  HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
  HAL_Delay(200);

  // Create Tasks and Start Scheduler
  // status = xTaskCreate(modbus_comm_task, "Modbus_comm", 200, NULL, 3, &modbus_task_handle);
  // configASSERT(status==pdPASS);

  // status = xTaskCreate(task1_handler, "Task_1", 200, NULL, 1, &task1_handle);
  // configASSERT(status==pdPASS);

  // vTaskStartScheduler();
  /* Chip selection call back */

  // HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
  // HAL_Delay(200);
  HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
  HAL_Delay(200);
  // vTaskDelay(pdMS_TO_TICKS(200));

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
  PRINT_NETINFO(gWIZNETINFO);

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

  PHYStatusCheck();
  PrintPHYConf();
  HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
  HAL_Delay(200);
  HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
  HAL_Delay(200);

  HAL_TIM_Base_Start(&htim10);
  delayUS(10000);
  // CommandSubcommands(BQ769x2_RESET); // Resets the BQ769x2 registers
  // delayUS(60000);
  BQ769x2_Init(); // Configure all of the BQ769x2 register settings
  delayUS(10000);
  CommandSubcommands(FET_ENABLE); // Enable the CHG and DSG FETs
  delayUS(10000);
  CommandSubcommands(SLEEP_DISABLE); // Sleep mode is enabled by default. For this example, Sleep is disabled to
                                     // demonstrate full-speed measurements in Normal mode.

  delayUS(60000);
  delayUS(60000);
  delayUS(60000);
  delayUS(60000);

  status = xTaskCreate(modbus_comm_task, "Modbus_comm", 200, NULL, 3, &modbus_task_handle);
  configASSERT(status == pdPASS);

  status = xTaskCreate(task1_handler, "Led_blink", 200, NULL, 1, &task1_handle);
  configASSERT(status == pdPASS);

  status = xTaskCreate(readVoltage_task, "read_voltage", 200, NULL, 2, &readVoltage_handle);
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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
  HAL_GPIO_WritePin(GPIOA, GPIO_W5500_RESET_Pin | GPIO_W5500_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_1_Pin | LED_2_Pin | SPI_INT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : GPIO_W5500_RESET_Pin GPIO_W5500_CS_Pin */
  GPIO_InitStruct.Pin = GPIO_W5500_RESET_Pin | GPIO_W5500_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_1_Pin LED_2_Pin SPI_INT_Pin */
  GPIO_InitStruct.Pin = LED_1_Pin | LED_2_Pin | SPI_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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

static void modbus_comm_task(void *parameters)
{
  PRINT_STR("MODBUS WHILE LOOP\r\n");

  while (1)
  {
    HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
    printf("\r\nInitializing server socket\r\n");
    vTaskDelay(pdMS_TO_TICKS(100));
    // Parameters in order socket_id, protocol TCP or UDP, Port number, Flags=0
    // Return value is socket ID on success
    if (socket(1, Sn_MR_TCP, LISTEN_PORT, 0) != 1)
    {
      // error
      printf("Cannot create Socket!\r\n");
      vTaskDelay(pdMS_TO_TICKS(200)); // Delay before retrying
      continue;                       // Retry initialization
    }

    // success
    printf("Socket Created Successfully ! \r\n");

    uint8_t socket_io_mode = SOCK_IO_BLOCK;
    ctlsocket(1, CS_SET_IOMODE, &socket_io_mode); // set blocking IO mode

    printf("Start listening on port %d ! \r\n", LISTEN_PORT);
    printf("Waiting for a client connection. \r\n");

    // Make it a passive socket (i.e. listen for connection)
    if (listen(1) != SOCK_OK) // our socket id is 1 (w5500 have 8 sockets from 0-7)
    {
      // error
      printf("Cannot listen on port %d", LISTEN_PORT);
      vTaskDelay(pdMS_TO_TICKS(1000)); // Delay before retrying
      continue;                        // Retry initialization
    }

    uint8_t sr = 0x00;                                               // socket status register
    TickType_t xTimeOut = xTaskGetTickCount() + pdMS_TO_TICKS(5000); // 5-second timeout

    // Wait for a client connection with timeout
    do
    {
      sr = getSn_SR(1);               // read status reg (SR of socket 1)
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
      // we come here only when a client has connected.
      // Now we can read data from the socket
      printf("A client connected!\r\n");
      printf("Waiting for Client Data ...!\r\n");

      while (1)
      {
        int len = recv(1, RX_buffer, RX_buffer_length);
        if (len == SOCKERR_SOCKSTATUS)
        {
          // client has disconnected
          printf("Client has disconnected\r\n");
          printf("*** SESSION OVER ***\r\n\r\n");
          break; // Exit client communication loop
        }

        RX_buffer[len] = '\0';
        printf("Received %d bytes from client\r\n", len);
        printf("Data Received: %s", RX_buffer);

        memset(TX_buffer, 0, sizeof(TX_buffer));
        modbus_receive(RX_buffer, TX_buffer, len, holding_register);
        // holding_register[3] = holding_register[3] + 1;

        printf("\r\nECHO sent back to client\r\n");

        // Look for quit message and quit if received
        if (strcmp((char *)RX_buffer, "QUIT") == 0)
        {
          printf("Received QUIT command from client\r\n");
          printf("Disconnecting ... \r\n");
          printf("*** SESSION OVER ***\r\n\r\n");
          disconnect(1); // disconnect from the client
          break;         // Exit client communication loop
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Allow other tasks to run
      } // While loop (as long as client is connected)
    } // if block, client connect success
  } // outer while loop
}

static void task1_handler(void *parameters)
{

  while (1)
  {
    PRINT_STR("TASK2 WHILE LOOP\r\n");

    HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
    vTaskDelay(pdMS_TO_TICKS(100));
    // HAL_Delay(500);

    // taskYIELD();
  }
}

static void readVoltage_task(void *parameters)
{

  while (1)
  {

    BQ769x2_ReadAllVoltages();
    uint16_t stackVoltage = BQ769x2_ReadVoltage(StackVoltage);
    holding_register[0] = CellVoltage[0];
    holding_register[1] = CellVoltage[1];
    holding_register[2] = CellVoltage[2];
    holding_register[3] = CellVoltage[3];
    holding_register[4] = CellVoltage[4];
    holding_register[5] = CellVoltage[15];
    holding_register[6] = stackVoltage;
    holding_register[7] = BQ769x2_ReadCurrent();
    holding_register[8] = BQ769x2_ReadVoltage(PACKPinVoltage);

    // HAL_Delay(5000);
    uint16_t alarm;
    alarm = BQ769x2_ReadAlarmStatus();
    AlarmBits = BQ769x2_ReadAlarmStatus();
    if (AlarmBits & 0x80)
    { // Check if FULLSCAN is complete. If set, new measurements are available
      BQ769x2_ReadAllVoltages();
      Pack_Current = BQ769x2_ReadCurrent();
      // Temperature[0] = BQ769x2_ReadTemperature(TS1Temperature);
      // Temperature[1] = BQ769x2_ReadTemperature(TS3Temperature);
      DirectCommands(AlarmStatus, 0x0080, W); // Clear the FULLSCAN bit
                                              //	  			HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
                                              //	  				  HAL_Delay(100);
    }
    else
    {
    }

    vTaskDelay(pdMS_TO_TICKS(500));
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

void PHYStatusCheck(void)
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
