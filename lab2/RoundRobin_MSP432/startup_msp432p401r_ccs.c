/******************************************************************************
* 
*  Copyright (C) 2012 - 2016 Texas Instruments Incorporated - http://www.ti.com/ 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
* 
*   Redistributions of source code must retain the above copyright 
*   notice, this list of conditions and the following disclaimer.
* 
*   Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the 
*   documentation and/or other materials provided with the   
*   distribution.
* 
*   Neither the name of Texas Instruments Incorporated nor the names of
*   its contributors may be used to endorse or promote products derived
*   from this software without specific prior written permission.
* 
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
* 
*  MSP432P401R Interrupt Vector Table and startup code for CCS TI ARM
* 
*****************************************************************************/

#include <stdint.h>

/* Forward declaration of the default fault handlers. */
static void resetISR(void);
static void nmiISR(void);
static void faultISR(void);
static void defaultISR(void);

extern void SVC_Handler(void) __attribute__((weak));                            /* SVCall handler            */
extern void DebugMon_Handler(void) __attribute__((weak));                       /* Debug monitor handler     */
extern void PendSV_Handler(void) __attribute__((weak));                         /* The PendSV handler        */
extern void SysTick_Handler(void) __attribute__((weak));                        /* The SysTick handler       */
extern void PSS_IRQHandler(void) __attribute__((weak));                         /* PSS ISR                   */
extern void CS_IRQHandler(void) __attribute__((weak));                          /* CS ISR                    */
extern void PCM_IRQHandler(void) __attribute__((weak));                         /* PCM ISR                   */
extern void WDT_A_IRQHandler(void) __attribute__((weak));                       /* WDT ISR                   */
extern void FPU_IRQHandler(void) __attribute__((weak));                         /* FPU ISR                   */
extern void FLCTL_IRQHandler(void) __attribute__((weak));                       /* FLCTL ISR                 */
extern void COMP_E0_IRQHandler(void) __attribute__((weak));                     /* COMP0 ISR                 */
extern void COMP_E1_IRQHandler(void) __attribute__((weak));                     /* COMP1 ISR                 */
extern void TA0_0_IRQHandler(void) __attribute__((weak));                       /* TA0_0 ISR                 */
extern void TA0_N_IRQHandler(void) __attribute__((weak));                       /* TA0_N ISR                 */
extern void TA1_0_IRQHandler(void) __attribute__((weak));                       /* TA1_0 ISR                 */
extern void TA1_N_IRQHandler(void) __attribute__((weak));                       /* TA1_N ISR                 */
extern void TA2_0_IRQHandler(void) __attribute__((weak));                       /* TA2_0 ISR                 */
extern void TA2_N_IRQHandler(void) __attribute__((weak));                       /* TA2_N ISR                 */
extern void TA3_0_IRQHandler(void) __attribute__((weak));                       /* TA3_0 ISR                 */
extern void TA3_N_IRQHandler(void) __attribute__((weak));                       /* TA3_N ISR                 */
extern void EUSCIA0_IRQHandler(void) __attribute__((weak));                     /* EUSCIA0 ISR               */
extern void EUSCIA1_IRQHandler(void) __attribute__((weak));                     /* EUSCIA1 ISR               */
extern void EUSCIA2_IRQHandler(void) __attribute__((weak));                     /* EUSCIA2 ISR               */
extern void EUSCIA3_IRQHandler(void) __attribute__((weak));                     /* EUSCIA3 ISR               */
extern void EUSCIB0_IRQHandler(void) __attribute__((weak));                     /* EUSCIB0 ISR               */
extern void EUSCIB1_IRQHandler(void) __attribute__((weak));                     /* EUSCIB1 ISR               */
extern void EUSCIB2_IRQHandler(void) __attribute__((weak));                     /* EUSCIB2 ISR               */
extern void EUSCIB3_IRQHandler(void) __attribute__((weak));                     /* EUSCIB3 ISR               */
extern void ADC14_IRQHandler(void) __attribute__((weak));                       /* ADC14 ISR                 */
extern void T32_INT1_IRQHandler(void) __attribute__((weak));                    /* T32_INT1 ISR              */
extern void T32_INT2_IRQHandler(void) __attribute__((weak));                    /* T32_INT2 ISR              */
extern void T32_INTC_IRQHandler(void) __attribute__((weak));                    /* T32_INTC ISR              */
extern void AES256_IRQHandler(void) __attribute__((weak));                      /* AES ISR                   */
extern void RTC_C_IRQHandler(void) __attribute__((weak));                       /* RTC ISR                   */
extern void DMA_ERR_IRQHandler(void) __attribute__((weak));                     /* DMA_ERR ISR               */
extern void DMA_INT3_IRQHandler(void) __attribute__((weak));                    /* DMA_INT3 ISR              */
extern void DMA_INT2_IRQHandler(void) __attribute__((weak));                    /* DMA_INT2 ISR              */
extern void DMA_INT1_IRQHandler(void) __attribute__((weak));                    /* DMA_INT1 ISR              */
extern void DMA_INT0_IRQHandler(void) __attribute__((weak));                    /* DMA_INT0 ISR              */
extern void PORT1_IRQHandler(void) __attribute__((weak));                       /* PORT1 ISR                 */
extern void PORT2_IRQHandler(void) __attribute__((weak));                       /* PORT2 ISR                 */
extern void PORT3_IRQHandler(void) __attribute__((weak));                       /* PORT3 ISR                 */
extern void PORT4_IRQHandler(void) __attribute__((weak));                       /* PORT4 ISR                 */
extern void PORT5_IRQHandler(void) __attribute__((weak));                       /* PORT5 ISR                 */
extern void PORT6_IRQHandler(void) __attribute__((weak));                       /* PORT6 ISR                 */

