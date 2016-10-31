// BSP.c
// Runs on the MSP432 with an Educational BoosterPack MKII (BOOSTXL-EDUMKII)
// This file contains the software interface to the MKII BoosterPack.
// This board support package (BSP) is an abstraction layer,
//   forming a bridge between the low-level hardware and the high-level software.

// Daniel and Jonathan Valvano
// September 20, 2016

/* This example accompanies the books
   "Embedded Systems: Introduction to the MSP432 Microcontroller",
   ISBN: 978-1512185676, Jonathan Valvano, copyright (c) 2016

   "Embedded Systems: Real-Time Interfacing to the MSP432 Microcontroller",
   ISBN: 978-1514676585, Jonathan Valvano, copyright (c) 2016

 Copyright 2016 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

//  J1   J3               J4   J2
// [ 1] [21]             [40] [20]
// [ 2] [22]             [39] [19]
// [ 3] [23]             [38] [18]
// [ 4] [24]             [37] [17]
// [ 5] [25]             [36] [16]
// [ 6] [26]             [35] [15]
// [ 7] [27]             [34] [14]
// [ 8] [28]             [33] [13]
// [ 9] [29]             [32] [12]
// [10] [30]             [31] [11]

// Connected pins in physical order
// J1.1 +3.3V (power)
// J1.2 joystick horizontal (X) (analog) {TM4C123 PB5/AIN11, MSP432 P6.0}
// J1.3 UART from Bluetooth to LaunchPad (UART) {TM4C123 PB0, MSP432 P3.2}
// J1.4 UART from LaunchPad to Bluetooth (UART) {TM4C123 PB1, MSP432 P3.3}
// J1.5 joystick Select button (digital) {TM4C123 PE4, MSP432 P4.1}
// J1.6 microphone (analog)              {TM4C123 PE5/AIN8, MSP432 P4.3}
// J1.7 LCD SPI clock (SPI)              {TM4C123 PB4, MSP432 P1.5}
// J1.8 ambient light (OPT3001) interrupt (digital) {TM4C123 PA5, MSP432 P4.6}
// J1.9 ambient light (OPT3001) and temperature sensor (TMP006) I2C SCL (I2C)  {TM4C123 PA6, MSP432 P6.5}
// J1.10 ambient light (OPT3001) and temperature sensor (TMP006) I2C SDA (I2C) {TM4C123 PA7, MSP432 P6.4}
//--------------------------------------------------
// J2.11 temperature sensor (TMP006) interrupt (digital) {TM4C123 PA2, MSP432 P3.6}
// J2.12 nothing                         {TM4C123 PA3, MSP432 P5.2}
// J2.13 LCD SPI CS (SPI)                {TM4C123 PA4, MSP432 P5.0}
// J2.14 nothing                         {TM4C123 PB6, MSP432 P1.7}
// J2.15 LCD SPI data (SPI)              {TM4C123 PB7, MSP432 P1.6}
// J2.16 nothing (reset)
// J2.17 LCD !RST (digital)              {TM4C123 PF0, MSP432 P5.7}
// J2.18 Profile 4                       {TM4C123 PE0, MSP432 P3.0}
// J2.19 servo PWM                       {TM4C123 PB2, MSP432 P2.5}
// J2.20 GND (ground)
//--------------------------------------------------
// J3.21 +5V (power)
// J3.22 GND (ground)
// J3.23 accelerometer X (analog)        {TM4C123 PD0/AIN7, MSP432 P6.1}
// J3.24 accelerometer Y (analog)        {TM4C123 PD1/AIN6, MSP432 P4.0}
// J3.25 accelerometer Z (analog)        {TM4C123 PD2/AIN5, MSP432 P4.2}
// J3.26 joystick vertical (Y) (analog)  {TM4C123 PD3/AIN4, MSP432 P4.4}
// J3.27 Profile 0                       {TM4C123 PE1, MSP432 P4.5}
// J3.28 Profile 1                       {TM4C123 PE2, MSP432 P4.7}
// J3.29 Profile 2                       {TM4C123 PE3, MSP432 P5.4}
// J3.30 Profile 3                       {TM4C123 PF1, MSP432 P5.5}
//--------------------------------------------------
// J4.31 LCD RS (digital)                {TM4C123 PF4, MSP432 P3.7}
// J4.32 user Button2 (bottom) (digital) {TM4C123 PD7, MSP432 P3.5}
// J4.33 user Button1 (top) (digital)    {TM4C123 PD6, MSP432 P5.1}
// J4.34 Profile 6/gator hole switch     {TM4C123 PC7, MSP432 P2.3}
// J4.35 nothing                         {TM4C123 PC6, MSP432 P6.7}
// J4.36 Profile 5                       {TM4C123 PC5, MSP432 P6.6}
// J4.37 RGB LED blue (PWM)              {TM4C123 PC4, MSP432 P5.6}
// J4.38 RGB LED green (PWM)             {TM4C123 PB3, MSP432 P2.4}
// J4.39 RGB LED red (jumper up) or LCD backlight (jumper down) (PWM) {TM4C123 PF3, MSP432 P2.6}
// J4.40 buzzer (PWM)                    {TM4C123 PF2, MSP432 P2.7}
//--------------------------------------------------
// Connected pins in logic order
// power and reset
// J1.1 +3.3V (power)
// J3.21 +5V (power)
// J3.22 GND (ground)
// J2.20 GND (ground)
// J2.16 nothing (reset)
//--------------------------------------------------
// LCD graphics
// J1.7 LCD SPI clock (SPI)              {TM4C123 PB4, MSP432 P1.5}
// J2.13 LCD SPI CS (SPI)                {TM4C123 PA4, MSP432 P5.0}
// J2.15 LCD SPI data (SPI)              {TM4C123 PB7, MSP432 P1.6}
// J2.17 LCD !RST (digital)              {TM4C123 PF0, MSP432 P5.7}
// J4.31 LCD RS (digital)                {TM4C123 PF4, MSP432 P3.7}
//--------------------------------------------------
// 3-color LED
// J4.37 RGB LED blue (PWM)              {TM4C123 PC4, MSP432 P5.6}
// J4.38 RGB LED green (PWM)             {TM4C123 PB3, MSP432 P2.4}
// J4.39 RGB LED red (jumper up) or LCD backlight (jumper down) (PWM) {TM4C123 PF3, MSP432 P2.6}
//--------------------------------------------------
// user buttons
// J4.32 user Button2 (bottom) (digital) {TM4C123 PD7, MSP432 P3.5}
// J4.33 user Button1 (top) (digital)    {TM4C123 PD6, MSP432 P5.1}
//--------------------------------------------------
// buzzer output
// J4.40 buzzer (PWM)                    {TM4C123 PF2, MSP432 P2.7}
//--------------------------------------------------
// Joystick
// J1.5 joystick Select button (digital) {TM4C123 PE4, MSP432 P4.1}
// J1.2 joystick horizontal (X) (analog) {TM4C123 PB5/AIN11, MSP432 P6.0}
// J3.26 joystick vertical (Y) (analog)  {TM4C123 PD3/AIN4, MSP432 P4.4}
//--------------------------------------------------
// accelerometer
// J3.23 accelerometer X (analog)        {TM4C123 PD0/AIN7, MSP432 P6.1}
// J3.24 accelerometer Y (analog)        {TM4C123 PD1/AIN6, MSP432 P4.0}
// J3.25 accelerometer Z (analog)        {TM4C123 PD2/AIN5, MSP432 P4.2}
//--------------------------------------------------
// microphone
// J1.6 microphone (analog)              {TM4C123 PE5/AIN8, MSP432 P4.3}
//--------------------------------------------------
// light and temperature sensors (I2C)
// J1.8 ambient light (OPT3001) interrupt (digital) {TM4C123 PA5, MSP432 P4.6}
// J1.9 ambient light (OPT3001) and temperature sensor (TMP006) I2C SCL (I2C)  {TM4C123 PA6, MSP432 P6.5}
// J1.10 ambient light (OPT3001) and temperature sensor (TMP006) I2C SDA (I2C) {TM4C123 PA7, MSP432 P6.4}
// J2.11 temperature sensor (TMP006) interrupt (digital) {TM4C123 PA2, MSP432 P3.6}
//--------------------------------------------------
// Bluetooth booster
// J1.3 UART from Bluetooth to LaunchPad (UART) {TM4C123 PB0, MSP432 P3.2}
// J1.4 UART from LaunchPad to Bluetooth (UART) {TM4C123 PB1, MSP432 P3.3}
//--------------------------------------------------
// profile pins
// J3.27 Profile 0                       {TM4C123 PE1, MSP432 P4.5}
// J3.28 Profile 1                       {TM4C123 PE2, MSP432 P4.7}
// J3.29 Profile 2                       {TM4C123 PE3, MSP432 P5.4}
// J3.30 Profile 3                       {TM4C123 PF1, MSP432 P5.5}
// J2.18 Profile 4                       {TM4C123 PE0, MSP432 P3.0}
// J4.36 Profile 5                       {TM4C123 PC5, MSP432 P6.6}
// J4.34 Profile 6                       {TM4C123 PC7, MSP432 P2.3}
//--------------------------------------------------
// unconnected pins
// J2.12 nothing                         {TM4C123 PA3, MSP432 P5.2}
// J2.14 nothing                         {TM4C123 PB6, MSP432 P1.7}
// J2.19 servo PWM                       {TM4C123 PB2, MSP432 P2.5}
// J4.35 nothing                         {TM4C123 PC6, MSP432 P6.7}

#include <stdint.h>
#include "../inc/BSP.h"
#include "../inc/CortexM.h"
#include "../inc/msp432p401r.h"

static uint32_t ClockFrequency = 3000000; // cycles/second
static uint32_t SubsystemFrequency = 3000000; // cycles/second

// ------------BSP_Button1_Init------------
// Initialize a GPIO pin for input, which corresponds
// with BoosterPack pin J4.33.
// Input: none
// Output: none
void BSP_Button1_Init(void){
  P5SEL0 &= ~0x02;
  P5SEL1 &= ~0x02;                 // configure P5.1 as GPIO
  P5DIR &= ~0x02;                  // make P5.1 in
  P5REN &= ~0x02;                  // disable pull resistor on P5.1
}

// ------------BSP_Button1_Input------------
// Read and return the immediate status of Button1.
// Button de-bouncing is not considered.
// Input: none
// Output: non-zero if Button1 unpressed
//         zero if Button1 pressed
// Assumes: BSP_Button1_Init() has been called
#define BUTTON1   (*((volatile uint8_t *)(0x42000000+32*0x4C40+4*1)))  /* Port 5.1 Input */
uint8_t BSP_Button1_Input(void){
  return BUTTON1;                  // return 0(pressed) or 0x01(not pressed)
}

// ------------BSP_Button2_Init------------
// Initialize a GPIO pin for input, which corresponds
// with BoosterPack pin J4.32.
// Input: none
// Output: none
void BSP_Button2_Init(void){
  P3SEL0 &= ~0x20;
  P3SEL1 &= ~0x20;                 // configure P3.5 as GPIO
  P3DIR &= ~0x20;                  // make P3.5 in
  P3REN &= ~0x20;                  // disable pull resistor on P3.5
}

// ------------BSP_Button2_Input------------
// Read and return the immediate status of Button2.
// Button de-bouncing is not considered.
// Input: none
// Output: non-zero if Button2 unpressed
//         zero if Button2 pressed
// Assumes: BSP_Button2_Init() has been called
#define BUTTON2   (*((volatile uint8_t *)(0x42000000+32*0x4C20+4*5)))  /* Port 3.5 Input */
uint8_t BSP_Button2_Input(void){
  return BUTTON2;                  // return 0(pressed) or 0x01(not pressed)
}

// There are six analog inputs on the Educational BoosterPack MKII:
// microphone (J1.6/P4.3/A10)
// joystick X (J1.2/P6.0/A15) and Y (J3.26/P4.4/A9)
// accelerometer X (J3.23/P6.1/A14), Y (J3.24/P4.0/A13), and Z (J3.25/P4.2/A11)
// All six initialization functions can use this general ADC
// initialization.  The joystick uses sequence starting at 0,
// the accelerometer uses sequence starting at 2, and the
// microphone uses sequence starting at 5.
void static adcinit(void){
/*reference not used--analog peripherals are 0 to 3.3V
  while(REFCTL0&0x0400){};         // wait for the reference to be idle before reconfiguring (see REF4 in errata)
  // 15-14 reserved                          00b (reserved)
  // 13    REFBGRDY   bandgap voltage ready   0b (read only)
  // 12    REFGENRDY  reference voltage ready 0b (read only)
  // 11    BGMODE     bandgap mode            0b (read only)
  // 10    REFGENBUSY no reconfigure if busy  0b (read only)
  // 9     REFBGACT   bandgap active          0b (read only)
  // 8     REFGENACT  reference active        0b (read only)
  // 7     REFBGOT    trigger bandgap         0b = no software trigger set
  // 6     REFGENOT   trigger reference       0b = no software trigger set
  // 5-4   REFVSEL    voltage level select   11b = 2.5V
  // 3     REFTCOFF   temperature disable     1b = temperature sensor disabled to save power
  // 2     reserved                           0b (reserved)
  // 1     REFOUT     reference output buffer 0b = reference output not on P5.6 (see also REFBURST in ADC14CTL1, P5SEL1, and P5SEL0)
  // 0     REFON      reference enable        1b = reference enabled in static mode
  REFCTL0 = 0x0039;                // 1) configure reference for static 2.5V
  while((REFCTL0&0x1000) == 0){};  // wait for the reference to stabilize before continuing (optional, see REF8 in errata)
*/
  ADC14CTL0 &= ~0x00000002;        // 2) ADC14ENC = 0 to allow programming
  while(ADC14CTL0&0x00010000){};   // 3) wait for BUSY to be zero
  // 31-30 ADC14PDIV  predivider,            00b = Predivide by 1
  // 29-27 ADC14SHSx  SHM source            000b = ADC14SC bit
  // 26    ADC14SHP   SHM pulse-mode          1b = SAMPCON the sampling timer
  // 25    ADC14ISSH  invert sample-and-hold  0b = not inverted
  // 24-22 ADC14DIVx  clock divider         000b = /1
  // 21-19 ADC14SSELx clock source select   100b = SMCLK
  // 18-17 ADC14CONSEQx mode select          01b = Sequence-of-channels
  // 16    ADC14BUSY  ADC14 busy              0b (read only)
  // 15-12 ADC14SHT1x sample-and-hold time 0011b = 32 clocks
  // 11-8  ADC14SHT0x sample-and-hold time 0011b = 32 clocks
  // 7     ADC14MSC   multiple sample         1b = continue conversions automatically after first SHI signal trigger
  // 6-5   reserved                          00b (reserved)
  // 4     ADC14ON    ADC14 on                1b = powered up
  // 3-2   reserved                          00b (reserved)
  // 1     ADC14ENC   enable conversion       0b = ADC14 disabled
  // 0     ADC14SC    ADC14 start             0b = No start (yet)
  ADC14CTL0 = 0x04223390;          // 4) single, SMCLK, on, disabled, /1, 32 SHM
  // 20-16 STARTADDx  start addr          00000b = ADC14MEM0
  // 15-6  reserved                  0000000000b (reserved)
  // 5-4   ADC14RES   ADC14 resolution       11b = 14 bit, 16 clocks
  // 3     ADC14DF    data read-back format   0b = Binary unsigned
  // 2     REFBURST   reference buffer burst  0b = reference on continuously
  // 1-0   ADC14PWRMD ADC power modes        00b = Regular power mode
  ADC14CTL1 = 0x00000030;          // 5) ADC14MEM0, 14-bit, ref on, regular power
                                   // 6) different for each initialization function
  ADC14IER0 = 0;
  ADC14IER1 = 0;                   // 7) no interrupts
}

