################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middleware/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.c 

OBJS += \
./Middleware/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.o 

C_DEPS += \
./Middleware/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.d 


# Each subdirectory must supply rules for building sources it contributes
Middleware/SEGGER/OS/%.o Middleware/SEGGER/OS/%.su Middleware/SEGGER/OS/%.cyclo: ../Middleware/SEGGER/OS/%.c Middleware/SEGGER/OS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"/home/frank/thesis_ws/GETjag_bms/Middleware/FreeRTOS" -I"/home/frank/thesis_ws/GETjag_bms/Middleware/SEGGER/Config" -I"/home/frank/thesis_ws/GETjag_bms/Middleware/SEGGER/OS" -I"/home/frank/thesis_ws/GETjag_bms/Middleware/SEGGER/SEGGER" -I"/home/frank/thesis_ws/GETjag_bms/Middleware/FreeRTOS" -I"/home/frank/thesis_ws/GETjag_bms/Middleware/FreeRTOS/include" -I"/home/frank/thesis_ws/GETjag_bms/Middleware/FreeRTOS/portable/GCC/ARM_CM4F" -I"/home/frank/thesis_ws/GETjag_bms/Core/Src/w5500" -I"/home/frank/thesis_ws/GETjag_bms/Core/Src/w5500/W5500" -I/home/frank/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/STM32F4xx_HAL_Driver/Inc -I/home/frank/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/home/frank/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/home/frank/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middleware-2f-SEGGER-2f-OS

clean-Middleware-2f-SEGGER-2f-OS:
	-$(RM) ./Middleware/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.cyclo ./Middleware/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.d ./Middleware/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.o ./Middleware/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.su

.PHONY: clean-Middleware-2f-SEGGER-2f-OS