/* External declaration for the reset handler that is to be called when the */
/* processor is started                                                     */
extern void _c_int00(void);

/* External declaration for system initialization function                  */
extern void SystemInit(void);

/* Linker variable that marks the top of the stack. */
extern unsigned long __STACK_END;


/* External declarations for the interrupt handlers used by the application. */
/* To be added by user */


/* Interrupt vector table.  Note that the proper constructs must be placed on this to */
/* ensure that it ends up at physical address 0x0000.0000 or at the start of          */
/* the program if located at a start address other than 0.                            */
#pragma RETAIN(interruptVectors)
#pragma DATA_SECTION(interruptVectors, ".intvecs")
void (* const interruptVectors[])(void) =
{
    (void (*)(void))((uint32_t)&__STACK_END),
                                            /* The initial stack pointer */
    resetISR,                               /* The reset handler         */
    nmiISR,                                 /* The NMI handler           */
    faultISR,                               /* The hard fault handler    */
    defaultISR,                             /* The MPU fault handler     */
    defaultISR,                             /* The bus fault handler     */
    defaultISR,                             /* The usage fault handler   */
    0,                                      /* Reserved                  */
    0,                                      /* Reserved                  */
    0,                                      /* Reserved                  */
    0,                                      /* Reserved                  */
	SVC_Handler,                            /* SVCall handler            */
	DebugMon_Handler,                       /* Debug monitor handler     */
    0,                                      /* Reserved                  */
	PendSV_Handler,                         /* The PendSV handler        */
	SysTick_Handler,                        /* The SysTick handler       */
	PSS_IRQHandler,                         /* PSS ISR                   */
	CS_IRQHandler,                          /* CS ISR                    */
	PCM_IRQHandler,                         /* PCM ISR                   */
	WDT_A_IRQHandler,                       /* WDT ISR                   */
	FPU_IRQHandler,                         /* FPU ISR                   */
	FLCTL_IRQHandler,                       /* FLCTL ISR                 */
	COMP_E0_IRQHandler,                     /* COMP0 ISR                 */
	COMP_E1_IRQHandler,                     /* COMP1 ISR                 */
	TA0_0_IRQHandler,                       /* TA0_0 ISR                 */
	TA0_N_IRQHandler,                       /* TA0_N ISR                 */
	TA1_0_IRQHandler,                       /* TA1_0 ISR                 */
	TA1_N_IRQHandler,                       /* TA1_N ISR                 */
	TA2_0_IRQHandler,                       /* TA2_0 ISR                 */
	TA2_N_IRQHandler,                       /* TA2_N ISR                 */
	TA3_0_IRQHandler,                       /* TA3_0 ISR                 */
	TA3_N_IRQHandler,                       /* TA3_N ISR                 */
	EUSCIA0_IRQHandler,                     /* EUSCIA0 ISR               */
	EUSCIA1_IRQHandler,                     /* EUSCIA1 ISR               */
	EUSCIA2_IRQHandler,                     /* EUSCIA2 ISR               */
	EUSCIA3_IRQHandler,                     /* EUSCIA3 ISR               */
	EUSCIB0_IRQHandler,                     /* EUSCIB0 ISR               */
	EUSCIB1_IRQHandler,                     /* EUSCIB1 ISR               */
	EUSCIB2_IRQHandler,                     /* EUSCIB2 ISR               */
	EUSCIB3_IRQHandler,                     /* EUSCIB3 ISR               */
	ADC14_IRQHandler,                       /* ADC14 ISR                 */
	T32_INT1_IRQHandler,                    /* T32_INT1 ISR              */
	T32_INT2_IRQHandler,                    /* T32_INT2 ISR              */
	T32_INTC_IRQHandler,                    /* T32_INTC ISR              */
	AES256_IRQHandler,                      /* AES ISR                   */
	RTC_C_IRQHandler,                       /* RTC ISR                   */
	DMA_ERR_IRQHandler,                     /* DMA_ERR ISR               */
	DMA_INT3_IRQHandler,                    /* DMA_INT3 ISR              */
	DMA_INT2_IRQHandler,                    /* DMA_INT2 ISR              */
	DMA_INT1_IRQHandler,                    /* DMA_INT1 ISR              */
	DMA_INT0_IRQHandler,                    /* DMA_INT0 ISR              */
	PORT1_IRQHandler,                       /* PORT1 ISR                 */
	PORT2_IRQHandler,                       /* PORT2 ISR                 */
	PORT3_IRQHandler,                       /* PORT3 ISR                 */
	PORT4_IRQHandler,                       /* PORT4 ISR                 */
	PORT5_IRQHandler,                       /* PORT5 ISR                 */
	PORT6_IRQHandler,                       /* PORT6 ISR                 */
    defaultISR,                             /* Reserved 41               */
    defaultISR,                             /* Reserved 42               */
    defaultISR,                             /* Reserved 43               */
    defaultISR,                             /* Reserved 44               */
    defaultISR,                             /* Reserved 45               */
    defaultISR,                             /* Reserved 46               */
    defaultISR,                             /* Reserved 47               */
    defaultISR,                             /* Reserved 48               */
    defaultISR,                             /* Reserved 49               */
    defaultISR,                             /* Reserved 50               */
    defaultISR,                             /* Reserved 51               */
    defaultISR,                             /* Reserved 52               */
    defaultISR,                             /* Reserved 53               */
    defaultISR,                             /* Reserved 54               */
    defaultISR,                             /* Reserved 55               */
    defaultISR,                             /* Reserved 56               */
    defaultISR,                             /* Reserved 57               */
    defaultISR,                             /* Reserved 58               */
    defaultISR,                             /* Reserved 59               */
    defaultISR,                             /* Reserved 60               */
    defaultISR,                             /* Reserved 61               */
    defaultISR,                             /* Reserved 62               */
    defaultISR,                             /* Reserved 63               */
    defaultISR                              /* Reserved 64               */
};


