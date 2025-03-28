/*
 * BQ_common.h
 *
 *  Created on: Jul 28, 2024
 *      Author: frank
 */

#ifndef INC_BQ_COMMON_H_
#define INC_BQ_COMMON_H_

#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"
#include "BQ769x2Header.h"
#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;
extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

// BQ76952 definations
#define EEPROM_ADDRESS 0x57 << 1
#define DEV_ADDR 0x10 // BQ769x2 address is 0x10 including R/W bit or 0x8 as 7-bit address
#define CRC_Mode 0    // 0 for disabled, 1 for enabled
#define MAX_BUFFER_SIZE 10
#define R 0  // Read; Used in DirectCommands and Subcommands functions
#define W 1  // Write; Used in DirectCommands and Subcommands functions
#define W2 2 // Write data with two bytes; Used in Subcommands function
#define NEW_I2C_ADDR 0x12
#define FRAC_DIVISOR 0xFFFFFFFFUL
// External variable declarations
extern uint8_t RX_data[2];
extern uint8_t RX_32Byte[32];
extern uint16_t CellVoltage[16];
extern float Temperature[3];
extern uint16_t Stack_Voltage;
extern uint16_t Pack_Voltage;
extern uint16_t LD_Voltage;
extern uint16_t Pack_Current;

extern uint16_t AlarmBits;
extern uint8_t value_SafetyStatusA;
extern uint8_t value_SafetyStatusB;
extern uint8_t value_SafetyStatusC;
extern uint8_t value_PFStatusA;
extern uint8_t value_PFStatusB;
extern uint8_t value_PFStatusC;
extern uint8_t FET_Status;
extern uint16_t CB_ActiveCells;

extern uint8_t UV_Fault;
extern uint8_t OV_Fault;
extern uint8_t SCD_Fault;
extern uint8_t OCD_Fault;
extern uint8_t ProtectionsTriggered;

extern uint8_t LD_ON;
extern uint8_t DSG;
extern uint8_t CHG;
extern uint8_t PCHG;
extern uint8_t PDSG;

extern int32_t AccumulatedCharge_Int; // in BQ769x2_READPASSQ func
extern int32_t AccumulatedCharge_Frac;// in BQ769x2_READPASSQ func
extern int32_t AccumulatedCharge_Time;// in BQ769x2_READPASSQ func

extern float coulumbs;

typedef enum
{
	Ethernet_OK,

} ErrorState;

typedef enum
{
	BMS_STATE_OFF,      ///< Off state (charging and discharging disabled)
	BMS_STATE_CHG,      ///< Charging state (discharging disabled)
	BMS_STATE_DIS,      ///< Discharging state (charging disabled)
	BMS_STATE_NORMAL,   ///< Normal operating mode (both charging and discharging enabled)
	BMS_STATE_SHUTDOWN, ///< BMS starting shutdown sequence

} bms_state;

typedef struct
{
	bms_state bms_state;

	/** Manual enable/disable setting for charging */
	bool charge_enable;
	/** Manual enable/disable setting for discharging */
	bool discharge_enable;

	/** CV charging to cell_chg_voltage_limit finished */
	bool battery_full;
	/** Battery is discharged below cell_dis_voltage_limit */
	bool battery_empty;

	/** Calculated State of Charge (%) */
	float soc;

	/** Nominal capacity of battery pack (Ah) */
	float nominal_capacity_Ah;

	uint16_t Stack_Voltage;
	uint16_t Pack_Voltage;
	uint16_t LD_Voltage;
	//uint16_t Pack_Current = 0x00;

	uint8_t bms_device;

}bms_context;

void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count);
unsigned char Checksum(unsigned char *ptr, unsigned char len);
unsigned char CRC8(unsigned char *ptr, unsigned char len);
void I2C_WriteReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count);
int I2C_ReadReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count);
void BQ769x2_SetRegister(uint16_t reg_addr, uint32_t reg_data, uint8_t datalen);
uint16_t BQ769x2_ReadRegister(uint16_t reg_addr);
void I2C_WriteReg(uint8_t reg, uint8_t *data, uint8_t length);
void BQ769x2_WriteRegister(uint16_t reg_addr, uint8_t *reg_data, uint8_t datalen);
void CommandSubcommands(uint16_t command);
void Subcommands(uint16_t command, uint16_t data, uint8_t type);
void DirectCommands(uint8_t command, uint16_t data, uint8_t type);
void BQ769x2_Init();
void BQ769x2_BOTHOFF();
void BQ769x2_RESET_BOTHOFF();
void BQ769x2_ReadFETStatus();
void BQ769x2_ShutdownPin();
void BQ769x2_ReleaseShutdownPin();
uint16_t BQ769x2_ReadAlarmStatus();
void BQ769x2_ReadSafetyStatus();
void BQ769x2_ReadPFStatus();
uint16_t BQ769x2_ReadVoltage(uint8_t command, bms_context *bms);
void BQ769x2_ReadAllVoltages(bms_context *bms);
uint16_t BQ769x2_ReadCurrent();
float BQ769x2_ReadTemperature(uint8_t command);
float BQ769x2_ReadPassQ();
uint8_t EEPROM_ReadByte(uint16_t MemAddress);
bool bms_charge_error();
bool bms_discharge_error();
bool bms_charge_allowed(bms_context *bms);
bool bms_discharge_allowed(bms_context *bms);
void bms_state_machine(bms_context *bms);

//void BQ769x2_ReadAllVoltages(bms_context *bms);

#endif /* INC_BQ_COMMON_H_ */
