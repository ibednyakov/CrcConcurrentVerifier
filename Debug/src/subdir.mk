################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CRC32.cpp \
../src/CRCVerifier.cpp \
../src/DataGenerator.cpp \
../src/Dispatcher.cpp \
../src/InputParametersReader.cpp \
../src/WorkerContainer.cpp 

OBJS += \
./src/CRC32.o \
./src/CRCVerifier.o \
./src/DataGenerator.o \
./src/Dispatcher.o \
./src/InputParametersReader.o \
./src/WorkerContainer.o 

CPP_DEPS += \
./src/CRC32.d \
./src/CRCVerifier.d \
./src/DataGenerator.d \
./src/Dispatcher.d \
./src/InputParametersReader.d \
./src/WorkerContainer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


