################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/infineon_libraries/iLLD/TC37A/Tricore/Iom/Iom/IfxIom_Iom.c 

OBJS += \
./Libraries/infineon_libraries/iLLD/TC37A/Tricore/Iom/Iom/IfxIom_Iom.o 

COMPILED_SRCS += \
./Libraries/infineon_libraries/iLLD/TC37A/Tricore/Iom/Iom/IfxIom_Iom.src 

C_DEPS += \
./Libraries/infineon_libraries/iLLD/TC37A/Tricore/Iom/Iom/IfxIom_Iom.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/infineon_libraries/iLLD/TC37A/Tricore/Iom/Iom/%.src: ../Libraries/infineon_libraries/iLLD/TC37A/Tricore/Iom/Iom/%.c Libraries/infineon_libraries/iLLD/TC37A/Tricore/Iom/Iom/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc37x "-fC:/Users/Usami/AURIX-v1.6.0-workspace/UU/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

Libraries/infineon_libraries/iLLD/TC37A/Tricore/Iom/Iom/%.o: ./Libraries/infineon_libraries/iLLD/TC37A/Tricore/Iom/Iom/%.src Libraries/infineon_libraries/iLLD/TC37A/Tricore/Iom/Iom/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