// ------------BSP_Joystick_Init------------
// Initialize a GPIO pin for input, which corresponds
// with BoosterPack pin J1.5 (Select button).
// Initialize two ADC pins, which correspond with
// BoosterPack pins J1.2 (X) and J3.26 (Y).
// Input: none
// Output: none
void BSP_Joystick_Init(void){
  adcinit();
  // 15   ADC14WINCTH Window comp threshold   0b = not used
  // 14   ADC14WINC   Comparator enable       0b = Comparator disabled
  // 13   ADC14DIF    Differential mode       0b = Single-ended mode enabled
  // 12   reserved                            0b (reserved)
  // 11-8 ADC14VRSEL  V(R+) and V(R-)      0000b = V(R+) = AVCC, V(R-) = AVSS
  // 7    ADC14EOS    End of sequence         0b = Not end of sequence
  // 6-5  reserved                           00b (reserved)
  // 4-0  ADC14INCHx  Input channel       01111b = A15, P6.0
  ADC14MCTL0 = 0x0000000F;         // 6a) 0 to 3.3V, channel 15
  // 15   ADC14WINCTH Window comp threshold   0b = not used
  // 14   ADC14WINC   Comparator enable       0b = Comparator disabled
  // 13   ADC14DIF    Differential mode       0b = Single-ended mode enabled
  // 12   reserved                            0b (reserved)
  // 11-8 ADC14VRSEL  V(R+) and V(R-)      0000b = V(R+) = AVCC, V(R-) = AVSS
  // 7    ADC14EOS    End of sequence         1b = End of sequence
  // 6-5  reserved                           00b (reserved)
  // 4-0  ADC14INCHx  Input channel       01001b = A9, P4.4
  ADC14MCTL1 = 0x00000089;         // 6b) 0 to 3.3V, channel 9
  P6SEL0 |= 0x01;
  P6SEL1 |= 0x01;                  // 8a) analog mode on P6.0/A15
  P4SEL0 |= 0x10;
  P4SEL1 |= 0x10;                  // 8b) analog mode on P4.4/A9
  ADC14CTL0 |= 0x00000002;         // 9) enable
  P4SEL0 &= ~0x02;
  P4SEL1 &= ~0x02;                 // configure P4.1 as GPIO
  P4DIR &= ~0x02;                  // make P4.1 in
  P4REN &= ~0x02;                  // disable pull resistor on P4.1
}

// ------------BSP_Joystick_Input------------
// Read and return the immediate status of the
// joystick.  Button de-bouncing for the Select
// button is not considered.  The joystick X- and
// Y-positions are returned as 10-bit numbers,
// even if the ADC on the LaunchPad is more precise.
// Input: x is pointer to store X-position (0 to 1023)
//        y is pointer to store Y-position (0 to 1023)
//        select is pointer to store Select status (0 if pressed)
// Output: none
// Assumes: BSP_Joystick_Init() has been called
#define SELECT    (*((volatile uint8_t *)(0x42000000+32*0x4C21+4*1)))  /* Port 4.1 Input */
void BSP_Joystick_Input(uint16_t *x, uint16_t *y, uint8_t *select){
  ADC14CTL0 &= ~0x00000002;        // 1) ADC14ENC = 0 to allow programming
  while(ADC14CTL0&0x00010000){};   // 2) wait for BUSY to be zero
  ADC14CTL1 = (ADC14CTL1&~0x001F0000) | // clear STARTADDx bit field
              (0 << 16);           // 3) configure for STARTADDx = 0
  ADC14CTL0 |= 0x00000002;         // 4) enable conversions
  while(ADC14CTL0&0x00010000){};   // 5) wait for BUSY to be zero
  ADC14CTL0 |= 0x00000001;         // 6) start single conversion
  while((ADC14IFGR0&0x02) == 0){}; // 7) wait for ADC14IFG1
  *x = ADC14MEM0>>4;               // 8) P6.0/A15 result 0 to 1023
  *y = ADC14MEM1>>4;               //    P4.4/A9 result 0 to 1023
  *select = SELECT;                // return 0(pressed) or 0x01(not pressed)
}

// ------------BSP_RGB_Init------------
// Initialize the GPIO and PWM or timer modules which
// correspond with BoosterPack pins J4.39 (red),
// J4.38 (green), and J4.37 (blue).  The frequency
// must be fast enough to not appear to flicker, and
// the duty cycle is represented as a 10-bit number.
// 1023 is fully (or nearly fully) on.
// 0 is fully (or nearly fully) off.
// Input: red is 10-bit duty cycle for red
//        green is 10-bit duty cycle for green
//        blue is 10-bit duty cycle for blue
// Output: none
static uint16_t PWMCycles;         // number of PWM cycles per period
void BSP_RGB_Init(uint16_t red, uint16_t green, uint16_t blue){
  if((red > 1023) || (green > 1023) || (blue > 1023)){
    return;                        // invalid input
  }
  // ***************** Timer A2 initialization *****************
  // RGB LED blue connected to J4.37, which is P5.6/TA2.1 on MSP432 LaunchPad
  TA2CTL &= ~0x0030;               // halt Timer A2
  // bits15-10=XXXXXX, reserved
  // bits9-8=10,       clock source to SMCLK
  // bits7-6=00,       input clock divider /1
  // bits5-4=00,       stop mode
  // bit3=X,           reserved
  // bit2=0,           set this bit to clear
  // bit1=0,           interrupt disable
  // bit0=0,           clear interrupt pending
  TA2CTL = 0x0200;
  // bits15-14=00,     no capture mode
  // bits13-12=XX,     capture/compare input select
  // bit11=X,          synchronize capture source
  // bit10=X,          synchronized capture/compare input
  // bit9=X,           reserved
  // bit8=0,           compare mode
  // bits7-5=111,      reset/set output mode
  // bit4=0,           disable capture/compare interrupt
  // bit3=X,           read capture/compare input from here
  // bit2=X,           output this value in output mode 0
  // bit1=X,           capture overflow status
  // bit0=0,           clear capture/compare interrupt pending
  TA2CCTL1 = 0x00E0;
  PWMCycles = SubsystemFrequency/2048;
  TA2CCR1 = (blue*PWMCycles)>>10;  // defines when output signal is cleared
  TA2CCR0 = PWMCycles - 1;         // defines when output signal is set
  TA2EX0 &= ~0x0007;               // configure for input clock divider /1
  TA2CTL |= 0x0014;                // reset and start Timer A2 in up mode
  // ***************** Timer A0 initialization *****************
  // RGB LED green connected to J4.38, which is P2.4/TA0.1 on MSP432 LaunchPad
  // RGB LED red (jumper up) connected to J4.39 (PWM), which is P2.6/TA0.3 on MSP432 LaunchPad
  TA0CTL &= ~0x0030;               // halt Timer A0
  // bits15-10=XXXXXX, reserved
  // bits9-8=10,       clock source to SMCLK
  // bits7-6=00,       input clock divider /1
  // bits5-4=00,       stop mode
  // bit3=X,           reserved
  // bit2=0,           set this bit to clear
  // bit1=0,           interrupt disable
  // bit0=0,           clear interrupt pending
  TA0CTL = 0x0200;
  // bits15-14=00,     no capture mode
  // bits13-12=XX,     capture/compare input select
  // bit11=X,          synchronize capture source
  // bit10=X,          synchronized capture/compare input
  // bit9=X,           reserved
  // bit8=0,           compare mode
  // bits7-5=111,      reset/set output mode
  // bit4=0,           disable capture/compare interrupt
  // bit3=X,           read capture/compare input from here
  // bit2=X,           output this value in output mode 0
  // bit1=X,           capture overflow status
  // bit0=0,           clear capture/compare interrupt pending
  TA0CCTL1 = 0x00E0;
  TA0CCR1 = (green*PWMCycles)>>10; // defines when output signal is cleared
  // bits15-14=00,     no capture mode
  // bits13-12=XX,     capture/compare input select
  // bit11=X,          synchronize capture source
  // bit10=X,          synchronized capture/compare input
  // bit9=X,           reserved
  // bit8=0,           compare mode
  // bits7-5=111,      reset/set output mode
  // bit4=0,           disable capture/compare interrupt
  // bit3=X,           read capture/compare input from here
  // bit2=X,           output this value in output mode 0
  // bit1=X,           capture overflow status
  // bit0=0,           clear capture/compare interrupt pending
  TA0CCTL3 = 0x00E0;
  TA0CCR3 = (red*PWMCycles)>>10;   // defines when output signal is cleared
  TA0CCR0 = PWMCycles - 1;         // defines when output signals are set
  TA0EX0 &= ~0x0007;               // configure for input clock divider /1
  TA0CTL |= 0x0014;                // reset and start Timer A0 in up mode
  // ***************** GPIO initialization *****************
  P2SEL0 |= 0x50;
  P2SEL1 &= ~0x50;                 // configure P2.6 and P2.4 as timer out
  P2DIR |= 0x50;                   // make P2.4 and P2.6 out
  P5SEL0 |= 0x40;
  P5SEL1 &= ~0x40;                 // configure P5.6 as timer out
  P5DIR |= 0x40;                   // make P5.6 out
}

// ------------BSP_RGB_Set------------
// Set new duty cycles for the RGB LEDs.
// 1023 is fully (or nearly fully) on.
// 0 is fully (or nearly fully) off.
// Input: red is 10-bit duty cycle for red
//        green is 10-bit duty cycle for green
//        blue is 10-bit duty cycle for blue
// Output: none
// Assumes: BSP_RGB_Init() has been called
void BSP_RGB_Set(uint16_t red, uint16_t green, uint16_t blue){
  if((red > 1023) || (green > 1023) || (blue > 1023)){
    return;                        // invalid input
  }
  TA0CCR3 = (red*PWMCycles)>>10;   // defines when output signal is cleared
  TA0CCR1 = (green*PWMCycles)>>10; // defines when output signal is cleared
  TA2CCR1 = (blue*PWMCycles)>>10;  // defines when output signal is cleared
}

// ------------BSP_RGB_D_Init------------
// Initialize the GPIO pins for output which
// correspond with BoosterPack pins J4.39 (red),
// J4.38 (green), and J4.37 (blue).  Instead of using
// PWM or timer modules, this configuration uses just
// digital fully on or fully off.
// non-zero is fully on.
// 0 is fully off.
// Input: red is initial status for red
//        green is initial status for green
//        blue is initial status for blue
// Output: none
#define RED       (*((volatile uint8_t *)(0x42000000+32*0x4C03+4*6)))  /* Port 2.6 Output */
#define GREEN     (*((volatile uint8_t *)(0x42000000+32*0x4C03+4*4)))  /* Port 2.4 Output */
#define BLUE      (*((volatile uint8_t *)(0x42000000+32*0x4C42+4*6)))  /* Port 5.6 Output */
void BSP_RGB_D_Init(int red, int green, int blue){
  if(red){
    RED = 0x01;
  } else{
    RED = 0x00;
  }
  if(green){
    GREEN = 0x01;
  } else{
    GREEN = 0x00;
  }
  if(blue){
    BLUE = 0x01;
  } else{
    BLUE = 0x00;
  }
  P2SEL0 &= ~0x50;
  P2SEL1 &= ~0x50;                 // configure P2.6 and P2.4 as GPIO
  P2DIR |= 0x50;                   // make P2.4 and P2.6 out
  P5SEL0 &= ~0x40;
  P5SEL1 &= ~0x40;                 // configure P5.6 as GPIO
  P5DIR |= 0x40;                   // make P5.6 out
}

// ------------BSP_RGB_D_Set------------
// Set new statuses for the RGB LEDs.
// non-zero is fully on.
// 0 is fully off.
// Input: red is status for red
//        green is status for green
//        blue is status for blue
// Output: none
// Assumes: BSP_RGB_D_Init() has been called
void BSP_RGB_D_Set(int red, int green, int blue){
  if(red){
    RED = 0x01;
  } else{
    RED = 0x00;
  }
  if(green){
    GREEN = 0x01;
  } else{
    GREEN = 0x00;
  }
  if(blue){
    BLUE = 0x01;
  } else{
    BLUE = 0x00;
  }
}

// ------------BSP_RGB_D_Toggle------------
// Toggle the statuses of the RGB LEDs.
// non-zero is toggle.
// 0 is do not toggle.
// Input: red is toggle for red
//        green is toggle for green
//        blue is toggle for blue
// Output: none
// Assumes: BSP_RGB_D_Init() has been called
void BSP_RGB_D_Toggle(int red, int green, int blue){
  if(red){
    RED = RED^0x01;
  }
  if(green){
    GREEN = GREEN^0x01;
  }
  if(blue){
    BLUE = BLUE^0x01;
  }
}

// ------------BSP_Buzzer_Init------------
// Initialize the GPIO and PWM or timer modules which
// correspond with BoosterPack pin J4.40.  The
// frequency is 2048 Hz, and the duty cycle is
// represented as a 10-bit number.
// Input: duty is 10-bit duty cycle for the buzzer
// Output: none
void BSP_Buzzer_Init(uint16_t duty){
  if(duty > 1023){
    return;                        // invalid input
  }
  // ***************** Timer A0 initialization *****************
  // buzzer connected to J4.40 (PWM), which is P2.7/TA0.4 on MSP432 LaunchPad
  TA0CTL &= ~0x0030;               // halt Timer A0
  // bits15-10=XXXXXX, reserved
  // bits9-8=10,       clock source to SMCLK
  // bits7-6=00,       input clock divider /1
  // bits5-4=00,       stop mode
  // bit3=X,           reserved
  // bit2=0,           set this bit to clear
  // bit1=0,           interrupt disable
  // bit0=0,           clear interrupt pending
  TA0CTL = 0x0200;
  // bits15-14=00,     no capture mode
  // bits13-12=XX,     capture/compare input select
  // bit11=X,          synchronize capture source
  // bit10=X,          synchronized capture/compare input
  // bit9=X,           reserved
  // bit8=0,           compare mode
  // bits7-5=111,      reset/set output mode
  // bit4=0,           disable capture/compare interrupt
  // bit3=X,           read capture/compare input from here
  // bit2=X,           output this value in output mode 0
  // bit1=X,           capture overflow status
  // bit0=0,           clear capture/compare interrupt pending
  TA0CCTL4 = 0x00E0;
  PWMCycles = SubsystemFrequency/2048;
  TA0CCR4 = (duty*PWMCycles)>>10;  // defines when output signal is cleared
  TA0CCR0 = PWMCycles - 1;         // defines when output signals are set
  TA0EX0 &= ~0x0007;               // configure for input clock divider /1
  TA0CTL |= 0x0014;                // reset and start Timer A0 in up mode
  // ***************** GPIO initialization *****************
  P2SEL0 |= 0x80;
  P2SEL1 &= ~0x80;                 // configure P2.7 as timer out
  P2DIR |= 0x80;                   // make P2.7 out
}

// ------------BSP_Buzzer_Set------------
// Set new duty cycle for the buzzer.
// 512 is typical for sound (50% duty cycle).
// 0 is typical for silent (0% duty cycle).
// Input: duty is 10-bit duty cycle for the buzzer
// Output: none
// Assumes: BSP_Buzzer_Init() has been called
void BSP_Buzzer_Set(uint16_t duty){
  if(duty > 1023){
    return;                        // invalid input
  }
  TA0CCR4 = (duty*PWMCycles)>>10;  // defines when output signal is cleared
}

