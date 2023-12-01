################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/Driver/BNO055/bno055.cpp 

OBJS += \
./libs/Driver/BNO055/bno055.o 

CPP_DEPS += \
./libs/Driver/BNO055/bno055.d 


# Each subdirectory must supply rules for building sources it contributes
libs/Driver/BNO055/%.o libs/Driver/BNO055/%.su libs/Driver/BNO055/%.cyclo: ../libs/Driver/BNO055/%.cpp libs/Driver/BNO055/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libs-2f-Driver-2f-BNO055

clean-libs-2f-Driver-2f-BNO055:
	-$(RM) ./libs/Driver/BNO055/bno055.cyclo ./libs/Driver/BNO055/bno055.d ./libs/Driver/BNO055/bno055.o ./libs/Driver/BNO055/bno055.su

.PHONY: clean-libs-2f-Driver-2f-BNO055

