################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Configurations/Ifx_Cfg_Ssw.c \
../Configurations/Ifx_Cfg_SswBmhd.c 

OBJS += \
./Configurations/Ifx_Cfg_Ssw.o \
./Configurations/Ifx_Cfg_SswBmhd.o 

COMPILED_SRCS += \
./Configurations/Ifx_Cfg_Ssw.src \
./Configurations/Ifx_Cfg_SswBmhd.src 

C_DEPS += \
./Configurations/Ifx_Cfg_Ssw.d \
./Configurations/Ifx_Cfg_SswBmhd.d 


# Each subdirectory must supply rules for building sources it contributes
Configurations/%.src: ../Configurations/%.c Configurations/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc37x "-fC:/Users/Usami/AURIX-v1.6.0-workspace/UU/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

Configurations/%.o: ./Configurations/%.src Configurations/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


