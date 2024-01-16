################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/Driver/STEERING_MOTOR/steering_motor.cpp 

OBJS += \
./libs/Driver/STEERING_MOTOR/steering_motor.o 

CPP_DEPS += \
./libs/Driver/STEERING_MOTOR/steering_motor.d 


# Each subdirectory must supply rules for building sources it contributes
libs/Driver/STEERING_MOTOR/%.o libs/Driver/STEERING_MOTOR/%.su libs/Driver/STEERING_MOTOR/%.cyclo: ../libs/Driver/STEERING_MOTOR/%.cpp libs/Driver/STEERING_MOTOR/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libs-2f-Driver-2f-STEERING_MOTOR

clean-libs-2f-Driver-2f-STEERING_MOTOR:
	-$(RM) ./libs/Driver/STEERING_MOTOR/steering_motor.cyclo ./libs/Driver/STEERING_MOTOR/steering_motor.d ./libs/Driver/STEERING_MOTOR/steering_motor.o ./libs/Driver/STEERING_MOTOR/steering_motor.su

.PHONY: clean-libs-2f-Driver-2f-STEERING_MOTOR