// ------------BSP_Accelerometer_Init------------
// Initialize three ADC pins, which correspond with
// BoosterPack pins J3.23 (X), J3.24 (Y), and
// J3.25 (Y).
// Input: none
// Output: none
void BSP_Accelerometer_Init(void){
  adcinit();
  // 15   ADC14WINCTH Window comp threshold   0b = not used
  // 14   ADC14WINC   Comparator enable       0b = Comparator disabled
  // 13   ADC14DIF    Differential mode       0b = Single-ended mode enabled
  // 12   reserved                            0b (reserved)
  // 11-8 ADC14VRSEL  V(R+) and V(R-)      0000b = V(R+) = AVCC, V(R-) = AVSS
  // 7    ADC14EOS    End of sequence         0b = Not end of sequence
  // 6-5  reserved                           00b (reserved)
  // 4-0  ADC14INCHx  Input channel       01110b = A14, P6.1
  ADC14MCTL2 = 0x0000000E;         // 6a) 0 to 3.3V, channel 14
  // 15   ADC14WINCTH Window comp threshold   0b = not used
  // 14   ADC14WINC   Comparator enable       0b = Comparator disabled
  // 13   ADC14DIF    Differential mode       0b = Single-ended mode enabled
  // 12   reserved                            0b (reserved)
  // 11-8 ADC14VRSEL  V(R+) and V(R-)      0000b = V(R+) = AVCC, V(R-) = AVSS
  // 7    ADC14EOS    End of sequence         0b = not end of sequence
  // 6-5  reserved                           00b (reserved)
  // 4-0  ADC14INCHx  Input channel       01101b = A13, P4.0
  ADC14MCTL3 = 0x0000000D;         // 6b) 0 to 3.3V, channel 13
  // 15   ADC14WINCTH Window comp threshold   0b = not used
  // 14   ADC14WINC   Comparator enable       0b = Comparator disabled
  // 13   ADC14DIF    Differential mode       0b = Single-ended mode enabled
  // 12   reserved                            0b (reserved)
  // 11-8 ADC14VRSEL  V(R+) and V(R-)      0000b = V(R+) = AVCC, V(R-) = AVSS
  // 7    ADC14EOS    End of sequence         1b = End of sequence
  // 6-5  reserved                           00b (reserved)
  // 4-0  ADC14INCHx  Input channel       01011b = A11, P4.2
  ADC14MCTL4 = 0x0000008B;         // 6c) 0 to 3.3V, channel 11
  P6SEL0 |= 0x02;
  P6SEL1 |= 0x02;                  // 8a) analog mode on P6.1/A14
  P4SEL0 |= 0x05;
  P4SEL1 |= 0x05;                  // 8bc) analog mode on P4.2/A11 and P4.0/A13
  ADC14CTL0 |= 0x00000002;         // 9) enable
}

// ------------BSP_Accelerometer_Input------------
// Read and return the immediate status of the
// accelerometer.  The accelerometer X-, Y-, and
// Z-measurements are returned as 10-bit numbers,
// even if the ADC on the LaunchPad is more precise.
// Input: x is pointer to store X-measurement (0 to 1023)
//        y is pointer to store Y-measurement (0 to 1023)
//        z is pointer to store Z-measurement (0 to 1023)
// Output: none
// Assumes: BSP_Accelerometer_Init() has been called
void BSP_Accelerometer_Input(uint16_t *x, uint16_t *y, uint16_t *z){
  ADC14CTL0 &= ~0x00000002;        // 1) ADC14ENC = 0 to allow programming
  while(ADC14CTL0&0x00010000){};   // 2) wait for BUSY to be zero
  ADC14CTL1 = (ADC14CTL1&~0x001F0000) | // clear STARTADDx bit field
              (2 << 16);           // 3) configure for STARTADDx = 2
  ADC14CTL0 |= 0x00000002;         // 4) enable conversions
  while(ADC14CTL0&0x00010000){};   // 5) wait for BUSY to be zero
  ADC14CTL0 |= 0x00000001;         // 6) start single conversion
  while((ADC14IFGR0&0x10) == 0){}; // 7) wait for ADC14IFG4
  *x = ADC14MEM2>>4;               // 8) P6.1/A14 result 0 to 1023
  *y = ADC14MEM3>>4;               //    P4.0/A13 result 0 to 1023
  *z = ADC14MEM4>>4;               //    P4.2/A11 result 0 to 1023
}

// ------------BSP_Microphone_Init------------
// Initialize one ADC pin, which corresponds with
// BoosterPack pin J1.6.
// Input: none
// Output: none
void BSP_Microphone_Init(void){
  adcinit();
  // 15   ADC14WINCTH Window comp threshold   0b = not used
  // 14   ADC14WINC   Comparator enable       0b = Comparator disabled
  // 13   ADC14DIF    Differential mode       0b = Single-ended mode enabled
  // 12   reserved                            0b (reserved)
  // 11-8 ADC14VRSEL  V(R+) and V(R-)      0000b = V(R+) = AVCC, V(R-) = AVSS
  // 7    ADC14EOS    End of sequence         1b = End of sequence
  // 6-5  reserved                           00b (reserved)
  // 4-0  ADC14INCHx  Input channel       01010b = A10, P4.3
  ADC14MCTL5 = 0x0000008A;         // 6) 0 to 3.3V, channel 10
  P4SEL0 |= 0x08;
  P4SEL1 |= 0x08;                  // 8) analog mode on P4.3/A10
  ADC14CTL0 |= 0x00000002;         // 9) enable
}

// ------------BSP_Microphone_Input------------
// Read and return the immediate status of the
// microphone.  The sound measurement is returned
// as a 10-bit number, even if the ADC on the
// LaunchPad is more precise.
// Input: mic is pointer to store sound measurement (0 to 1023)
// Output: none
// Assumes: BSP_Microphone_Init() has been called
void BSP_Microphone_Input(uint16_t *mic){
  ADC14CTL0 &= ~0x00000002;        // 1) ADC14ENC = 0 to allow programming
  while(ADC14CTL0&0x00010000){};   // 2) wait for BUSY to be zero
  ADC14CTL1 = (ADC14CTL1&~0x001F0000) | // clear STARTADDx bit field
              (5 << 16);           // 3) configure for STARTADDx = 5
  ADC14CTL0 |= 0x00000002;         // 4) enable conversions
  while(ADC14CTL0&0x00010000){};   // 5) wait for BUSY to be zero
  ADC14CTL0 |= 0x00000001;         // 6) start single conversion
  while((ADC14IFGR0&0x20) == 0){}; // 7) wait for ADC14IFG5
  *mic = ADC14MEM5>>4;             // 8) P4.3/A10 result 0 to 1023
}


/* ********************** */
/*      LCD Section       */
/* ********************** */
// This section is based on ST7735.c, which itself is based
// on example code originally from Adafruit.  Some sections
// such as the font table and initialization functions were
// copied verbatim from Adafruit's example and are subject
// to the following disclosure.
/***************************************************
  This is a library for the Adafruit 1.8" SPI display.
  This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
  as well as Adafruit raw 1.8" TFT displayun
  ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
// some flags for ST7735_InitR()
enum initRFlags{
  none,
  INITR_GREENTAB,
  INITR_REDTAB,
  INITR_BLACKTAB
};

#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 128


// Color definitions
#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0x001F
#define ST7735_RED     0xF800
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF


// 12 rows (0 to 11) and 21 characters (0 to 20)
// Requires (11 + size*size*6*8) bytes of transmission for each character
uint32_t StX=0; // position along the horizonal axis 0 to 20
uint32_t StY=0; // position along the vertical axis 0 to 11
uint16_t StTextColor = ST7735_YELLOW;

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

#define TFT_CS                  (*((volatile uint8_t *)(0x42000000+32*0x4C42+4*0)))  /* Port 5 Output, bit 0 is TFT CS */
#define DC                      (*((volatile uint8_t *)(0x42000000+32*0x4C22+4*7)))  /* Port 3 Output, bit 7 is DC */
#define RESET                   (*((volatile uint8_t *)(0x42000000+32*0x4C42+4*7)))  /* Port 5 Output, bit 7 is RESET*/

// standard ascii 5x7 font
// originally from glcdfont.c from Adafruit project
static const uint8_t Font[] = {
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
  0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
  0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
  0x18, 0x3C, 0x7E, 0x3C, 0x18,
  0x1C, 0x57, 0x7D, 0x57, 0x1C,
  0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
  0x00, 0x18, 0x3C, 0x18, 0x00,
  0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
  0x00, 0x18, 0x24, 0x18, 0x00,
  0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
  0x30, 0x48, 0x3A, 0x06, 0x0E,
  0x26, 0x29, 0x79, 0x29, 0x26,
  0x40, 0x7F, 0x05, 0x05, 0x07,
  0x40, 0x7F, 0x05, 0x25, 0x3F,
  0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
  0x7F, 0x3E, 0x1C, 0x1C, 0x08,
  0x08, 0x1C, 0x1C, 0x3E, 0x7F,
  0x14, 0x22, 0x7F, 0x22, 0x14,
  0x5F, 0x5F, 0x00, 0x5F, 0x5F,
  0x06, 0x09, 0x7F, 0x01, 0x7F,
  0x00, 0x66, 0x89, 0x95, 0x6A,
  0x60, 0x60, 0x60, 0x60, 0x60,
  0x94, 0xA2, 0xFF, 0xA2, 0x94,
  0x08, 0x04, 0x7E, 0x04, 0x08,
  0x10, 0x20, 0x7E, 0x20, 0x10,
  0x08, 0x08, 0x2A, 0x1C, 0x08,
  0x08, 0x1C, 0x2A, 0x08, 0x08,
  0x1E, 0x10, 0x10, 0x10, 0x10,
  0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
  0x30, 0x38, 0x3E, 0x38, 0x30,
  0x06, 0x0E, 0x3E, 0x0E, 0x06,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x5F, 0x00, 0x00,
  0x00, 0x07, 0x00, 0x07, 0x00,
  0x14, 0x7F, 0x14, 0x7F, 0x14,
  0x24, 0x2A, 0x7F, 0x2A, 0x12,
  0x23, 0x13, 0x08, 0x64, 0x62,
  0x36, 0x49, 0x56, 0x20, 0x50,
  0x00, 0x08, 0x07, 0x03, 0x00,
  0x00, 0x1C, 0x22, 0x41, 0x00,
  0x00, 0x41, 0x22, 0x1C, 0x00,
  0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
  0x08, 0x08, 0x3E, 0x08, 0x08,
  0x00, 0x80, 0x70, 0x30, 0x00,
  0x08, 0x08, 0x08, 0x08, 0x08,
  0x00, 0x00, 0x60, 0x60, 0x00,
  0x20, 0x10, 0x08, 0x04, 0x02,
  0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
  0x00, 0x42, 0x7F, 0x40, 0x00, // 1
  0x72, 0x49, 0x49, 0x49, 0x46, // 2
  0x21, 0x41, 0x49, 0x4D, 0x33, // 3
  0x18, 0x14, 0x12, 0x7F, 0x10, // 4
  0x27, 0x45, 0x45, 0x45, 0x39, // 5
  0x3C, 0x4A, 0x49, 0x49, 0x31, // 6
  0x41, 0x21, 0x11, 0x09, 0x07, // 7
  0x36, 0x49, 0x49, 0x49, 0x36, // 8
  0x46, 0x49, 0x49, 0x29, 0x1E, // 9
  0x00, 0x00, 0x14, 0x00, 0x00,
  0x00, 0x40, 0x34, 0x00, 0x00,
  0x00, 0x08, 0x14, 0x22, 0x41,
  0x14, 0x14, 0x14, 0x14, 0x14,
  0x00, 0x41, 0x22, 0x14, 0x08,
  0x02, 0x01, 0x59, 0x09, 0x06,
  0x3E, 0x41, 0x5D, 0x59, 0x4E,
  0x7C, 0x12, 0x11, 0x12, 0x7C, // A
  0x7F, 0x49, 0x49, 0x49, 0x36, // B
  0x3E, 0x41, 0x41, 0x41, 0x22, // C
  0x7F, 0x41, 0x41, 0x41, 0x3E, // D
  0x7F, 0x49, 0x49, 0x49, 0x41, // E
  0x7F, 0x09, 0x09, 0x09, 0x01, // F
  0x3E, 0x41, 0x41, 0x51, 0x73, // G
  0x7F, 0x08, 0x08, 0x08, 0x7F, // H
  0x00, 0x41, 0x7F, 0x41, 0x00, // I
  0x20, 0x40, 0x41, 0x3F, 0x01, // J
  0x7F, 0x08, 0x14, 0x22, 0x41, // K
  0x7F, 0x40, 0x40, 0x40, 0x40, // L
  0x7F, 0x02, 0x1C, 0x02, 0x7F, // M
  0x7F, 0x04, 0x08, 0x10, 0x7F, // N
  0x3E, 0x41, 0x41, 0x41, 0x3E, // O
  0x7F, 0x09, 0x09, 0x09, 0x06, // P
  0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
  0x7F, 0x09, 0x19, 0x29, 0x46, // R
  0x26, 0x49, 0x49, 0x49, 0x32, // S
  0x03, 0x01, 0x7F, 0x01, 0x03, // T
  0x3F, 0x40, 0x40, 0x40, 0x3F, // U
  0x1F, 0x20, 0x40, 0x20, 0x1F, // V
  0x3F, 0x40, 0x38, 0x40, 0x3F, // W
  0x63, 0x14, 0x08, 0x14, 0x63, // X
  0x03, 0x04, 0x78, 0x04, 0x03, // Y
  0x61, 0x59, 0x49, 0x4D, 0x43, // Z
  0x00, 0x7F, 0x41, 0x41, 0x41,
  0x02, 0x04, 0x08, 0x10, 0x20,
  0x00, 0x41, 0x41, 0x41, 0x7F,
  0x04, 0x02, 0x01, 0x02, 0x04,
  0x40, 0x40, 0x40, 0x40, 0x40,
  0x00, 0x03, 0x07, 0x08, 0x00,
  0x20, 0x54, 0x54, 0x78, 0x40, // a
  0x7F, 0x28, 0x44, 0x44, 0x38, // b
  0x38, 0x44, 0x44, 0x44, 0x28, // c
  0x38, 0x44, 0x44, 0x28, 0x7F, // d
  0x38, 0x54, 0x54, 0x54, 0x18, // e
  0x00, 0x08, 0x7E, 0x09, 0x02, // f
  0x18, 0xA4, 0xA4, 0x9C, 0x78, // g
  0x7F, 0x08, 0x04, 0x04, 0x78, // h
  0x00, 0x44, 0x7D, 0x40, 0x00, // i
  0x20, 0x40, 0x40, 0x3D, 0x00, // j
  0x7F, 0x10, 0x28, 0x44, 0x00, // k
  0x00, 0x41, 0x7F, 0x40, 0x00, // l
  0x7C, 0x04, 0x78, 0x04, 0x78, // m
  0x7C, 0x08, 0x04, 0x04, 0x78, // n
  0x38, 0x44, 0x44, 0x44, 0x38, // o
  0xFC, 0x18, 0x24, 0x24, 0x18, // p
  0x18, 0x24, 0x24, 0x18, 0xFC, // q
  0x7C, 0x08, 0x04, 0x04, 0x08, // r
  0x48, 0x54, 0x54, 0x54, 0x24, // s
  0x04, 0x04, 0x3F, 0x44, 0x24, // t
  0x3C, 0x40, 0x40, 0x20, 0x7C, // u
  0x1C, 0x20, 0x40, 0x20, 0x1C, // v
  0x3C, 0x40, 0x30, 0x40, 0x3C, // w
  0x44, 0x28, 0x10, 0x28, 0x44, // x
  0x4C, 0x90, 0x90, 0x90, 0x7C, // y
  0x44, 0x64, 0x54, 0x4C, 0x44, // z
  0x00, 0x08, 0x36, 0x41, 0x00,
  0x00, 0x00, 0x77, 0x00, 0x00,
  0x00, 0x41, 0x36, 0x08, 0x00,
  0x02, 0x01, 0x02, 0x04, 0x02,
  0x3C, 0x26, 0x23, 0x26, 0x3C,
  0x1E, 0xA1, 0xA1, 0x61, 0x12,
  0x3A, 0x40, 0x40, 0x20, 0x7A,
  0x38, 0x54, 0x54, 0x55, 0x59,
  0x21, 0x55, 0x55, 0x79, 0x41,
  0x21, 0x54, 0x54, 0x78, 0x41,
  0x21, 0x55, 0x54, 0x78, 0x40,
  0x20, 0x54, 0x55, 0x79, 0x40,
  0x0C, 0x1E, 0x52, 0x72, 0x12,
  0x39, 0x55, 0x55, 0x55, 0x59,
  0x39, 0x54, 0x54, 0x54, 0x59,
  0x39, 0x55, 0x54, 0x54, 0x58,
  0x00, 0x00, 0x45, 0x7C, 0x41,
  0x00, 0x02, 0x45, 0x7D, 0x42,
  0x00, 0x01, 0x45, 0x7C, 0x40,
  0xF0, 0x29, 0x24, 0x29, 0xF0,
  0xF0, 0x28, 0x25, 0x28, 0xF0,
  0x7C, 0x54, 0x55, 0x45, 0x00,
  0x20, 0x54, 0x54, 0x7C, 0x54,
  0x7C, 0x0A, 0x09, 0x7F, 0x49,
  0x32, 0x49, 0x49, 0x49, 0x32,
  0x32, 0x48, 0x48, 0x48, 0x32,
  0x32, 0x4A, 0x48, 0x48, 0x30,
  0x3A, 0x41, 0x41, 0x21, 0x7A,
  0x3A, 0x42, 0x40, 0x20, 0x78,
  0x00, 0x9D, 0xA0, 0xA0, 0x7D,
  0x39, 0x44, 0x44, 0x44, 0x39,
  0x3D, 0x40, 0x40, 0x40, 0x3D,
  0x3C, 0x24, 0xFF, 0x24, 0x24,
  0x48, 0x7E, 0x49, 0x43, 0x66,
  0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
  0xFF, 0x09, 0x29, 0xF6, 0x20,
  0xC0, 0x88, 0x7E, 0x09, 0x03,
  0x20, 0x54, 0x54, 0x79, 0x41,
  0x00, 0x00, 0x44, 0x7D, 0x41,
  0x30, 0x48, 0x48, 0x4A, 0x32,
  0x38, 0x40, 0x40, 0x22, 0x7A,
  0x00, 0x7A, 0x0A, 0x0A, 0x72,
  0x7D, 0x0D, 0x19, 0x31, 0x7D,
  0x26, 0x29, 0x29, 0x2F, 0x28,
  0x26, 0x29, 0x29, 0x29, 0x26,
  0x30, 0x48, 0x4D, 0x40, 0x20,
  0x38, 0x08, 0x08, 0x08, 0x08,
  0x08, 0x08, 0x08, 0x08, 0x38,
  0x2F, 0x10, 0xC8, 0xAC, 0xBA,
  0x2F, 0x10, 0x28, 0x34, 0xFA,
  0x00, 0x00, 0x7B, 0x00, 0x00,
  0x08, 0x14, 0x2A, 0x14, 0x22,
  0x22, 0x14, 0x2A, 0x14, 0x08,
  0xAA, 0x00, 0x55, 0x00, 0xAA,
  0xAA, 0x55, 0xAA, 0x55, 0xAA,
  0x00, 0x00, 0x00, 0xFF, 0x00,
  0x10, 0x10, 0x10, 0xFF, 0x00,
  0x14, 0x14, 0x14, 0xFF, 0x00,
  0x10, 0x10, 0xFF, 0x00, 0xFF,
  0x10, 0x10, 0xF0, 0x10, 0xF0,
  0x14, 0x14, 0x14, 0xFC, 0x00,
  0x14, 0x14, 0xF7, 0x00, 0xFF,
  0x00, 0x00, 0xFF, 0x00, 0xFF,
  0x14, 0x14, 0xF4, 0x04, 0xFC,
  0x14, 0x14, 0x17, 0x10, 0x1F,
  0x10, 0x10, 0x1F, 0x10, 0x1F,
  0x14, 0x14, 0x14, 0x1F, 0x00,
  0x10, 0x10, 0x10, 0xF0, 0x00,
  0x00, 0x00, 0x00, 0x1F, 0x10,
  0x10, 0x10, 0x10, 0x1F, 0x10,
  0x10, 0x10, 0x10, 0xF0, 0x10,
  0x00, 0x00, 0x00, 0xFF, 0x10,
  0x10, 0x10, 0x10, 0x10, 0x10,
  0x10, 0x10, 0x10, 0xFF, 0x10,
  0x00, 0x00, 0x00, 0xFF, 0x14,
  0x00, 0x00, 0xFF, 0x00, 0xFF,
  0x00, 0x00, 0x1F, 0x10, 0x17,
  0x00, 0x00, 0xFC, 0x04, 0xF4,
  0x14, 0x14, 0x17, 0x10, 0x17,
  0x14, 0x14, 0xF4, 0x04, 0xF4,
  0x00, 0x00, 0xFF, 0x00, 0xF7,
  0x14, 0x14, 0x14, 0x14, 0x14,
  0x14, 0x14, 0xF7, 0x00, 0xF7,
  0x14, 0x14, 0x14, 0x17, 0x14,
  0x10, 0x10, 0x1F, 0x10, 0x1F,
  0x14, 0x14, 0x14, 0xF4, 0x14,
  0x10, 0x10, 0xF0, 0x10, 0xF0,
  0x00, 0x00, 0x1F, 0x10, 0x1F,
  0x00, 0x00, 0x00, 0x1F, 0x14,
  0x00, 0x00, 0x00, 0xFC, 0x14,
  0x00, 0x00, 0xF0, 0x10, 0xF0,
  0x10, 0x10, 0xFF, 0x10, 0xFF,
  0x14, 0x14, 0x14, 0xFF, 0x14,
  0x10, 0x10, 0x10, 0x1F, 0x00,
  0x00, 0x00, 0x00, 0xF0, 0x10,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
  0xFF, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xFF, 0xFF,
  0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
  0x38, 0x44, 0x44, 0x38, 0x44,
  0x7C, 0x2A, 0x2A, 0x3E, 0x14,
  0x7E, 0x02, 0x02, 0x06, 0x06,
  0x02, 0x7E, 0x02, 0x7E, 0x02,
  0x63, 0x55, 0x49, 0x41, 0x63,
  0x38, 0x44, 0x44, 0x3C, 0x04,
  0x40, 0x7E, 0x20, 0x1E, 0x20,
  0x06, 0x02, 0x7E, 0x02, 0x02,
  0x99, 0xA5, 0xE7, 0xA5, 0x99,
  0x1C, 0x2A, 0x49, 0x2A, 0x1C,
  0x4C, 0x72, 0x01, 0x72, 0x4C,
  0x30, 0x4A, 0x4D, 0x4D, 0x30,
  0x30, 0x48, 0x78, 0x48, 0x30,
  0xBC, 0x62, 0x5A, 0x46, 0x3D,
  0x3E, 0x49, 0x49, 0x49, 0x00,
  0x7E, 0x01, 0x01, 0x01, 0x7E,
  0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
  0x44, 0x44, 0x5F, 0x44, 0x44,
  0x40, 0x51, 0x4A, 0x44, 0x40,
  0x40, 0x44, 0x4A, 0x51, 0x40,
  0x00, 0x00, 0xFF, 0x01, 0x03,
  0xE0, 0x80, 0xFF, 0x00, 0x00,
  0x08, 0x08, 0x6B, 0x6B, 0x08,
  0x36, 0x12, 0x36, 0x24, 0x36,
  0x06, 0x0F, 0x09, 0x0F, 0x06,
  0x00, 0x00, 0x18, 0x18, 0x00,
  0x00, 0x00, 0x10, 0x10, 0x00,
  0x30, 0x40, 0xFF, 0x01, 0x01,
  0x00, 0x1F, 0x01, 0x01, 0x1E,
  0x00, 0x19, 0x1D, 0x17, 0x12,
  0x00, 0x3C, 0x3C, 0x3C, 0x3C,
  0x00, 0x00, 0x00, 0x00, 0x00,
};


