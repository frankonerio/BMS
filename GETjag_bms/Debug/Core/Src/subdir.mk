################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/bq_common.c \
../Core/Src/delay.c \
../Core/Src/look_up_table.c \
../Core/Src/main.c \
../Core/Src/modbus.c \
../Core/Src/soc_soh.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_hal_timebase_tim.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/utils.c 

OBJS += \
./Core/Src/bq_common.o \
./Core/Src/delay.o \
./Core/Src/look_up_table.o \
./Core/Src/main.o \
./Core/Src/modbus.o \
./Core/Src/soc_soh.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_hal_timebase_tim.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/utils.o 

C_DEPS += \
./Core/Src/bq_common.d \
./Core/Src/delay.d \
./Core/Src/look_up_table.d \
./Core/Src/main.d \
./Core/Src/modbus.d \
./Core/Src/soc_soh.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_hal_timebase_tim.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"/home/frank/thesis_ws/GETjag_bms/Middleware/FreeRTOS" -I"/home/frank/thesis_ws/GETjag_bms/Middleware/SEGGER/Config" -I"/home/frank/thesis_ws/GETjag_bms/Middleware/SEGGER/OS" -I"/home/frank/thesis_ws/GETjag_bms/Middleware/SEGGER/SEGGER" -I"/home/frank/thesis_ws/GETjag_bms/Middleware/FreeRTOS" -I"/home/frank/thesis_ws/GETjag_bms/Middleware/FreeRTOS/include" -I"/home/frank/thesis_ws/GETjag_bms/Middleware/FreeRTOS/portable/GCC/ARM_CM4F" -I"/home/frank/thesis_ws/GETjag_bms/Core/Src/w5500" -I"/home/frank/thesis_ws/GETjag_bms/Core/Src/w5500/W5500" -I/home/frank/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/STM32F4xx_HAL_Driver/Inc -I/home/frank/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/home/frank/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/home/frank/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/bq_common.cyclo ./Core/Src/bq_common.d ./Core/Src/bq_common.o ./Core/Src/bq_common.su ./Core/Src/delay.cyclo ./Core/Src/delay.d ./Core/Src/delay.o ./Core/Src/delay.su ./Core/Src/look_up_table.cyclo ./Core/Src/look_up_table.d ./Core/Src/look_up_table.o ./Core/Src/look_up_table.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/modbus.cyclo ./Core/Src/modbus.d ./Core/Src/modbus.o ./Core/Src/modbus.su ./Core/Src/soc_soh.cyclo ./Core/Src/soc_soh.d ./Core/Src/soc_soh.o ./Core/Src/soc_soh.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_hal_timebase_tim.cyclo ./Core/Src/stm32f4xx_hal_timebase_tim.d ./Core/Src/stm32f4xx_hal_timebase_tim.o ./Core/Src/stm32f4xx_hal_timebase_tim.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/utils.cyclo ./Core/Src/utils.d ./Core/Src/utils.o ./Core/Src/utils.su

.PHONY: clean-Core-2f-Src

