/*
 * BQ_common.c
 *
 *  Created on: Jul 28, 2024
 *      Author: frank
 */

#include <bq_common.h>

// BQ78952 Private Variables
uint8_t RX_data[2] = {0x00, 0x00};
uint8_t RX_32Byte[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint16_t CellVoltage[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
float Temperature[3] = {0, 0, 0};
uint16_t Stack_Voltage = 0x00;
uint16_t Pack_Voltage = 0x00;
uint16_t LD_Voltage = 0x00;
uint16_t Pack_Current = 0x00;

uint16_t AlarmBits = 0x00;
uint8_t value_SafetyStatusA;
uint8_t value_SafetyStatusB;
uint8_t value_SafetyStatusC;
uint8_t value_PFStatusA;
uint8_t value_PFStatusB;
uint8_t value_PFStatusC;
uint8_t FET_Status;
uint16_t CB_ActiveCells;

uint8_t UV_Fault = 0;
uint8_t OV_Fault = 0;
uint8_t SCD_Fault = 0;
uint8_t OCD_Fault = 0;
uint8_t ProtectionsTriggered = 0;

uint8_t LD_ON = 0;
uint8_t DSG = 0;
uint8_t CHG = 0;
uint8_t PCHG = 0;
uint8_t PDSG = 0;

int32_t AccumulatedCharge_Int = 0; // in BQ769x2_READPASSQ func
int32_t AccumulatedCharge_Frac = 0;// in BQ769x2_READPASSQ func
int32_t AccumulatedCharge_Time = 0;// in BQ769x2_READPASSQ func

float coulumbs = 0;

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
    TX_Buffer[1] = 0x05; // combined length of register address and data
    uint8_t statusLow = 0xFF;
    uint8_t statusHigh = 0xFF;
    do
    {
      I2C_ReadReg(0x3E, &statusLow, 1);
      I2C_ReadReg(0x3F, &statusHigh, 1);
    } while (statusLow == 0xFF && statusHigh == 0xFF);

    I2C_WriteReg(0x60, TX_Buffer, 2); // Write the checksum and length
    delayUS(2000);
    break;
  case 2: // 2 byte datalength
    TX_RegData[3] = (reg_data >> 8) & 0xff;
    I2C_WriteReg(0x3E, TX_RegData, 4);
    delayUS(2000);
    TX_Buffer[0] = Checksum(TX_RegData, 4);
    TX_Buffer[1] = 0x06; // combined length of register address and data
    statusLow = 0xFF;
    statusHigh = 0xFF;
    do
    {
      I2C_ReadReg(0x3E, &statusLow, 1);
      I2C_ReadReg(0x3F, &statusHigh, 1);
    } while (statusLow == 0xFF && statusHigh == 0xFF);

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
  BQ769x2_SetRegister(REG0Config, 0x01, 1);

  // 'REG12 Config' - Enable REG1 with 3.3V output (0x0D for 3.3V, 0x0F for 5V)
  BQ769x2_SetRegister(REG12Config, 0x0D, 1);

  // Set DFETOFF pin to control BOTH CHG and DSG FET - 0x92FB = 0x42 (set to 0x00 to disable)
  // BQ769x2_SetRegister(DFETOFFPinConfig, 0x42, 1);

  // Set up ALERT Pin - 0x92FC = 0x2A
  // This configures the ALERT pin to drive high (REG1 voltage) when enabled.
  // The ALERT pin can be used as an interrupt to the MCU when a protection has triggered or new measurements are available
  BQ769x2_SetRegister(ALERTPinConfig, 0xAA, 1);

  // Set TS1 to measure Cell Temperature - 0x92FD = 0x07
  BQ769x2_SetRegister(TS1Config, 0x07, 1);

  // Set TS3 to measure FET Temperature - 0x92FF = 0x0F
  BQ769x2_SetRegister(TS3Config, 0x07, 1);

  // Set HDQ to measure Cell Temperature - 0x9300 = 0x07
  BQ769x2_SetRegister(HDQPinConfig, 0x00, 1); // No thermistor installed on EVM HDQ pin, so set to 0x00

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
  BQ769x2_SetRegister(DefaultAlarmMask, 0xFFFF, 2);

  // Set up Cell Balancing Configuration - 0x9335 = 0x03   -  Automated balancing while in Relax or Charge modes
  // Also see "Cell Balancing with BQ769x2 Battery Monitors" document on ti.com
  // BQ769x2_SetRegister(BalancingConfiguration, 0x03, 1);

  // Set up CUV (under-voltage) Threshold - 0x9275 = 0x31 (2479 mV)
  // CUV Threshold is this value multiplied by 50.6mV

  // BQ769x2_SetRegister(CUVThreshold, 0x14, 1);

  BQ769x2_SetRegister(CUVThreshold, 0x31, 1);

  // Set up COV (over-voltage) Threshold - 0x9278 = 0x55 (4301 mV)
  // COV Threshold is this value multiplied by 50.6mV
  // BQ769x2_SetRegister(COVThreshold, 0x55, 1);

  BQ769x2_SetRegister(COVThreshold, 0x55, 1);

  // Set up OCC (over-current in charge) Threshold - 0x9280 = 0x05 (10 mV = 10A across 1mOhm sense resistor) Units in 2mV
  BQ769x2_SetRegister(OCCThreshold, 0x05, 1);

  // Set up OCD1 Threshold - 0x9282 = 0x0A (20 mV = 20A across 1mOhm sense resistor) units of 2mV
  BQ769x2_SetRegister(OCD1Threshold, 0x0A, 1);

  // Set up SCD Threshold - 0x9286 = 0x05 (100 mV = 100A across 1mOhm sense resistor)  0x05=100mV
  BQ769x2_SetRegister(SCDThreshold, 0x01, 1);

  // Set up SCD Delay - 0x9287 = 0x03 (30 us) Enabled with a delay of (value - 1) * 15 µs; min value of 1
  BQ769x2_SetRegister(SCDDelay, 0x03, 1);

  // Set up SCDL Latch Limit to 1 to set SCD recovery only with load removal 0x9295 = 0x01
  // If this is not set, then SCD will recover based on time (SCD Recovery Time parameter).
  BQ769x2_SetRegister(SCDLLatchLimit, 0x01, 1);

  BQ769x2_SetRegister(MfgStatusInit, 0x0050, 2); // disable test mode

  BQ769x2_SetRegister(FETOptions, 0x3F, 1); // 0x3F

  // BQ769x2_SetRegister(TS1Config, 0x0B, 1);
  //  Exit CONFIGUPDATE mode  - Subcommand 0x0092

  // BQ769x2_SetRegister(CCGain, 0x41C9C800, 4);
  BQ769x2_SetRegister(DAConfiguration, 0x05, 1); // set user Amps to 1mA

  Subcommands(FET_CONTROL, 0x03, W);

  CommandSubcommands(EXIT_CFGUPDATE);
}
//  ********************************* FET Control Commands  ***************************************

void BQ769x2_BOTHOFF()
{
  // Disables all FETs using the DFETOFF (BOTHOFF) pin
  // The DFETOFF pin on the BQ76952EVM should be connected to the MCU board to use this function
  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // DFETOFF pin (BOTHOFF) set high
}

void BQ769x2_RESET_BOTHOFF()
{
  // Resets DFETOFF (BOTHOFF) pin
  // The DFETOFF pin on the BQ76952EVM should be connected to the MCU board to use this function
  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); // DFETOFF pin (BOTHOFF) set low
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
  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // Sets RST_SHUT pin
}