static uint8_t ColStart, RowStart; // some displays need this changed
//static uint8_t Rotation;           // 0 to 3
//static enum initRFlags TabColor;
static int16_t _width = ST7735_TFTWIDTH;   // this could probably be a constant, except it is used in Adafruit_GFX and depends on image rotation
static int16_t _height = ST7735_TFTHEIGHT;


// The Data/Command pin must be valid when the eighth bit is
// sent.  The eUSCI module has no hardware input or output
// FIFOs, so this implementation is much simpler than it was
// for the Tiva LaunchPads.
// All operations wait until all data has been sent,
// configure the Data/Command pin, queue the message, and
// return the reply once it comes in.

// This is a helper function that sends an 8-bit command to the LCD.
// Inputs: c  8-bit code to transmit
// Outputs: 8-bit reply
// Assumes: UCB0 and ports have already been initialized and enabled
uint8_t static writecommand(uint8_t c) {
  while((UCB0IFG&0x0002)==0x0000){};    // wait until UCB0TXBUF empty
  DC = 0x00;
  TFT_CS = 0x00;
  UCB0TXBUF = c;                        // command out
  while((UCB0IFG&0x0001)==0x0000){};    // wait until UCB0RXBUF full
  TFT_CS = 0x01;
  return UCB0RXBUF;                     // return the response
}


// This is a helper function that sends a piece of 8-bit data to the LCD.
// Inputs: c  8-bit data to transmit
// Outputs: 8-bit reply
// Assumes: UCB0 and ports have already been initialized and enabled
uint8_t static writedata(uint8_t c) {
  while((UCB0IFG&0x0002)==0x0000){};    // wait until UCB0TXBUF empty
  DC = 0x01;
  TFT_CS = 0x00;
  UCB0TXBUF = c;                        // data out
  while((UCB0IFG&0x0001)==0x0000){};    // wait until UCB0RXBUF full
  TFT_CS = 0x01;
  return UCB0RXBUF;                     // return the response
}


// delay function for testing
// which delays about 6*ulCount cycles
// ulCount=8000 => 1ms = 8000*6cycle/loop/48,000
#ifdef __TI_COMPILER_VERSION__
  //Code Composer Studio Code
  void parrotdelay(unsigned long ulCount){
  __asm (  "pdloop:  subs    r0, #1\n"
      "    bne    pdloop\n");
}

#else
  //Keil uVision Code
  __asm void
  parrotdelay(unsigned long ulCount)
  {
    subs    r0, #1
    bne     parrotdelay
    bx      lr
  }

#endif


// Rather than a bazillion writecommand() and writedata() calls, screen
// initialization commands and arguments are organized in these tables
// stored in ROM.  The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code.  Companion function follows.
#define DELAY 0x80
/*static const uint8_t
  Bcmd[] = {                  // Initialization commands for 7735B screens
    18,                       // 18 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, no args, w/delay
      50,                     //     50 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, no args, w/delay
      255,                    //     255 = 500 ms delay
    ST7735_COLMOD , 1+DELAY,  //  3: Set color mode, 1 arg + delay:
      0x05,                   //     16-bit color
      10,                     //     10 ms delay
    ST7735_FRMCTR1, 3+DELAY,  //  4: Frame rate control, 3 args + delay:
      0x00,                   //     fastest refresh
      0x06,                   //     6 lines front porch
      0x03,                   //     3 lines back porch
      10,                     //     10 ms delay
    ST7735_MADCTL , 1      ,  //  5: Memory access ctrl (directions), 1 arg:
      0x08,                   //     Row addr/col addr, bottom to top refresh
    ST7735_DISSET5, 2      ,  //  6: Display settings #5, 2 args, no delay:
      0x15,                   //     1 clk cycle nonoverlap, 2 cycle gate
                              //     rise, 3 cycle osc equalize
      0x02,                   //     Fix on VTL
    ST7735_INVCTR , 1      ,  //  7: Display inversion control, 1 arg:
      0x0,                    //     Line inversion
    ST7735_PWCTR1 , 2+DELAY,  //  8: Power control, 2 args + delay:
      0x02,                   //     GVDD = 4.7V
      0x70,                   //     1.0uA
      10,                     //     10 ms delay
    ST7735_PWCTR2 , 1      ,  //  9: Power control, 1 arg, no delay:
      0x05,                   //     VGH = 14.7V, VGL = -7.35V
    ST7735_PWCTR3 , 2      ,  // 10: Power control, 2 args, no delay:
      0x01,                   //     Opamp current small
      0x02,                   //     Boost frequency
    ST7735_VMCTR1 , 2+DELAY,  // 11: Power control, 2 args + delay:
      0x3C,                   //     VCOMH = 4V
      0x38,                   //     VCOML = -1.1V
      10,                     //     10 ms delay
    ST7735_PWCTR6 , 2      ,  // 12: Power control, 2 args, no delay:
      0x11, 0x15,
    ST7735_GMCTRP1,16      ,  // 13: Magical unicorn dust, 16 args, no delay:
      0x09, 0x16, 0x09, 0x20, //     (seriously though, not sure what
      0x21, 0x1B, 0x13, 0x19, //      these config values represent)
      0x17, 0x15, 0x1E, 0x2B,
      0x04, 0x05, 0x02, 0x0E,
    ST7735_GMCTRN1,16+DELAY,  // 14: Sparkles and rainbows, 16 args + delay:
      0x0B, 0x14, 0x08, 0x1E, //     (ditto)
      0x22, 0x1D, 0x18, 0x1E,
      0x1B, 0x1A, 0x24, 0x2B,
      0x06, 0x06, 0x02, 0x0F,
      10,                     //     10 ms delay
    ST7735_CASET  , 4      ,  // 15: Column addr set, 4 args, no delay:
      0x00, 0x02,             //     XSTART = 2
      0x00, 0x81,             //     XEND = 129
    ST7735_RASET  , 4      ,  // 16: Row addr set, 4 args, no delay:
      0x00, 0x02,             //     XSTART = 1
      0x00, 0x81,             //     XEND = 160
    ST7735_NORON  ,   DELAY,  // 17: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,   DELAY,  // 18: Main screen turn on, no args, w/delay
      255 };                  //     255 = 500 ms delay*/
static const uint8_t
  Rcmd1[] = {                 // Init for 7735R, part 1 (red or green tab)
    15,                       // 15 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, 0 args, w/delay
      150,                    //     150 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, 0 args, w/delay
      255,                    //     500 ms delay
    ST7735_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
      0x01, 0x2C, 0x2D,       //     Dot inversion mode
      0x01, 0x2C, 0x2D,       //     Line inversion mode
    ST7735_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
      0x07,                   //     No inversion
    ST7735_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                   //     -4.6V
      0x84,                   //     AUTO mode
    ST7735_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
      0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
      0x0A,                   //     Opamp current small
      0x00,                   //     Boost frequency
    ST7735_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
      0x8A,                   //     BCLK/2, Opamp current small & Medium low
      0x2A,
    ST7735_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
      0x0E,
    ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
    ST7735_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
      0xC8,                   //     row addr/col addr, bottom to top refresh
    ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
      0x05 };                 //     16-bit color
static const uint8_t
  Rcmd2green[] = {            // Init for 7735R, part 2 (green tab only)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x02,             //     XSTART = 0
      0x00, 0x7F+0x02,        //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x01,             //     XSTART = 0
      0x00, 0x7F+0x01 };      //     XEND = 127
