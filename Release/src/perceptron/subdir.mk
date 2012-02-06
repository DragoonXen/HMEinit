################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/perceptron/backpropagation_perceptron.cpp 

OBJS += \
./src/perceptron/backpropagation_perceptron.o 

CPP_DEPS += \
./src/perceptron/backpropagation_perceptron.d 


# Each subdirectory must supply rules for building sources it contributes
src/perceptron/%.o: ../src/perceptron/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


