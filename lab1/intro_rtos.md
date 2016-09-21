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



|Part |number	|RAM	|Flash	|I/O	|I/O modules
|----|----|----|----|----|----|
|MSP432P401RIPZ	|64	|256	|84	|floating point, DMA|
|LM4F120H5QR	|32	|256	|43	|floating point, CAN, DMA, USB|
|TM4C123GH6PM	|32	|256	|43	floating point, CAN, DMA, USB, PWM|
|STM32F051R8T6	|8	|64	|55	|DAC, Touch sensor, DMA, I2S, HDMI, PWM|
|MKE02Z64VQH2	|4	|64v	|53	|PWM|
||KiB|	KiB|	pins||	
Table 1.2. Memory and I/O modules (all have SysTick, RTC, timers, UART, I2C, SSI, and ADC).