static const uint8_t
  Rcmd2red[] = {              // Init for 7735R, part 2 (red tab only)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F,             //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F };           //     XEND = 127
static const uint8_t
  Rcmd3[] = {                 // Init for 7735R, part 3 (red or green tab)
    4,                        //  4 commands in list:
    ST7735_GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
      0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
      0x03, 0x1d, 0x07, 0x06,
      0x2E, 0x2C, 0x29, 0x2D,
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST7735_NORON  ,    DELAY, //  3: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,    DELAY, //  4: Main screen turn on, no args w/delay
      100 };                  //     100 ms delay


// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in ROM byte array.
void static commandList(const uint8_t *addr) {

  uint8_t numCommands, numArgs;
  uint16_t ms;

  numCommands = *(addr++);               // Number of commands to follow
  while(numCommands--) {                 // For each command...
    writecommand(*(addr++));             //   Read, issue command
    numArgs  = *(addr++);                //   Number of args to follow
    ms       = numArgs & DELAY;          //   If hibit set, delay follows args
    numArgs &= ~DELAY;                   //   Mask out delay bit
    while(numArgs--) {                   //   For each argument...
      writedata(*(addr++));              //     Read, issue argument
    }

    if(ms) {
      ms = *(addr++);             // Read post-command delay time (ms)
      if(ms == 255) ms = 500;     // If 255, delay for 500 ms
      BSP_Delay1ms(ms);
    }
  }
}


// Initialization code common to both 'B' and 'R' type displays
void static commonInit(const uint8_t *cmdList) {
  ColStart  = RowStart = 0; // May be overridden in init func

  // toggle RST low to reset; CS low so it'll listen to us
  // UCB0STE is not available, so use GPIO on P5.0
  P3SEL0 &= ~0x80;
  P3SEL1 &= ~0x80;                      // configure J4.31/P3.7 (D/C) as GPIO
  P3DIR |= 0x80;                        // make J4.31/P3.7 (D/C) out
  P5SEL0 &= ~0x81;
  P5SEL1 &= ~0x81;                      // configure J2.17/P5.7 (Reset) and J2.13/P5.0 (TFT_CS) as GPIO
  P5DIR |= 0x81;                        // make J2.17/P5.7 (Reset) and J2.13/P5.0 (TFT_CS) out
  TFT_CS = 0x00;
  RESET = 0x01;
  BSP_Delay1ms(500);
  RESET = 0x00;
  BSP_Delay1ms(500);
  RESET = 0x01;
  BSP_Delay1ms(500);
  TFT_CS = 0x01;

  // initialize eUSCI
  UCB0CTLW0 = 0x0001;                   // hold the eUSCI module in reset mode
  // configure UCB0CTLW0 for:
  // bit15      UCCKPH = 1; data shifts in on first edge, out on following edge
  // bit14      UCCKPL = 0; clock is low when inactive
  // bit13      UCMSB = 1; MSB first
  // bit12      UC7BIT = 0; 8-bit data
  // bit11      UCMST = 1; master mode
  // bits10-9   UCMODEx = 2; UCSTE active low
  // bit8       UCSYNC = 1; synchronous mode
  // bits7-6    UCSSELx = 2; eUSCI clock SMCLK
  // bits5-2    reserved
  // bit1       UCSTEM = 1; UCSTE pin enables slave
  // bit0       UCSWRST = 1; reset enabled
  UCB0CTLW0 = 0xAD83;
  // set the baud rate for the eUSCI which gets its clock from SMCLK
  // Clock_Init48MHz() from ClockSystem.c sets SMCLK = HFXTCLK/4 = 12 MHz
  // if the SMCLK is set to 12 MHz, divide by 3 for 4 MHz baud clock
  UCB0BRW = 3;
  // modulation is not used in SPI mode, so clear UCB0MCTLW
//  UCB0MCTLW = 0;                        // not actually a register in eUSCIB
  P1SEL0 |= 0x60;
  P1SEL1 &= ~0x60;                      // configure P1.6 and P1.5 as primary module function
  UCB0CTLW0 &= ~0x0001;                 // enable eUSCI module
  UCB0IE &= ~0x0003;                    // disable interrupts

  if(cmdList) commandList(cmdList);
}


/*//------------ST7735_InitB------------
// Initialization for ST7735B screens.
// Input: none
// Output: none
void static ST7735_InitB(void) {
  commonInit(Bcmd);
  BSP_LCD_SetCursor(0,0);
  StTextColor = ST7735_YELLOW;
  BSP_LCD_FillScreen(0);                // set screen to black
}*/


//------------ST7735_InitR------------
// Initialization for ST7735R screens (green or red tabs).
// Input: option one of the enumerated options depending on tabs
// Output: none
void static ST7735_InitR(enum initRFlags option) {
  commonInit(Rcmd1);
  if(option == INITR_GREENTAB) {
    commandList(Rcmd2green);
    ColStart = 2;
    RowStart = 3;
  } else {
    // colstart, rowstart left at default '0' values
    commandList(Rcmd2red);
  }
  commandList(Rcmd3);

  // if black, change MADCTL color filter
  if (option == INITR_BLACKTAB) {
    writecommand(ST7735_MADCTL);
    writedata(0xC0);
  }
//  TabColor = option;
  BSP_LCD_SetCursor(0,0);
  StTextColor = ST7735_YELLOW;
  BSP_LCD_FillScreen(0);                // set screen to black
}


// ------------BSP_LCD_Init------------
// Initialize the SPI and GPIO, which correspond with
// BoosterPack pins J1.7 (SPI CLK), J2.13 (SPI CS), J2.15
// (SPI MOSI), J2.17 (LCD ~RST), and J4.31 (LCD DC).
// Input: none
// Output: none
void BSP_LCD_Init(void){
  ST7735_InitR(INITR_GREENTAB);
}


// Set the region of the screen RAM to be modified
// Pixel colors are sent left to right, top to bottom
// (same as Font table is encoded; different from regular bitmap)
// Requires 11 bytes of transmission
void static setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {

  writecommand(ST7735_CASET); // Column addr set
  writedata(0x00);
  writedata(x0+ColStart);     // XSTART
  writedata(0x00);
  writedata(x1+ColStart);     // XEND

  writecommand(ST7735_RASET); // Row addr set
  writedata(0x00);
  writedata(y0+RowStart);     // YSTART
  writedata(0x00);
  writedata(y1+RowStart);     // YEND

  writecommand(ST7735_RAMWR); // write to RAM
}


// Send two bytes of data, most significant byte first
// Requires 2 bytes of transmission
void static pushColor(uint16_t color) {
  writedata((uint8_t)(color >> 8));
  writedata((uint8_t)color);
}


//------------BSP_LCD_DrawPixel------------
// Color the pixel at the given coordinates with the given color.
// Requires 13 bytes of transmission
// Input: x     horizontal position of the pixel, columns from the left edge
//               must be less than 128
//               0 is on the left, 126 is near the right
//        y     vertical position of the pixel, rows from the top edge
//               must be less than 128
//               126 is near the wires, 0 is the side opposite the wires
//        color 16-bit color, which can be produced by BSP_LCD_Color565()
// Output: none
void BSP_LCD_DrawPixel(int16_t x, int16_t y, uint16_t color) {

  if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;

//  setAddrWindow(x,y,x+1,y+1); // original code, bug???
  setAddrWindow(x,y,x,y);

  pushColor(color);
}


//------------BSP_LCD_DrawFastVLine------------
// Draw a vertical line at the given coordinates with the given height and color.
// A vertical line is parallel to the longer side of the rectangular display
// Requires (11 + 2*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the start of the line, columns from the left edge
//        y     vertical position of the start of the line, rows from the top edge
//        h     vertical height of the line
//        color 16-bit color, which can be produced by BSP_LCD_Color565()
// Output: none
void BSP_LCD_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  uint8_t hi = color >> 8, lo = color;

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((y+h-1) >= _height) h = _height-y;
  setAddrWindow(x, y, x, y+h-1);

  while (h--) {
    writedata(hi);
    writedata(lo);
  }
}


//------------BSP_LCD_DrawFastHLine------------
// Draw a horizontal line at the given coordinates with the given width and color.
// A horizontal line is parallel to the shorter side of the rectangular display
// Requires (11 + 2*w) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the start of the line, columns from the left edge
//        y     vertical position of the start of the line, rows from the top edge
//        w     horizontal width of the line
//        color 16-bit color, which can be produced by BSP_LCD_Color565()
// Output: none
void BSP_LCD_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  uint8_t hi = color >> 8, lo = color;

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;
  setAddrWindow(x, y, x+w-1, y);

  while (w--) {
    writedata(hi);
    writedata(lo);
  }
}


//------------BSP_LCD_FillScreen------------
// Fill the screen with the given color.
// Requires 33,293 bytes of transmission
// Input: color 16-bit color, which can be produced by BSP_LCD_Color565()
// Output: none
void BSP_LCD_FillScreen(uint16_t color) {
  BSP_LCD_FillRect(0, 0, _width, _height, color);  // original
//  screen is actually 129 by 129 pixels, x 0 to 128, y goes from 0 to 128
}


//------------BSP_LCD_FillRect------------
// Draw a filled rectangle at the given coordinates with the given width, height, and color.
// Requires (11 + 2*w*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the top left corner of the rectangle, columns from the left edge
//        y     vertical position of the top left corner of the rectangle, rows from the top edge
//        w     horizontal width of the rectangle
//        h     vertical height of the rectangle
//        color 16-bit color, which can be produced by BSP_LCD_Color565()
// Output: none
void BSP_LCD_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  uint8_t hi = color >> 8, lo = color;

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;

  setAddrWindow(x, y, x+w-1, y+h-1);

  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
      writedata(hi);
      writedata(lo);
    }
  }
}


//------------BSP_LCD_Color565------------
// Pass 8-bit (each) R,G,B and get back 16-bit packed color.
// Input: r red value
//        g green value
//        b blue value
// Output: 16-bit color
uint16_t BSP_LCD_Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}


//------------BSP_LCD_SwapColor------------
// Swaps the red and blue values of the given 16-bit packed color;
// green is unchanged.
// Input: x 16-bit color in format B, G, R
// Output: 16-bit color in format R, G, B
uint16_t BSP_LCD_SwapColor(uint16_t x) {
  return (x << 11) | (x & 0x07E0) | (x >> 11);
}


//------------BSP_LCD_DrawBitmap------------
// Displays a 16-bit color BMP image.  A bitmap file that is created
// by a PC image processing program has a header and may be padded
// with dummy columns so the data have four byte alignment.  This
// function assumes that all of that has been stripped out, and the
// array image[] has one 16-bit halfword for each pixel to be
// displayed on the screen (encoded in reverse order, which is
// standard for bitmap files).  An array can be created in this
// format from a 24-bit-per-pixel .bmp file using the associated
// converter program.
// (x,y) is the screen location of the lower left corner of BMP image
// Requires (11 + 2*w*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the bottom left corner of the image, columns from the left edge
//        y     vertical position of the bottom left corner of the image, rows from the top edge
//        image pointer to a 16-bit color BMP image
//        w     number of pixels wide
//        h     number of pixels tall
// Output: none
// Must be less than or equal to 128 pixels wide by 128 pixels high
void BSP_LCD_DrawBitmap(int16_t x, int16_t y, const uint16_t *image, int16_t w, int16_t h){
  int16_t skipC = 0;                      // non-zero if columns need to be skipped due to clipping
  int16_t originalWidth = w;              // save this value; even if not all columns fit on the screen, the image is still this width in ROM
  int i = w*(h - 1);

  if((x >= _width) || ((y - h + 1) >= _height) || ((x + w) <= 0) || (y < 0)){
    return;                             // image is totally off the screen, do nothing
  }
  if((w > _width) || (h > _height)){    // image is too wide for the screen, do nothing
    //***This isn't necessarily a fatal error, but it makes the
    //following logic much more complicated, since you can have
    //an image that exceeds multiple boundaries and needs to be
    //clipped on more than one side.
    return;
  }
  if((x + w - 1) >= _width){            // image exceeds right of screen
    skipC = (x + w) - _width;           // skip cut off columns
    w = _width - x;
  }
  if((y - h + 1) < 0){                  // image exceeds top of screen
    i = i - (h - y - 1)*originalWidth;  // skip the last cut off rows
    h = y + 1;
  }
  if(x < 0){                            // image exceeds left of screen
    w = w + x;
    skipC = -1*x;                       // skip cut off columns
    i = i - x;                          // skip the first cut off columns
    x = 0;
  }
  if(y >= _height){                     // image exceeds bottom of screen
    h = h - (y - _height + 1);
    y = _height - 1;
  }

  setAddrWindow(x, y-h+1, x+w-1, y);

  for(y=0; y<h; y=y+1){
    for(x=0; x<w; x=x+1){
                                        // send the top 8 bits
      writedata((uint8_t)(image[i] >> 8));
                                        // send the bottom 8 bits
      writedata((uint8_t)image[i]);
      i = i + 1;                        // go to the next pixel
    }
    i = i + skipC;
    i = i - 2*originalWidth;
  }
}


//------------BSP_LCD_DrawCharS------------
// Simple character draw function.  This is the same function from
// Adafruit_GFX.c but adapted for this processor.  However, each call
// to BSP_LCD_DrawPixel() calls setAddrWindow(), which needs to send
// many extra data and commands.  If the background color is the same
// as the text color, no background will be printed, and text can be
// drawn right over existing images without covering them with a box.
// Requires (11 + 2*size*size)*6*8 bytes of transmission (image fully on screen; textcolor != bgColor)
// Input: x         horizontal position of the top left corner of the character, columns from the left edge
//        y         vertical position of the top left corner of the character, rows from the top edge
//        c         character to be printed
//        textColor 16-bit color of the character
//        bgColor   16-bit color of the background
//        size      number of pixels per character pixel (e.g. size==2 prints each pixel of font as 2x2 square)
// Output: none
void BSP_LCD_DrawCharS(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size){
  uint8_t line; // vertical column of pixels of character in font
  int32_t i, j;
  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + 6 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

  for (i=0; i<6; i++ ) {
    if (i == 5)
      line = 0x0;
    else
      line = Font[(c*5)+i];
    for (j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          BSP_LCD_DrawPixel(x+i, y+j, textColor);
        else {  // big size
          BSP_LCD_FillRect(x+(i*size), y+(j*size), size, size, textColor);
        }
      } else if (bgColor != textColor) {
        if (size == 1) // default size
          BSP_LCD_DrawPixel(x+i, y+j, bgColor);
        else {  // big size
          BSP_LCD_FillRect(x+i*size, y+j*size, size, size, bgColor);
        }
      }
      line >>= 1;
    }
  }
}


//------------BSP_LCD_DrawChar------------
// Advanced character draw function.  This is similar to the function
// from Adafruit_GFX.c but adapted for this processor.  However, this
// function only uses one call to setAddrWindow(), which allows it to
// run at least twice as fast.
// Requires (11 + size*size*6*8) bytes of transmission (assuming image fully on screen)
// Input: x         horizontal position of the top left corner of the character, columns from the left edge
//        y         vertical position of the top left corner of the character, rows from the top edge
//        c         character to be printed
//        textColor 16-bit color of the character
//        bgColor   16-bit color of the background
//        size      number of pixels per character pixel (e.g. size==2 prints each pixel of font as 2x2 square)
// Output: none
void BSP_LCD_DrawChar(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size){
  uint8_t line; // horizontal row of pixels of character
  int32_t col, row, i, j;// loop indices
  if(((x + 6*size - 1) >= _width)  || // Clip right
     ((y + 8*size - 1) >= _height) || // Clip bottom
     ((x + 6*size - 1) < 0)        || // Clip left
     ((y + 8*size - 1) < 0)){         // Clip top
    return;
  }

  setAddrWindow(x, y, x+6*size-1, y+8*size-1);

  line = 0x01;        // print the top row first
  // print the rows, starting at the top
  for(row=0; row<8; row=row+1){
    for(i=0; i<size; i=i+1){
      // print the columns, starting on the left
      for(col=0; col<5; col=col+1){
        if(Font[(c*5)+col]&line){
          // bit is set in Font, print pixel(s) in text color
          for(j=0; j<size; j=j+1){
            pushColor(textColor);
          }
        } else{
          // bit is cleared in Font, print pixel(s) in background color
          for(j=0; j<size; j=j+1){
            pushColor(bgColor);
          }
        }
      }
      // print blank column(s) to the right of character
      for(j=0; j<size; j=j+1){
        pushColor(bgColor);
      }
    }
    line = line<<1;   // move up to the next row
  }
}


//------------BSP_LCD_DrawString------------
// String draw function.
// 13 rows (0 to 12) and 21 characters (0 to 20)
// Requires (11 + size*size*6*8) bytes of transmission for each character
// Input: x         columns from the left edge (0 to 20)
//        y         rows from the top edge (0 to 12)
//        pt        pointer to a null terminated string to be printed
//        textColor 16-bit color of the characters
// bgColor is Black and size is 1
// Output: number of characters printed
uint32_t BSP_LCD_DrawString(uint16_t x, uint16_t y, char *pt, int16_t textColor){
  uint32_t count = 0;
  if(y>12) return 0;
  while(*pt){
    BSP_LCD_DrawChar(x*6, y*10, *pt, textColor, ST7735_BLACK, 1);
    pt++;
    x = x+1;
    if(x>20) return count;  // number of characters printed
    count++;
  }
  return count;  // number of characters printed
}


//-----------------------fillmessage-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
char Message[12];
uint32_t Messageindex;

