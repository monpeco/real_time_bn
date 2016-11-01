################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
BSP.obj: D:/Dropbox/EdX/RTOS/MSP432ValvanoWare/inc/BSP.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include" --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --c99 --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="BSP.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

CortexM.obj: D:/Dropbox/EdX/RTOS/MSP432ValvanoWare/inc/CortexM.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include" --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --c99 --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CortexM.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Lab2.obj: ../Lab2.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include" --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --c99 --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Lab2.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Profile.obj: D:/Dropbox/EdX/RTOS/MSP432ValvanoWare/inc/Profile.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include" --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --c99 --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Profile.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Texas.obj: ../Texas.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include" --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --c99 --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Texas.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

msp432_startup_ccs.obj: ../msp432_startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include" --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --c99 --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="msp432_startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os.obj: ../os.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include" --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --c99 --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="os.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

osasm.obj: ../osasm.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include" --include_path="C:/ti/ccs613/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/ti/ccs613/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --c99 --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="osasm.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


