################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Code/qfsm/qfsm_dis.c \
../Code/qfsm/qfsm_ini.c \
../Code/qfsm/qfsm_user.c 

OBJS += \
./Code/qfsm/qfsm_dis.o \
./Code/qfsm/qfsm_ini.o \
./Code/qfsm/qfsm_user.o 

COMPILED_SRCS += \
./Code/qfsm/qfsm_dis.src \
./Code/qfsm/qfsm_ini.src \
./Code/qfsm/qfsm_user.src 

C_DEPS += \
./Code/qfsm/qfsm_dis.d \
./Code/qfsm/qfsm_ini.d \
./Code/qfsm/qfsm_user.d 


# Each subdirectory must supply rules for building sources it contributes
Code/qfsm/%.src: ../Code/qfsm/%.c Code/qfsm/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc37x "-fC:/Users/Usami/AURIX-v1.6.0-workspace/UU/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

Code/qfsm/%.o: ./Code/qfsm/%.src Code/qfsm/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