void static fillmessage(uint32_t n){
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
  if(n >= 10){
    fillmessage(n/10);
    n = n%10;
  }
  Message[Messageindex] = (n+'0'); /* n is between 0 and 9 */
  if(Messageindex<11)Messageindex++;
}
void static fillmessage4(uint32_t n){
  if(n>9999)n=9999;
  if(n>=1000){  // 1000 to 9999
    Messageindex = 0;
  } else if(n>=100){  // 100 to 999
    Message[0] = ' ';
    Messageindex = 1;
  }else if(n>=10){ //
    Message[0] = ' '; /* n is between 10 and 99 */
    Message[1] = ' ';
    Messageindex = 2;
  }else{
    Message[0] = ' '; /* n is between 0 and 9 */
    Message[1] = ' ';
    Message[2] = ' ';
    Messageindex = 3;
  }
  fillmessage(n);
}
void static fillmessage5(uint32_t n){
  if(n>99999)n=99999;
  if(n>=10000){  // 10000 to 99999
    Messageindex = 0;
  } else if(n>=1000){  // 1000 to 9999
    Message[0] = ' ';
    Messageindex = 1;
  }else if(n>=100){  // 100 to 999
    Message[0] = ' ';
    Message[1] = ' ';
    Messageindex = 2;
  }else if(n>=10){ //
    Message[0] = ' '; /* n is between 10 and 99 */
    Message[1] = ' ';
    Message[2] = ' ';
    Messageindex = 3;
  }else{
    Message[0] = ' '; /* n is between 0 and 9 */
    Message[1] = ' ';
    Message[2] = ' ';
    Message[3] = ' ';
    Messageindex = 4;
  }
  fillmessage(n);
}
void static fillmessage2_1(uint32_t n){
  if(n>999)n=999;
  if(n>=100){  // 100 to 999
    Message[0] = (n/100+'0'); /* tens digit */
    n = n%100; //the rest
  }else { // 0 to 99
    Message[0] = ' '; /* n is between 0.0 and 9.9 */
  }
  Message[1] = (n/10+'0'); /* ones digit */
  n = n%10; //the rest
  Message[2] = '.';
  Message[3] = (n+'0'); /* tenths digit */
  Message[4] = 0;
}

void static fillmessage2_Hex(uint32_t n){ char digit;
  if(n>255){
    Message[0] = '*';
    Message[1] = '*';
  }else{
    digit = n/16;
    if(digit<10){
      digit = digit+'0';
    }else{
      digit = digit+'A'-10;
    }
    Message[0] = digit; /* 16's digit */
    digit = n%16;
    if(digit<10){
      digit = digit+'0';
    }else{
      digit = digit+'A'-10;
    }
    Message[1] = digit; /* ones digit */
  }
  Message[2] = ',';
  Message[3] = 0;
}

//********BSP_LCD_SetCursor*****************
// Move the cursor to the desired X- and Y-position.  The
// next character of the next unsigned decimal will be
// printed here.  X=0 is the leftmost column.  Y=0 is the top
// row.
// inputs: newX  new X-position of the cursor (0<=newX<=20)
//         newY  new Y-position of the cursor (0<=newY<=12)
// outputs: none
void BSP_LCD_SetCursor(uint32_t newX, uint32_t newY){
  if((newX > 20) || (newY > 12)){       // bad input
    return;                             // do nothing
  }
  StX = newX;
  StY = newY;
}


//-----------------------BSP_LCD_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Position determined by BSP_LCD_SetCursor command
// Input: n         32-bit number to be transferred
//        textColor 16-bit color of the numbers
// Output: none
// Variable format 1-10 digits with no space before or after
void BSP_LCD_OutUDec(uint32_t n, int16_t textColor){
  StTextColor = textColor;
  Messageindex = 0;
  fillmessage(n);
  Message[Messageindex] = 0; // terminate
  BSP_LCD_DrawString(StX,StY,Message,textColor);
  StX = StX+Messageindex;
  if(StX>20){
    StX = 20;
    BSP_LCD_DrawChar(StX*6,StY*10,'*',ST7735_RED,ST7735_BLACK, 1);
  }
}


//-----------------------BSP_LCD_OutUDec4-----------------------
// Output a 32-bit number in unsigned 4-digit decimal format
// Position determined by BSP_LCD_SetCursor command
// Input: 32-bit number to be transferred
//        textColor 16-bit color of the numbers
// Output: none
// Fixed format 4 digits with no space before or after
void BSP_LCD_OutUDec4(uint32_t n, int16_t textColor){
  Messageindex = 0;
  fillmessage4(n);
  Message[Messageindex] = 0; // terminate
  BSP_LCD_DrawString(StX,StY,Message,textColor);
  StX = StX+Messageindex;
  if(StX>20){
    StX = 20;
    BSP_LCD_DrawChar(StX*6,StY*10,'*',ST7735_RED,ST7735_BLACK, 1);
  }
}


//-----------------------BSP_LCD_OutUDec5-----------------------
// Output a 32-bit number in unsigned 5-digit decimal format
// Position determined by BSP_LCD_SetCursor command
// Input: 32-bit number to be transferred
//        textColor 16-bit color of the numbers
// Output: none
// Fixed format 5 digits with no space before or after
void BSP_LCD_OutUDec5(uint32_t n, int16_t textColor){
  Messageindex = 0;
  fillmessage5(n);
  Message[Messageindex] = 0; // terminate
  BSP_LCD_DrawString(StX,StY,Message,textColor);
  StX = StX+Messageindex;
  if(StX>20){
    StX = 20;
    BSP_LCD_DrawChar(StX*6,StY*10,'*',ST7735_RED,ST7735_BLACK, 1);
  }
}


//-----------------------BSP_LCD_OutUFix2_1-----------------------
// Output a 32-bit number in unsigned 3-digit fixed point, 0.1 resolution
// numbers 0 to 999 printed as " 0.0" to "99.9"
// Position determined by BSP_LCD_SetCursor command
// Input: 32-bit number to be transferred
//        textColor 16-bit color of the numbers
// Output: none
// Fixed format 4 characters with no space before or after
void BSP_LCD_OutUFix2_1(uint32_t n, int16_t textColor){
  fillmessage2_1(n);
  BSP_LCD_DrawString(StX,StY,Message,textColor);
  StX = StX+4;
  if(StX>20){
    StX = 20;
    BSP_LCD_DrawChar(StX*6,StY*10,'*',ST7735_RED,ST7735_BLACK, 1);
  }
}

//-----------------------BSP_LCD_OutUHex2-----------------------
// Output a 32-bit number in unsigned 2-digit hexadecimal format
// numbers 0 to 255 printed as "00," to "FF,"
// Position determined by BSP_LCD_SetCursor command
// Input: 32-bit number to be transferred
//        textColor 16-bit color of the numbers
// Output: none
// Fixed format 3 characters with comma after
void BSP_LCD_OutUHex2(uint32_t n, int16_t textColor){
  fillmessage2_Hex(n);
  BSP_LCD_DrawString(StX,StY,Message,textColor);
  StX = StX+3;
  if(StX>20){
    StX = 20;
    BSP_LCD_DrawChar(StX*6,StY*10,'*',ST7735_RED,ST7735_BLACK, 1);
  }
}


int TimeIndex;               // horizontal position of next point to plot on graph (0 to 99)
int32_t Ymax, Ymin, Yrange;  // vertical axis max, min, and range (units not specified)
uint16_t PlotBGColor;        // background color of the plot used whenever clearing plot area

// ------------BSP_LCD_Drawaxes------------
// Set up the axes, labels, and other variables to
// allow data to be plotted in a chart using the
// functions BSP_LCD_PlotPoint() and
// BSP_LCD_PlotIncrement().
// Input: axisColor   16-bit color for axes, which can be produced by BSP_LCD_Color565()
//        bgColor     16-bit color for plot background, which can be produced by BSP_LCD_Color565()
//        xLabel      pointer to a null terminated string for x-axis (~4 character space)
//        yLabel1     pointer to a null terminated string for top of y-axis (~3-5 character space)
//        label1Color 16-bit color for y-axis label1, which can be produced by BSP_LCD_Color565()
//        yLabel2     pointer to a null terminated string for bottom of y-axis (~3 character space)
//                      if yLabel2 is empty string, no yLabel2 is printed, and yLabel1 is centered
//        label2Color 16-bit color for y-axis label2, which can be produced by BSP_LCD_Color565()
//        ymax        maximum value to be printed
//        ymin        minimum value to be printed
// Output: none
// Assumes: BSP_LCD_Init() has been called
void BSP_LCD_Drawaxes(uint16_t axisColor, uint16_t bgColor, char *xLabel,
  char *yLabel1, uint16_t label1Color, char *yLabel2, uint16_t label2Color,
  int32_t ymax, int32_t ymin){
  int i;
  // assume that ymax > ymin
  Ymax = ymax;
  Ymin = ymin;
  Yrange = Ymax - Ymin;
  TimeIndex = 0;
  PlotBGColor = bgColor;
  BSP_LCD_FillRect(0, 17, 111, 111, bgColor);
  BSP_LCD_DrawFastHLine(10, 117, 101, axisColor);
  BSP_LCD_DrawFastVLine(10, 17, 101, axisColor);
  for(i=20; i<=110; i=i+10){
    BSP_LCD_DrawPixel(i, 118, axisColor);
  }
  for(i=17; i<117; i=i+10){
    BSP_LCD_DrawPixel(9, i, axisColor);
  }
  i = 50;
  while((*xLabel) && (i < 100)){
    BSP_LCD_DrawChar(i, 120, *xLabel, axisColor, bgColor, 1);
    i = i + 6;
    xLabel++;
  }
  if(*yLabel2){ // two labels
    i = 26;
    while((*yLabel2) && (i < 50)){
      BSP_LCD_DrawChar(0, i, *yLabel2, label2Color, bgColor, 1);
      i = i + 8;
      yLabel2++;
    }
    i = 82;
  }else{ // one label
    i = 42;
  }
  while((*yLabel1) && (i < 120)){
    BSP_LCD_DrawChar(0, i, *yLabel1, label1Color, bgColor, 1);
    i = i + 8;
    yLabel1++;
  }
}


// ------------BSP_LCD_PlotPoint------------
// Plot a point on the chart.  To plot several points in the
// same column, call this function repeatedly before calling
// BSP_LCD_PlotIncrement().  The units of the data are the
// same as the ymax and ymin values specified in the
// initialization function.
// Input: data1  value to be plotted (units not specified)
//        color1 16-bit color for the point, which can be produced by BSP_LCD_Color565()
// Output: none
// Assumes: BSP_LCD_Init() and BSP_LCD_Drawaxes() have been called
void BSP_LCD_PlotPoint(int32_t data1, uint16_t color1){
  data1 = ((data1 - Ymin)*100)/Yrange;
  if(data1 > 98){
    data1 = 98;
    color1 = LCD_RED;
  }
  if(data1 < 0){
    data1 = 0;
    color1 = LCD_RED;
  }
  BSP_LCD_DrawPixel(TimeIndex + 11, 116 - data1, color1);
  BSP_LCD_DrawPixel(TimeIndex + 11, 115 - data1, color1);
}


// ------------BSP_LCD_PlotIncrement------------
// Increment the plot between subsequent calls to
// BSP_LCD_PlotPoint().  Automatically wrap and clear the
// column to be printed to.
// Input: none
// Output: none
// Assumes: BSP_LCD_Init() and BSP_LCD_Drawaxes() have been called
void BSP_LCD_PlotIncrement(void){
  TimeIndex = TimeIndex + 1;
  if(TimeIndex > 99){
    TimeIndex = 0;
  }
  BSP_LCD_DrawFastVLine(TimeIndex + 11, 17, 100, PlotBGColor);
}
/* ********************** */
/*   End of LCD Section   */
/* ********************** */

// ------------BSP_Clock_InitFastest------------
// Configure the system clock to run at the fastest
// and most accurate settings.  For example, if the
// LaunchPad has a crystal, it should be used here.
// Call BSP_Clock_GetFreq() to get the current system
// clock frequency for the LaunchPad.
// Input: none
// Output: none
uint32_t Prewait = 0;                   // loops between BSP_Clock_InitFastest() called and PCM idle (expect 0)
uint32_t CPMwait = 0;                   // loops between Power Active Mode Request and Current Power Mode matching requested mode (expect small)
uint32_t Postwait = 0;                  // loops between Current Power Mode matching requested mode and PCM module idle (expect about 0)
uint32_t IFlags = 0;                    // non-zero if transition is invalid
uint32_t Crystalstable = 0;             // loops before the crystal stabilizes (expect small)
void BSP_Clock_InitFastest(void){
  // wait for the PCMCTL0 and Clock System to be write-able by waiting for Power Control Manager to be idle
  while(PCMCTL1&0x00000100){
    Prewait = Prewait + 1;
    if(Prewait >= 100000){
      return;                           // time out error
    }
  }
  // request power active mode LDO VCORE1 to support the 48 MHz frequency
  PCMCTL0 = (PCMCTL0&~0xFFFF000F) |     // clear PCMKEY bit field and AMR bit field
            0x695A0000 |                // write the proper PCM key to unlock write access
            0x00000001;                 // request power active mode LDO VCORE1
  // check if the transition is invalid (see Figure 7-3 on p344 of datasheet)
  if(PCMIFG&0x00000004){
    IFlags = PCMIFG;                    // bit 2 set on active mode transition invalid; bits 1-0 are for LPM-related errors; bit 6 is for DC-DC-related error
    PCMCLRIFG = 0x00000004;             // clear the transition invalid flag
    // to do: look at CPM bit field in PCMCTL0, figure out what mode you're in, and step through the chart to transition to the mode you want
    // or be lazy and do nothing; this should work out of reset at least, but it WILL NOT work if Clock_Int32kHz() or Clock_InitLowPower() has been called
    return;
  }
  // wait for the CPM (Current Power Mode) bit field to reflect a change to active mode LDO VCORE1
  while((PCMCTL0&0x00003F00) != 0x00000100){
    CPMwait = CPMwait + 1;
    if(CPMwait >= 500000){
      return;                           // time out error
    }
  }
  // wait for the PCMCTL0 and Clock System to be write-able by waiting for Power Control Manager to be idle
  while(PCMCTL1&0x00000100){
    Postwait = Postwait + 1;
    if(Postwait >= 100000){
      return;                           // time out error
    }
  }
  // initialize PJ.3 and PJ.2 and make them HFXT (PJ.3 built-in 48 MHz crystal out; PJ.2 built-in 48 MHz crystal in)
  PJSEL0 |= 0x0C;
  PJSEL1 &= ~0x0C;                      // configure built-in 48 MHz crystal for HFXT operation
//  PJDIR |= 0x08;                        // make PJ.3 HFXTOUT (unnecessary)
//  PJDIR &= ~0x04;                       // make PJ.2 HFXTIN (unnecessary)
  CSKEY = 0x695A;                       // unlock CS module for register access
  CSCTL2 = (CSCTL2&~0x00700000) |       // clear HFXTFREQ bit field
           0x00600000 |                 // configure for 48 MHz external crystal
           0x00010000 |                 // HFXT oscillator drive selection for crystals >4 MHz
           0x01000000;                  // enable HFXT
  CSCTL2 &= ~0x02000000;                // disable high-frequency crystal bypass
  // wait for the HFXT clock to stabilize
  while(CSIFG&0x00000002){
    CSCLRIFG = 0x00000002;              // clear the HFXT oscillator interrupt flag
    Crystalstable = Crystalstable + 1;
    if(Crystalstable > 100000){
      return;                           // time out error
    }
  }
  // configure for 2 wait states (minimum for 48 MHz operation) for flash Bank 0
  FLCTL_BANK0_RDCTL = (FLCTL_BANK0_RDCTL&~0x0000F000)|FLCTL_BANK0_RDCTL_WAIT_2;
  // configure for 2 wait states (minimum for 48 MHz operation) for flash Bank 1
  FLCTL_BANK1_RDCTL = (FLCTL_BANK1_RDCTL&~0x0000F000)|FLCTL_BANK1_RDCTL_WAIT_2;
  CSCTL1 = 0x20000000 |                 // configure for SMCLK divider /4
           0x00100000 |                 // configure for HSMCLK divider /2
           0x00000200 |                 // configure for ACLK sourced from REFOCLK
           0x00000050 |                 // configure for SMCLK and HSMCLK sourced from HFXTCLK
           0x00000005;                  // configure for MCLK sourced from HFXTCLK
  CSKEY = 0;                            // lock CS module from unintended access
  ClockFrequency = 48000000;
  SubsystemFrequency = 12000000;
}