/* This is the code that gets called when the processor first starts execution */
/* following a reset event.  Only the absolutely necessary set is performed,   */
/* after which the application supplied entry() routine is called.  Any fancy  */
/* actions (such as making decisions based on the reset cause register, and    */
/* resetting the bits in that register) are left solely in the hands of the    */
/* application.                                                                */
void resetISR(void)
{
    SystemInit();

    /* Jump to the CCS C Initialization Routine. */

    __asm("     .global _c_int00\n"
          "     LDR  R0,WDT\n"
          "     LDR  R1,HOLD\n"
          "     STRH R1,[R0]\n"         // turn off watchdog
          "     b.w     _c_int00\n"
          "WDT  .field 0x4000480C,32\n"  // pointer to 16-bit register
          "HOLD .field 0x00005A80,32");  // WDTPW | WDTHOLD
}

/* This is the code that gets called when the processor receives a NMI.  This  */
/* simply enters an infinite loop, preserving the system state for examination */
/* by a debugger.                                                              */
static void nmiISR(void)
{
    /* Fault trap exempt from ULP advisor */
    #pragma diag_push
    #pragma CHECK_ULP("-2.1")

    /* Enter an infinite loop. */
    while(1)
    {
    }

    #pragma diag_pop
}


/* This is the code that gets called when the processor receives a fault        */
/* interrupt.  This simply enters an infinite loop, preserving the system state */
/* for examination by a debugger.                                               */
static void faultISR(void)
{
    /* Fault trap exempt from ULP advisor */
    #pragma diag_push
    #pragma CHECK_ULP("-2.1")

    /* Enter an infinite loop. */
    while(1)
    {
    }

    #pragma diag_pop
}


