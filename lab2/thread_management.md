###Objectives

* Definition of RTOS
* Interrupts, SysTick, and critical sections
* Threads and a robin preemptive scheduler
* Spin-lock semaphores
* Thread synchronization and communication

A real time operating system is an operating system that has to adhere to some strict performance criteria.
And at the heart, an operating system is a manager of resources. And to do so, we're going to need ___Interrupts___.
So we'll talk about interrupts in general, and in specific, we'll look at ___SysTick interrupts___.
And once we have two threads that are running with interrupts, we have a problem called critical sections.

So if this thread is running, accessing a global variable, and this thread is running as well, and accessing that same global variable,
we have a potential consistency problem called a ___critical section___.And so what we're going to do is manage that critical section here
in your operating system.

And so what we have now is the basic concept of a context for execution. So that's a ___thread___.
So now that we have these threads, one of the fundamental tasks that an operating system performs is how to ___schedule___ them.
That is, how to order their execution. So, at any point in time, ___only one thread runs___. But we switch from one thread to another using the scheduler.

So the scheduler will run a thread for a certain amount of time, an interrupt occurs, it switches to another thread, and then it switches to another thread.
And it does cycle through these threads in a round robin fashion. So our first scheduler will be a basic round robin, preemptive scheduler.

throughout the class, we're going to use semaphores, which is a way to synchronize.
But here in Chapter 2, we're going to start with the simplest embodiment of a semaphore called a ___spinlock semaphore___.
So if a resource is unavailable, what a spinlock semaphore will do is take that thread and make it spin, or wait, until that resource is available.

There are a lot of problems that we solve using spinlock semaphores in general. But we'll identify them into two classes.

One is called the ___classical mutual exclusion problem___, where we want to make sure that multiple threads that are accessing a 
resource can access it in a mutually exclusive way.

The second category we will classify these problems into is called ___synchronization problem___.
The synchronization problem is basically trying to establish an order of execution between the threads.


###2.1.1. Motivation

