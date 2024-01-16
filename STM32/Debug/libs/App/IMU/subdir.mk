################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/App/IMU/imu.cpp 

OBJS += \
./libs/App/IMU/imu.o 

CPP_DEPS += \
./libs/App/IMU/imu.d 


# Each subdirectory must supply rules for building sources it contributes
libs/App/IMU/%.o libs/App/IMU/%.su libs/App/IMU/%.cyclo: ../libs/App/IMU/%.cpp libs/App/IMU/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libs-2f-App-2f-IMU

clean-libs-2f-App-2f-IMU:
	-$(RM) ./libs/App/IMU/imu.cyclo ./libs/App/IMU/imu.d ./libs/App/IMU/imu.o ./libs/App/IMU/imu.su

.PHONY: clean-libs-2f-App-2f-IMU