/* This is the code that gets called when the processor receives an unexpected  */
/* interrupt.  This simply enters an infinite loop, preserving the system state */
/* for examination by a debugger.                                               */
static void defaultISR(void)
{
    /* Fault trap exempt from ULP advisor */
    #pragma diag_push
    #pragma CHECK_ULP("-2.1")

    /* Enter an infinite loop. */
    while(1)
    {
    }

    #pragma diag_pop
}

//These Functions are all weakly defined, so the user can write over them in an external file
void SVC_Handler(void){ while(1){}}                           /* SVCall handler            */
void DebugMon_Handler(void){ while(1){}}                 /* Debug monitor handler     */
void PendSV_Handler(void){ while(1){}}
void SysTick_Handler(void){ while(1){}}
void PSS_IRQHandler(void){ while(1){}}                        /* PSS ISR                   */
void CS_IRQHandler(void){ while(1){}}                         /* CS ISR                    */
void PCM_IRQHandler(void){ while(1){}}                        /* PCM ISR                   */
void WDT_A_IRQHandler(void){ while(1){}}                      /* WDT ISR                   */
void FPU_IRQHandler(void){ while(1){}}                        /* FPU ISR                   */
void FLCTL_IRQHandler(void){ while(1){}}                      /* FLCTL ISR                 */
void COMP_E0_IRQHandler(void){ while(1){}}                    /* COMP0 ISR                 */
void COMP_E1_IRQHandler(void){ while(1){}}                    /* COMP1 ISR                 */
void TA0_0_IRQHandler(void){ while(1){}}                      /* TA0_0 ISR                 */
void TA0_N_IRQHandler(void){ while(1){}}                      /* TA0_N ISR                 */
void TA1_0_IRQHandler(void){ while(1){}}                      /* TA1_0 ISR                 */
void TA1_N_IRQHandler(void){ while(1){}}                      /* TA1_N ISR                 */
void TA2_0_IRQHandler(void){ while(1){}}                      /* TA2_0 ISR                 */
void TA2_N_IRQHandler(void){ while(1){}}                      /* TA2_N ISR                 */
void TA3_0_IRQHandler(void){ while(1){}}                      /* TA3_0 ISR                 */
void TA3_N_IRQHandler(void){ while(1){}}                      /* TA3_N ISR                 */
void EUSCIA0_IRQHandler(void){ while(1){}}                    /* EUSCIA0 ISR               */
void EUSCIA1_IRQHandler(void){ while(1){}}                    /* EUSCIA1 ISR               */
void EUSCIA2_IRQHandler(void){ while(1){}}                    /* EUSCIA2 ISR               */
void EUSCIA3_IRQHandler(void){ while(1){}}                    /* EUSCIA3 ISR               */
void EUSCIB0_IRQHandler(void){ while(1){}}                    /* EUSCIB0 ISR               */
void EUSCIB1_IRQHandler(void){ while(1){}}                    /* EUSCIB1 ISR               */
void EUSCIB2_IRQHandler(void){ while(1){}}                    /* EUSCIB2 ISR               */
void EUSCIB3_IRQHandler(void){ while(1){}}                    /* EUSCIB3 ISR               */
void ADC14_IRQHandler(void){ while(1){}}                      /* ADC14 ISR                 */
void T32_INT1_IRQHandler(void){ while(1){}}                   /* T32_INT1 ISR              */
void T32_INT2_IRQHandler(void){ while(1){}}                   /* T32_INT2 ISR              */
void T32_INTC_IRQHandler(void){ while(1){}}                   /* T32_INTC ISR              */
void AES256_IRQHandler(void){ while(1){}}                     /* AES ISR                   */
void RTC_C_IRQHandler(void){ while(1){}}                      /* RTC ISR                   */
void DMA_ERR_IRQHandler(void){ while(1){}}                    /* DMA_ERR ISR               */
void DMA_INT3_IRQHandler(void){ while(1){}}                   /* DMA_INT3 ISR              */
void DMA_INT2_IRQHandler(void){ while(1){}}                   /* DMA_INT2 ISR              */
void DMA_INT1_IRQHandler(void){ while(1){}}                   /* DMA_INT1 ISR              */
void DMA_INT0_IRQHandler(void){ while(1){}}                   /* DMA_INT0 ISR              */
void PORT1_IRQHandler(void){ while(1){}}                      /* PORT1 ISR                 */
void PORT2_IRQHandler(void){ while(1){}}                      /* PORT2 ISR                 */
void PORT3_IRQHandler(void){ while(1){}}                      /* PORT3 ISR                 */
void PORT4_IRQHandler(void){ while(1){}}                      /* PORT4 ISR                 */
void PORT5_IRQHandler(void){ while(1){}}                      /* PORT5 ISR                 */
void PORT6_IRQHandler(void){ while(1){}}                      /* PORT6 ISR                 */