*Introduction to threads*
![Introduction to threads](https://youtu.be/JSFkUR94uLg)

Consider a system with one input task, one output tasks and two non I/O tasks, as shown in Figure 2.1. The non-I/O tasks are called 
function3 and function4. Here are two possible ways of structuring a solution to the problem. The left side of the figure shows a 
___busy-wait solution___, where a single main program runs through the tasks by checking to see if the conditions for running the 
task have occurred. Busy-wait solution is appropriate for problems where the execution patterns for tasks are fixed and well-known, 
and the tasks are tightly coupled. An alternative to busy-wait is to assign ___one thread per task___. Interrupt synchronization is 
appropriate for I/O even if the execution pattern for I/O is unknown or can dynamically change at run time. The difficultly with the 
single-foreground multiple-background threaded solutions developed without an operating system stems from answering, “how to handle 
complex systems with multiple foreground tasks that are loosely coupled?” A real-time operating system (RTOS) with a ___thread 
scheduler___ allows us to run multiple foreground threads, as shown on the right side of the figure. As a programmer we simply write 
multiple programs that all “look” like main programs. Once we have an operating system, we write Task1, Task2, Task3, and Task4 such 
that each behaves like a main program. One of the features implemented in an RTOS is a thread scheduler, which will run all threads 
in a manner that satisfies the constraints of the system.

![Figure 2.1](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/4070c0097f36a84d278a3c51cf521da1/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_01_MultipleInputOutputFlowcharts.jpg)
Figure 2.1. Flowcharts of a system with four loosely coupled tasks.

--
--


###2.1.2. Introduction to Threads

####Threads
![Threads](https://youtu.be/9IY1iVy_uk4)

A Threads is context of execution. So in practice, we have a program.
A program is a static entity. It doesn't have a life. It's just a piece of code.
What happens when a program is in a state of execution is it has other features associated with it.
So the code when it's executed, it becomes a thread. So a thread is not just a static part, which
is the program, which is the code of it, but also the dynamic part.

So every thread inside it has its code, its set of registers, and its own stack.

So what we know is we still have a single CPU. So the single CPU is going to run one thread at a time.
So the context of the CPU, will be the elements of the thread, the registers, the stack pointer, etc.

They can't talk to each other, because threads have their own context. If we want them to talk to each other,
we have to create a mechanism by which they communicate.
The only way to do that is either communicate using some I/O ports, or they communicate using global data.

Now, this global data can be either just variables, or they can be structured (FIFOs, queues, etc)

###Thread Classification by Periodicity
* ___Periodic___: a periodic threat the one that is going to arrive at some point in time,
it runs for some amount of time, and then it repeats itself. 
* ___Aperiodic___: an aperiodic tasks does don't recur at a predictable rate (user press a button). 
We expect them to happen, just can't predict when they do happen.
* ___Sporadic___: These are typically exceptional events, means that we don't expect them to run, 
but if they do run, they're very important, and they have to be treated with care, as in they have 
to be executed immediately. Some examples include __temperature overflow__, or __pressure
too large__, or the __supply voltage getting too low__. These are things you don't want to have happen, 
but if they do happen, you gotta take care of them right away.

###Thread Classification by their Triggers
(What triggers the thread to run?)
* ___Event-triggered threads___: we call them ___event threads___ for short. This are threads that are
executed in the presence of a particular event. The event could be an input, an output, a time, another 
thread, etc.
* ___main threads___: that need to run, but they don't have any requirements, we call them 
* ___cycle-stealing threads___.

A ___program___ is a sequence of software commands connected together to affect a desired outcome. 
Programs perform input, make decisions, record information, and generate outputs. Programmers generate 
software using an editor with a keyboard and display. Programs are compiled and downloaded into the 
flash ROM of our microcontroller. Programs themselves are static and lifeless entities. However, when 
we apply power to the microcontroller, the processor executes the machine code of the programs in the ROM. 

A ___thread___ is defined as either execution itself or the action caused by the execution. Either way 
we see that threads are dynamic, and thus it is threads that breathe life into our systems. A thread 
therefore is a program in action, accordingly, in addition to the program (instructions) to execute it 
also has the state of the program. The ___thread state___ is captured by the current contents of the 
registers and the local variables, both of which are stored on the thread’s stack. For example, Figure 2.2 
shows a system with four programs. We define Thread1 as the execution of Task1. Another name for thread 
is light-weight process. Multiple threads typically cooperate to implement the desired functionality of 
the system. We could use hardware-triggered interrupts to create multiple threads. However, in this class 
the RTOS will create the multiple threads that make up our system. Figure 2.2 shows the threads having 
separate programs. All threads do have a program to execute, but it is acceptable for multiple threads 
to run the same program. Since each thread has a separate stack, its local variables are private, which 
means it alone has access to its own local variables.

![Figure 2.2](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/ffade197c76276d93a846b1759d9c923/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_02_Threads.jpg)
*Figure 2.2. Each thread has its own registers and stack.*

It looks like in Figure 2.2 that threads have physically separate registers. The stacks will be physically 
separate, but in reality there is just one set of registers that is switched between the threads as the 
thread scheduler operates. The thread switcher will suspend one thread by pushing all the registers on its 
stack, saving the SP, changing the SP to point to the stack of the next thread to run, then pulling all 
the registers off the new stack. Since threads interact for a common goal, they do share resources such 
as global memory, and I/O devices (Figure 2.3). However, to reduce complexity it is the best to limit the 
amount of sharing. It is better to use a well-controlled means to pass data and synchronize threads.

![Figure 2.3](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/404a003329819c7486bc5bfd788fc921/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_03_Threads.jpg)
Figure 2.3. Threads share global memory and I/O ports.

Some simple examples of multiple threads are the interrupt-driven I/O. In each of these examples, the background 
thread (interrupt service routine) executes when the I/O device is done performing the required I/O operation. A 
single foreground thread (main program) executes during the times when no interrupts are needed. A global data 
structure is used to communicate between threads. Notice that data stored on the stack or in registers by one 
thread are not accessible by another thread.

####CHECKPOINT 2.1

*What is the difference between a program and a thread?*
A program is a list of commands, while a thread is the action cause by the execution of software. For example, 
there might be one copy of a program that searches the card catalog of a library, while separate threads are 
created for each user that logs into a terminal to perform a search. Similarly, there might be one set of programs 
that implement the features of a window (open, minimize, maximize, etc.), while there will be a separate thread for 
each window created.

###CHECKPOINT 2.8
*Why can't threads pass parameters to each other on the stack like regular functions do? How do threads communicate 
with each other?*
Threads can't communicate with each other using the stack, because they have physically separate stacks. Global 
variables will be used, because one thread can write to the global, and another can read from it.


###2.1.3. States of a main thread

*States*
!(States)[https://youtu.be/iep01EUwbqo]

A main thread can be in one of four states, as shown in Figure 2.4. The arrows in Figure 2.4 describe the condition causing the thread to change states. 
In Chapter 2, threads oscillate between the active and run states. In Chapter 2, we will create all main threads at initialization and these main threads 
will never block, sleep, or die.

A main thread is in the ___run state___ if it currently executing. On a microcontroller with a single processor like the Cortex M, there can be at most one 
thread running at a time. As computational requirements for an embedded system rise, we can expect microcontrollers in the future to have multicore processors, 
like the ones seen now in our desktop PC. For a multicore processor, there can be multiple threads in the run state.

A main thread is in the ___active state___ if it ready to run but waiting for its turn. In Lab 2, we will implement four threads that are either running or active.

Sometimes a main thread needs to wait for a fixed amount of time. The OS will not run a main thread if it is in the ___sleep state___. After the prescribed amount of 
time, the OS will make the thread active again. Sleeping would be used for tasks that are not real-time. Sleeping will be presented in Chapter 3.

A main thread is in the ___blocked state___ when it is waiting for some external event like input/output (keyboard input available, printer ready, I/O device available.) 
We will implement blocking in the next chapter.


![Figure 2.4](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/ff62a206f8abebbcda4cefb9c507f6cd/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_04_Threads.jpg)
*Figure 2.4. A main thread can be in one of four states.*

The OS manages the execution of threads. An important situation to manage is when a thread is stuck and cannot make progress. For example, a thread may 
need data from another thread, a thread may be waiting on I/O, or a thread may need to wait for a specified amount of time. In Lab 3, when a thread is 
waiting because it cannot make progress it will ___block___, meaning it will not run until the time at which it can make progress. Similarly, in Lab 3 when a 
thread needs to wait for a prescribed amount of time, it will ___sleep___, meaning it will not run until the elapsed wait time has passed. Blocking and sleeping 
will free up the processor to perform actual work. In Lab 2 main threads will not block or sleep, but more simply we will ___spin___ if a thread is waiting on 
an event. A thread that is spinning remains in the active state, and wastes its entire time slice checking the condition over and over.


--
--

###2.1.4. Real-time systems

*Real-time systems*
![Real-time systems](https://youtu.be/yc1rMDKZrec)

Designing a RTOS requires many decisions to be made. Therefore, it is important to have performance criteria with which to evaluate one alternative to another. 
A common performance criterion used in Real-Time Systems is ___Deadline___, a timing constraint with many definitions in the literature. In this class we will 
define specific timing constraints that apply to design of embedded systems. ___Bandwidth___ is defined as the information rate. It specifies the amount of 
actual data per unit time that are input, processed, or output.

In a real-time system operations performed must meet logical correctness and also be completed on time (i.e., meet timing constraints). Non real-time systems 
require logical correctness but have no timing requirements. The tolerance of a real-time system towards failure to meet the timing requirements determines 
whether we classify it as hard real time, firm real time, or soft real time. If missing a timing constraint is unacceptable, we call it a ___hard real-time system___. 
In a ___firm real-time system___ the value of an operation completed past its timing constraint is considered zero but not harmful. In a ___soft real-time system___ 
the value of an operation diminishes the further it completes after the timing constraint.

* ___Hard real time___: For example, if the pressure inside a module in a chemical plant rises above a threshold, failure to respond through an automated 
corrective operation of opening a pressure valve within a timing constraint can be catastrophic. The system managing the operations in such a scenario is 
a hard real-time operating system.

* ___Firm real time___: An example of a firm real-time system is a streaming multimedia communication system where failure to render one video frame on 
time in a 30 frames per second stream can be perceived as a loss of quality but does not affect the user experience significantly.

* ___Soft real time___: An example of a soft real-time system is an automated stock trading system where excessive delay in formulating an automated response 
to buy/sell may diminish the monetary value one can gain from the trade. The delivery of email is usually soft real time, because the value of the information 
reduces the longer it takes.

Please understand that the world may not reach consensus of the definitions of hard, firm and soft. Rather than classify names to the real-time system, think 
of this issue is as a continuum. There is a continuous progression of the consequence of missing a deadline: 
* catastrophic (hard)
* zero effect and no harm (firm) 
* still some good can come from finishing after deadline (soft). 

Similarly: there is a continuous progression for the value of missing a deadline: negative value (hard), zero value (firm) and some but diminishing positive 
value (soft).

To better understand real-time systems, ___timing constraints___ can be classified into two types. The first type is ___event-response___. The event is a software 
or hardware trigger that signifies something important has occurred and must be handled. The response is the system’s reaction to that event. Examples of 
event-response tasks include:

* Operator pushes a button ->    Software performs action
* Temperature is too hot      ->    Turn on cooling fan
* Supply voltage is too low   ->    Activate back up battery
* Input device has new data ->    Read and process input data
* Output device is idle           ->    Perform another output

The specific timing constraint for this type of system is called latency, which is the time between the event and the completion of the response. Let Ei be the 
times that events occur in our system, and Ti be the times these events are serviced. Latency is defined as

```
Δi = Ti – Ei for i = 0, 1, 2, …, n-1
```

where n is the number of measurements collected. The timing constraint is the maximum value for latency, Δi, that is acceptable. In most cases, the system will 
not be able to anticipate the event, so latency for this type of system will always be positive.

A second type of timing constraint occurs with prescheduled tasks. For example, we could schedule a task to run periodically. If we define fs as the desired 
frequency of a periodic task, then the desired period is Δt=1/fs. Examples of prescheduled tasks include:

* Every 30 seconds                  ->    Software checks for smoke
* At 22 kHz                                ->    Output new data to DACs creating sound
* At 1 week, 1 month, 1 year ->     Perform system maintenance
* At 300 Hz                                ->    Input new data from ADC measuring EKG
* At 6 months of service         ->    Deactivate system because it is at end of life

For periodic, the desired time to run the i'th periodic instance of the task is given as
```
    Di = T0 +i*Δt for i = 0, 1, 2, …, n-1
```

where T0 is the starting time for the system. For prescheduled tasks, we define jitter as the difference between desired time a task is supposed 
to run and the actual time it is run. Let Ti be the actual times the task is run, so in this case jitter is

```
     δti = Ti – Di for i = 0, 1, 2, …, n-1
```

Notice for prescheduled tasks the jitter can be positive (late) or negative (early). For some situations running the task early is acceptable but 
being late is unacceptable. If I have the newspaper delivered to my door each morning, I do not care how early the paper comes, as long as it 
arrives before I wake up. In this case, the timing constraint is the maximum value for jitter δti that is acceptable. On the other hand, for 
some situations, it is unacceptable to be early and it is acceptable to be late. For example, with tasks involving DACs and ADCs, as shown in 
Figure 2.5, we can correlate voltage error in the signal to time jitter. If dV/dt is the slew rate (slope) of the voltage signal, then the voltage 
error (noise) caused by jitter is

```
     δVi = δti * dV/dt for i = 0, 1, 2, …, n-1
```

The error occurs because we typically store sampled data in a simple array and assume it was sampled at fs = 1/Δt. In other words, we do not 
record exactly when the sample was actually performed.

![Figure 2.5](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/583b288b74dc87d429f3dc92bd91bba2/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_05_samplingJitter.jpg)
*Figure 2.5. Effect of jitter on sampled data. True input is a sinusoidal. Blue lines depict when the voltage should be sampled. Red lines depict when the voltage was actually be sampled. There is time jitter such that every other sample is early and every other sample is late. In the zoomed in portion this sample is late; the consequence of being late is the actual sampled data is lowered than the correct value. Sampling jitter causes noise in the data.*

For cases where the starting time, T0, does not matter, we can simplify the analysis by looking at time differences between when the task is 
`run, ΔTi = (Ti – Ti-1)`. In this case, jitter is simply

```
     δti = ΔTi - Δt for i = 0, 1, 2, …, n-1
```

We will classify a system with periodic tasks as real-time if the jitter is always less than a small but acceptable value. In other words the 
software task always meets its timing constraint. More specifically, we must be able to place an upper bound, k, on the time jitter.

```
     -k ≤ δti ≤ +k for all i
```

Previously in Lab 1, we defined a similar performance metric as

```
    Min = minimum δti for all measurements i
    Max = maximum δti for all measurements i
    Jitter = Max - Min = (maximum δti – minimum δti)
```

In most situations, the time jitter will be dominated by the time the microcontroller runs with interrupts disabled. For lower priority 
interrupts, it is also affected by the length and frequency of higher priority interrupt requests.

To further clarify this situation we must clearly identify the times at which the Ti measurements are collected. We could define 
this time as when the task is started or when the task is completed. When sampling an ADC the important time is when the ADC 
sampling is started. More specifically, it is the time the ADC sample/hold module is changed from sample to hold mode. This is 
because the ADC captures or latches the analog input at the moment the sample/hold is set to hold. For tasks with a DAC, the 
important time is when the DAC is updated. More specifically, it is the time the DAC is told to update its output voltage.


###CHECKPOINT 2.3

Consider a task that inputs data from the serial port. When new data arrives the serial port triggers an event. When the software 
services that event, it reads and processes the new data. The serial port has a hardware to store incoming data (2 on the MSP432, 
16 on the TM4C123) such that if the buffer is full and more data arrives, the new data is lost. Is this system hard, firm, or 
soft real time?

It is hard real time because if the response is late, data may be lost.


###CHECKPOINT 2.4

Consider a hearing aid that inputs sounds from a microphone, manipulates the sound data, and then outputs the data to a speaker. 
The system usually has small and bounded jitter, but occasionally other tasks in the hearing aid cause some data to be late, 
causing a noise pulse on the speaker. Is this system hard, firm or soft real time?

It is firm real time because it causes an error that can be perceived but the effect is harmless and does not significantly 
alter the quality of the experience.

###CHECKPOINT 2.5

Consider a task that outputs data to a printer. When the printer is idle the printer triggers an event. When the software 
services that event, it sends more data to the printer. Is this system hard, firm or soft real time?
Hide Answer
It is soft real time because the faster it responses the better, but the value of the system (bandwidth is amount of data 
printed per second) diminishes with latency.

