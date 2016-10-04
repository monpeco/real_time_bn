###Introduction

Operating Systems, particulary in the real time area, have evolved over the years. 
As systems have evolved and complexity has grown, what we have today is an 
operating system, which is a manager of resources that are provided by the underlying 
hardware.

Whenever we build a system with complexity it is important for us to consider 
performance metrics. In other words, how well does it actually work and in this class
we're going to consider two performance metrics all throughout.

The first are ___deadlines___ and what that means is, does this task execute not 
only to get the correct answer but to get the correct answer at the right time?

And the second performance metric is ___reliability___. In other words, does the 
system run in a predictable and reliable fashion because embedded systems are 
often used in safety critical applications.

And the other thing that we will dwell a significant amount of time on
is how do these tasks or threads act in conjunction
with each other, which means they have to synchronize their activities
or they have to communicate between each other.
So a significant part of our time will be spent on synchronization operations
and communication primitives.

Whenever we start a new design it's good practice to look around for an existing 
design that already works and modify that design to solve our current problem.
That's a process we call ___porting___ and our operating system has to keep track 
or lend itself to the ability to change from one problem to another.

The RTOS must ___manage resources___ like memory, processor and I/O. The RTOS will 
guarantee ___strict timing constraints___ and provide ___reliable___ operation. 
The RTOS will support ___synchronization___ and ___communication___ between tasks. 
As complex systems are built the RTOS manages the ___integration of components___. 
___Evolution___ is the notion of a system changing to improve performance, features 
and reliability. The RTOS must manage change. When designing a new system, it is 
good design practice to build a new system by changing an existing system. 
The notion of ___portability___ is the ease at which one system can be changed 
or adapted to create another system. The ___response time___ or ___latency___ is 
the delay from a request to the beginning of the service of that request. 
There are many definitions of ___bandwidth___. In this book we define bandwidth 
as the number of information bytes/sec that can be transferred or processed.

We can compare and contrast regular operating systems with real-time operating systems

| Regular OS | Real-time OS |
|------------|--------------|
|Complex	    |   Simple  |
|Best effort	|   Guaranteed response |
|Fairness	    |   Strict timing constraints   |
|Average bandwidth      |    Minimum and maximum limits  |
|Unknown components 	|   Known components    |
|Unpredictable behavior	|   Predictable behavior    |
|Plug and play      	|   Upgradable  |

table: Table 1.1. Comparison of regular and real-time operating systems.

From Table 1.1 we see that real-time operating systems have to be simple so they may 
be predictable. While traditional operating systems gauge their performance in 
terms of response time and fairness, real-time operating systems target strict 
timing constraints and upper, lower bounds on bandwidth. One can expect to know 
all the components of the system at design time and component changes happen much 
more infrequently.


###Embedded Systems

Let's look at the components of an embedded system. And we said that the embedded 
system had a computer inside. So we'll represent the computer as this little integrated 
circuit.

And we're going to embed this into many things.
We could put them in our cars, our automotive. We could put them in our bodies.
We could put it in our toys, our consumer electronics, our homes. The military uses 
a lot of embedded devices, in our cell phones and anything that has to do with
communication. And lastly, but not least, is we will see that embedded systems are
ubiquitous in industry.

Let's talk about the computer. As we see, embedded systems are everywhere.
There are estimated over 5 billion of these embedded systems in our world.
And these are computers hidden inside of devices.
And when you say computer most people think of an x86, which is the standard
computer in personal computers. But now we have, coming along, ___ARM computers___.
And the ARM computer that you probably are most aware are the ___A series___, which
are in your cell phones, your iPads, and those types of devices.
But the computers we're going to talk about are ARM computers in the ___M
Series___, or the microcontrollers.

There's another series while we're on it, the ___R series___, which is for real time.
A bunch of those in your cell phones as well. Inside the computer, we also have 
___memory___ to store things. So this is our storage element.
And what you're going to learn in this class is ___I/O___. I/O is where the
interface happens. This is the glue which will connect the components together.
And so we will see that this I/O will have a couple of components.

And before we go on, let's make a list of the important considerations when 
building an embedded system. First and foremost is ___the ability to test or to verify___.
We're going to place embedded systems in important life critical situations,
where if we make a mistake we could lose not only money, but loss of life.
So when building an embedded system the ability to test or to verify it
works will be extremely important.

Of course, we're engineers and we would like to make some money.
So another important consideration in embedded systems is ___the ability to
make profit___, which, as you know, is a combination of market share, and
that's making something that everybody wants to buy, and reducing the cost,
which is both development cost and manufacturing cost.

As we saw also that these systems are powered often by batteries.
So ___power___, which affects size, and heat, and cost, is extremely important.
___Size___, we want to fit it in our pocket.

And the last consideration that we will have for embedded systems is this
idea of ___time___. In an embedded system, we not only need the correct answer, but we need
the correct answer at the right time.


An embedded system is a smart device with a processor that has a special and dedicated 
purpose. The user usually does not or cannot upgrade the hardware/software or 
change what the system does. ___Real time___ means that the embedded system must 
respond to critical events within a strictly defined time, called the deadline. 
A guarantee to meet all deadlines can only be made if the behavior of the operating 
system can be predicted. In other words the timing must be deterministic. There are 
___five types of software functions___ the processor can perform in an embedded system. 
Similar to a general-purpose computer, it can perform ___mathematical and/or data 
processing operations___. It can analyze data and make decisions based on the data. 
A second type involves ___handling and managing time___: as an input (e.g., measure 
period), an output (e.g., output waveforms), and a means to synchronize tasks 
(e.g., run 1000 times a second). A third type involves ___real-time input/output 
for the purpose of measurement or control___. The fourth type involves ___digital 
signal processing (DSP)___, which are mathematical calculations on data streams. 
Examples include audio, video, radar, and sonar. The last type is ___communication 
and networking___. As embedded systems become more complex, how the components are 
linked together will become increasingly important.

___Six constraints typify an embedded system___. First, they are __small size__. 
For example, many systems must be handheld. Second, they must have ___low weight___. 
If the device is deployed in a system that moves, e.g., attached to a human, aircraft 
or vehicle, then weight incurs an energy cost. Third, they often must be ___low 
power___. For example, they might need to operate for a long time on battery power. 
Low power also impacts the amount of heat they are allowed to generate. 
Fourth, embedded systems often ___must operate in harsh environments___, such as 
heat, pressure, vibrations, and shock. They may be subject to noisy power, RF 
interference, water, and chemicals. Fifth, embedded systems are often used in 
safety critical systems. Real-time behavior is essential. For these systems 
they must function properly at extremely ___high levels of reliability___. Lastly, 
embedded systems are extremely sensitive to ___cost___. Most applications are 
profit-driven. For high-volume systems a difference in pennies can significantly 
affect profit.


###Architecture

####RTOS manages resources
___Architecture (ARM)___
* Processor
* Instruction set (ASM)
* Registers (GP / Specials)
* Memory Layout
* I/O => Memory mapped
* Stack
* Interruptions

--
A ___computer___ combines a central processing unit (CPU), random access memory (RAM), 
read only memory (ROM), and input/output (I/O) ports. The common bus in Figure 1.3 
defines the Von Neumann architecture.