//******************************************************************************
//
// Useful functions.
//
//******************************************************************************
extern void  DisableInterrupts(void) ;
extern void  EnableInterrupts(void) ;
extern void  StartCritical(void) ;
extern void  EndCritical(void);
extern void  WaitForInterrupt (void);

//*********** DisableInterrupts ***************
// disable interrupts
// inputs:  none
// outputs: none
void DisableInterrupts(void){
  __asm ("    CPSID  I\n"
         "    BX     LR\n");
}

//*********** EnableInterrupts ***************
// emable interrupts
// inputs:  none
// outputs: none
void EnableInterrupts(void){
  __asm  ("    CPSIE  I\n"
          "    BX     LR\n");
}
//*********** StartCritical ************************
// make a copy of previous I bit, disable interrupts
// inputs:  none
// outputs: previous I bit
void StartCritical(void){
  __asm  ("    MRS    R0, PRIMASK  	; save old status \n"
          "    CPSID  I         	; mask all (except faults)\n"
          "    BX     LR\n");
}

//*********** EndCritical ************************
// using the copy of previous I bit, restore I bit to previous value
// inputs:  previous I bit
// outputs: none
void EndCritical(void){
  __asm  ("    MSR    PRIMASK, R0\n"
          "    BX     LR\n");
}

//*********** WaitForInterrupt ************************
// go to low power mode while waiting for the next interrupt
// inputs:  none
// outputs: none
void WaitForInterrupt(void){
  __asm  ("    WFI\n"
          "    BX     LR\n");
}


