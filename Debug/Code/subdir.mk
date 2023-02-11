################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Code/AD5124.c \
../Code/ADC_Queued_Scan.c \
../Code/GTM_ATOM_Interrupt.c \
../Code/ICM42605.c \
../Code/bsp_draw.c \
../Code/ccu6_pwm.c \
../Code/hall.c \
../Code/imgproc.c \
../Code/imu.c \
../Code/motor.c \
../Code/move_filter.c \
../Code/pid.c \
../Code/processImage.c 

OBJS += \
./Code/AD5124.o \
./Code/ADC_Queued_Scan.o \
./Code/GTM_ATOM_Interrupt.o \
./Code/ICM42605.o \
./Code/bsp_draw.o \
./Code/ccu6_pwm.o \
./Code/hall.o \
./Code/imgproc.o \
./Code/imu.o \
./Code/motor.o \
./Code/move_filter.o \
./Code/pid.o \
./Code/processImage.o 

COMPILED_SRCS += \
./Code/AD5124.src \
./Code/ADC_Queued_Scan.src \
./Code/GTM_ATOM_Interrupt.src \
./Code/ICM42605.src \
./Code/bsp_draw.src \
./Code/ccu6_pwm.src \
./Code/hall.src \
./Code/imgproc.src \
./Code/imu.src \
./Code/motor.src \
./Code/move_filter.src \
./Code/pid.src \
./Code/processImage.src 

C_DEPS += \
./Code/AD5124.d \
./Code/ADC_Queued_Scan.d \
./Code/GTM_ATOM_Interrupt.d \
./Code/ICM42605.d \
./Code/bsp_draw.d \
./Code/ccu6_pwm.d \
./Code/hall.d \
./Code/imgproc.d \
./Code/imu.d \
./Code/motor.d \
./Code/move_filter.d \
./Code/pid.d \
./Code/processImage.d 


# Each subdirectory must supply rules for building sources it contributes
Code/%.src: ../Code/%.c Code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc37x "-fC:/Users/Usami/AURIX-v1.6.0-workspace/UU/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

Code/%.o: ./Code/%.src Code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


