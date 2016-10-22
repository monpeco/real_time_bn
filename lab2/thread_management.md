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
[Introduction to threads](https://youtu.be/JSFkUR94uLg)

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
[Threads](https://youtu.be/9IY1iVy_uk4)

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
(States)[https://youtu.be/iep01EUwbqo]

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
[Real-time systems](https://youtu.be/yc1rMDKZrec)

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

--
--
#2.1.5. Producer/Consumer problem

[Producer/consumer](https://youtu.be/fClPWeY1Q84)

One of the classic problems our operating system must handle is communication between threads. We define a ___producer___ thread as one 
that creates or produces data. A ___consumer___ thread is a thread that consumes (and removes) data. The communication mechanism we will 
use in this chapter is a mailbox (Figure 2.6). The mailbox has a Data field and a Status field. Mailboxes will be statically 
allocated global structures. Because they are global variables, it means they will exist permanently and can be carefully shared 
by more than one task. The advantage of using a structure like a mailbox for a data flow problem is that we can decouple the 
producer and consumer threads. In chapter 3, we will replace the mailbox with a first in first out (FIFO) queue. The use of a 
FIFO can significantly improve system performance.

![Figure 2.6](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/40fb30d8a2eea9f26202641a259dc6cc/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_06_MailBox.jpg)
*Figure 2.6. The mailbox is used to send data from the producer thread to the consumer thread.*


There are many producer/consumer applications in the field of embedded systems. In Table 2.1 the threads on the left are 
producers that create data, while the threads on the right are consumers that process data.

| Source/Producer	| Sink/Consumer |
|-------------------|---------------|
|Keyboard input	| Program that interprets|
|Software that has data	| Printer output|
|Software sends message	| Software receives message|
|Microphone and ADC	| Software that saves sound data|
|Software that has sound data	| DAC and speaker|

*Table 2.1. Producer consumer examples.*

Figure 2.7 shows how one could use a mailbox to pass data from a background thread (interrupt service routine) to a foreground 
thread (main program) if there were no operating system.

![Figure 2.7](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/255e1db381316a256e36850e180866ba/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_07_mailboxFlowChart.jpg)
Figure 2.7. Use of a mailbox without an operating system.

####CHECKPOINT 2.6

What happens if the ISR in Figure 2.7 runs twice before the main program has a chance to read and process the Mail?

With the flowchart in Figure 2.7, the Status will be set twice and the first data value will be lost. We will fix this 
error in Chapter 3 using a first in first out (FIFO) queue.

--
--

###2.1.6. Scheduler

[Scheduler](https://youtu.be/yYZeApNx7Hg)

A ___scheduler___ is a OS function that gives threads the notion of Concurrent processing where multiple threads are active. 
If we look from a distance (zoom out in time) it appears they are running simultaneously, when in fact only one thread is 
running at any time. On the Cortex-M with one processor only a single thread can run at any given time while other ready 
threads contend for processing. The scheduler therefore runs the ready threads one by one, switching between them to give 
us the illusion that all are running simultaneously.

In this class, the OS will schedule both main threads and event threads. However, in this section we will discuss scheduling 
main threads. To envision a scheduler, we first list the main threads that are ready to run. When the processor is free, 
the scheduler will choose one main thread from the ready list and cause it to run. In a ___preemptive scheduler___, main 
threads are suspended by a periodic interrupt, the scheduler chooses a new main thread to run, and the return from interrupt 
will launch this new thread. In this situation, the OS itself decides when a running thread will be suspended, returning it 
to the active state. In Program 2.1, there exist four threads as illustrated in Figure 2.8. The preemptive scheduler in the 
RTOS runs the four main threads concurrently. In reality, the threads are run one at time in sequence.

```c
void Task1(void){
  Init1();
  while(1){
    if(Status1())
      Input1();
  }
}
```
```c
void Task2(void){
  Init2();
  while(1){
    if(Status2())
      Output2();
  }
}
```c
```
void Task3(void){
  Init3();
  while(1){
    function3();
  }
}
```
```c
void Task4(void){
  Init4();
  while(1){
    function4();
  }
}
```

*Program 2.1. Four main threads run concurrently using a preemptive scheduler.*

![Figure 2.8](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/a107499a45da7d7fca6c5d90b59ec355/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_08_MultipleInputOutputFlowcharts.jpg)
*Figure 2.8. Four main threads.*

In a ___cooperative___ or ___nonpreemptive scheduler___, the main threads themselves decide when to stop running. This is 
typically implemented by having a thread call a function like __OS_Suspend__. This function will suspend the running 
thread (putting the old thread in the Active state), run the scheduler (which chooses a new thread), and launch the new 
thread. The new thread is now in the Run state. Although easy to implement because it doesn’t require interrupts, a 
cooperative scheduler is not appropriate for real-time systems. In Program 2.2, the cooperative scheduler runs the four 
main threads in a cyclic manner.

```c
void Task1(void){
 Init1();
 while(1){
  if(Status1()){
   Input1();
  }
  OS_Suspend();
 }
}
```
```c
void Task2(void){
 Init2();
 while(1){
  if(Status2()){
   Output2();
 }
 OS_Suspend();
 } 
}
```c
```
void Task3(void){
 Init3();
 while(1){
   function3();
   OS_Suspend();
 }
}
```
```c
void Task4(void){
 Init4();
 while(1){
   function4();
   OS_Suspend();
 }
}
```
*Program 2.2. Four threads run in a cooperative manner.*

There are many scheduling algorithms one can use to choose the next thread to run. A ___round robin scheduler___ simply runs 
the ready threads in circular fashion, giving each the same amount of time to execute. A ___weighted round robin scheduler___ 
runs the ready threads in circular fashion, but gives threads unequal weighting. One way to implement weighting is to vary the 
time each thread is allowed to run according to its importance. Another way to implement weighting is to run important threads 
more often. E.g., assume there are three threads 1 2 3, and thread 1 is more important. We could run the threads in this 
repeating pattern: 1, 2, 1, 3, 1, 2, 1, 3… Notice that very other time slice is given to thread 1. In this simple example, 
Thread 1 receives 50% of the processor time, and threads 2 and 3 each receive 25%. A ___priority scheduler___ assigns each 
thread a priority number (e.g., 1 is the highest). Two or more threads can have the same priority. A priority 2 thread is run 
only if no priority 1 threads are ready to run. Similarly, we run a priority 3 thread only if no priority 1 or priority 2 threads 
are ready. If all threads have the same priority, then the scheduler reverts to a round-robin system. The advantage of priority 
is that we can reduce the latency (response time) for important tasks by giving those tasks a high priority. The disadvantage 
is that on a busy system, low priority threads may never be run. This situation is called ___starvation___.

Schedulers for real-time systems may use other metrics to decide thread importance/priority. A ___deadline___ is when a task 
should complete relative to when it is ready to run. The ___time-to-deadline___ is the time between now and the deadline. If 
you have a paper due on Friday, and it is Tuesday, the time-to-deadline is 3 days. Furthermore, we define ___slack time___ as 
the time-to-deadline minus the how long it will take to complete the task. If you have a paper due on Friday, it is Tuesday 
and it will take you one day to write the paper, your slack time is 2 days. Once the slack time becomes negative, you will 
miss your deadline. There are many other ways to assign priority:

* Minimize latency for real-time tasks
* Assign a dollar cost for delayed service and minimize cost
* Give priority to I/O bound tasks over CPU bound tasks
* Give priority to tasks that need to run more frequently
* Smallest time-to-deadline first
* Smallest slack time first

A thread's priority may be statically assigned or can be changed dynamically as the system progresses. An ___exponential queue___ 
is a dynamic scheduling algorithm, with varying priorities and time slices. If a thread blocks on I/O, its priority is increased 
and its time slice is halved. If it runs to completion of a time slice, its priority is decreased and its time slice is doubled.

Another dynamic scheduling algorithm uses the notion of ___aging___ to solve starvation. In this scheme, threads have a permanent 
fixed priority and a temporary working priority. The permanent priority is assigned according the rules of the previous 
paragraph, but the temporary priority is used to actually schedule threads. Periodically the OS increases the temporary 
priority of threads that have not been run in a long time. Once a thread is run, its temporary priority is reset back to 
its permanent priority.

Assigning priority to tasks according to how often they are required to run (their periodicity) is called a Rate Monotonic 
Scheduler. Assume we have m tasks that are periodic, running with periods Tj (0 ≤ j ≤ m-1). We assign priorities according 
to these periods with more frequent tasks having higher priorities. Furthermore, let Ej be the maximum time to execute each 
task. Assuming there is little interaction between tasks, the Rate Monotonic Theorem can be used to predict if a scheduling 
solution exists. Tasks can be scheduled if

![scheduler_rate_monotonic_theorem](https://cloud.githubusercontent.com/assets/16638078/19609070/5f18fa08-97ab-11e6-8a1d-f95b2db067e6.png)

What this means is, as long as the total utilization of the set of tasks is below 69.32% (ln(2) ≈ 0.6932) RMS will guarantee 
to meet all timing constraints. The practical application of the Rate Monotonic Theorem is extremely limited because most 
systems exhibit a high degree of coupling between tasks. Nevertheless, it does motivate a consideration that applies to all 
real-time operating systems. Let E_j be the time to execute each task, and let T_j be the time between executions of each task. 
In general, E_j/T_j will be the percentage of time Task j needs to run. The sum of these percentages across all tasks yields 
a parameter that estimates processor utilization.

If utilization is over 100% there will be no solution. If utilization is below 5%, the processor may be too fast for your problem. 
The solution could be to slow down the clock and save power. As the sum goes over 50% and begins to approach 100%, it will be more 
and more difficult to schedule all tasks. The solution will be to use a faster processor or simplify the tasks. An effective system 
will operate in the 5 to 50% range.


--
--
###2.1.7. Function Pointers

[Function pointers](https://youtu.be/5LakKUvmOpI)

As we work our way towards constructing an OS there are some advanced programming concepts we require the reader to be 
familiar with. One such concept is "___function pointers___". Normally, when a software in module A wishes to invoke software 
in module B, module A simply calls a function in module B. The function in module B performs some action and returns to 
A. At this point, typically, this exchange is complete. A ___callback___ is a mechanism through which the software in module B 
can call back a preset function in module A at a later time. Another name for callback is ___hook___. To illustrate this concept, 
let module A be the user code and module B be the operating system. To setup a callback, we first write a user function 
(e.g., `CallMe`), and then the user calls the OS passing this function as a parameter.

```c
int count;
void Callme(void){
  count++;
}
```

The OS immediately returns to the user, but at some agreed upon condition, the OS can invoke a call back to the user 
by executing this function.

As we initialize the operating system, the user code must tell the OS a list of tasks that should be run. More 
specifically, the user code will pass into the operating system pointers to user functions. In C on the Cortex M, 
all pointers are 32-bit addresses regardless of the type of pointer. A function pointer is simply a pointer to a 
function. In this class all tasks or threads will be defined as void-void functions, like `Callme`. In other words, 
threads take no inputs and return no output. There are three operations we can perform on function pointers. The 
first is declaring a function pointer variable. Just like other pointers, we specify the type and add * in front 
of the name. We think it is good style to include ___p___, ___pt___, or ___ptr___ in pointer names. In this case, 
the syntax looks like this

```c
void (*TaskPt)(void);
```

Although the above line looks a little bit like a prototype, we can read this declaration as `TaskPt` is a pointer 
to a function that takes no input and returns no output.

Just like other variables, we need to set its value before using it. To set a function pointer we assign it a 
value of the proper type. In this case, TaskPt is a pointer to a void-void function, so we assign it the address 
of a void-void function by executing this code at run time.

```c
TaskPt = &Callme; // TaskPt points to Callme
```

Just like other pointers (to variables), to access what a pointer is pointing to, we dereference it using *. 
In this case, to run the function we execute

```c
TaskPt = &Callme; // TaskPt points to Callme
```

As an example, let’s look at one of the features in the BSP package. The function `BSP_PeriodicTask_Init` will 
initialize a timer so a user function will run periodically. Notice the user function is called from inside 
the interrupt service routine.

```c
void (*PeriodicTask)(void); // user function
void BSP_PeriodicTask_Init(
  void(*task)(void), // user function
  uint32_t freq,     // frequency in Hz
  uint8_t priority){ // priority
// . . . PeriodicTask = task; // user function
// . . .
}
void T32_INT1_IRQHandler(void){
  TIMER32_INTCLR1 = 0x00000001; // acknowledge interrupt
  (*PeriodicTask)();            // execute user task
}
```

The user code creates a void-void function and calls BSP_PeriodicTask_Init to attach this function to the 
periodic interrupt:

```c
BSP_PeriodicTask_Init(&checkbuttons, 10, 2);
```

You will NOT use `BSP_PeriodicTask_Init` in Lab 2, but will add it within Lab 3. However you will need 
to understand function pointers to implement Lab 2.

Another application of function pointers is a hook. A hook is an OS feature that allows the user to attach 
functions to strategic places in the OS. Examples of places we might want to place hooks include: whenever 
the OS has finished initialization, the OS is running the scheduler, or whenever a new thread is created. 
To use a hook, the user writes a function, calls the OS and passes a function pointer. When that event 
occurs, the OS calls the user function. Hooks are extremely useful for debugging.

--
--

###2.2.1. NVIC

[Interrupts](https://youtu.be/eskTBU6oF1k)

On the ARM Cortex-M processor, exceptions include resets, software interrupts and hardware interrupts. Each 
exception has an associated 32-bit vector that points to the memory location where the ISR that handles the 
exception is located. Vectors are stored in ROM at the beginning of memory. Program 2.3 shows the first few 
vectors as defined in the startup_TM4C123.s file for the TM4C123 and the startup_msp432.s file for the MSP432. 
DCD is an assembler pseudo-op that defines a 32-bit constant. ROM location 0x0000.0000 has the initial stack 
pointer, and location 0x0000.0004 contains the initial program counter, which is called the reset vector. It 
holds the address of a function called the reset handler, which is the first thing executed following reset. 
There are hundreds of possible interrupt sources and their 32-bit vectors are listed in order starting with 
location 0x0000.0008. From a programming perspective, we can attach ISRs to interrupts by writing the ISRs as 
regular assembly subroutines or C functions with no input or output parameters and editing the startup_TM4C123.s 
or startup_msp432.s file to specify those functions for the appropriate interrupt. In this class, we will write 
our ISRs using standard function names so that the startup files need not be edited. For example, we will 
simply name the ISR for SysTick periodic interrupt as SysTick_Handler. The ISR for this interrupt is a 32-bit 
pointer located at ROM address 0x0000.003C. Because the vectors are in ROM, this linkage is defined at compile 
time and not at run time. After the first 16 vectors, each processor will be different so check the data sheet.