![Figure 1.3](https://cloud.githubusercontent.com/assets/16638078/18671942/6c54f322-7f1c-11e6-86de-d27c164d8a30.jpg)
*Figure 1.3. The basic components of a computer system include processor, memory and I/O.*

___Software___ is an ordered sequence of very specific instructions that are stored in 
memory, defining exactly what and when certain tasks are to be performed.

--

![Figure 1.4](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/38f0df45b15b2e32c2af142abc77de4e/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_04_processor.jpg)
*Figure 1.4. The four basic components of a processor.*

The ___ARM Cortex-M processor___ has four major components, as illustrated in Figure 1.4. 
There are___Bus Interface Units (BIU)___ that read data from the bus during a read 
cycle and write data onto the bus during a write cycle.
The ___BIU___ always drives the address bus and the control signals of the bus. 
___The Effective Address Register (EAR)___ contains the memory address used to 
fetch the data needed for the current instruction. Cortex-M microcontrollers execute 
[Thumb instructions](http://stackoverflow.com/questions/10638130/what-is-the-arm-thumb-instruction-set)
extended with Thumb-2 technology. An overview of these instructions will be presented in 
Section 1.5. Many functions in an operating system will require detailed understanding 
of the architecture and assembly language.

The ___Control Unit (CU)___ orchestrates the sequence of operations in the processor. 
The CU issues commands to the other three components. The ___Instruction Register (IR)___ 
contains the operation code (or ___op code___) for the current instruction. 
When extended with Thumb-2 technology, op codes are either 16 or 32 bits wide.

The ___Arithmetic Logic Unit (ALU)___ performs arithmetic and logic operations. 
Addition, subtraction, multiplication and division are examples of arithmetic operations. 
Examples of logic operations are, and, or, exclusive-or, and shift. Many processors 
used in embedded applications support specialized operations such as table lookup, 
multiply and accumulate, and overflow detection.

--

A very small microcomputer, called a ___microcontroller___, contains all the components 
of a computer (processor, memory, I/O) on a single chip. The Atmel ATtiny and the 
TI TM4C123 are examples of microcontrollers. Because a microcomputer is a small 
computer, this term can be confusing because it is used to describe a wide range 
of systems from a 6-pin ATtiny4 running at 1 MHz with 512 bytes of program memory 
to a personal computer with state-of-the-art 64-bit multi-core processor running 
at multi-GHz speeds having terabytes of storage.

In an embedded system the software is converted to machine code, which is a list 
of instructions, and stored in nonvolatile flash ROM. As instructions are fetched, 
they are placed in a ___Pipeline___. This allows instruction fetching to run ahead 
of execution. Instructions on the Cortex-M processor are fetched in order and 
executed in order. However, it can execute one instruction while fetching the next. 
Many high-speed processors allow out of order execution, support parallel execution 
on multiple cores, and employ branch prediction.

On the ARM Cortex-M processor, an instruction may read memory or write memory, 
but does not read and write memory in the same instruction. Each of the phases 
may require one or more bus cycles to complete. Each bus cycle reads or writes 
one piece of data. Because of the multiple bus architecture, most instructions 
execute in one or two cycles. For more information on the time to execute 
instructions, see Table 3.1 in the Cortex-M Technical Reference Manual.

![Figure 1.5](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/6be6b8642a893026d5cc524548077776/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_05_CortexM.jpg)
*Figure 1.5. Harvard architecture of an ARM Cortex-M-based microcontroller.*

Figure 1.5 shows a simplified block diagram of a microcontroller based on the 
ARM Cortex-M processor. It is a ___Harvard architecture___ because it has separate 
data and instruction buses. The instruction set combines the high performance 
typical of a 32-bit processor with high code density typical of 8-bit and 16-bit 
microcontrollers. Instructions are fetched from flash ROM using the ___ICode bus___. 
Data are exchanged with memory and I/O via the ___System Bus Interface___. 
There are many sophisticated debugging features utilizing the ___DCode Bus___. 
An interrupt is a hardware-triggered software function, which is extremely important 
for real-time embedded systems. The ___Latency of an interrupt service__ is the 
time between hardware trigger and software response. Some internal peripherals, 
like the ___nested vectored interrupt controller (NVIC)__, communicate directly 
with the processor via the ___Private Peripheral Bus (PPB)___. The tight integration 
of the processor and interrupt controller provides fast execution of ___Interrupt 
service routines (ISRs)___, dramatically reducing the interrupt latency.

--

###Memory

|Resource|Function|
|----|----|
|ROM|Instructions|  
|RAM|Data. Our local variables, or data structures, the stack|
|I/O|These are all the pins that come out of our board|


|TM4C123 Resource|Adresses|
|----|----|
|ROM 256 KiB|0x0000.0000 - 0x0003.FFFF|
|RAM 32 KiB |0x2000.0000 - 0x2000.7FFF|
|I/O Devices Registers |0x400F.0000 - 0x400F.FFFF|
|Internal I/O|0xE000.0000 - 0xE004.1FFF|

![Figure 1.6](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/8c92e9c95878272a9ebd01774948d4af/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_06_memoryMap.jpg)
*Figure 1.6. Memory map of the TM4C123 with 256k ROM and 32k RAM and the MSP432 with 256k ROM and 64k RAM.*

The microcontrollers in the Cortex-M family differ by the amount of memory and by 
the types of I/O modules. There are hundreds of members in this family; some of 
them are listed in Table 1.2. The memory maps of TM4C123 and MSP432 are shown in 
Figure 1.6. Although this course focuses on two microcontrollers from Texas Instruments, 
all ARM Cortex-M microcontrollers have similar memory maps. In general, Flash ROM 
begins at address 0x0000.0000, RAM begins at 0x2000.0000, the peripheral I/O 
space is from 0x4000.0000 to 0x5FFF.FFFF, and I/O modules on the private peripheral 
bus exist from 0xE000.0000 to 0xE00F.FFFF. In particular, the only differences in 
the memory map for the various members of the Cortex-M family are the ending addresses 
of the flash and RAM.

|Part number	|RAM	|Flash	|I/O	|I/O modules
|----|----|----|----|----|
|MSP432P401RIPZ	|64	|256	|84	|floating point, DMA|
|LM4F120H5QR	|32	|256	|43	|floating point, CAN, DMA, USB|
|TM4C123GH6PM	|32	|256	|43	|floating point, CAN, DMA, USB, PWM|
|STM32F051R8T6	|8	|64	|55	|DAC, Touch sensor, DMA, I2S, HDMI, PWM|
|MKE02Z64VQH2	|4	|64v	|53	|PWM|
||KiB|	KiB|	pins||	

*Table 1.2. Memory and I/O modules (all have SysTick, RTC, timers, UART, I2C, SSI, and ADC).*

--

Having multiple buses means the processor can perform multiple tasks in parallel. 
On the TM4C123, ___general purpose input/output (GPIO)___ ports can be accessed 
using either the ___PPB___ or ___AHPB___. The following is some of the tasks that 
can occur in parallel

* ICode bus Fetch opcode from ROM
* DCode bus Read constant data from ROM
* System bus Read/write data from RAM or I/O, fetch opcode from RAM
* PPB Read/write data from internal peripherals like the NVIC
* AHPB Read/write data from internal peripherals like the USB

Instructions and data are accessed using a common bus on a von Neumann machine. 
The Cortex-M processor is a Harvard architecture because instructions are fetched on 
the ICode bus and data accessed on the system bus. The address signals on the ARM 
Cortex-M processor include 32 lines, which together specify the memory address 
(0x0000.0000 to 0xFFFF.FFFF) that is currently being accessed. The address specifies 
both which module (input, output, RAM, or ROM) as well as which cell within 
the module will communicate with the processor. The data signals contain the 
information that is being transferred and also include 32 bits. However, on the 
system bus it can also transfer 8-bit or 16-bit data. The control signals specify 
the timing, the size, and the direction of the transfer.

--

###Registers
So inside the processor, we have our arithmetic logic unit and the control unit, and focus
on the Arithmetich Logic Unit, which contains the ___Registers___.

|Type of Register|Register|Function|
|---|---|---|
|General Purpose Registers| R1, R2, ... , R12|data or addresses|
|Special Registers|R13, R14, and R15||
||R13 |Stack Pointer (SP)|
||R14 |Link Register (LR)|
|Other Special Registers|||
||Processor Status Register (PSR)| Info condition code bits, ISR number, etc|
||PRIMASK|Enable/Disable interrupt|
||Base Priority Register|Priority level of the current operation|
||Control register|Info about status of the processor (High/Low Privilege)|

    
The registers on an ARM Cortex-M processor are depicted in Figure 1.7. R0 to R12 are 
___General Purpose Registers (GPR)___ and contain either data or addresses. 
Register ___R13___ (also called the stack pointer, SP) points to the top element 
of the stack. Actually, there are two stack pointers: the ___Main Stack Pointer (MSP)___ 
and the ___Process Stack Pointer (PSP)___. Only one stack pointer is active at a time. 
In a high-reliability operating system, we could activate the PSP for user software 
and the MSP for operating system software. This way the user program could crash 
without disturbing the operating system. Most of the commercially available real-time 
operating systems available on the Cortex M will use the PSP for user code and MSP 
for OS code. Register ___R14___ (also called the Link Register, LR) is used to 
store the return location for functions. The LR is also used in a special way during 
exceptions, such as interrupts. Register ___R15___ (also called the program counter, PC) 
points to the next instruction to be fetched from memory. The processor fetches 
an instruction using the PC and then increments the PC by the length (in bytes) of 
the instruction fetched.

![Figure 1.7](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/1e0bbbb43f4ab3cf696535e7e8c06059/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_07_registers.jpg)
*Figure 1.7. The registers on the ARM Cortex-M processor.*


The ___ARM Architecture Procedure Call Standard, AAPCS___, part of the ARM Application 
Binary Interface (ABI), uses registers R0, R1, R2, and R3 to pass input parameters 
into a C function or an assembly subroutine. Also according to AAPCS we place the 
return parameter in Register R0. The standard requires functions to preserve the 
contents of R4-R11. In other words, functions save R4-R11, use R4-R11, and then 
restore R4-R11 before returning. Another restriction is to keep the stack aligned 
to 64 bits, by pushing and popping an even number of registers.

--

There are three status registers named ___Application Program Status Register (APSR)___, 
the Interrupt Program Status Register (IPSR), and the ___Execution Program Status 
Register (EPSR)___ as shown in Figure 1.8. These registers can be accessed 
individually or in combination as the ___Program Status Register (PSR)___.


![Figure 1.8](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/1de71c785578c22878cb866793ff0dc5/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_08_statusregister.jpg)
*Figure 1.8. The program status register of the ARM Cortex-M processor.*

The N, Z, V, C, and Q bits signify the status of the previous ALU operation. Many 
instructions set these bits to signify the result of the operation. In general, 
the ___N bit___ is set after an arithmetical or logical operation signifying whether or 
not the result is negative. Similarly, the ___Z bit___ is set if the result is zero. 
The ___C bit___ means carry and is set on an unsigned overflow, and the ___V bit___ 
signifies signed overflow. The ___Q bit___ is the sticky saturation flag, indicating 
that “saturation” has occurred, and is set by the SSAT and USAT instructions.

The ___T bit___ will always be 1, indicating the ARM Cortex-M processor is executing 
Thumb instructions. The ___ICI/IT bits___ are used by interrupts and by IF-THEN 
instructions. The ___ISR_NUMBER___ indicates which interrupt, if any, the processor 
is handling. ___Bit 0___ of the special register ___PRIMASK___ is the interrupt mask 
bit, or ___I bit___. If this bit is 1 most interrupts and exceptions are not allowed. 
If the bit is 0, then interrupts are allowed. __Bit 0__ of the special register 
___FAULTMASK___ is the fault mask bit. If this bit is 1 all interrupts and faults 
are disallowed. If the bit is 0, then interrupts and faults are allowed. The 
___Nonmaskable Interrupt (NMI)__ is not affected by these mask bits. The ___BASEPRI___ 
register defines the priority of the executing software. It prevents interrupts 
with lower or equal priority from interrupting the current execution but allows 
higher priority interrupts. For example if BASEPRI equals 3, then requests with 
level 0, 1, and 2 can interrupt, while requests at levels 3 and higher will be 
postponed. The details of interrupt processing will be presented in Chapters 2 and 3.


--
--

###Stack

The stack is a region of memory inside the RAM. The Stack Pointer allways points to 
the botton part of the Stack, which is called the "Higher Adress", and the end is called
"Lower Adrees". The two operations that allow us to grow or shrink the stack are 
the push and pop operations. The stack is a FIFO Structure

####Guidelines
* Equal number of Pushes and Pops (balancing the stack)
* Perform Push and Pop operations in the allocated area
* Do not read or write to the free area
* Push / Pop are 32-bits operation

--

The ___Stack___ is a last-in-first-out temporary storage. Managing the stack is an 
important function for the operating system. To create a stack, a block of RAM 
is allocated for this temporary storage. On the ARM Cortex-M processor, the stack 
always operates on 32-bit data. All stack accesses are word aligned, which means 
the least significant two bits of SP must always be 0. The stack pointer (SP) 
points to the 32-bit data on the top of the stack.

To ___Push___ data we first decrement the SP by 4 then store 32-bit data at the SP. We 
refer to the most recent item pushed as the “top of the stack”. If though it is 
called the “top”, this item is actually the stored at the lowest address! 
When data is pushed it is saved on the stack.

To ___Pop___ data from the stack, the 32-bit information pointed to by SP is first 
retrieved, and then the stack pointer is incremented by 4. SP points to the last 
item pushed, which will also be the next item to be popped. A stack is a last in 
first out (LIFO) storage, meaning the pop operation will retrieve the newest or 
most recently saved value. When data is popped it is removed from the stack.

--
The boxes in Figure 1.9 represent 32-bit storage elements in RAM. The colored boxes 
in the figure refer to actual data stored on the stack. The white boxes refer to 
locations in the allocated stack area that do not contain data. These allocated 
but not used locations are called the ___free area___. This figure illustrates how the 
stack is used to push the contents of Registers R1, and R2 in that order. Assume 
Register R0 initially contains the value 13, R1 contains 2 and R2 contains 5. 
The drawing on the left shows the initial stack. The software executes these 
three instructions, first pushing two elements, and then popping one.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/b91bac073abc6ebc86b48d3aaebe5eb9/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_09_stack.jpg)
*Figure 1.9. Stack picture showing two pushes and one pop. Push stores data onto the stack, pop retrieves/removes data from the stack.*

The instruction PUSH {R1} saves the value of R1 on the stack. It first decrements 
SP by 4, and then it stores the contents of R1 into the memory location pointed 
to by SP. Assuming R1, R2 had values 13, 5 respectively, after the two push 
instructions the stack contains the numbers 13 and 5, with 5 on top, (third 
picture in Figure 1.9). The instruction POP {R0} retrieves the most recent data 
from the stack. It first moves the value from memory pointed to by SP into R0, 
and then it increments SP by 4.

In Figure 1.9 we pushed two elements and then popped one, so the stack has more 
data than when we started. Normally, all blocks of software will first push and 
then pop, where the number of pops equals the number of pushes. Having an equal 
number of pushes and pops is defined as ___balancing the stack___.


We define the 32-bit word pointed to by SP as the top entry of the stack. If it 
exists, we define the 32-bit data immediately below the top, at SP+4, as next to 
top. Proper use of the stack requires following these important guidelines

* Functions should have an equal number of pushes and pops
* Stack accesses (push or pop) should not be performed outside the allocated area
* Stack reads and writes should not be performed within the free area
* Push and pop are 32-bit operation

Functions that violate rule number 1 will probably crash when incorrect data are 
popped off at a later time. Violations of rule number 2 usually result from a 
stack underflow or overflow. Overflow occurs when the number of elements became 
larger than the allocated space. Stack underflow is caused when there are more 
pops than pushes, and is always the result of a software bug. A stack overflow 
can be caused by two reasons. If the software mistakenly pushes more than it pops, 
then the stack pointer will eventually overflow its bounds. Even when there is 
exactly one pop for each push, a stack overflow can occur if the stack is not 
allocated large enough. The processor will generate a bus fault when the software
tries read from or write to an address that doesn’t exist. If valid RAM exists 
below the stack then further stack operations will corrupt data in this memory.

When debugging Lab 2, it will be important to develop techniques to visualize the 
stack. Stack errors represent typical failure modes of an operating system.

The stack plays an important role in interrupt processing. Executing an interrupt 
service routine will automatically push eight 32-bit words onto the stack. Since 
interrupts are triggered by hardware events, exactly when interrupts occur is not 
under software control. Therefore, violations of rule 3 will cause erratic behavior 
when operating with interrupts.

The processor allows for two stacks, the main stack (MSP) and the process stack 
(PSP), with independent copies of the stack pointer. The OS would run safer if 
the application code used the PSP and the OS code used the MSP. However to make 
the OS simpler we will run both the application and the OS using the MSP.

--
###Startup

The ___Reset Button___, is a hardware event which causes a sequence of operations to occur, ending up with our program starting.

In the ROM, the initial location is the Stack Pointer (SP), so the first thing that happens is a 32-bit value out of ROM is loaded into the stack pointer.
So our stack is now initialized.

The second event that will happend is location 4, 5, 6 and 7 are the initial Program Counter (PC), so that is loaded into the Program Counter.

Also, set to the Link Register (LR) a 0xFFFF|FFFF, and this code means this link register is not valid.

It will also set the T bit, which is in the Program Status Register (PSR) equal to 1, that means Thumb, so we will never go into ARM mode.

We will notice some other things about the mode, the mode is going to set us up the privilege level, the Thread Privilege Level (TPL) will be set to privilege. This means we have access to everything.

We're going to notice that there's actually two stack pointers in the machine.So we have the ability to have a system stack,
and the main stack pointer will point to it.
And we can have a user stack, and the process stack pointer
could point to it.

And on reset, it's going to activate or use the main stack pointer, so the value is going to be equal to the first location on ROM.

--

A reset occurs immediately after power is applied and can also occur by pushing 
the reset button available on most boards. After a reset, the processor is in 
___thread mode___, running at a privileged level, and using the ___MSP stack pointer___. 
The 32-bit value at flash ROM location 0 is loaded into the SP. A reset also 
loads the 32-bit value at location 4 into the PC. This value is called the ___reset 
vector___. All instructions are halfword aligned. Thus, the least significant bit of 
PC must be 0. However, the assembler will set the least significant bit in the 
reset vector, so the processor will properly initialize the thumb bit (T) in the 
PSR. On the ARM Cortex-M, the T bit should always be set to 1. On reset, the 
processor initializes the LR to 0xFFFFFFFF.

The ARM Cortex-M processor has two privilege levels called ___privileged___ and 
___unprivileged___. Bit 0 of the CONTROL register is the thread privilege level 
(TPL). If TPL is 1 the processor level is privileged. If the bit is 0, then 
processor level is unprivileged. Running at the unprivileged level prevents access 
to various features, including the system timer and the interrupt controller. 
Bit 1 of the CONTROL register is the active stack pointer selection (ASPSEL). 
If ASPSEL is 1, the processor uses the PSP for its stack pointer. If ASPSEL is 0, 
the MSP is used. When designing a high-reliability operating system, we will run 
the user code at an unprivileged level using the PSP and the OS code at the 
privileged level using the MSP.

The processor knows whether it is running in the ___foreground___ (i.e., the main 
program) or in the ___background___ (i.e., an interrupt service routine). ARM 
defines the foreground as ___thread mode___, and the background as ___handler mode__. 
Switching between thread and handler modes occurs automatically. The processor 
begins in thread mode, signified by ISR_NUMBER=0. Whenever it is servicing an 
interrupt it switches to handler mode, signified by setting ISR_NUMBER to specify 
which interrupt is being processed. All interrupt service routines run using the MSP. 
In particular, the context is saved onto whichever stack pointer is active, but 
during the execution of the ISR, the MSP is used. For a high reliability operation 
all interrupt service routines will reside in the operating system. User code can 
be run under interrupt control by providing hooks, which are function pointers. 
The user can set function pointers during initialization, and the operating system 
will call the function during the interrupt service routine.

> Observation: Processor modes and the stack are essential components of building a 
reliable operating system. In particular the processor mode is an architectural 
feature that allows the operating system to restrict access to critical system resources.

###Input/output

I/O is an important part of embedded systems in general. One of the important 
features of an operating system is to manage I/O. Input and output are the means 
of an embedded system to interact with its world. The external devices attached 
to the microcontroller provide functionality for the system. These devices connect 
to the microcontroller through ports. A pin is a specific wire on the microcontroller 
through which we perform input or output. A collection of pins grouped by common 
functionality is called a ___port___. An ___input port___ is hardware on the microcontroller 
that allows information about the external world to enter into the computer. The 
microcontroller also has hardware called an ___output port___ to send information out 
to the external world. The ___GPIO (General Purpose Input Output)___ pins on a microcontroller 
are programmable to be digital input, digital output, analog input or complex and 
protocol (like UART etc.) specific. Microcontrollers use most of their pins for I/O 
(called GPIO), see Figure 1.10. Only a few pins are not used for I/O. Examples of 
pins not used for I/O include power, ground, reset, debugging, and the clock. More 
specifically, the TM4C123 uses 43 of its 64 pins for I/O. Similarly, the MSP432 uses 
84 of its 100 pins for I/O.


![Figure 1.10](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/3b1aa5a48ed71e08dd84d8189390de77/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_10_microcontrollerChips.jpg)
*Figure 1.10. Most of the pins on the microcontroller can perform input/output.*

An ___interface___ is defined as the collection of the I/O port, external electronics, 
physical devices, and the software, which combine to allow the computer to communicate 
with the external world. An example of an ___input interface___ is a switch, where the 
operator toggles the switch, and the software can recognize the switch position. 
An example of an ___output interface___ is a light-emitting diode (LED), where 
the software can turn the light on and off, and the operator can see whether or 
not the light is shining. There is a wide range of possible inputs and outputs, 
which can exist in either digital or analog form. In general, we can classify I/O 
interfaces into four categories

* ___Parallel/digital___ - binary data are available simultaneously on a group of lines
* ___Serial___ - binary data are available one bit at a time on a single line
* ___Analog___ - data are encoded as an electrical voltage, current or power
* ___Time___ - data are encoded as a period, frequency, pulse width or phase shift

In a system with ___memory-mapped I/O___, as shown in Figure 1.11, the I/O ports are 
connected to the processor in a manner similar to memory. I/O ports are assigned 
addresses, and the software accesses I/O using reads and writes to the specific 
I/O addresses. These addresses appear like regular memory addresses, except 
accessing them results in manipulation of a functionality of the mapped I/O port, 
hence the term memory-mapped I/O. As a result, the software inputs from an input 
port using the same instructions as it would if it were reading from memory. 
Similarly, the software outputs from an output port using the same instructions 
as it would if it were writing to memory.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/be60ef178121f5c38cad73de524d6693/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_11_MemoryMappedIOCortexM.jpg)
*Figure 1.11. Memory-mapped input/output.*

