################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middleware/FreeRTOS/portable/MemMang/heap_4.c 

OBJS += \
./Middleware/FreeRTOS/portable/MemMang/heap_4.o 

C_DEPS += \
./Middleware/FreeRTOS/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
Middleware/FreeRTOS/portable/MemMang/%.o Middleware/FreeRTOS/portable/MemMang/%.su Middleware/FreeRTOS/portable/MemMang/%.cyclo: ../Middleware/FreeRTOS/portable/MemMang/%.c Middleware/FreeRTOS/portable/MemMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"/home/frank/thesis_ws/GETjag_bms_backup/Middleware/FreeRTOS" -I"/home/frank/thesis_ws/GETjag_bms_backup/Middleware/SEGGER/Config" -I"/home/frank/thesis_ws/GETjag_bms_backup/Middleware/SEGGER/OS" -I"/home/frank/thesis_ws/GETjag_bms_backup/Middleware/SEGGER/SEGGER" -I"/home/frank/thesis_ws/GETjag_bms_backup/Middleware/FreeRTOS" -I"/home/frank/thesis_ws/GETjag_bms_backup/Middleware/FreeRTOS/include" -I"/home/frank/thesis_ws/GETjag_bms_backup/Middleware/FreeRTOS/portable/GCC/ARM_CM4F" -I"/home/frank/thesis_ws/GETjag_bms_backup/Core/Src/w5500" -I"/home/frank/thesis_ws/GETjag_bms_backup/Core/Src/w5500/W5500" -I/home/frank/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/STM32F4xx_HAL_Driver/Inc -I/home/frank/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/home/frank/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/home/frank/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middleware-2f-FreeRTOS-2f-portable-2f-MemMang

clean-Middleware-2f-FreeRTOS-2f-portable-2f-MemMang:
	-$(RM) ./Middleware/FreeRTOS/portable/MemMang/heap_4.cyclo ./Middleware/FreeRTOS/portable/MemMang/heap_4.d ./Middleware/FreeRTOS/portable/MemMang/heap_4.o ./Middleware/FreeRTOS/portable/MemMang/heap_4.su

.PHONY: clean-Middleware-2f-FreeRTOS-2f-portable-2f-MemMang

