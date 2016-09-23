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