void BQ769x2_ReleaseShutdownPin()
{
  // Releases the RST_SHUT pin
  // The RST_SHUT pin on the BQ76952EVM should be connected to the MCU board to use this function
  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); // Resets RST_SHUT pin
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

uint16_t BQ769x2_ReadVoltage(uint8_t command, bms_context *bms)
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

void BQ769x2_ReadAllVoltages(bms_context *bms)
// Reads all cell voltages, Stack voltage, PACK pin voltage, and LD pin voltage
{
  int cellvoltageholder = Cell1Voltage; // Cell1Voltage is 0x14
  for (int x = 0; x < 16; x++)
  { // Reads all cell voltages
    CellVoltage[x] = BQ769x2_ReadVoltage(cellvoltageholder, bms);
    cellvoltageholder = cellvoltageholder + 2;
  }
  bms->Stack_Voltage = BQ769x2_ReadVoltage(StackVoltage, bms);
  bms->Pack_Voltage = BQ769x2_ReadVoltage(PACKPinVoltage, bms);
  bms->LD_Voltage = BQ769x2_ReadVoltage(LDPinVoltage, bms);
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

float BQ769x2_ReadPassQ(){ // Read Accumulated Charge and Time from DASTATUS6

		Subcommands(DASTATUS6, 0x00, R);

		AccumulatedCharge_Int = ((uint32_t)RX_32Byte[3] << 24) | ((uint32_t)RX_32Byte[2] << 16) |
	                            ((uint32_t)RX_32Byte[1] << 8) | RX_32Byte[0];
		AccumulatedCharge_Frac = ((uint32_t)RX_32Byte[7] << 24) | ((uint32_t)RX_32Byte[6] << 16) |
	                             ((uint32_t)RX_32Byte[5] << 8) | RX_32Byte[4];
	    AccumulatedCharge_Time = ((uint32_t)RX_32Byte[11] << 24) | ((uint32_t)RX_32Byte[10] << 16) |
	                             ((uint32_t)RX_32Byte[9] << 8) | RX_32Byte[8];
//	    uint32_t charge;
//	    int16_t current = BQ769x2_ReadCurrent();
//
//	    if(current < 0){
//	    	charge = 0xFFFFFFFF - AccumulatedCharge_Int;
//	    }
//	    else{
//	    	charge = AccumulatedCharge_Int;
//	    }

	    //charge = 0xFFFFFFFF - AccumulatedCharge_Int;
	    float totalAccumulatedCharge_mAh = (float)AccumulatedCharge_Int + ((float)AccumulatedCharge_Frac / FRAC_DIVISOR);
	    //float totalAccumulatedCharge_Ah = totalAccumulatedCharge_mAh / 1000.0f;

	    coulumbs = totalAccumulatedCharge_mAh;
	    return coulumbs;
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

bool bms_charge_error()
{
  return ProtectionsTriggered && (OV_Fault || SCD_Fault || OCD_Fault);
}

bool bms_discharge_error()
{
  return ProtectionsTriggered && (UV_Fault || SCD_Fault || OCD_Fault);
}

bool bms_charge_allowed(bms_context *bms)
{

  return !bms->battery_full && !bms_charge_error() && bms->charge_enable;
}

bool bms_discharge_allowed(bms_context *bms)
{
  // Use logical AND (&&) for combining boolean conditions
  return !bms->battery_empty && !bms_discharge_error() && bms->discharge_enable;
}

void bms_state_machine(bms_context *bms)
{
	BQ769x2_ReadSafetyStatus();
  switch (bms->bms_state)
  {
  case BMS_STATE_OFF:
    if (bms_discharge_allowed(bms))
    {
      // bms_ic_set_switches(bms->ic_dev, BMS_SWITCH_DIS, true);
      CommandSubcommands(ALL_FETS_ON);
      bms->bms_state = BMS_STATE_DIS;
      // LOG_INF("OFF -> DIS (error flags: 0x%08x)", bms->ic_data.error_flags);
    }
    else if (bms_charge_allowed(bms))
    {
      // bms_ic_set_switches(bms->ic_dev, BMS_SWITCH_CHG, true);
      CommandSubcommands(ALL_FETS_ON);
      bms->bms_state = BMS_STATE_CHG;
      // LOG_INF("OFF -> CHG (error flags: 0x%08x)", bms->ic_data.error_flags);
    }
    break;

  case BMS_STATE_CHG:
    if (!bms_charge_allowed(bms))
    {
      // bms_ic_set_switches(bms->ic_dev, BMS_SWITCH_CHG, false);
      /* DIS switch may be on on because of ideal diode control */
      // bms_ic_set_switches(bms->ic_dev, BMS_SWITCH_DIS, false);
      // CommandSubcommands(ALL_FETS_OFF);
      bms->bms_state = BMS_STATE_OFF;
      // LOG_INF("CHG -> OFF (error flags: 0x%08x)", bms->ic_data.error_flags);
    }
    else if (bms_discharge_allowed(bms))
    {
      CommandSubcommands(ALL_FETS_ON);
      bms->bms_state = BMS_STATE_NORMAL;
      // LOG_INF("CHG -> NORMAL (error flags: 0x%08x)", bms->ic_data.error_flags);
    }
    break;

  case BMS_STATE_DIS:
    if (!bms_discharge_allowed(bms))
    {
      // CommandSubcommands(ALL_FETS_OFF);
      bms->bms_state = BMS_STATE_OFF;
      // LOG_INF("DIS -> OFF (error flags: 0x%08x)", bms->ic_data.error_flags);
    }
    else if (bms_charge_allowed(bms))
    {
      CommandSubcommands(ALL_FETS_ON);
      bms->bms_state = BMS_STATE_NORMAL;
      // LOG_INF("DIS -> NORMAL (error flags: 0x%08x)", bms->ic_data.error_flags);
    }
    break;

  case BMS_STATE_NORMAL:
    if (!bms_discharge_allowed(bms))
    {
      // CommandSubcommands(ALL_FETS_OFF);
      bms->bms_state = BMS_STATE_CHG;
      // LOG_INF("NORMAL -> CHG (error flags: 0x%08x)", bms->ic_data.error_flags);
    }
    else if (!bms_charge_allowed(bms))
    {
      // CommandSubcommands(ALL_FETS_OFF);
      bms->bms_state = BMS_STATE_DIS;
      // LOG_INF("NORMAL -> DIS (error flags: 0x%08x)", bms->ic_data.error_flags);
    }
    break;

  case BMS_STATE_SHUTDOWN:
    /* do nothing and wait until shutdown is completed */
    break;
  }
}