Most pins on Cortex M microcontrollers can be used for general purpose I/O (GPIO) 
called regular functions or for more complex functions called ___alternate functions___. 
For example, port pins PA1 and PA0 on the TM4C123 can be either regular parallel 
port pins, or an asynchronous serial port called universal asynchronous 
receiver/transmitter (UART). Some of the alternative functions used in this class are:

* ___UART___ Universal asynchronous receiver/transmitter
* ___SSI___ or ___SPI___ Synchronous serial interface or serial peripheral interface
* ___I2C___ Inter-integrated circuit
* ___Timer___ Periodic interrupts
* ___PWM___ Pulse width modulation
* ___ADC___ Analog to digital converter, measurement analog signals

The ___UART___ can be used for serial communication between computers. It is asynchronous 
and allows for simultaneous communication in both directions. The ___SSI___ (also called ___SPI___) 
is used to interface medium-speed I/O devices. In this class, we will use ___SSI___ to 
interface a graphics display. ___I2C___ is a simple I/O bus that we will use to interface 
low speed peripheral devices. In this class we use ___I2C___ to interface a light sensor 
and a temperature sensor. We will use the timer modules to create periodic interrupts. 
___PWM___ outputs could be used to apply variable power to motor interfaces. However, 
in this class we use ___PWM___ to adjust the volume of the buzzer. The ___ADC___ will be used 
to measure the amplitude of analog signals, and will be important in data acquisition 
systems. In this class we will connect the microphone, joystick and accelerometer 
to the ADC.

