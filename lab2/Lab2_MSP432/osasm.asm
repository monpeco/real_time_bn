;/*****************************************************************************/
; OSasm.asm: low-level OS commands, written in assembly                       */
; Runs on LM4F120/TM4C123/MSP432
; Lab 2 starter file
; February 10, 2016
;

        .thumb
        .text
        .align 2
        .global  RunPt            ; currently running thread
        .global  StartOS
        .global  SysTick_Handler
        .global  Scheduler

RunPtAddr .field RunPt,32
SysTick_Handler:  .asmfunc      ; 1) Saves R0-R3,R12,LR,PC,PSR
    CPSID   I                  ; 2) Prevent interrupt during switch
    PUSH    {R4-R11}           ; 3) Save remaining regs r4-11

    CPSIE   I                  ; 9) tasks run with interrupts enabled
    BX      LR                 ; 10) restore R0-R3,R12,LR,PC,PSR

       .endasmfunc
StartOS: .asmfunc


    CPSIE   I                  ; Enable interrupts at processor level
    BX      LR                 ; start first thread

       .endasmfunc
      .end

