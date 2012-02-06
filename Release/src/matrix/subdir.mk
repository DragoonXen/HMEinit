################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/matrix/matrix.cpp 

OBJS += \
./src/matrix/matrix.o 

CPP_DEPS += \
./src/matrix/matrix.d 


# Each subdirectory must supply rules for building sources it contributes
src/matrix/%.o: ../src/matrix/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