___Joint Test Action Group (JTAG)___, standardized as the IEEE 1149.1, is a standard test 
access port used to program and debug the microcontroller board. Each microcontroller 
uses four port pins for the JTAG interface.


--
--


###Tiva microcontroller (TM4C123 Microcontroller)

___TM4C123 microcontroller___. It has 43 pins. A ___port___ is a collection of 
pins that are grouped by function. So a microcontroller is a single-chip computer that
has multiple pins for doing I/O. Now, our ![LaunchPad](http://www.ti.com/lit/ml/slau596/slau596.pdf) 
(MSP-EXP432P401R) is a board which has the TM4C123 on it. There are two resistors on the LaunchPad labeled R9 and R10.
And you're going to have to remove them in order to use the graphic display.

--

![Figure 1.12](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/6943111bcd0e1f4374af3393cd10ce76/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_12_TM4C123.jpg)
*Figure 1.12. All the I/O port pins for the TM4C123GH6PM microcontroller.*


Figure 1.12 draws the I/O port structure for the TM4C123GH6PM, the microcontroller 
is used on the EK-TM4C123GXL LaunchPad. Pins on the TM4C family can be assigned to 
as many as eight different I/O functions. Pins can be configured for digital I/O, 
analog input, timer I/O, or serial I/O. For example PB4 can be a digital I/O, ADC, 
SSI, PWM, timer or CAN pin. The TM4C123GH6PM has eight UART ports, four SSI ports, 
four I2C ports, two 12-bit ADCs, twelve timers, two PWMs, a CAN port, and a USB 
interface. There are 43 I/O lines. There are twelve ADC inputs; each ADC can 
convert up to 1M samples per second.

![Figure 1.13](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/e12abffeea394803ea81fbad7d2bae66/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_13_TM4C123_MKII.jpg)
*Figure 1.13. I/O port pins for the TM4C123GH6PM used in this class with the Educational MKII BoosterPack (BOOSTXL-EDUMKII). PD7 means Port D pin 7.*

Figure 1.13 shows the port pin connections for the hardware using in this class. There are six ports (A, B, C, D, E, and F). You can see from this figure that all of the ports share operation with multiple devices. For example, Port A is used for

* UART to PC
* Light sensor input
* Temperature sensor input
* LCD output

This overlap of features must be managed by the operating system. More information 
about the hardware/software interfaces used in this class will be presented later 
in section 1.6.


![Figure 1.14](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/a9251a58b46b499a63a87e3965896019/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_14CaptionedLaunchPad.jpg)
*Figure 1.14. Tiva TM4C123 Launchpad Evaluation Board based on the TM4C123GH6PM.*

Figure 1.14 shows the TM4C123 LaunchPad. In this class you can use either the 
LaunchPad or the MSP432 LaunchPad. There are some older LaunchPads based on the 
LM4F120, which are virtually identical with the TM4C123. If you have an LM4F120 
system all the TM4C123 code will run on the LM4F120 without modification.

Unfortunately, the TM4C123/LM4F120 LaunchPad connects PB6 to PD0, and PB7 to PD1. 
this class you MUST remove the R9 and R10 resistor in order for the LCD to operate properly.

--

The TM4C123 LaunchPad evaluation board has two switches and one 3-color LED. See 
Figure 1.15. In this class we will not use the switches and LED on the LaunchPad, 
but rather focus on the hardware provided by the MK-II BoosterPack.

![Figure 1.15](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/fa7184847b9bdd54e76c78b75a18a43f/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_15_circuit.jpg)
*Figure 1.15. Switch and LED interfaces on the Texas Instruments TM4C123 LaunchPad Evaluation Board. The zero ohm resistors can be removed so the corresponding pin can be used for its regular purpose. The LM4F120 is similar (except for the USB interface).*


The LaunchPad has four 10-pin connectors, labeled as J1 J2 J3 J4 in Figures 1.14 
and 1.16, to which you can attach your external signals. The top side of these 
connectors has male pins and the bottom side has female sockets. The intent is 
to stack boards together to make a layered system. Texas Instruments also supplies 
BoosterPacks, which are pre-made external devices that will plug into this 40-pin 
connector.

![Figure 1.16](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/ef6be3ef92365e8d7c40e59058dab2e9/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_16_pins.jpg)
Figure 1.16. Interface connectors on the Texas Instruments TM4C123 LaunchPad Evaluation Board.

The intent is to stack boards together to make a layered system, see Figure 1.17. 
The engineering community has developed BoosterPacks, which are pre-made external 
devices that will plug into this 40-pin connector. Figure 1.17 shows a system with 
a LaunchPad and two BoosterPacks.

![Figure 1.16](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/815b620593d19eed67d85e23638ad852/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_17_TM4C123_CC2650_MKII.JPG)
Figure 1.17. A BoosterPack plugs into either the top or bottom of a LaunchPad. In this figure the CC2650 BoosterPack is on the bottom and the MKII BoosterPack is on the top.

There are a number of good methods to connect external circuits to the LaunchPad. 
One method is to purchase a male to female jumper cable (e.g., item number 826 at www.adafruit.com). 
A second method is to solder a solid wire into a female socket (e.g., Hirose DF11-2428SCA) 
creating a male to female jumper wire. In this class we will use BoosterPacks, 
so you will not need to connect individual wires to the LaunchPad.

It is not our goal to teach I/O interfacing in this class, but rather use the 
I/O as a platform to develop and test real-time operating systems with Bluetooth 
connectivity. If you would like more information on the hardware/software aspects 
of interfacing, see Volume 2 of the series.

Embedded Systems: Real-Time Interfacing to ARM Cortex-M Microcontrollers, 
ISBN: 978-1463590154, Jonathan Valvano, http://users.ece.utexas.edu/~valvano/arm/outline.htm



--
--


###MSP432 Microcontroller
There are 84 pins on this processor, which means it has a lot of capabilities.

Now, the I/O that we're going focus on in this class
is centered around these 40 pins, which will connect up to the BoosterPack.
Now, there are two buttons, two switches, and two LEDs on this board,
but we won't be using them in this class.

And one of the interesting features is speed. It turns out some of these sensors are really, really slow.
The light sensor and the temperature sensor are very slow. They operate on once a second.
And then the accelerometer is a medium-speed device, which operates and 10 to 20 times per second.
And then the microphone is a very fast device, which operates in thousands of samples per second.


--


![Figure 1.18](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/2e491a5823d373dcb63e7f8a54e201da/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_18_MSP432.jpg)
*Figure 1.18. I/O port pins for the MSP432P401R microcontroller. (Six pins on Port J not shown).*

Figure 1.18 draws the I/O port structure for the MSP432P401R. This microcontroller is 
used on the MSP-EXP432P401R LaunchPad. Pins can be configured for digital I/O, analog 
input, timer I/O, or serial I/O. For example P1.2 can be digital I/O or serial receive input.

Because of the multiple buses, the microcontroller can perform I/O bus cycles 
simultaneous with instruction fetches from flash ROM. The MSP432P401R has four 
UART ports, eight SPI ports, four I2C ports, a 14-bit ADC, and four timers. 
There are 84 I/O lines. There are 24 ADC inputs, and the ADC can convert up to 1 
million samples per second.


![Figure 1.19](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/52bb8a68357e632882402844a09af892/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_19_MSP432_MKII.jpg)
*Figure 1.19. I/O port pins for the MSP432 used in this class with the Educational BoosterPack MKII BOOSTXL-EDUMKII. P3.5 means Port 3 pin 5.*

Figure 1.19 shows the port pin connections for the hardware using in this class. 
There are 10 ports (1, 2, 3, … 10). You can see from this figure that many of the 
ports share operation with multiple devices. For example, Port 3 is used for

* UART to Bluetooth
* Temperature sensor input
* LCD output
* Button 2 input

This overlap of features must be managed by the operating system. More information 
about the hardware/software interfaces used in this class will be presented later in section 1.6.

![Figure 1.20](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/501aeb35fabfd4ae32eb885b3df15cd2/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_20_MSP432LaunchPad.jpg)
*Figure 1.20. LaunchPad based on the MSP432P401RIPZ.*

The MSP432 LaunchPad evaluation board (Figure 1.20) is a low-cost development board 
available as part number MSP-EXP432P401R from www.ti.com and from regular electronic 
distributors like Digikey, Mouser, element14, and Avnet. The board includes XDS110-ET, 
an open-source onboard debugger, which allows programming and debugging of the onboard 
MSP432 microcontroller. The USB interface is used by the debugger and includes a serial channel.

--

![Figure 1.21](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/405c87955aad4e665710fa1f5f0eebfd/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_21_LaunchPadCircuit.jpg)
*Figure 1.21. Switch and LED interfaces on the LaunchPad Evaluation Board. The jumpers can be removed so the corresponding pin can be used without connection to the external circuits.*

The MSP432 LaunchPad evaluation board has two switches, one 3-color LED and one red 
LED, as shown in Figure 1.21. The switches are negative logic and will require activation 
of the internal pull-up resistors. In this class we will not use the switches and LEDs on 
the LaunchPad, but rather focus on the hardware provided by the MK-II BoosterPack.


--

The LaunchPad has four 10-pin connectors, labeled as J1 J2 J3 J4 in Figure 1.22, to which 
you can attach your external signals. The top side of these connectors has male pins, and t
he bottom side has female sockets.


![Figure 1.22](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/f4a4ae30843e33df6810580e7239bc86/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_22_LaunchPadPins.jpg)
*Figure 1.22. Interface connectors on the MSP432 LaunchPad Evaluation Board, 67 I/O pins.*


In this class we will use BoosterPacks, so you will not need to connect individual wires 
to the LaunchPad. Figure 1.24 shows the MSP432 with a CC2650 BoosterPack. There are two 
possible CC2650 modules that could be used for Lab 6, BOOSTXL-CC2650MA or the LAUNCHXL-CC2650.

![Figure 1.24](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/5b708c1caacdf92474bda9db8992c7ea/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_24_MSP432_CC2650.JPG)
*Figure 1.24. A MSP432 LaunchPad with a BOOSTXL-CC2650MA BoosterPack.*

--
--

###Assembly Language Programming

![Figure 1.25](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/b475828f2193e978ccb07798fdf38121/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/fig01_25_assemblySyntax.jpg)
*Figure 1.25. Assembly instructions have four fields: labels, opcodes, operands, and comments.*


___Assembly language____ instructions have four fields separated by spaces or tabs as illustrated in Figure 1.25.

___Labels___: The label field is optional and starts in the first column and is used to identify 
the position in memory of the current instruction. You must choose a unique name for each label.

___Opcodes___ or ____pseudo-ops___: The ___opcode--- field specifies which processor command to execute. The twelve op 
codes we will present in this class are ___LDR STR MOV PUSH POP B BL BX ADD SUB CPSID___ and ___CPSIE___. 
_If there is a label there must be at least one space or one tab between the label and the opcode_. 
_If there is no label then there must be at least one space or one tab at the beginning of the line_. 
There are also ___pseudo-ops___ that the assembler uses to control features of the assembly process. 
Examples of pseudo-ops you will encounter in this class are ___AREA EQU IMPORT EXPORT___ and ___ALIGN___. 
An op code generates machine instructions that get executed by the processor at run time, while a pseudo-op 
code generates instructions to the assembler that get interpreted at assembly time.

___Operands___: The operand field specifies where to find the data to execute the instruction. 
Thumb instructions have 0, 1, 2, 3, or more operands, separated by commas.

___Comments___: The comment field is optional and is ignored by the assembler, but allows you to describe 
the software, making it easier to understand. You can add optional spaces between operands in the 
operand field. However, a semicolon must separate the operand and comment fields. Good programmers add 
comments to explain what you are doing, why you are doing it, how it was tested, and how to change 
it in the future. Everything after the semicolon is a comment.

The assembler translates assembly source code into object code, which are the machine instructions executed by the processor. All object code is halfword-aligned. With Thumb-2, instructions can be 16 or 32 bits wide, and the program counter bit 0 will always be 0. The listing is a text file containing a mixture of the object code generated by the assembler together with our original source code.

Address   Object code Label Opcode Operand  comment
0000006A  F1000001    Incr  ADD    R0,R0,#1 ;increment the count
0000006E  4770              BX     LR       ;return

When we build a project all files are assembled or compiled, then linked together. The address values shown in the 
listing are the relative to the particular file being assembled. When the entire project is built, 
the files are linked together, and the linker decides exactly where in memory everything will be. 
After building the project, it can be downloaded, which programs the object code into flash ROM.

|Address|   Object code| Label| Opcode| Operand|  comment|
|-------|--------------|------|-------|--------|---------|
|0000006A|  F1000001   | Incr | ADD   |R0,R0,#1| ;increment the count|
|0000006E|  4770       |      |  BX   |  LR    |  ;return|

In general, the assembler creates for each label an entry in the symbol table that maps the symbolic 
label to the address in memory of that line of code. The exception to this rule is when a label is 
used with the EQU pseudo-op. The result of an EQU pseudo-op is to place an entry in the symbol table 
mapping the symbolic label with the value of the operand.




--
--


###Addressing modes
A fundamental issue in software design is the differentiation between data and addresses. Another name 
for address is pointer. It is in assembly language programming in general and ___addressing modes___ in 
specific that this differentiation becomes clear. When we put the number 1000 into Register R0, 
whether this is data or address depends on how the 1000 is used. The addressing mode is the format 
the instruction uses to specify the memory location to read or write data. We will see five addressing 
modes in this class:

|Address Mode   |Location of Data    |Example       |
|---------------|--------------------|--------------|
|Immediate	         |Data within the instruction	    |MOV  R0,#1|
|Indexed	         |Data pointed to by register	    |LDR  R0,[R1]|
|Indexed with offset |Data pointed to by register	    |LDR  R0,[R1,#4]|
|PC-relative	     |Location is offset relative to PC	|BL   Incr|
|Register-list	     |List of registers	                |PUSH {R4,LR}|


__No addressing mode__: Some instructions operate completely within the processor and require no memory data fetches. 
For example, the ___ADD R1,R2,R3___ instruction performs R2+R3 and stores the sum into R1.

__Immediate addressing mode__: If the data is found in the instruction itself, like ___MOV R0,#1___, the instruction 
uses immediate addressing mode.

__Indexed addressing mode__: A register that contains the address or location of data is called a pointer or index 
register. Indexed addressing mode uses a register pointer to access memory. There are many variations of indexed 
addressing. In this class, you will use two types of indexed addressing. The form [Rx] uses Register ___Rx as a 
pointer___, where Rx is any of the Registers from R0 to R12. The second type you will need is called ___indexed with offset___, 
which has the form [Rx,#n], where n is a number from -255 to 4095. This addressing mode will access memory at Rx+n, 
without modifying Rx.

__PC-relative addressing mode__: The addressing mode that uses the PC as the pointer is called PC-relative addressing mode. It is used for branching, for calling functions, and accessing constant data stored in ROM. The addressing mode is called PC-relative because the machine code contains the address difference between where the program is now and the address to which the program will access.

There are many more addressing modes, but for now, these few addressing modes, as illustrated below, are enough to get us started.

--
--


###List of twelve instructions

We will only need 12 assembly instructions in order to design our own real-time operating system. The following lists the load and store instructions we will need.

```asm
  LDR Rd, [Rn]     ; load 32-bit memory at [Rn] to Rd 
  STR Rt, [Rn]     ; store Rt to 32-bit memory at [Rn] 
  LDR Rd, [Rn, #n] ; load 32-bit memory at [Rn+n] to Rd 
  STR Rt, [Rn, #n] ; store Rt to 32-bit memory at [Rn+n] 
```

Let M be the 32-bit value specified by the 12-bit constant #imm12. When Rd is absent for add and subtract, 
the result is placed back in Rn. The following lists a few more instructions we will need.

```asm
  MOV   Rd, Rn         ;Rd = Rn 
  MOV   Rd, #imm12     ;Rd = M
  ADD   Rd, Rn, Rm     ;Rd = Rn + Rm 
  ADD   Rd, Rn, #imm12 ;Rd = Rn + M
  SUB   Rd, Rn, Rm     ;Rd = Rn - Rm 
  SUB   Rd, Rn, #imm12 ;Rd = Rn - M
  CPSID I              ;disable interrupts, I=1 
  CPSIE I              ;enable interrupts, I=0 
```

Normally the computer executes one instruction after another in a linear fashion. In particular, the next 
instruction to execute is typically found immediately following the current instruction. We use branch 
instructions to deviate from this straight line path. These branches use PC-relative addressing.


```asm
  B  label ;branch to label 
  BX Rm    ;branch indirect to location specified by Rm
  BL label ;branch to subroutine at label 
```

These are the push and pop instructions we will need

```asm
  PUSH {Rn,Rm} ; push Rn and Rm onto the stack 
  PUSH {Rn-Rm} ; push all registers from Rn to Rm onto the stack 
  POP  {Rn,Rm} ; pop two 32-bit numbers off stack into Rn, Rm 
  POP  {Rn-Rm} ; pop multiple 32-bit numbers off stack to Rn through Rm 
```

When pushing and popping multiple registers, it does not matter the order specified in the instruction. 
Rather, the registers are stored in memory such that the register with the smaller number is stored at 
the address with a smaller value. For example, consider the execution of PUSH {R1,R4-R6}. Assume the 
registers R1, R4, R5, and R6 initially contain the values 1, 4, 5, and 6 respectively. Figure 1.26 
shows the value from lowest-numbered R1 is positioned at the lowest stack address. If four entries 
are popped with the POP {R0,R2,R7,R9} instruction, the value from the lowest stack address is loaded 
into the lowest-numbered R0.

![Figure 1.26](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/24c32219b93b8b5de35e388640fa20ad/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_26_stack.jpg)
*Figure 1.26. Stack drawings showing how multiple registered are pushed and popped.*


--
--


###Accessing memory

One of the basic operations we must perform is reading and writing global variables. Since all calculations are performed 
in registers, we must first bring the value into a register, modify the register value, and then store the new value back 
into memory. Consider a simple operation of incrementing a global variable in both C and assembly language. Variables
can exist anywhere in RAM, however for this illustration assume the variable count is located in memory at 0x20000100. 
* The first LDR instruction gets a pointer to the variable in R0 as illustrated in Figure 1.27. This means R0 will have the 
value 0x20000100. This value is a pointer to the variable count. The way it actually works is the assembler places a 
constant 0x20000100 in code space and translates the =count into the correct PC-relative access to the constant 
(e.g., LDR R0,[PC,#28]). 
* The second LDR dereferences the pointer to fetch the value of the variable into R1. 
More specifically, the second LDR will read the 32-bit contents at 0x20000100 and put it in R1. 
* The ADD instruction  increments the value,
* and the STR instruction writes the new value back into the global variable. More specifically, 
the STR instruction will store the 32-bit value from R1 into at memory at 0x20000100. The following assembly implements 
the C code count = count+1;

```asm
  LDR R0,=count
  LDR R1,[R0] ;value of count
  ADD R1,#1
  STR R1,[R0] ;store new value
```
![Figure 1.27](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/fc0ed534c07c91edb33bc7b2fd506839/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_27_IndexedAddressing.jpg)
*Figure 1.27. Indexed addressing using R0 as a register pointer to access memory. Data is moved into R1. Code space is where we place programs, and data space is where we place variables. The dotted arrows in this figure represent the motion of information, and the solid arrow is a pointer.*

Let's work through code similar to what we will use in Chapter 2 as part of our operating system. The above 
example used ___indexed addressing___ with an implicit offset of 0. However, you will also need to understand 
___indexed addressing with an explicit offset___. In this example, assume ___RunPt___ points to a linked list as 
shown in Figure 1.28. A node of the list is a structure (struct in C) with multiple entries of different types. 
A linked list is a set of nodes where one of the entries of the node is a pointer or link to another node of the 
same type. In this example, the second entry of the list is a pointer to the next node in the list. Figure 1.28 
shows three of many nodes that are strung together in a sequence defined by their pointers.

![Figure 1.28](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/b0a776b3ea8a5ce960a2c77992c189fa/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_28_ListedListIndexedAddressing.jpg)
Figure 1.28. A linked list where the second entry is a pointer to the next node. Arrows are pointers or links, and dotted lines are used to label components in the figure

As our operating system runs it will need to traverse the list. RunPt will always points to a node in the list. 
However, we may wish to change it to point to the next node in the list. In C, we would execute RunPt=RunPt->next; 
However, in assembly this translates to

```asm
   LDR  R1,=RunPt   ; R1 points to variable RunPt, using PC-relative
   LDR  R0,[R1]     ; R0= value of variable RunPt
   LDR  R2,[R0,#4]  ; next entry
   STR  R2,[R1]     ; update RunPt
```

Figure 1.29 draws the action caused by above the four instructions. Assume initially RunPt points to the middle node of the list. 
Each entry of the node is 32 bits or four bytes of memory. The first two instructions read the value of RunPt into R0. 
Since RunPt points to the middle node in the linked list in this figure, R0 will also point to this node. Since each entry is 
4 bytes, R0+4 points to the second entry, which is the next pointer. The instruction LDR R2,[R0,#4] will read the 32-bit value 
pointed to by R0+4 and place it in R2. Even though the memory address is calculated as R0+4, the Register R0 itself is not 
modified by this instruction. R2 now points to the right-most node in the list. The last instruction updates RunPt so it now 
points to the right-most node shown in the Figure 1.29.

![Figure 1.29](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/d0cd2038ff347e0d1a22acc8bc9125ca/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_29_IndexedAddressing.jpg)
*Figure 1.29. An example of indexed addressing mode with offset, data is in memory. Arrows in this figure represent pointers (not the motion of information).*

___A really important concept___. We use the LDR instruction to load data from RAM to a register and the STR instruction to store 
data from a register to RAM. In real life, when we move a box to the basement, push a broom across the floor, load bags into the 
trunk, store spoons in a drawer, pop a candy into your mouth, or transfer employees to a new location, there is a physical object 
and the action changes the location of that object. Assembly language uses these same verbs, but the action will be different. In 
most cases, the processor creates a copy of the data and places the copy at the new location. In other words, since the original 
data still exists in the previous location, there are now two copies of the information. The exception to this 
memory-access-creates-two-copies-rule is a stack pop. When we pop data from the stack, it no longer exists on the stack leaving us 
just one copy. Having the information in two places will create a very tricky problem that our operating system must handle.

Let’s revisit the simple example of incrementing a global variable. In C, the code would be count=count+1; In assembly, the compiler 
creates code like this:

```asm
   LDR R0,=count
   LDR R1,[R0]    ;value of count
;two copies of count: in memory and in R1
   ADD R1,#1
;two copies of count with different values
   STR R1,[R0]    ;store new value
```

The instruction ___LDR R1,[R0]___ loads the contents of the variable count into R1. At this point, there are two copies of the data, 
the original in RAM and the copy in R1. After the ADD instruction, the two copies have different values. When designing an 
operating system, we will take special care to handle shared information stored in global RAM, making sure we access the 
proper copy. In Chapter 2, we will discuss in detail the concept of ___race conditions___ and ___critical sections___. 
These very important problems arise from the problem generated by this concept of having multiple copies of information.

--
--

###Functions


Subroutines, procedures, and functions are programs that can be called to perform specific tasks. They are important 
conceptual tools because they allow us to develop modular software. The programming languages Pascal, Fortran, and Ada 
distinguish between functions, which return values, and procedures, which do not. On the other hand, the programming 
languages C, C++, Java, and Lisp do not make this distinction and treat functions and procedures as synonymous. 
Object-oriented programming languages use the term method to describe functions that are part of classes; Objects 
being instantiation of classes. In assembly language, we use the term subroutine for all subprograms whether or not 
they return a value. Modular programming allows us to build complex systems using simple components. In this section 
we present a short introduction on the syntax for defining assembly subroutines. We define a subroutine by giving it a 
name in the label field, followed by instructions, which when executed, perform the desired effect. The last instruction 
in a subroutine will be BX LR, which we use to return from the subroutine.

The function in Program 1.1 and Figure 1.30 will increment the global variable count. The AREA DATA directive specifies 
the following lines are placed in data space (typically RAM). The SPACE 4 pseudo-op allocates 4 uninitialized bytes. 
The AREA CODE directive specifies the following lines are placed in code space (typically ROM). The |.text| connects 
this program to the C code generated by the compiler. ALIGN=2 will force the machine code to be halfword-aligned as required.

In assembly language, we will use the BL instruction to call this subroutine. At run time, the BL instruction will save the 
return address in the LR register. The return address is the location of the instruction immediately after the BL instruction. 
At the end of the subroutine, the BX LR instruction will get the return address from the LR register, returning the program 
to the place from which the subroutine was called. More precisely, it returns to the instruction immediately after the 
instruction that performed the subroutine call. The comments specify the order of execution. The while-loop causes instructions 
4–10 to be repeated over and over.

![Figure 1.30](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/44b2064dee8cdd827b5a0288cd9a4a0c/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig01_30_functionFlowchart.jpg)
*Figure 1.30. A flowchart of a simple function that adds 1 to a global variable.*

```asm
      AREA DATA
count SPACE 4        ; 32-bit data
      AREA |.text|,CODE,READONLY,ALIGN=2
function 
      LDR R0,=count  ;5
      LDR R1,[R0]    ;6 value of count
      ADD R1,#1      ;7
      STR R1,[R0]    ;8 store new value
      BX LR ;9
Start LDR R0,=count  ;1
      MOV R1,#0      ;2
      STR R1,[R0]    ;3 store new value
loop  BL function    ;4
      B loop         ;10
```

```c
uint32_t count

void function(void){ 
  count++; // 5,6,7,8
}          // 9


int main(void){
  count = 0;   // 1,2,3
  while(1){
    function(); // 4
  }             // 10
}
```
*Program 1.1. Assembly and C versions that initialize a global array of ten elements. The numbers illustrate the execution sequence.*

While using a register (LR) to store the return address is very effective, it does pose a problem if one 
function were to call a second function. In Program 1.2 someother calls function. Because the return address 
is saved in the LR, if one function calls another function it must save the LR before calling and restore 
the LR after the call. In Program 1.2, the saving and restoring is performed by the PUSH and POP instructions.


```asm
function
; ....... 
; ....... 
    BX LR 
someother
; .......
    PUSH {R4,LR}
    BL   function
    POP  {R4,LR}
; ....... 
    BX LR
```

```c
void function(void){ 
  // .......
  // .......
}
void someother(void){
  // .......
  function();
  // .......
}
```

*Program 1.2. Assembly and C versions that define a simple function.*  
  
--
--


###Arm Architecture Procedure Call Standart (AAPCS)
C <--> Assembly
This is the way control can be transfer from C to Assembly or viceversa.

###AAPCS General Rules
1. Inputs will use R0, R1, R2, R3
2. Output will use R0
3. R0-R3 and R12 any function ir free to use and destroy these registers
4. R4-R11 must be preserve (by pushing them on the Stack, and then popping them off at the end)
5. Stack aligment of 64-bits, push something, we will push (or pop) a even number of register


The ___ARM Architecture Procedure Call Standard___, ___AAPCS___, part of the ___ARM Application Binary Interface (ABI)___, uses 
registers ___R0___, ___R1___, ___R2___, and ___R3___ to pass input parameters into a C function. Functions must preserve the 
values of registers ___R4–R11___. Also according to AAPCS we place the return parameter in Register ___R0___. AAPCS requires 
we ___push and pop an even number of registers___ to maintain an 8-byte alignment on the stack. In this book, the SP will 
always be the ___Main Stack Pointer___ (___MSP___), not the ___Process Stack Pointer___ (___PSP___). Recall that all object 
code is ___halfword aligned___, meaning bit 0 of the PC is always clear. When the ___BL___ instruction is executed, bits 31–1 
of register LR are loaded with the address of the instruction after the BL, and bit 0 is set to one. When the ___BX LR___ 
instruction is executed, bits 31–1 of register LR are put back into the PC, and bit 0 of LR goes into the ___T___ bit. On the ARM 
Cortex-M processor, the ___T___ bit should always be 1, meaning the processor is always in the Thumb state. Normally, the 
proper value of bit 0 is assigned automatically.

ARM’s ___Cortex Microcontroller Software Interface Standard___ (___CMSIS___) is a standardized hardware abstraction layer for 
the Cortex-M processor series. The purpose of the CMSIS initiative is to standardize a fragmented industry on one superior 
hardware and software microcontroller architecture.

The ___CMSIS___ enables consistent and simple software interfaces to the processor and core MCU peripherals for silicon vendors 
and middleware providers, simplifying software re-use, reducing the learning curve for new microcontroller developers, and 
reducing the time to market for new devices. Learn more about CMSIS directly from ARM at www.onarm.com.

The CMSIS is defined in close cooperation with various silicon and software vendors and provides a common approach to interface 
to peripherals, real-time operating systems, and middleware components. The CMSIS is intended to enable the combination of 
software components from multiple middleware vendors. The CMSIS components are:

* ___CMSIS-CORE___: API for the Cortex-M processor core and peripherals. It provides at standardized interface for Cortex-M0, 
Cortex-M3, Cortex-M4, SC000, and SC300. Included are also SIMD intrinsic functions for Cortex-M4 SIMD instructions.

* ___CMSIS-DSP___: DSP Library Collection with over 60 Functions for various data types: fixed-point (fractional q7, q15, q31) 
and single precision floating-point (32-bit). The library is available for Cortex-M0, Cortex-M3, and Cortex-M4. 
The Cortex-M4 implementation is optimized for the SIMD instruction set.

* ___CMSIS-RTOS API___: Common API for Real-Time operating systems. It provides a standardized programming interface that is 
portable to many RTOS and enables software templates, middleware, libraries, and other components that can work across 
supported RTOS systems.

* ___CMSIS-SVD___: System View Description for Peripherals. Describes the peripherals of a device in an XML file and can be 
used to create peripheral awareness in debuggers or header files with peripheral register and interrupt definitions.


What is the purpose of AAPCS?
Standards allows software written by one company to work properly with software written by another company. A similar concept 
is CMSIS, which allows the standardization of I/O functions, see http://www.keil.com/pack/doc/CMSIS/General/html/index.html

  
###Board Support Package

The purpose of using a BoosterPack in this course is to provide a rich set of input/output while at the same time allowing students to focus on the writing of the operating system. This way all students have the same hardware configuration as each other, and more importantly the automated lab graders are programed to understand this hardware. Figure 1.31 shows the lab kit, which comprises of a LaunchPad and the MKII BoosterPack. Later in Chapter 6 we will add a second BoosterPack to provide Bluetooth communication.

![Figure 1.31](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/3b959d4b64a236bfbaae81981d6d8e65/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/fig01_31_kit.JPG)
Figure 1.31. The lab hardware includes a LaunchPad, either a TM4C123 (EK-TM4C123GXL) or an MSP432 (MSP-EXP432P401R), together with an Educational BoosterPack MKII (BOOSTXL-EDUMKII).

The MKII provides a number of sensors that necessitate real-time processing: microphone for sound, temperature, 3-axis acceleration, 
and a light sensor. Furthermore, it has some I/O devices to interact with a human. For input it has buttons and a joystick, and for 
output it has LEDs, a buzzer and a color LCD display.

###Showing the code

This course deals with creating a real-time operating system for embedded systems. One of the important resources the OS must manage 
is I/O. It is good design practice to provide an abstraction for the I/O layer. Three equivalent names for this abstraction are ___Hardware 
Abstraction Layer (HAL)___, device driver, and ____Board Support Package (BSP)___. From an operating system perspective, the goal is the make it 
easier to port the system from one hardware platform to another. The system becomes more portable if we create a BSP for our hardware 
devices. We provide a BSP for the MKII BoosterPack that encapsulates the following:

* Button input
* Joystick input
* LED output
* Buzzer output
* Acceleration input
* Microphone input
* LCD graphics output
* Light sensor input
* Temperature sensor input
* The processor clock

One of the advantages of this BSP is the operating systems we create together in class will actually run on either the MSP432 or the TM4C123. 
This class provides just enough information to understand what the I/O does, so that you can focus on the operating system.

If you want to understand how the I/O devices work, you should print its circuit diagram on pages 17 and 18 of the MKII Users Guide 
<a href="https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/ffcfd13a81c05c9264813d1078aadf59/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/MK-II_CircuitDiagram.pdf" target="_blank">MK-II_CircuitDiagram.pdf</a>.
You can also review the <a href="https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/3d969619c88dfd53e5f9dce06facb83a/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/MK-II_usersGuide.pdf" target="_blank">MK-II_usersGuide.pdf</a>.



--
--

###Buttons

![Buttons](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/d1776f64af5dae05d154948f5da07acf/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/BSP_buttons.jpg)

The initialization functions configure the I/O port for the two buttons [MJTP1212A_button.pdf](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/f3574f5125caa28b81558df5752fe934/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/MJTP1212A_button.pdf). 
The input functions return the current status of the buttons. For information on how to use the functions, 
see the ___BSP.h___ file and look for functions that begin with BSP_Button. For information on how 
the interface operates, see the ___BSP.c___ file and the data sheet for your microcontroller.


###Joystick

![Joystick](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/16d604cbeba9205efda33ec595333241/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/BSP_joystick.jpg)

The joystick has two potentiometers and a momentary switch [IM130330001Joystick.pdf](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/561a4d7f2bcec8e839948d4f75da2d5c/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/IM130330001Joystick.pdf). One analog input is a
function of the X-position of the joystick and another analog input is a function of the Y-position. 
The microcontroller uses its analog to digital converter (ADC) to measure the joystick position. The 
initialization functions configure the I/O ports for the joystick. The input functions return the current 
status of the joystick. For information on how to use the functions, see the BSP.h file and look for functions 
that begin with BSP_Joystick. For information on how the interface operates, see the BSP.c file and 
the data sheet for the joystick and for your microcontroller.