// ------------BSP_Clock_GetFreq------------
// Return the current system clock frequency for the
// LaunchPad.
// Input: none
// Output: system clock frequency in cycles/second
uint32_t BSP_Clock_GetFreq(void){
  return ClockFrequency;
}

// ------------BSP_PeriodicTask_Init------------
// Activate an interrupt to run a user task periodically.
// Give it a priority 0 to 6 with lower numbers
// signifying higher priority.  Equal priority is
// handled sequentially.
// Input:  task is a pointer to a user function
//         freq is number of interrupts per second
//           1 Hz to 10 kHz
//         priority is a number 0 to 6
// Output: none
void (*PeriodicTask)(void);    // user function
void BSP_PeriodicTask_Init(void(*task)(void), uint32_t freq, uint8_t priority){long sr;
  if((freq == 0) || (freq > 10000)){
    return;                        // invalid input
  }
  if(priority > 6){
    priority = 6;
  }
  sr = StartCritical();
  PeriodicTask = task;             // user function
                                   // timer reload value
  TIMER32_LOAD1 = (BSP_Clock_GetFreq()/freq - 1);
  TIMER32_INTCLR1 = 0x00000001;    // clear Timer32 Timer 1 interrupt
  // bits31-8=X...X,   reserved
  // bit7=1,           timer enable
  // bit6=1,           timer in periodic mode
  // bit5=1,           interrupt enable
  // bit4=X,           reserved
  // bits3-2=00,       input clock divider /1
  // bit1=1,           32-bit counter
  // bit0=0,           wrapping mode
  TIMER32_CONTROL1 = 0x000000E2;
// interrupts enabled in the main program after all devices initialized
  NVIC_IPR6 = (NVIC_IPR6&0xFFFF00FF)|(priority<<13);
  NVIC_ISER0 = 0x02000000;         // enable interrupt 25 in NVIC
  EndCritical(sr);
}

void T32_INT1_IRQHandler(void){
  TIMER32_INTCLR1 = 0x00000001;    // acknowledge Timer32 Timer 1 interrupt
  (*PeriodicTask)();               // execute user task
}

// ------------BSP_PeriodicTask_Stop------------
// Deactivate the interrupt running a user task
// periodically.
// Input: none
// Output: none
void BSP_PeriodicTask_Stop(void){
  TIMER32_INTCLR1 = 0x00000001;    // clear Timer32 Timer 1 interrupt
  NVIC_ICER0 = 0x02000000;         // disable interrupt 25 in NVIC
}

// ***************** BSP_PeriodicTask_InitB ****************
// Activate 16-bit Timer A1 interrupts to run user task periodically
// assumes SMCLK is 12MHz, using divide by 24, 500kHz/65536=7.6 Hz
// Input:  task is a pointer to a user function
//         freq is number of interrupts per second
//           8 Hz to 10 kHz
//         priority is a number 0 to 6
// Outputs: none
// comment: it is accurate if 500000/freq is an integer
void (*PeriodicTaskB)(void);   // user function
void BSP_PeriodicTask_InitB(void(*task)(void), uint32_t freq, uint8_t priority){long sr;
  if((freq < 8) || (freq > 10000)){
    return;                        // invalid input
  }
  if(priority > 6){
    priority = 6;
  }
  sr = StartCritical();
  PeriodicTaskB = task;  // user function
  TA1CTL &= ~0x0030;     // halt Timer A1
  // bits15-10=XXXXXX, reserved
  // bits9-8=10,       clock source to SMCLK
  // bits7-6=10,       input clock divider /4
  // bits5-4=00,       stop mode
  // bit3=X,           reserved
  // bit2=0,           set this bit to clear
  // bit1=0,           no interrupt on timer
  // bit0=0,           clear interrupt pending
  TA1CTL = 0x0280;
  TA1EX0 = 0x0005;    // configure for input clock divider /6
  // bits15-14=00,     no capture mode
  // bits13-12=XX,     capture/compare input select
  // bit11=X,          synchronize capture source
  // bit10=X,          synchronized capture/compare input
  // bit9=X,           reserved
  // bit8=0,           compare mode
  // bits7-5=XXX,      output mode
  // bit4=1,           enable capture/compare interrupt on CCIFG
  // bit3=X,           read capture/compare input from here
  // bit2=0,           output this value in output mode 0
  // bit1=X,           capture overflow status
  // bit0=0,           clear capture/compare interrupt pending
  TA1CCTL0 = 0x0010;
  TA1CCR0 = (BSP_Clock_GetFreq()/96/freq) - 1;  // compare match value
// interrupts enabled in the main program after all devices initialized
  NVIC_IPR2 = (NVIC_IPR2&0xFF00FFFF)|(priority<<21);

  NVIC_ISER0 = 0x00000400; // enable interrupt 10 in NVIC
  TA1CTL |= 0x0014;        // reset and start Timer A1 in up mode
  EndCritical(sr);
}

// ------------BSP_PeriodicTask_StopB------------
// Deactivate the interrupt running a user task periodically.
// Input: none
// Output: none
void BSP_PeriodicTask_StopB(void){
  TA1CTL &= ~0x0030;           // halt Timer A1
  NVIC_ICER0 = 0x00000400;     // disable interrupt 10 in NVIC
}

void TA1_0_IRQHandler(void){
  TA1CCTL0 &= ~0x0001;          // acknowledge capture/compare interrupt TA1_0
  (*PeriodicTaskB)();           // execute user task
}

// ***************** BSP_PeriodicTask_InitC ****************
// Activate 16-bit Timer A2 interrupts to run user task periodically
// assumes SMCLK is 12MHz, using divide by 24, 500kHz/65536=7.6 Hz
// Input:  task is a pointer to a user function
//         freq is number of interrupts per second
//           8 Hz to 10 kHz
//         priority is a number 0 to 6
// Outputs: none
// comment: it is accurate if 500000/freq is an integer
void (*PeriodicTaskC)(void);   // user function
void BSP_PeriodicTask_InitC(void(*task)(void), uint32_t freq, uint8_t priority){long sr;
  if((freq < 8) || (freq > 10000)){
    return;                        // invalid input
  }
  if(priority > 6){
    priority = 6;
  }
  sr = StartCritical();
  PeriodicTaskC = task; // user function
  TA2CTL &= ~0x0030;    // halt Timer A2
  // bits15-10=XXXXXX, reserved
  // bits9-8=10,       clock source to SMCLK
  // bits7-6=10,       input clock divider /4
  // bits5-4=00,       stop mode
  // bit3=X,           reserved
  // bit2=0,           set this bit to clear
  // bit1=0,           no interrupt on timer
  // bit0=0,           clear interrupt pending
  TA2CTL = 0x0280;
  TA2EX0 = 0x0005;    // configure for input clock divider /6
  // bits15-14=00,     no capture mode
  // bits13-12=XX,     capture/compare input select
  // bit11=X,          synchronize capture source
  // bit10=X,          synchronized capture/compare input
  // bit9=X,           reserved
  // bit8=0,           compare mode
  // bits7-5=XXX,      output mode
  // bit4=1,           enable capture/compare interrupt on CCIFG
  // bit3=X,           read capture/compare input from here
  // bit2=0,           output this value in output mode 0
  // bit1=X,           capture overflow status
  // bit0=0,           clear capture/compare interrupt pending
  TA2CCTL0 = 0x0010;
  TA2CCR0 = (BSP_Clock_GetFreq()/96/freq) - 1;  // compare match value
// interrupts enabled in the main program after all devices initialized
  NVIC_IPR3 = (NVIC_IPR3&0xFFFFFF00)|(priority<<5);

  NVIC_ISER0 = 0x00001000; // enable interrupt 12 in NVIC
  TA2CTL |= 0x0014;        // reset and start Timer A2 in up mode
  EndCritical(sr);
}

// ------------BSP_PeriodicTask_StopC------------
// Deactivate the interrupt running a user task periodically.
// Input: none
// Output: none
void BSP_PeriodicTask_StopC(void){
  TA2CTL &= ~0x0010;           // halt Timer A2
  NVIC_ICER0 = 0x00001000;     // disable interrupt 12 in NVIC
}

void TA2_0_IRQHandler(void){
  TA2CCTL0 &= ~0x0001;          // acknowledge capture/compare interrupt TA2_0
  (*PeriodicTaskC)();           // execute user task
}

// ------------BSP_Time_Init------------
// Activate a 32-bit timer to count the number of
// microseconds since the timer was initialized.
// Input: none
// Output: none
// Assumes: BSP_Clock_InitFastest() has been called
//          so clock = 48/16 = 3 MHz
void BSP_Time_Init(void){long sr;
  sr = StartCritical();
  TIMER32_LOAD2 = 0xFFFFFFFF;      // timer reload value
  TIMER32_INTCLR2 = 0x00000001;    // clear Timer32 Timer 2 interrupt
  // bits31-8=X...X,   reserved
  // bit7=1,           timer enable
  // bit6=1,           timer in periodic mode
  // bit5=0,           interrupt enable
  // bit4=X,           reserved
  // bits3-2=01,       input clock divider /16
  // bit1=1,           32-bit counter
  // bit0=0,           wrapping mode
  TIMER32_CONTROL2 = 0x000000C6;
  EndCritical(sr);
}

// ------------BSP_Time_Get------------
// Return the system time in microseconds, which is the
// number of 32-bit timer counts since the timer was
// initialized.  This will work properly for at least 23
// minutes after which it could roll over.
// Input: none
// Output: system time in microseconds
// Assumes: BSP_Time_Init() has been called
uint32_t BSP_Time_Get(void){
  // 2*32/3,000,000 = 1431 seconds, about 23 minutes
  return (0xFFFFFFFF - TIMER32_VALUE2)/3;
}

// ------------BSP_Delay1ms------------
// Simple delay function which delays about n
// milliseconds.
// Inputs: n  number of 1 msec to wait
// Outputs: none
void BSP_Delay1ms(uint32_t n){
  while(n){
    parrotdelay(8000);                  // 1 msec, tuned at 48 MHz, originally part of LCD module
    n--;
  }
}

// There are two I2C devices on the Educational BoosterPack MKII:
// OPT3001 Light Sensor
// TMP006 Temperature sensor
// Both initialization functions can use this general I2C
// initialization.
void static i2cinit(void){
  // initialize eUSCI
  UCB1CTLW0 = 0x0001;                // hold the eUSCI module in reset mode
  // configure UCB1CTLW0 for:
  // bit15      UCA10 = 0; own address is 7-bit address
  // bit14      UCSLA10 = 0; address slave with 7-bit address
  // bit13      UCMM = 0; single master environment
  // bit12      reserved
  // bit11      UCMST = 1; master mode
  // bits10-9   UCMODEx = 3; I2C mode
  // bit8       UCSYNC = 1; synchronous mode
  // bits7-6    UCSSELx = 2; eUSCI clock SMCLK
  // bit5       UCTXACK = X; transmit ACK condition in slave mode
  // bit4       UCTR = X; transmitter/receiver
  // bit3       UCTXNACK = X; transmit negative acknowledge in slave mode
  // bit2       UCTXSTP = X; transmit stop condition in master mode
  // bit1       UCTXSTT = X; transmit start condition in master mode
  // bit0       UCSWRST = 1; reset enabled
  UCB1CTLW0 = 0x0F81;
  // configure UCB1CTLW1 for:
  // bits15-9   reserved
  // bit8       UCETXINT = X; early UCTXIFG0 in slave mode
  // bits7-6    UCCLTO = 3; timeout clock low after 165,000 SYSCLK cycles
  // bit5       UCSTPNACK = 0; send negative acknowledge before stop condition in master receiver mode
  // bit4       UCSWACK = 0; slave address acknowledge controlled by hardware
  // bits3-2    UCASTPx = 2; generate stop condition automatically after UCB0TBCNT bytes
  // bits1-0    UCGLITx = 0 deglitch time of 50 ns
  UCB1CTLW1 = 0x00C8;
  UCB1TBCNT = 2;                     // generate stop condition after this many bytes
  // set the baud rate for the eUSCI which gets its clock from SMCLK
  // Clock_Init48MHz() from ClockSystem.c sets SMCLK = HFXTCLK/4 = 12 MHz
  // if the SMCLK is set to 12 MHz, divide by 120 for 100 kHz baud clock
  UCB1BRW = 120;
  P6SEL0 |= 0x30;
  P6SEL1 &= ~0x30;                   // configure P6.5 and P6.4 as primary module function
  UCB1CTLW0 &= ~0x0001;              // enable eUSCI module
  UCB1IE = 0x0000;                   // disable interrupts
}

/*// receives one byte from specified slave
// Note for HMC6352 compass only:
// Used with 'r' and 'g' commands
// Note for TMP102 thermometer only:
// Used to read the top byte of the contents of the pointer register
//  This will work but is probably not what you want to do.
uint8_t static I2C_Recv(int8_t slave){
  int8_t data1;
  while(UCB1STATW&0x0010){};         // wait for I2C ready
  UCB1CTLW0 |= 0x0001;               // hold the eUSCI module in reset mode
  UCB1TBCNT = 1;                     // generate stop condition after this many bytes
  UCB1CTLW0 &= ~0x0001;              // enable eUSCI module
  UCB1I2CSA = slave;                 // I2CCSA[6:0] is slave address
  UCB1CTLW0 = ((UCB1CTLW0&~0x0014)   // clear bit4 (UCTR) for receive mode
                                     // clear bit2 (UCTXSTP) for no transmit stop condition
                | 0x0002);           // set bit1 (UCTXSTT) for transmit start condition
  while((UCB1IFG&0x0001) == 0){      // wait for complete character received
    if(UCB1IFG&0x0030){              // bit5 set on not-acknowledge; bit4 set on arbitration lost
      i2cinit();                     // reset to known state
      return 0xFF;
    }
  }
  data1 = UCB1RXBUF&0xFF;            // get the reply
  return data1;
}*/

// receives two bytes from specified slave
// Note for HMC6352 compass only:
// Used with 'A' commands
// Note for TMP102 thermometer only:
// Used to read the contents of the pointer register
uint16_t static I2C_Recv2(int8_t slave){
  uint8_t data1, data2;
  while(UCB1STATW&0x0010){};         // wait for I2C ready
  UCB1CTLW0 |= 0x0001;               // hold the eUSCI module in reset mode
  UCB1TBCNT = 2;                     // generate stop condition after this many bytes
  UCB1CTLW0 &= ~0x0001;              // enable eUSCI module
  UCB1I2CSA = slave;                 // I2CCSA[6:0] is slave address
  UCB1CTLW0 = ((UCB1CTLW0&~0x0014)   // clear bit4 (UCTR) for receive mode
                                     // clear bit2 (UCTXSTP) for no transmit stop condition
                | 0x0002);           // set bit1 (UCTXSTT) for transmit start condition
  while((UCB1IFG&0x0001) == 0){      // wait for complete character received
    if(UCB1IFG&0x0030){              // bit5 set on not-acknowledge; bit4 set on arbitration lost
      i2cinit();                     // reset to known state
      return 0xFFFF;
    }
  }
  data1 = UCB1RXBUF&0xFF;            // get the reply
  while((UCB1IFG&0x0001) == 0){      // wait for complete character received
    if(UCB1IFG&0x0030){              // bit5 set on not-acknowledge; bit4 set on arbitration lost
      i2cinit();                     // reset to known state
      return 0xFFFF;
    }
  }
  data2 = UCB1RXBUF&0xFF;            // get the reply
  return (data1<<8)+data2;
}

