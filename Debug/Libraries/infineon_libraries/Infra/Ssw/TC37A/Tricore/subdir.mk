################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Infra.c \
../Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc0.c \
../Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc1.c \
../Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc2.c 

OBJS += \
./Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Infra.o \
./Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc0.o \
./Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc1.o \
./Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc2.o 

COMPILED_SRCS += \
./Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Infra.src \
./Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc0.src \
./Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc1.src \
./Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc2.src 

C_DEPS += \
./Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Infra.d \
./Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc0.d \
./Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc1.d \
./Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc2.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/%.src: ../Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/%.c Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc37x "-fC:/Users/Usami/AURIX-v1.6.0-workspace/UU/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/%.o: ./Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/%.src Libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