// sends one byte to specified slave
// Note for HMC6352 compass only:
// Used with 'S', 'W', 'O', 'C', 'E', 'L', and 'A' commands
//  For 'A' commands, I2C_Recv2() should also be called
// Note for TMP102 thermometer only:
// Used to change the pointer register
// Returns 0 if successful, nonzero if error
uint16_t static I2C_Send1(int8_t slave, uint8_t data1){
  uint16_t debugdump;                // save status register here in case of error
  while(UCB1STATW&0x0010){};         // wait for I2C ready
  UCB1CTLW0 |= 0x0001;               // hold the eUSCI module in reset mode
  UCB1TBCNT = 1;                     // generate stop condition after this many bytes
  UCB1CTLW0 &= ~0x0001;              // enable eUSCI module
  UCB1I2CSA = slave;                 // I2CCSA[6:0] is slave address
  UCB1CTLW0 = ((UCB1CTLW0&~0x0004)   // clear bit2 (UCTXSTP) for no transmit stop condition
                                     // set bit1 (UCTXSTT) for transmit start condition
                | 0x0012);           // set bit4 (UCTR) for transmit mode
  while(UCB1CTLW0&0x0002){};         // wait for slave address sent
  UCB1TXBUF = data1&0xFF;            // TXBUF[7:0] is data
  while(UCB1STATW&0x0010){           // wait for I2C idle
    if(UCB1IFG&0x0030){              // bit5 set on not-acknowledge; bit4 set on arbitration lost
      debugdump = UCB1IFG;           // snapshot flag register for calling program
      i2cinit();                     // reset to known state
      return debugdump;
    }
  }
  return 0;
}

/*// sends two bytes to specified slave
// Note for HMC6352 compass only:
// Used with 'r' and 'g' commands
//  For 'r' and 'g' commands, I2C_Recv() should also be called
// Note for TMP102 thermometer only:
// Used to change the top byte of the contents of the pointer register
//  This will work but is probably not what you want to do.
// Returns 0 if successful, nonzero if error
uint16_t static I2C_Send2(int8_t slave, uint8_t data1, uint8_t data2){
  uint16_t debugdump;                // save status register here in case of error
  while(UCB1STATW&0x0010){};         // wait for I2C ready
  UCB1CTLW0 |= 0x0001;               // hold the eUSCI module in reset mode
  UCB1TBCNT = 2;                     // generate stop condition after this many bytes
  UCB1CTLW0 &= ~0x0001;              // enable eUSCI module
  UCB1I2CSA = slave;                 // I2CCSA[6:0] is slave address
  UCB1CTLW0 = ((UCB1CTLW0&~0x0004)   // clear bit2 (UCTXSTP) for no transmit stop condition
                                     // set bit1 (UCTXSTT) for transmit start condition
                | 0x0012);           // set bit4 (UCTR) for transmit mode
  while(UCB1CTLW0&0x0002){};         // wait for slave address sent
  UCB1TXBUF = data1&0xFF;            // TXBUF[7:0] is data
  while((UCB1IFG&0x0002) == 0){      // wait for first data sent
    if(UCB1IFG&0x0030){              // bit5 set on not-acknowledge; bit4 set on arbitration lost
      debugdump = UCB1IFG;           // snapshot flag register for calling program
      i2cinit();                     // reset to known state
      return debugdump;
    }
  }
  UCB1TXBUF = data2&0xFF;            // TXBUF[7:0] is data
  while(UCB1STATW&0x0010){           // wait for I2C idle
    if(UCB1IFG&0x0030){              // bit5 set on not-acknowledge; bit4 set on arbitration lost
      debugdump = UCB1IFG;           // snapshot flag register for calling program
      i2cinit();                     // reset to known state
      return debugdump;
    }
  }
  return 0;
}*/

// sends three bytes to specified slave
// Note for HMC6352 compass only:
// Used with 'w' and 'G' commands
// Note for TMP102 thermometer only:
// Used to change the contents of the pointer register
// Returns 0 if successful, nonzero if error
uint16_t static I2C_Send3(int8_t slave, uint8_t data1, uint8_t data2, uint8_t data3){
  uint16_t debugdump;                // save status register here in case of error
  while(UCB1STATW&0x0010){};         // wait for I2C ready
  UCB1CTLW0 |= 0x0001;               // hold the eUSCI module in reset mode
  UCB1TBCNT = 3;                     // generate stop condition after this many bytes
  UCB1CTLW0 &= ~0x0001;              // enable eUSCI module
  UCB1I2CSA = slave;                 // I2CCSA[6:0] is slave address
  UCB1CTLW0 = ((UCB1CTLW0&~0x0004)   // clear bit2 (UCTXSTP) for no transmit stop condition
                                     // set bit1 (UCTXSTT) for transmit start condition
                | 0x0012);           // set bit4 (UCTR) for transmit mode
  while((UCB1IFG&0x0002) == 0){};    // wait for slave address sent
  UCB1TXBUF = data1&0xFF;            // TXBUF[7:0] is data
  while((UCB1IFG&0x0002) == 0){      // wait for first data sent
    if(UCB1IFG&0x0030){              // bit5 set on not-acknowledge; bit4 set on arbitration lost
      debugdump = UCB0IFG;           // snapshot flag register for calling program
      i2cinit();                     // reset to known state
      return debugdump;
    }
  }
  UCB1TXBUF = data2&0xFF;            // TXBUF[7:0] is data
  while((UCB1IFG&0x0002) == 0){      // wait for second data sent
    if(UCB1IFG&0x0030){              // bit5 set on not-acknowledge; bit4 set on arbitration lost
      debugdump = UCB1IFG;           // snapshot flag register for calling program
      i2cinit();                     // reset to known state
      return debugdump;
    }
  }
  UCB1TXBUF = data3&0xFF;            // TXBUF[7:0] is data
  while(UCB1STATW&0x0010){           // wait for I2C idle
    if(UCB1IFG&0x0030){              // bit5 set on not-acknowledge; bit4 set on arbitration lost
      debugdump = UCB1IFG;           // snapshot flag register for calling program
      i2cinit();                     // reset to known state
      return debugdump;
    }
  }
  return 0;
}

// ------------BSP_LightSensor_Init------------
// Initialize a GPIO pin for input, which corresponds
// with BoosterPack pins J1.8 (Light Sensor interrupt).
// Initialize two I2C pins, which correspond with
// BoosterPack pins J1.9 (SCL) and J1.10 (SDA).
// Input: none
// Output: none
void BSP_LightSensor_Init(void){
  i2cinit();
  P4SEL0 &= ~0x40;
  P4SEL1 &= ~0x40;                 // configure P4.6 as GPIO
  P4DIR &= ~0x40;                  // make P4.6 in
  P4REN &= ~0x40;                  // disable pull resistor on P4.6
}

// Send the appropriate codes to initiate a
// measurement with an OPT3001 light sensor at I2C
// slave address 'slaveAddress'.
// Assumes: BSP_LightSensor_Init() has been called
void static lightsensorstart(uint8_t slaveAddress){
  // configure Low Limit Register (0x02) for:
  // INT pin active after each conversion completes
  I2C_Send3(slaveAddress, 0x02, 0xC0, 0x00);
  // configure Configuration Register (0x01) for:
  // 15-12 RN         range number         1100b = automatic full-scale setting mode
  // 11    CT         conversion time         1b = 800 ms
  // 10-9  M          mode of conversion     01b = single-shot
  // 8     OVF        overflow flag field     0b (read only)
  // 7     CRF        conversion ready field  0b (read only)
  // 6     FH         flag high field         0b (read only)
  // 5     FL         flag low field          0b (read only)
  // 4     L          latch                   1b = latch interrupt if measurement exceeds programmed ranges
  // 3     POL        polarity                0b = INT pin reports active low
  // 2     ME         mask exponent           0b = do not mask exponent (more math later)
  // 1-0   FC         fault count            00b = 1 fault triggers interrupt
  I2C_Send3(slaveAddress, 0x01, 0xCA, 0x10);
  I2C_Recv2(slaveAddress);         // read Configuration Register to reset conversion ready
}

// Send the appropriate codes to end a measurement
// with an OPT3001 light sensor at I2C slave address
// 'slaveAddress'.  Return results (units 100*lux).
// Assumes: BSP_LightSensor_Init() has been called and measurement is ready
int32_t static lightsensorend(uint8_t slaveAddress){
  uint16_t raw, config;
  I2C_Send1(slaveAddress, 0x00);   // pointer register 0x00 = Result Register
  raw = I2C_Recv2(slaveAddress);
  // force the INT pin to clear by clearing and resetting the latch bit of the Configuration Register (0x01)
  I2C_Send1(slaveAddress, 0x01);   // pointer register 0x01 = Configuration Register
  config = I2C_Recv2(slaveAddress);// current Configuration Register
  I2C_Send3(slaveAddress, 0x01, (config&0xFF00)>>8, (config&0x00FF)&~0x0010);
  I2C_Send3(slaveAddress, 0x01, (config&0xFF00)>>8, (config&0x00FF)|0x0010);
  return (1<<(raw>>12))*(raw&0x0FFF);
}

// ------------BSP_LightSensor_Input------------
// Query the OPT3001 light sensor for a measurement.
// Wait until the measurement is ready, which may
// take 800 ms.
// Input: none
// Output: light intensity (units 100*lux)
// Assumes: BSP_LightSensor_Init() has been called
#define LIGHTINT  (*((volatile uint8_t *)(0x42000000+32*0x4C21+4*6)))  /* Port 4.6 Input */
int LightBusy = 0;                 // 0 = idle; 1 = measuring
uint32_t BSP_LightSensor_Input(void){
  uint32_t light;
  LightBusy = 1;
  lightsensorstart(0x44);
  while(LIGHTINT == 0x01){};       // wait for conversion to complete
  light = lightsensorend(0x44);
  LightBusy = 0;
  return light;
}

// ------------BSP_LightSensor_Start------------
// Start a measurement using the OPT3001.
// If a measurement is currently in progress, return
// immediately.
// Input: none
// Output: none
// Assumes: BSP_LightSensor_Init() has been called
void BSP_LightSensor_Start(void){
  if(LightBusy == 0){
    // no measurement is in progress, so start one
    LightBusy = 1;
    lightsensorstart(0x44);
  }
}

// ------------BSP_LightSensor_End------------
// Query the OPT3001 light sensor for a measurement.
// If no measurement is currently in progress, start
// one and return zero immediately.  If the measurement
// is not yet complete, return zero immediately.  If
// the measurement is complete, store the result in the
// pointer provided and return one.
// Input: light is pointer to store light intensity (units 100*lux)
// Output: one if measurement is ready and pointer is valid
//         zero if measurement is not ready and pointer unchanged
// Assumes: BSP_LightSensor_Init() has been called
int BSP_LightSensor_End(uint32_t *light){
  uint32_t lightLocal;
  if(LightBusy == 0){
    // no measurement is in progress, so start one
    LightBusy = 1;
    lightsensorstart(0x44);
    return 0;                      // measurement needs more time to complete
  } else{
    // measurement is in progress
    if(LIGHTINT == 0x01){
      return 0;                    // measurement needs more time to complete
    } else{
      lightLocal = lightsensorend(0x44);
      *light = lightLocal;
      LightBusy = 0;
      return 1;                    // measurement is complete; pointer valid
    }
  }
}

// ------------BSP_TempSensor_Init------------
// Initialize a GPIO pin for input, which corresponds
// with BoosterPack pins J2.11 (Temperature Sensor
// interrupt).  Initialize two I2C pins, which
// correspond with BoosterPack pins J1.9 (SCL) and
// J1.10 (SDA).
// Input: none
// Output: none
void BSP_TempSensor_Init(void){
  i2cinit();
  P3SEL0 &= ~0x40;
  P3SEL1 &= ~0x40;                 // configure P3.6 as GPIO
  P3DIR &= ~0x40;                  // make P3.6 in
  P3REN &= ~0x40;                  // disable pull resistor on P3.6
}

// Send the appropriate codes to initiate a
// measurement with a TMP006 temperature sensor at
// I2C slave address 'slaveAddress'.
// Assumes: BSP_TempSensor_Init() has been called
void static tempsensorstart(uint8_t slaveAddress){
  // configure Configuration Register (0x02) for:
  // 15    RST        software reset bit      0b = normal operation
  // 14-12 MOD        mode of operation     111b = sensor and die continuous conversion
  // 11-9  CR         ADC conversion rate   010b = 1 sample/sec
  // 8     EN         interrupt pin enable    1b = ~DRDY pin enabled (J2.11/P3.6)
  // 7     ~DRDY      data ready bit          0b (read only, automatic clear)
  // 6-0   reserved                      000000b (reserved)
  I2C_Send3(slaveAddress, 0x02, 0x75, 0x00);
}

// Send the appropriate codes to end a measurement
// with a TMP006 temperature sensor at I2C slave
// address 'slaveAddress'.  Store the results at the
// provided pointers.
// Assumes: BSP_TempSensor_Init() has been called and measurement is ready
void static tempsensorend(uint8_t slaveAddress, int32_t *sensorV, int32_t *localT){
  int16_t raw;
  I2C_Send1(slaveAddress, 0x00);   // pointer register 0x00 = Sensor Voltage Register
  raw = I2C_Recv2(slaveAddress);
  *sensorV = raw*15625;            // 156.25 nV per LSB
  I2C_Send1(slaveAddress, 0x01);   // pointer register 0x01 = Local Temperature Register
  raw = I2C_Recv2(slaveAddress);
  *localT = (raw>>2)*3125;         // 0.03125 C per LSB
}

// ------------BSP_TempSensor_Input------------
// Query the TMP006 temperature sensor for a
// measurement.  Wait until the measurement is ready,
// which may take 4 seconds.
// Input: sensorV is signed pointer to store sensor voltage (units 100*nV)
//        localT is signed pointer to store local temperature (units 100,000*C)
// Output: none
// Assumes: BSP_TempSensor_Init() has been called
#define TEMPINT   (*((volatile uint8_t *)(0x42000000+32*0x4C20+4*6)))  /* Port 3.6 Input */
int TempBusy = 0;                  // 0 = idle; 1 = measuring
void BSP_TempSensor_Input(int32_t *sensorV, int32_t *localT){
  int32_t volt, temp;
  TempBusy = 1;
  tempsensorstart(0x40);
  while(TEMPINT == 0x01){};        // wait for conversion to complete
  tempsensorend(0x40, &volt, &temp);
  *sensorV = volt;
  *localT = temp;
  TempBusy = 0;
}

// ------------BSP_TempSensor_Start------------
// Start a measurement using the TMP006.
// If a measurement is currently in progress, return
// immediately.
// Input: none
// Output: none
// Assumes: BSP_TempSensor_Init() has been called
void BSP_TempSensor_Start(void){
  if(TempBusy == 0){
    // no measurement is in progress, so start one
    TempBusy = 1;
    tempsensorstart(0x40);
  }
}

// ------------BSP_TempSensor_End------------
// Query the TMP006 temperature sensor for a
// measurement.  If no measurement is currently in
// progress, start one and return zero immediately.
// If the measurement is not yet complete, return
// zero immediately.  If the measurement is complete,
// store the result in the pointers provided and
// return one.
// Input: sensorV is signed pointer to store sensor voltage (units 100*nV)
//        localT is signed pointer to store local temperature (units 100,000*C)
// Output: one if measurement is ready and pointers are valid
//         zero if measurement is not ready and pointers unchanged
// Assumes: BSP_TempSensor_Init() has been called
int BSP_TempSensor_End(int32_t *sensorV, int32_t *localT){
  int32_t volt, temp;
  if(TempBusy == 0){
    // no measurement is in progress, so start one
    TempBusy = 1;
    tempsensorstart(0x40);
    return 0;                      // measurement needs more time to complete
  } else{
    // measurement is in progress
    if(TEMPINT == 0x01){
      return 0;                    // measurement needs more time to complete
    } else{
      tempsensorend(0x40, &volt, &temp);
      *sensorV = volt;
      *localT = temp;
      TempBusy = 0;
      return 1;                    // measurement is complete; pointers valid
    }
  }
}
