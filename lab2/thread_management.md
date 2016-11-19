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
```

```c
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
*TaskPt(); // call the function it points to    
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

Another application of function pointers is a ___hook___. A hook is an OS feature that allows the user to attach 
functions to strategic places in the OS. Examples of places we might want to place hooks include: whenever 
the OS has finished initialization, the OS is running the scheduler, or whenever a new thread is created. 
To use a hook, the user writes a function, calls the OS and passes a function pointer. When that event 
occurs, the OS calls the user function. Hooks are extremely useful for debugging.

--
--

###2.2.1. NVIC

[Interrupts](https://youtu.be/eskTBU6oF1k)

![interrupts](https://cloud.githubusercontent.com/assets/16638078/19670786/9317fe02-9a40-11e6-94bc-b9ba721985f1.png)

####How Interrups work
1. Interrup Vector Table
2. Initializacion (one-time)
  * Setup communication mechanisn
  * ARM/Enable the device
  * Priority (3-bits, 0-Highest, 7-lowest)
  * Enable all interrups (I=0)
3. ISR (Interrupt Service Rutine)
  * ACK the interrupt
  * Action

####Hardware

![hardware](https://cloud.githubusercontent.com/assets/16638078/19671860/39d79e1c-9a48-11e6-898a-cd4d877bb0f6.png)

Condition for an Interrup to have effect

* Triggers
* I=0
* NVIC (Nested-Vector Interrupt Control) (Flag configured - ARM)
* Priority must be higher that the current thread


Saving state of current thread

* Push registers 8 of them (R0-R3, R12, PC(R15), LR(R14), PSR(NVZC) )
* PC <- IVT (Interrupt Vector Table)
* IPSR <- Interrup Number
* LR <- 0xFFFF FFF9



On the ARM Cortex-M processor, exceptions include resets, software interrupts and hardware interrupts. Each 
exception has an associated 32-bit vector that points to the memory location where the ISR that handles the 
exception is located. Vectors are stored in ROM at the beginning of memory. Program 2.3 shows the first few 
vectors as defined in the startup_TM4C123.s file for the TM4C123 and the ___startup_msp432.s___ file for the MSP432. 
DCD is an assembler pseudo-op that defines a 32-bit constant. ROM location 0x0000.0000 has the initial stack 
pointer, and location 0x0000.0004 contains the initial program counter, which is called the reset vector. It 
holds the address of a function called the reset handler, which is the first thing executed following reset. 
There are hundreds of possible interrupt sources and their 32-bit vectors are listed in order starting with 
location 0x0000.0008. From a programming perspective, we can attach ISRs to interrupts by writing the ISRs as 
regular assembly subroutines or C functions with no input or output parameters and editing the startup_TM4C123.s 
or ___startup_msp432.s___ file to specify those functions for the appropriate interrupt. In this class, we will write 
our ISRs using standard function names so that the startup files need not be edited. For example, we will 
simply name the ISR for SysTick periodic interrupt as ___SysTick_Handler___. The ISR for this interrupt is a 32-bit 
pointer located at ROM address 0x0000.003C. Because the vectors are in ROM, this linkage is defined at compile 
time and not at run time. After the first 16 vectors, each processor will be different so check the data sheet.

```asm
        EXPORT  __Vectors
__Vectors                             ; address    ISR
        DCD     StackMem + Stack      ; 0x00000000 Top of Stack
        DCD     Reset_Handler         ; 0x00000004 Reset Handler
        DCD     NMI_Handler           ; 0x00000008 NMI Handler
        DCD     HardFault_Handler     ; 0x0000000C Hard Fault Handler
        DCD     MemManage_Handler     ; 0x00000010 MPU Fault Handler
        DCD     BusFault_Handler      ; 0x00000014 Bus Fault Handler
        DCD     UsageFault_Handler    ; 0x00000018 Usage Fault Handler
        DCD     0                     ; 0x0000001C Reserved
        DCD     0                     ; 0x00000020 Reserved
        DCD     0                     ; 0x00000024 Reserved
        DCD     0                     ; 0x00000028 Reserved
        DCD     SVC_Handler           ; 0x0000002C SVCall Handler
        DCD     DebugMon_Handler      ; 0x00000030 Debug Monitor Handler
        DCD     0                     ; 0x00000034 Reserved
        DCD     PendSV_Handler        ; 0x00000038 PendSV Handler
        DCD     SysTick_Handler       ; 0x0000003C SysTick Handler
```
*Program 2.3. Software syntax to set the interrupt vectors for the first 16 vectors on the Cortex M processor.*

Table 2.2 lists the interrupt sources we will use on the TM4C123 and Table 
2.3 shows similar interrupts on the MSP432. Interrupt numbers 0 to 15 contain 
the faults, software interrupts and SysTick; these interrupts will be handled 
differently from interrupts 16 to 154.

| Vector address	| Number	| IRQ	| ISR name in Startup.s	| NVIC priority	| Priority bits |
|---------------|-----------|-------|-----------------------|---------------|---------------|
| 0x00000038	| 14	| -2	| PendSV_Handler	| SYS_PRI3	| 23 – 21 | 
| 0x0000003C	| 15	| -1	| SysTick_Handler	| SYS_PRI3	| 31 – 29 |
| 0x000001E0	| 120	| 104	| WideTimer5A_Handler	| NVIC_PRI26_R	| 7 – 5 |

*Table 2.2. Some of the interrupt vectors for the TM4C (goes to number 154 on the M4).*

| Vector address	| Number	| IRQ	| ISR name in Startup.s	| NVIC priority	| Priority bits |
|---------------|-----------|-------|-----------------------|---------------|---------------|
| 0x00000038	| 14	| -2	| PendSV_Handler	| SYS_PRI3	| 23 – 21 |
| 0x0000003C	| 15	| -1	| SysTick_Handler	| SYS_PRI3	| 31 – 29 |
| 0x000000A4	| 41	| 25	| T32_INT1_IRQHandler	| NVIC_IPR6	| 15 – 13 |

*Table 2.3. Some of the interrupt vectors for the MSP432 (goes to number 154 on the M4).*

Interrupts on the Cortex-M are controlled by the Nested Vectored Interrupt 
Controller (NVIC). To activate an interrupt source we need to set its priority 
and enable that source in the NVIC. SysTick interrupt only requires arming the 
SysTick module for interrupts and enabling interrupts on the processor 
(I=0 in the ___PRIMASK___). Other interrupts require additional initialization. 
addition to arming and enabling, we will set bit 8 in the ___NVIC_EN3_R___ to 
activate ___WideTimer5A___ interrupts on the TM4C123. Similarly, we will set bit 
25 in the ___NVIC_ISER0___ to activate ___T32_INT1___ interrupts on the MSP432. This 
activation is in addition to the arm and enable steps.

Each interrupt source has an 8-bit priority field. However, on the 
TM4C123 and MSP432 microcontrollers, only the top three bits of the 8-bit 
field are used. This allows us to specify the interrupt priority level 
for each device from 0 to 7, with 0 being the highest priority. The 
priority of the SysTick interrupt is found in bits 31 – 29 of the 
___SYS_PRI3___ register. Other interrupts have corresponding priority 
registers. The interrupt number (number column in Tables 2.2 and 2.3) 
is loaded into the ___IPSR___ register when an interrupt is being serviced. 
The servicing of interrupts does not set the I bit in the ___PRIMASK___, so a 
higher priority interrupt can suspend the execution of a lower priority 
ISR. If a request of equal or lower priority is generated while an ISR is 
being executed, that request is postponed until the ISR is completed. In 
particular, those devices that need prompt service should be given high 
priority.

Figure 2.10 shows the context switch from executing in the foreground to 
running a SysTick periodic interrupt. The I bit in the ___PRIMASK___ is 0 
signifying interrupts are enabled. Initially, the interrupt number 
(___ISRNUM___) in the ___IPSR___ register is 0, meaning we are running in ___Thread 
mode___ (i.e., the main program, and not an ISR). Handler mode is 
signified by a nonzero value in ___IPSR___. When ___BASEPRI___ register is 
zero, all interrupts are allowed and the ___BASEPRI___ register is not 
active.

When a SysTick interrupt is triggered, the current instruction is finished. 
(a) Eight registers are pushed on the stack with ___R0___ on top. These registers 
are pushed onto the stack using whichever stack pointer is active: either 
the ___MSP___ or ___PSP___. (b) The vector address is loaded into the ___PC___ (“Vector 
address” column in Tables 2.2 and 2.3). (c) The ___IPSR___ register is set to 
15 (“Number” column in Tables 2.2 and 2.3) (d) The top 24 bits of ___LR___ are 
set to 0xFFFFFF, signifying the processor is executing an ISR. The bottom 
eight bits specify how to return from interrupt.

```
0xE1 Return to Handler mode MSP (using floating point state)
0xE9 Return to Thread mode MSP (using floating point state)
0xED Return to Thread mode PSP (using floating point state)
0xF1 Return to Handler mode MSP
0xF9 Return to Thread mode MSP ← we will mostly be using this one
0xFD Return to Thread mode PSP
```

After pushing the registers, the processor always uses the main stack pointer 
(___MSP___) during the execution of the ISR. Events b, c, and d can occur simultaneously.

![Figure 2.10](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/6f55d6c67ffce5dbc647b7ef1091106e/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_09_Interrupt_stack.jpg)
*Figure 2.10. Stack before and after an interrupt, in this case a SysTick periodic interrupt.*

To ___return from an interrupt___, the ISR executes the typical function return 
statement: BX LR. However, since the top 24 bits of LR are 0xFFFFFF, it knows 
to return from interrupt by popping the eight registers off the stack. Since 
the bottom eight bits of LR in this case are 0b11111001, it returns to thread 
mode using the MSP as its stack pointer. Since the IPSR is part of the PSR that 
is popped, it is automatically reset to its previous state.

A nested interrupt occurs when a higher priority interrupt suspends an ISR. 
The lower priority interrupt will finish after the higher priority ISR completes. 
When one interrupt preempts another, the LR is set to 0xFFFFFFF1, so it knows to 
return to handler mode. ___Tail chaining___ occurs when one ISR executes immediately 
after another. Optimization occurs because the eight registers need not be 
popped only to be pushed once again. If an interrupt is triggered and is in 
the process of stacking registers when a higher priority interrupt is requested, 
this late arrival interrupt will be executed first.

On the Cortex-M4, if an interrupt occurs while in the floating point state, 
an additional 18 words are pushed on the stack. These 18 words will save the 
state of the floating point processor. Bits 7-4 of the LR will be 0b1110 (0xE), 
signifying it was interrupted during a floating point state. When the ISR 
returns, it knows to pull these 18 words off the stack and restore the state 
of the floating point processor. We will not use floating point in this class.

___Priority___ determines the order of service when two or more requests are made simultaneously. Priority also allows a higher priority request to suspend a lower priority request currently being processed. Usually, if two requests have the same priority, we do not allow them to interrupt each other. NVIC assigns a priority level to each interrupt trigger. This mechanism allows a higher priority trigger to interrupt the ISR of a lower priority request. Conversely, if a lower priority request occurs while running an ISR of a higher priority trigger, it will be postponed until the higher priority service is complete.


--
--

###2.2.2. SysTick periodic interrupts

[SysTick](https://youtu.be/Xk243_TJGjs)

![systick](https://cloud.githubusercontent.com/assets/16638078/19685775/899c57c0-9a94-11e6-977d-a7dd8703343a.png)

The ____SysTick Timer____ is a core device on the Cortex M architecture, which is most commonly used as a periodic timer. When used as a 
periodic timer one can setup the countdown to zero event to cause an interrupt. By setting up an initial reload value the timer 
is made to periodically interrupt at a predetermined rate decided by the reload value. Periodic timers as an interfacing technique 
are required for data acquisition and control systems, because software servicing must be performed at accurate time intervals. 
For a data acquisition system, it is important to establish an accurate sampling rate. The time in between ADC samples must be 
equal (and known) in order for the digital signal processing to function properly. Similarly, for microcontroller-based control 
systems, it is important to maintain both the input rate of the sensors and the output rate of the actuators. Periodic events are 
so important that most microcontrollers have multiple ways to generate periodic interrupts.

*In this class our operating system will use periodic interrupts to schedule threads.*

Assume we have a 1-ms periodic interrupt. This means the interrupt service routine (ISR) is triggered to run 1000 times per second. 
Let Count be a global variable that is incremented inside the ISR. Figure 2.11 shows how to use the interrupt to run Task 1 every N ms 
and run Task 2 every M ms.

![Figure 2.11](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/b7fc7c278a5e9f3dfdbba91580e1718e/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_10_PeriodicPollingFlowcharts.jpg)
*Figure 2.11. Using a 1-ms periodic interrupt to run Task 1 every N ms and run Task 2 every M ms.*

The SysTick timer exists on all Cortex-M microcontrollers, so using SysTick means the system will be easy to port to other microcontrollers. 
Table 2.4 shows the register definitions for SysTick. The basis of SysTick is a 24-bit down counter that runs at the bus clock frequency. 
To configure SysTick for periodic interrupts we first clear the ENABLE bit to turn off SysTick during initialization, see Program 2.4. 
Second, we set the ___STRELOAD___ register. Third, we write any value to the ____STCURRENT____, which will clear the counter and the flag. Lastly, we 
write the desired clock mode to the control register ____STCTRL____, also setting the ____INTEN____ bit to enable interrupts and enabling the timer (____ENABLE____). 
We establish the priority of the SysTick interrupts using the TICK field in the ____SYSPRI3____ register. When the ____STCURRENT____ value counts down from 
1 to 0, the COUNT flag is set. On the next clock, the ____STCURRENT____ is loaded with the STRELOAD value. In this way, the SysTick counter (____STCURRENT____) 
is continuously decrementing. If the ____STRELOAD____ value is n, then the SysTick counter operates at modulo n+1:


*…n, n-1, n-2 … 1, 0, n, n-1, …*


In other words, it rolls over every n+1 counts. Thus, the ____COUNT____ flag will be configured to trigger an interrupt every n+1 counts. The main program will 
enable interrupts in the processor after all variables and devices are initialized.

![Table 2.4](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/837c002d838029c4ffc498167982d8ec/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Table2.4_SysTickRegisters.jpg)
*Table 2.4. SysTick registers.*


The SysTick counter decrements every bus cycle. So it is important to know the bus frequency when using SysTick. TM4C123 projects run at 16 MHz until the system 
calls BSP_Clock_InitFastest, at which time it will run at 80 MHz. ___MSP432____ projects run at 3 MHz until the system calls `BSP_Clock_InitFastest`, at which time it will 
run at 48 MHz. In general, if the period of the core bus clock is t time units, then the ___COUNT___ flag will be set every (n+1)t time units. Reading the ___STCTRL___ control 
register will return the COUNT flag in bit 16, and then clear the flag. Also, writing any value to the ___STCURRENT___ register will reset the counter to zero and clear 
the COUNT flag. The ___COUNT___ flag is also cleared automatically as the interrupt service routine is executed.

Let fBUS be the frequency of the bus clock, and let n be the value of the ___STRELOAD___ register. The frequency of the periodic interrupt will be

<math xmlns="http://www.w3.org/1998/Math/MathML" display="block">
  <mfrac>
    <msub>
      <mi>f</mi>
      <mrow class="MJX-TeXAtom-ORD">
        <mi>B</mi>
        <mi>U</mi>
        <mi>S</mi>
      </mrow>
    </msub>
    <mrow>
      <mi>n</mi>
      <mo>+</mo>
      <mn>1</mn>
    </mrow>
  </mfrac>
</math>

```c
void SysTick_Init(uint32_t period){
  Profile_Init();
  Counts = 0;
  STCTRL = 0;         // disable SysTick during setup
  STRELOAD = period-1;// reload value
  STCURRENT = 0;      // any write to current clears it
  SYSPRI3 = (SYSPRI3&0x00FFFFFF)|0x40000000; // priority 2  
          STCTRL = 0x07;      // enable, core clock, interrupts
}
// Interrupt service routine
// Executed every (bus cycle)*(period)
void SysTick_Handler(void){
  Profile_Toggle0();          // toggle bit
  Profile_Toggle0();          // toggle bit
  Counts = Counts + 1;
  Profile_Toggle0();          // toggle bit
}
int main(void){  // TM4C123 with bus clock at 16 MHz
  SysTick_Init(1600000);      // initialize SysTick timer
  EnableInterrupts();
  while(1){                   // interrupts every 100ms, 5 Hz flash
    Profile_Toggle1();        // toggle bit
  }
 }
```

*Program 2.4. Implementation of a periodic interrupt using SysTick (SysTickInts_xxx).*

###Running SysTick on the real board

[Running SysTick on the real board](https://youtu.be/UTPeQQQnF1A)

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/840506fb439e6b073c2ef75b38ad8b5d/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/SysTickInterrupt.gif)

###CHECKPOINT 2.9

If the bus clock is 80 MHz (12.5ns), what reload value yields a 100 Hz (10ms) periodic interrupt?

(80MHz/100Hz - 1) =799999. 10ms = (799999+1)*12.5ns. Reload should be 799999.


Figure 2.12 shows a zoomed in view of the profile pin measured during one execution of the SysTick ISR. The first two toggles signify 
the ISR has started. The time from second to third toggle illustrates the body of the ISR takes 1.2 µs of execution time.

![Figure 2.12](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/3ab07b6adcb21c8c9bf21ae230475392/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_11_tripleToggleZoomIn.jpg)
*Figure 2.12. Profile of a single execution of the SysTick ISR measured on a TM4C123 running at 16 MHz.*

Figure 2.13 shows a zoomed out view of the profile pin measured during multiple executions of the SysTick ISR. This measurement verifies 
the ISR runs every 100 ms. Because of the time scale, the three toggles appear as a single toggle. This ___triple-toggle technique (TTT)___ 
allows us to measure both the time to execution of one instance of the ISR and to measure the time between ISR executions.


![Figure 2.13](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/9ae18d91b119ab8e52ad835baa848756/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_12_TripleToggeZoomOut.jpg)
*Figure 2.13. Profile of multiple executions of the SysTick ISR on a TM4C123 running at 16 MHz.*


--
--

###2.2.3. Critical sections

[Critical sections](https://youtu.be/X09ymbgQrLs)

![](![critical_section](https://cloud.githubusercontent.com/assets/16638078/19686922/cabdf506-9a99-11e6-92d0-2d1860d7ea2f.png))



An important consequence of multi-threading is the potential for the threads to manipulate 
(read/write) a shared object. With this potential comes the possibility of inconsistent 
updates to the shared object. A ___race condition____ occurs in a multi-threaded environment 
when there is a causal dependency between two or more threads. In other words, different 
behavior occurs depending on the order of execution of two threads. Consider a ___simple example___ 
of a race condition occurring where two thread initialize the same port in an unfriendly manner. 
Thread-1 initializes Port 4 bits 3 – 0 to be output using `P4DIR = 0x0F;` Thread-2 initializes 
Port 4 bits 6 – 4 to be output using `P4DIR = 0x70;` In particular, if Thread-1 runs first 
and Thread-2 runs second, then Port 4 bits 3 – 0 will be set to inputs. Conversely, if Thread-2 
runs first and Thread-1 runs second, then Port 4 bits 6 – 4 will be set to inputs. This is a 
race condition caused by unfriendly code. The solution to this problem is to write the two 
initializations in a friendly manner, and make both initializations atomic.

In a ___second example___ of a race condition, assume two threads are trying to get data from 
the same input device. Both call the input function to receive data from the input device. 
When data arrives at the input, the thread that executes first will capture the data.

In general, if two threads access the same global memory and one of the accesses is a write, 
then there is a ___causal dependency___ between the execution of the threads. Such dependencies 
when not properly handled cause unpredictable behavior where the execution order may affect 
the outcome. Such scenarios are referred to as race conditions. While shared global variables 
are important in multithreaded systems because they are required to pass data between threads, 
they result in complex behavior (and hard to find bugs). Therefore, a ___programmer must pay 
careful attention to avoid race conditions___.

A program segment is ___reentrant__ if it can be concurrently executed by two (or more) threads. 
Note that, to run concurrently means both threads are ready to run though only one thread is 
currently running. To implement ___reentrant software___, we place variables in registers or on the 
stack, and avoid storing into global memory variables. When writing in assembly, we use registers, 
or the stack for parameter passing to create reentrant subroutines. Typically, each thread will 
have its own set of registers and stack. A ___non-reentrant___ subroutine will have a section 
of code called a ___vulnerable window___ or ___critical section___. A critical section may exist 
when two different functions access and modify the same memory-resident data structure. An error 
occurs if 

1. One thread calls a non-reentrant function
2. It is executing in the critical section when interrupted by a second thread
3. The second thread calls the same non-reentrant function.

There are a number of scenarios that can happen next. In the ___most common scenario___, the second 
thread is allowed to complete the execution of the function, control is then returned to the 
first thread, and the first thread finishes the function. This first scenario is the usual case 
with interrupt programming. In the ___second scenario___, the second thread executes part of 
the critical section, is interrupted and then re-entered by a third thread, the third thread 
finishes, the control is returned to the second thread and it finishes, lastly the control 
is returned to the first thread and it finishes. This second scenario can happen in interrupt 
programming if the second interrupt has higher priority than the first. Program 2.5 shows 
two C functions and the corresponding assembly codes. These functions have critical sections 
because of their ___read-modify-write nonatomic access___ to the global variable, count. If an 
interrupt were to occur just before or just after the ADD or SUB instruction, and the ISR 
called the other function, then count would be in error.

```asm
count    SPACE  4    
Producer LDR  r1,[pc,#116] ; R0= &count
         LDR  r0,[r1]      ; R0=count
         ADD  r0,r0,#1
         STR  r0,[r1]      ; update 
         BX   lr
Consumer LDR  r1,[pc,#96]  ; R0= &count
         LDR  r0,[r1]      ; R0=count
         SUB  r0,r0,#1
         STR  r0,[r1]      ; update 
         BX   lr
         DCD  num     	int32_t volatile count;

```

```c
void Producer(void){  
  // other stuff  
  count = count + 1;
    // other stuff 
}
void Consumer(void){
  // other stuff
  count = count – 1;
  // other stuff
}
```

*Program 2.5. These functions are nonreentrant because of the read-modify-write access to a global. The critical section is just before and just after the ADD and SUB instructions.*

Assume there are two concurrent threads, where the main program calls Producer and a background 
ISR calls Consumer. ___Concurrent___ means that both threads are ready to run. Because there is 
only one computer, exactly one thread will be running at a time. Typically, the operating system 
switches execution control back and forth using interrupts. There are two places in the assembly 
code of Producer at which if an interrupt were to occur and the ISR called the Consumer function, 
the end value of count will be inconsistent. Assume for this example count is initially 4. An 
error occurs if:

1. The main program calls Producer
2. The main executes LDR r0,[r1]
  * making R0 = 4
3. The OS suspends the main (using an interrupt) and starts the ISR
4. The ISR calls Consumer
  * Executes count=count-1; making count equal to 3
5. The OS returns control back to the main program
  * R0 is back to its original value of 4
6. The producer finishes (adding 1 to R0) Making count equal to 5

The expected behavior with the producer and consumer executing once is that count would remain 
at 4. However, the race condition resulted in an inconsistency manifesting as a lost consumption. 
As the reader may have observed, the cause of the problem is the non-atomicity of the 
read-modify-write operation involved in reading and writing to the count (count=count+1 
or count=count-1) variable. An ___atomic operation___ is one that once started is guaranteed to finish. 
In most computers, once an assembly instruction has begun, the instruction must be finished 
before the computer can process an interrupt. The same is not the case with C instructions 
which themselves translate to multiple assembly instructions. In general, nonreentrant code 
can be grouped into three categories all involving 1) nonatomic sequences, 2) writes and 3) 
global variables. We will classify I/O ports as global variables for the consideration of 
critical sections. We will group registers into the same category as local variables because 
each thread will have its own registers and stack.

The first group is the ___read-modify-write___ sequence:

1. The software reads the global variable producing a copy of the data
2. The software modifies the copy (original variable is still unmodified)
3. The software writes the modification back into the global variable.

In the second group, we have a ___write followed by read___, where the global variable is used for 
temporary storage:

1. The software writes to the global variable (only copy of the information)
2. The software reads from the global variable expecting the original data to be there.

In the third group, we have a ___non-atomic multi-step write___ to a global variable:

1. The software writes part of the new value to a global variable
2. The software writes the rest of the new value to a global variable.

*Observation: When considering reentrant software and vulnerable windows we classify 
accesses to I/O ports the same as accesses to global variables.*

*Observation: Sometimes we store temporary information in global variables out of laziness. 
This practice is to be discouraged because it wastes memory and may cause the module to not 
be reentrant.*

Sometimes we can have a critical section between two different software functions (one function 
called by one thread, and another function called by a different thread). In addition to above 
three cases, a ___non-atomic multi-step___ read will be critical when paired with a 
___multi-step write___. 
For example, assume a data structure has multiple components (e.g., hours, minutes, and seconds). 
In this case, the write to the data structure will be atomic because it occurs in a high priority 
ISR. The critical section exists in the foreground between steps 1 and 3. In this case, a critical 
section exists even though no software has actually been reentered.


| Foreground thread |
| ----------------- |
| 1. The main reads some of the data |
| 3. The main reads the rest of the data |

| Background thread |
| ----------------- |
| 2. ISR writes to the data structure |

In a similar case, a non-atomic multi-step write will be critical when paired with a multi-step 
read. Again, assume a data structure has multiple components. In this case, the read from the 
data structure will be atomic because it occurs in a high priority ISR. The critical section 
exists in the foreground between steps 1 and 3.

| Foreground thread |
| ----------------- |
| 1. The main writes some of the data |
| 3. The main writes the rest of the data Background thread	|

| Background thread |
| ----------------- |
| 2. ISR reads from the data structure |

When multiple threads are active, it is possible for two threads to be executing the same program. For example, the system may be running in the foreground and calls a function. Part way through execution of the function, an interrupt occurs. If the ISR also calls the same function, two threads are simultaneously executing the function.

If critical sections do exist, we can either eliminate them by removing the access to the global 
variable or implement mutual exclusion, which simply means only one thread at a time is allowed 
to execute in the critical section. In general, if we can eliminate the global variables, then 
the subroutine becomes reentrant. Without global variables there are no "vulnerable" windows 
because each thread has its own registers and stack. Sometimes one must access global memory to 
implement the desired function. Remember that all I/O ports are considered global. Furthermore, 
global variables are necessary to pass data between threads. Program 2.6 shows four functions 
available in the starter projects for this class that can be used to implement mutual exclusion. 
The code is in the startup file and the prototypes are in the ___CortexM.h___ file.

```asm
;*********** DisableInterrupts ***************
; disable interrupts 
; inputs:  none
; outputs: none
DisableInterrupts
        CPSID  I
        BX     LR
;*********** EnableInterrupts ***************
; disable interrupts 
; inputs:  none
; outputs: none 
EnableInterrupts
       CPSIE  I
       BX     LR
;*********** StartCritical ************************
; make a copy of previous I bit, disable interrupts
; inputs:  none
; outputs: previous I bit
StartCritical
        MRS    R0, PRIMASK  ; save old status
        CPSID  I            ; mask all (except faults)
        BX     LR 
;*********** EndCritical ************************
; using the copy of previous I bit, restore I bit to previous value  
; inputs:  previous I bit
; outputs: none  
EndCritical    
        MSR    PRIMASK, R0
        BX     LR
```

*Program 2.6. Assembly functions needed for interrupt enabling and disabling.*

A simple way to implement mutual exclusion is to disable interrupts while executing the 
critical section. It is important to disable interrupts for as short a time as possible, 
so as to minimize the effect on the dynamic performance of the other threads. While we are 
running with interrupts disabled, time-critical events like power failure and danger 
warnings cannot be processed. The assembly code of Program 2.6 is in the startup file in 
our projects that use interrupts. Program 2.7 illustrates how to implement mutual 
exclusion and eliminate the critical section.

```c
uint32_t volatile count;
void Producer(void){   // simple option
  DisableInterrupts();  
  count = count + 1;
  EnableInterrupts();  
}
void Producer(void){   // safer option
long sr;
  sr = StartCritical();  
  count = count + 1;
  EndCritical(sr);  
}
```

*Program 2.7. This function is reentrant because of the read-modify-write access to the 
global is atomic. Use the simple option only if one critical section is not nested insid
e another critical section.*

When making code atomic with this simple method, make sure one critical section is not 
nested inside another critical section.

####CHECKPOINT 2.10

Although disabling interrupts does remove critical sections, it will add latency and 
jitter to real-time systems. Explain how latency and jitter are affected by the 
`DisableInterrupts()` and `EnableInterrupts()` functions.


####CHECKPOINT 2.11

Consider the situation of nested critical sections. For example, a function with a critical 
section calls another function that also has a critical section. What would happen if you 
simply added disable interrupts at the beginning and a re-enable interrupts at the end of 
each critical section?

Notice there are two disable interrupt and two enable interrupt functions, occurring in 
this order: 1) disable, 2) disable, 3) enable, 4) enable. Interrupts will be incorrectly 
enabled after step 3). Since the 1-4 represents a critical section and 2-3 is inside this 
section, a bug will probably be introduced.

Since real-time events trigger interrupts, and the ISR software services the requests, 
disabling interrupts will postpone the response causing latency or jitter. The maximum 
jitter will be the maximum time running with interrupts disabled.

###The Game

[The Game](https://youtu.be/kRvRXyGIRKg)


--
--

###2.3.1. Two types of threads

[Two types of threads](https://youtu.be/OInuxwPg37g)


A fundamental concept in Operating Systems is the notion of an execution context referred to as a ___thread___. We introduced threads and their components in section 2.1.2, we will now look at the types of threads and how they are treated differently in the OS.

We define two types of threads in our simple OS. ___Event___ threads are attached to hardware and should execute changes in hardware status. Examples include periodic threads that should be executed at a fixed rate (like the microphone, accelerometer and light measurements in Lab 1), input threads that should be executed when new data are available at the input device (like the operator pushed a button), and output threads that should be executed when the output device is idle and new data are available for output. They are typically defined as void-void functions. The time to execute an event thread should be short and bounded. In other words, event threads must execute and return. The time to execute an event thread must always be less than a small value (e.g., 10µs). In an embedded system without an OS, event threads are simply the interrupt service routines (ISRs). However, with a RTOS, we will have the OS manage the processor and I/O, and therefore the OS will manage the ISRs. The user will write the software executed as an event thread, but the OS will manage the ISR and call the appropriate event thread. Communication between threads will be managed by the OS. For example, threads could use a FIFO to pass data.

```c
void inputThread(void){
  data = ReadInput(); 
  Send(data);
}
void outputThread(void){
  data = Recv(); 
  WriteOutput(data);
}
void periodicThread(void){
  PerformTask();
}
```

The second type of thread is a ___main___ thread. Without an OS, embedded systems typically have one main program that is executed on start up. This main initializes the system and defines the high level behavior of the system. In an OS however, we will have multiple main threads. Main threads execute like main programs that never return. These threads execute an initialization once and then repeatedly execute a sequence of steps within a while loop. Here in chapter 2, we will specify all the main threads at initialization and these threads will exist indefinitely. However, in later chapters we will allow main threads to be created during execution, and we will allow main threads to be destroyed dynamically.

```c
void mainThread(void){
  Init(); 
  while(1){
    Body();
  }
}
```

Table 2.5 compares event and main threads. For now, main threads will run indefinitely, but later in the class we will allow main threads to be terminated if their task is complete. Also, in Chapter 2 we will create all the main threads statically at the time the OS launches. In subsequent chapters we will allow the user to create main threads dynamically at run time.

| Event Thread	| Main Thread| 
|---------------|------------|
| Triggered by hardware	| Created when OS launches| 
| Must return	| Runs indefinitely| 
| Short execution time	| Unbounded execution time| 
| No waiting	| Allowed to wait| 
| Finite number of loops (definite)	| Indefinite or infinite loops| 

*Table 2.5. Comparison of event and main threads.*


--
--

###2.3.2. Thread Control Block (TCB)

[ Thread Control Block ](https://youtu.be/syadcFUSwzA)


Figure 2.14 shows three threads. Each thread has a thread control block (TCB) encapsulating the state of the thread. For now, a thread’s TCB we will only maintain a link to its stack and a link to the TCB of the next thread. The ___RunPt___ points to the TCB of the thread that is currently running. The next field is a pointer chaining all three TCBs into a circular linked list. Each TCB has an sp field. If the thread is running it is using the real SP for its stack pointer. However, the other threads have their stack pointers saved in this field. Other fields that define a thread’s state such as, status, Id, sleeping, age, and priority will be added later. However, for your first RTOS, the sp and next fields will be sufficient. The scheduler traverses the linked list of TCBs to find the next thread to run.  In this example there are three threads in a circular linked list. Each thread runs for a fixed amount of time, and a periodic interrupt suspends the running thread and switches RunPt to the next thread in the circular list. The scheduler then launches the next thread.

[Figure 2.14](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/4db4300fcac73fbdb14309f25948d462/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_16_RoundRobinThreads.jpg)
Figure 2.14. Three threads have their TCBs in a circular linked list. 

The ___Thread Control Block (TCB)___ will store the information private to each thread. There will be a TCB structure and a stack for each thread. While a thread is running, it uses the actual Cortex M hardware registers (Figure 2.15). Program 2.8 shows a TCB structure with the necessary components for three threads: 
  1. A pointer so it can be chained into a linked list
  2. The value of its stack pointer

In addition to these necessary components, the TCB might also contain:
  3. Status, showing resources that this thread has or wants 
  4. A sleep counter used to implement sleep mode
  5. Thread number, type, or name
  6. Age, or how long this thread has been active
  7. Priority (not used in a round robin scheduler)

```c
#define NUMTHREADS 3  // maximum number of threads
#define STACKSIZE 100 // number of 32-bit words in stack
struct tcb{
  int32_t *sp;      // pointer to stack, valid for threads not running
  struct tcb *next; // linked-list pointer
};
typedef struct tcb tcbType;
tcbType tcbs[NUMTHREADS];
tcbType *RunPt;
int32_t Stacks[NUMTHREADS][STACKSIZE];
```
*Program 2.8. TCBs for up to 3 threads, each stack is 400 bytes.*

![Figure 2.15](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/87b50a40b12c1d5d8549816c77d05e48/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_15_ThreadSwitch.jpg)
*Figure 2.15. The running thread uses the actual registers, while the other threads have their register values saved on the stack. For the running thread the sp field is not valid, while the sp field on other threads points to the top of its stack.*

--
--

###2.3.3. Creation of threads

[Creation of threads](https://youtu.be/VRUlnQ-sHho)


Program 2.9 shows how to create three TCBs that will run three programs. First, the three TCBs are linked in a circular list. Next the initial stack for each thread is created in such a way that it looks like it has been running already and has been previously suspended. The PSR must have the T-bit equal to 1 because the Arm Cortex M processor always runs in Thumb mode. The PC field on the stack contains the starting address of each thread. The initial values for the other registers do not matter, so they have been initialized to values that will assist in debugging. This idea came from the ___os_cpu_c.c___ file in Micrium µC/OS-II. The allocation of the stack areas must be done such that the addresses are double-word aligned.

```c
void SetInitialStack(int i){ 
  tcbs[i].sp = &Stacks[i][STACKSIZE-16]; // thread stack pointer 
  Stacks[i][STACKSIZE-1] = 0x01000000; // Thumb bit 
  Stacks[i][STACKSIZE-3] = 0x14141414; // R14 
  Stacks[i][STACKSIZE-4] = 0x12121212; // R12 
  Stacks[i][STACKSIZE-5] = 0x03030303; // R3 
  Stacks[i][STACKSIZE-6] = 0x02020202; // R2 
  Stacks[i][STACKSIZE-7] = 0x01010101; // R1 
  Stacks[i][STACKSIZE-8] = 0x00000000; // R0 
  Stacks[i][STACKSIZE-9] = 0x11111111; // R11 
  Stacks[i][STACKSIZE-10] = 0x10101010; // R10 
  Stacks[i][STACKSIZE-11] = 0x09090909; // R9 
  Stacks[i][STACKSIZE-12] = 0x08080808; // R8 
  Stacks[i][STACKSIZE-13] = 0x07070707; // R7 
  Stacks[i][STACKSIZE-14] = 0x06060606; // R6 
  Stacks[i][STACKSIZE-15] = 0x05050505; // R5 
  Stacks[i][STACKSIZE-16] = 0x04040404; // R4 
} 
int OS_AddThreads(void(*task0)(void), void(*task1)(void), 
        void(*task2)(void)){ 
int32_t status; 
  status = StartCritical(); 
  tcbs[0].next = &tcbs[1]; // 0 points to 1 
  tcbs[1].next = &tcbs[2]; // 1 points to 2 
  tcbs[2].next = &tcbs[0]; // 2 points to 0 

  SetInitialStack(0); Stacks[0][STACKSIZE-2] = (int32_t)(task0); // PC
  SetInitialStack(1); Stacks[1][STACKSIZE-2] = (int32_t)(task1); // PC
  SetInitialStack(2); Stacks[2][STACKSIZE-2] = (int32_t)(task2); // PC
  RunPt = &tcbs[0];        // thread 0 will run first 
  EndCritical(status); 
  return 1; // successful 
}
```
*Program 2.9. OS code used to create three active threads.*

Even though the thread has not yet been run, it is created with an initial stack that “looks like” it had been previously suspended by a SysTick interrupt. Notice that the initial value loaded into the PSR when the thread runs for the first time has T=1.Program 2.10 shows simple user software that can be run on this RTOS. Each thread increments a counter and toggles an output pin. The three counters should be approximately equal. Profile bit 0 toggles quickly while thread 0 is running. Profile bits 1 and 2 toggle when running threads 1 and 2 respectively.

```c
void Task0(void){ 
  Count0 = 0; 
  while(1){ 
    Count0++; 
    Profile_Toggle0(); // toggle bit 
  } 
} 
void Task1(void){ 
  Count1 = 0; 
  while(1){ 
    Count1++; 
    Profile_Toggle1(); // toggle bit 
  } 
} 
void Task2(void){ 
  Count2 = 0; 
  while(1){ 
    Count2++; 
    Profile_Toggle2(); // toggle bit 
  } 
} 
#define THREADFREQ 500 // frequency in Hz 
int main(void){ // testmain2 
  OS_Init(); // initialize, disable interrupts 
  Profile_Init(); // enable digital I/O on profile pins 
  OS_AddThreads(&Task0, &Task1, &Task2); 
  OS_Launch(BSP_Clock_GetFreq()/THREADFREQ); // interrupts enabled in here 
  return 0; // this never executes 
}
```

*Program 2.10. Example user code with three threads.*

SysTick will be used to perform the preemptive thread switching. We will set the SysTick to the lowest level so we know it will only suspend foreground threads (Program 2.11).

```c
void OS_Init(void){ 
  DisableInterrupts(); 
  BSP_Clock_InitFastest();// set processor clock to fastest speed 
}
```

*Program 2.11. RTOS initialization.*

--
--


###2.3.4. Launching the OS

[Launching the OS](https://youtu.be/wyAETMk-zlg)

To start the RTOS, we write code that arms the SysTick interrupts and unloads the stack as if it were returning from an interrupt (Program 2.12). The units of `theTimeSlice`` are in bus cycles. The bus cycle time on the TM4C123 is 12.5ns, and on the MSP432 the bus cycle time is 20.83ns.
```c
void OS_Launch(uint32_t theTimeSlice){
  STCTRL = 0;                  // disable SysTick during setup
  STCURRENT = 0;               // any write to current clears it
  SYSPRI3 =(SYSPRI3&0x00FFFFFF)|0xE0000000; // priority 7
  STRELOAD = theTimeSlice - 1; // reload value
  STCTRL = 0x00000007;         // enable, core clock and interrupt arm
  StartOS();                   // start on the first task
}
```
*Program 2.12. RTOS launch.*

The `StartOS` is written in assembly (Program 2.13). In this simple implementation, the first user thread is launched by setting the stack pointer to the value of the first thread, then pulling all the registers off the stack explicitly. The stack is initially set up like it had been running previously, was interrupted (8 registers pushed), and then suspended (another 8 registers pushed). When launch the first thread for the first time we do not execute a return from interrupt (we just pull 16 registers from its stack). Thus, the state of the thread is initialized and is now ready to run.
```asm
StartOS
    LDR     R0, =RunPt   ; currently running thread
    LDR     R1, [R0]     ; R1 = value of RunPt
    LDR     SP, [R1]     ; new thread SP; SP = RunPt->sp;
    POP     {R4-R11}     ; restore regs r4-11
    POP     {R0-R3}      ; restore regs r0-3
    POP     {R12}
    ADD     SP, SP, #4   ; discard LR from initial stack
    POP     {LR}         ; start location
    ADD     SP, SP, #4   ; discard PSR
    CPSIE   I            ; Enable interrupts at processor level
    BX      LR           ; start first thread
```
*Program 2.13. Assembly code for the thread switcher.*


--
--

###2.3.5. Switching theads

[Switching theads](https://youtu.be/PDTutyruns0)

___context switch___ =  ___thread switching___

The SysTick ISR, written in assembly, performs the preemptive thread switch (Program 2.14). SysTick interrupts will be triggered at a 
fixed rate (e.g., every 2 ms in this example. Because SysTick is priority 7, it cannot preempt any background threads. This means SysTick 
can only suspend foreground threads. 
1. The processor automatically saves eight registers (R0-R3,R12, LR,PC and PSR) on the stack as it suspends execution of the main program and launches the ISR. 
2. Since the thread switcher has read-modify-write operations to the SP and to `RunPt`, we need to disable interrupts to make the ISR atomic. 
3. Here we explicitly save the remaining registers (R4-R11). Notice the 16 registers on the stack match exactly the order of the 16 registers established by the `OS_AddThreads` function. 
4. Register R1 is loaded with `RunPt`, which points to the TCB of the thread in the process of being suspended. 
5. By storing the actual SP into the sp field of the TCB, we have finished suspending the thread. To repeat, to suspend a thread we push all its registers on its stack and save its stack pointer in its TCB. 
6. To implement round robin, we simply choose the next thread in the circular linked list and update `RunPt` with the new value. The #4 is used because the next field is the second entry in the TCB. We will change this step later to implement sleeping, blocking, and priority scheduling. 
7. The first step of launching the new thread is to establish its stack pointer. 
8. We explicitly pull eight registers from the stack. 
9. We enable interrupts so the new thread runs with interrupts enabled. 
10. The LR contains 0xFFFFFFF9 because a main program using MSP was suspended by SysTick. The BX LR instruction will automatically pull the remaining eight registers from the stack, and now the processor will be running the new thread.

```asm
SysTick_Handler                ; 1) Saves R0-R3,R12,LR,PC,PSR
    CPSID   I                  ; 2) Prevent interrupt during switch
    PUSH    {R4-R11}           ; 3) Save remaining regs r4-11
    LDR     R0, =RunPt         ; 4) R0=pointer to RunPt, old thread
    LDR     R1, [R0]           ;    R1 = RunPt
    STR     SP, [R1]           ; 5) Save SP into TCB
    LDR     R1, [R1,#4]        ; 6) R1 = RunPt->next
    STR     R1, [R0]           ;    RunPt = R1
    LDR     SP, [R1]           ; 7) new thread SP; SP = RunPt->sp;
    POP     {R4-R11}           ; 8) restore regs r4-11
    CPSIE   I                  ; 9) tasks run with interrupts enabled
    BX      LR                 ; 10) restore R0-R3,R12,LR,PC,PSR

```
*Program 2.14. Assembly code for the thread switcher.*

![Figure 2.16](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/f52cb62182a200c8563b92c077036fd3/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/roundRobin.gif)
*Figure 2.16. Three threads have their TCBs in a circular linked list. “**sp**” means this field is invalid for the one thread that is actually running.*

The first time a thread runs, the only registers that must be set are PC, SP, the T-bit in the PSR (T=1), and the I-bit in the PSR (I=0). For debugging purposes, we do initialize the other registers the first time each thread is run, but these other initial values do not matter. We learned this trick of setting the initial register value to the register number (e.g., R5 is initially 0x05050505) from Micrium uC/OS-II. Notice in this simple example, the first time `Task0` runs it will be executed as a result of StartOS. However, the first time `Task1` and `Task2` are run, it will be executed as a result of running the `SysTick_Handler`. In particular, the initial LR and PSR for `Task0` are set explicitly in StartOS, while the initial LR and PSR for Task1 and Task2 are defined in the initial stack set in SetInitialStack. An alternative approach to launching would have been to set the SP to the R4 field of its stack, set the LR to 0xFFFFFFF9 and jump to line 8 of the scheduler. Most commercial RTOS use this alternative approach because it makes it easier to change. But we decided to present this StartOS because we feel it is easier to understand the steps needed to launch.

--
--

###2.3.6. Profiling the OS

[Profiling the OS](https://youtu.be/323Y4JUbREM)

You can find this simple RTOS in the starter projects as RTOS_xxx, where xxx refers to the specific microcontroller on which the example was tested. Figures 2.17 and 2.18 show profiles of this RTOS at different time scales. We can estimate the thread switch time to be about 0.8 µs, because of the gap between the last edge on one pin to the first edge on the next pin. In this case because the thread switch occurs every 2 ms, the 0.8-µs thread-switch overhead is not significant.

You can find this simple RTOS in the starter projects as RTOS_xxx, where xxx refers to the specific microcontroller on which the example was tested. Figures 2.17 and 2.18 show profiles of this RTOS at different time scales. We can estimate the thread switch time to be about 0.8 µs, because of the gap between the last edge on one pin to the first edge on the next pin. In this case because the thread switch occurs every 2 ms, the 0.8-µs thread-switch overhead is not significant.
![Figure 2.17](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/4d09e1a194908a237aed2c4e748d4377/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_16LogicAnalyzer.jpg)
*Figure 2.17. The RTOS runs three threads by giving each a 2ms, measured in simulator for the TM4C123.*

![Figure 2.18](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/1248e98434b8675db79b41aca00746bb/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_17LogicAnalyzerZoomedIn.jpg)
*Figure 2.18. Profile showing the thread switch time is about 0.8 µs, measured in simulator for the TM4C123.*

--
--

###2.3.7. Linking assembly to C

[Linking assembly to C](https://youtu.be/KwdUSp-uh1s)

One of the limitations of the previous scheduler is that it’s written entirely in assembly. Although fast, assembly programming is hard to extend and hard to debug. One simple way to extend this round robin scheduler is to have the assembly SysTick ISR call a C function, as shown in Program 2.15. The purpose of the C function is to run the scheduler and update the RunPt with the thread to run next. You can find this simple RTOS as `RoundRobin_xxx`, where xxx refers to the specific microcontroller on which the example was tested.
```c
void Scheduler(void){
  RunPt = RunPt->next;  // Round Robin
}
```
*Program 2.15. Round robin scheduler written in C.*

The new SysTick ISR calls the C function in order to find the next thread to run, Program 2.16. We must save R0 and LR because these registers will not be preserved by the C function. IMPORT is an assembly pseudo-op to tell the assembler to find the address of Scheduler from the linker when all the files are being stitched together. Since this is an ISR, recall that LR contains 0xFFFFFFF9, signifying we are running an ISR. We had to save the LR before calling the function because the BL instruction uses LR to save its return address. The POP instruction restores LR to 0xFFFFFFF9. According to AAPCS, we need to push/pop an even number of registers (8-byte alignment) and functions are allowed to freely modify R0-R3, R12. For these two reasons, we also pushed and popped R0. Note that the other registers, R1,R2,R3 and R12 are of no consequence to us, so we don’t bother saving them.

```c
IMPORT Scheduler
SysTick_Handler                ; 1) Saves R0-R3,R12,LR,PC,PSR
    CPSID   I                  ; 2) Prevent interrupt during switch
    PUSH    {R4-R11}           ; 3) Save remaining regs r4-11
    LDR     R0, =RunPt         ; 4) R0=pointer to RunPt, old thread
    LDR     R1, [R0]           ;    R1 = RunPt
    STR     SP, [R1]           ; 5) Save SP into TCB
;    LDR     R1, [R1,#4]        ; 6) R1 = RunPt->next
;    STR     R1, [R0]           ;    RunPt = R1
    PUSH    {R0,LR}
    BL      Scheduler
    POP     {R0,LR}
    LDR     R1, [R0]           ; 6) R1 = RunPt, new thread
    LDR     SP, [R1]           ; 7) new thread SP; SP = RunPt->sp;
    POP     {R4-R11}           ; 8) restore regs r4-11
    CPSIE   I                  ; 9) tasks run with interrupts enabled
    BX      LR                 ; 10) restore R0-R3,R12,LR,PC,PSR

```

*Program 2.16. Assembly code for the thread switcher with call to the scheduler written in C.*

In this implementation, we are running the C function ___Scheduler___ with interrupts disabled. On one hand this is good because all read-modify-write operations to shared globals will execute atomically, and not create critical sections. On the other hand, since interrupts are disabled, it will delay other possibly more important interrupts from being served. Running with interrupts disabled will cause time jitter for periodic threads and latency for event-response threads. In Lab 2 we will manage this problem by running all the real-time tasks inside this Scheduler function itself.

--
--

###2.3.8. Periodic threads

[Periodic threads](https://youtu.be/ohN9lIUcKOM)

A very appropriate feature of a RTOS is scheduling periodic tasks. If the number of periodic tasks is small, the OS can assign a unique periodic hardware timer for each task. Another simple solution is to run the periodic tasks in the scheduler. For example, assume the thread switch is occurring every 1 ms, and we wish to run the function `PeriodicUserTask()` every 10 ms, then we could modify the scheduler as shown in Figure 2.19 and Program 2.17. Assume the OS initialized the counter to 0. In order for this OS to run properly, the time to execute the periodic task must be very short and always return. These periodic tasks cannot spin or block.

![Figure 2.19](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/f4bf72cbbc9aedd70ff31e9f3e7e023f/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_xx_realtimeThreads.jpg)
Figure 2.19. Simple mechanism to implement periodic event threads is to run them in the scheduler.
```c
uint32_t Counter;
#define NUM 10
void (*PeriodicTask1)(void);  // pointer to user function
void Scheduler(void){
  if((++Counter) == NUM){
    (*PeriodicTask1)();       // runs every NUM ms
    Counter = 0;
  }
  RunPt = RunPt->next;         // Round Robin scheduler
}
```

*Program 2.17. Round robin scheduler with periodic tasks.*

This approach has very little time jitter because SysTick interrupts occur at a fixed and accurate rate. The SysTick ISR calls the Scheduler, and then the Scheduler calls the user task. The execution delay from the SysTick trigger to the running of the user task is a constant, so the time between executions of the user task is fixed and exactly equal to the SysTick trigger period.

If there are multiple real-time periodic tasks to run, then you should schedule at most one of them during each SysTick ISR execution. This way the time to execute one periodic task will not affect the time jitter of the other periodic tasks. For example, assume the thread switch is occurring every 1 ms, and we wish to run `PeriodicUserTask1()` every 10 ms, and run `PeriodicUserTask2()` every 25 ms. In this simple approach, the period of each task must be a multiple of the thread switch period. I.e., the periodic tasks must be multiples of 1 ms. First, we find the least common multiple of 10 and 25, which is 50. We let the counter run from 0 to 49, and schedule the two tasks at the desired rates, but at non-overlapping times as illustrated in Program 2.18.
```c
uint32_t Counter;
void Scheduler(void){
  Counter = (Counter+1)%50; // 0 to 49
  if((Counter%10) == 1){ // 1, 11, 21, 31 and 41
    PeriodUserTask1();
  }
  if((Counter%25) == 0){ // 0 and 25
    PeriodUserTask2();
  }
  RunPt = RunPt->next; // Round Robin scheduler
}
```
*Program 2.18. Round robin scheduler with two periodic tasks.*

Consider a more difficult example, where we wish to run Task0 every 1 ms, Task1 every 1.5 ms and Task2 every 2 ms. In order to create non-overlapping executions, we will need a thread switch period faster than 1 kHz, so we don’t have to run Task0 every interrupt. So, let’s try working it out for 2 kHz, or 0.5 ms. The common multiple of 1, 1.5 and 2 is 6 ms. So we use a counter from 0 to 11, and try to schedule the three tasks. Start with Task0 running every other, and then try to schedule Task1 running every third. There is a conflict at 4 and 10.
```
    Task0: runs every 1 ms at counter values 0, 2, 4, 6, 8, and 10
    Task1: runs every 1.5 ms at counter values 1, 4, 7, and 10
```
So, let's try running faster at 4 kHz or every 0.25 ms. The common multiple is still 6 ms, but now the counter goes from 0 to 23. We can find a solution
```
    Task0: runs every 1 ms at counter values 0, 4, 8, 12, 16, and 20
    Task1: runs every 1.5 ms at counter values 1, 7, 13, and 19
    Task2: runs every 2 ms at counter values 2, 10, and 18
```
In order this system to operate, the maximum time to execute each task must be very short compared to the period used to switch threads.

--
--

###2.4.1. Introduction to semaphores

[](https://youtu.be/xL6clN8ymHs)

Remember that when an embedded system employs a real-time operating system to manage threads, typically this system combines multiple hardware/software objects to solve one dedicated problem. In other words, the components of an embedded system are tightly coupled. For example, in lab all threads together implement a personal fitness device. The fact that an embedded system has many components that combine to solve a single problem leads to the criteria that threads must have mechanisms to interact with each other. The fact that an embedded system may be deployed in safety-critical environments also implies that these interactions be effective and reliable.

We will use semaphores to implement synchronization, sharing and communication between threads. A semaphore is a counter with three functions: `OS_InitSemaphore`, `OS_Wait`, and `OS_Signal`. Initialization occurs once at the start, but wait and signal are called at run time to provide synchronization between threads. Other names for wait are pend and P (derived from the Dutch word proberen, which means to test). Other names for signal are post and V (derived from the Dutch word verhogen, which means to increment).

The concept of a semaphore was originally conceived by the Dutch computer scientist Edsger Dijkstra in 1965. He received many awards including the 1972 Turing Award. He was the Schlumberger Centennial Chair of Computer Sciences at The University of Texas at Austin from 1984 until 2000. Interestingly he was one of the early critics of the GOTO instruction in high-level languages. Partly due to his passion, structured programming languages like C, C++ and Java have almost completely replaced non-structured languages like BASIC, COBOL, and FORTRAN.

In this class we will develop three implementations of semaphores, but we will begin with the simplest implementation called “spin-lock” (Figure 2.20). Each semaphore has a counter. If the thread calls `OS_Wait` with the counter equal to zero it will “spin” (do nothing) until the counter goes above zero (Program 2.20). Once the counter is greater than zero, the counter is decremented, and the wait function returns. In this simple implementation, the `OS_Signal` just increments the counter. In the context of the previous round robin scheduler, a thread that is “spinning” will perform no useful work, but eventually will be suspended by the SysTick handler, and then other threads will execute. It is important to allow interrupts to occur while the thread is spinning so that the software does not hang. The read-modify-write operations on the counter, s, is a critical section. So the read-modify-write sequence must be made atomic, because the scheduler might switch threads in between any two instructions that execute with the interrupts enabled. Program 2.20 shows this simple implementation the semaphore functions, which we will use in Lab 2.

![Figure 2.20](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/17bf336c865415b69d439e2ac39dd48a/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig02_19SemaphoreFlowChart.jpg)
*Figure 2.20. Flowcharts of a spinlock counting semaphore.*

In the C implementation of spinlock semaphores, the tricky part is to guarantee all read-modify-write sequences are atomic. The while-loop reads the counter, which is always run with interrupts disabled. If the counter is greater than 0, it will decrement and store, such that the entire read-modify-write sequence is run with interrupts disabled. The while-loop must spend some time with interrupts enabled to allow other threads an opportunity to run, and hence these other threads have an opportunity to call signal.
```c
void OS_Wait(int32_t *s){
  DisableInterrupts();
  while((*s) == 0){
    EnableInterrupts(); // interrupts can occur here
    DisableInterrupts();
  }
  (*s) = (*s) - 1;
  EnableInterrupts();
} 
void OS_Signal(int32_t *s){
  DisableInterrupts();
  (*s) = (*s) + 1;
  EnableInterrupts();
}
```
*Program 2.20. A spinlock counting semaphore.*

####CHECKPOINT 2.12

What happens if we remove just the EnableInterrupts DisableInterrupts operations from while-loop of the spinlock OS_Wait?

The function OS_Wait will crash because it is spinning with interrupts disabled.

####CHECKPOINT 2.13

What happens if we remove all the DisableInterrupts EnableInterrupts operations from the spinlock OS_Wait?

The function OS_Wait has a critical section around the read-modify-write access to the semaphore. If we remove the mutual exclusion, multiple threads could pass.

Spinlock semaphores are inefficient, wasting processor time when they spin on a counter with a value of zero. In the subsequent chapters we will develop more complicated schemes to recover this lost time.

--
--

###2.4.2. Applications of semaphores

[Applications of semaphores](https://youtu.be/eCUkaqVwTnk)


When we use a semaphore, we usually can assign a meaning or significance to the counter value. In the first application we could use a semaphore as a lock so only one thread at a time has access to a shared object. Another name for this semaphore is ___mutex___, because it provides ___mutual exclusion___. If the semaphore is 1 it means the object is free. If the semaphore is 0 it means the object is busy being used by another thread. For this application the initial value of the semaphore (x) is 1, because the object is initially free. A thread calls `OS_Wait` to capture the object (decrement counter) and that same thread calls `OS_Signal` to release the object (increment counter).
```c
void Thread1(void){
  Init1();
  while(1){
    OS_Wait(&x); 
    // exclusive access to object
    OS_Signal(&x);
    // other processing
  }
}
void Thread2(void){
  Init2();
  while(1){
    OS_Wait(&x); 
    // exclusive access to object
    OS_Signal(&x);
    // other processing
  }
}
```

In second application we could use a semaphore for ___synchronization___. One example of this synchronization is a condition variable. If the semaphore is 0 it means an event has not yet happened, or things are not yet ok. If the semaphore is 1 it means the event has occurred and things are ok. For this application the initial value of the semaphore is 0, because the event is yet to occur. A thread calls `OS_Wait` to wait for the event (decrement counter) and another thread calls `OS_Signal` to signal that the event has occurred (increment counter). Let y be a semaphore with initial value of 0.
```c
void Thread1(void){
  Init1();
  OS_Wait(&y); // wait for event
      // event to occur
  while(1){
      // other processing
  }
}
void Thread2(void){
  Init2();
// this thread knows the event has occurred
  OS_Signal(&y); // signal event 
  while(1){
    // other processing
  }
}
```

--
--

###2.5.1. Resource sharing, nonreentrant code or mutual exclusion

[Thread Synchronization](https://youtu.be/qQO5-cfsgso)


This section can be used in two ways. First it provides a short introduction to the kinds of problems that can be solved using semaphores. In other words, if you have a problem similar to one of these examples, then you should consider a thread scheduler with semaphores as one possible implementation. Second, this section provides the basic approach to solving these particular problems. An important design step when using semaphores is to ascribe a meaning to each semaphore and a meaning to each value that semaphore can have.

The objective of this example is to share a common resource on a one at a time basis, also refered to as “mutually exclusive” fashion. The critical section (or vulnerable window) of nonreentrant software is that region that should only be executed by one thread at a time. As an example, the common resource we will consider is a display device (LCD). Mutual exclusion in this context means that once a thread has begun executing a set of LCD functions, then no other thread is allowed to use the LCD. See Program 2.21. In other words, whichever thread starts to output to the LCD first will be allowed to finish outputting. The thread that arrives second will simply wait for the first to finish. Both will be allowed to output to the LCD, however, they will do so on a one at a time basis. The mechanism to create mutual exclusion is to initialize the semaphore to 1, execute `OS_Wait` at the start of the critical section, and then execute `OS_Signal` at the end of the critical section. In this way, the information sent to one part of the LCD is not mixed with information sent to another part of the LCD.

```c
void Task2(void){ 
  Init2();
  while(1){
    Unrelated2();
    OS_Wait(&LCDmutex);
    BSP_LCD_PlotPoint(Data, COLOR);
    BSP_LCD_PlotIncrement();
    OS_Signal(&LCDmutex);
  }
}
```

```c
void Task5(void){ 
  Init5();
  while(1){
    Unrelated5();
    OS_Wait(&LCDmutex);
    BSP_LCD_SetCursor(5, 0);
    BSP_LCD_OutUDec4(Time/10,COLOR);
    BSP_LCD_SetCursor(5, 1); 
    BSP_LCD_OutUDec4(Steps,COLOR);
    BSP_LCD_SetCursor(16, 0);
    BSP_LCD_OutUFix2_1(TempData,COLOR);
    BSP_LCD_SetCursor(16, 1);
    BSP_LCD_OutUDec4(SoundRMS,COLOR);
     OS_Signal(&LCDmutex); 
  }
}
```

*Program 2.21. Semaphores used to implement mutual exclusion, simplified from usage in Lab 2.*

Initially, the semaphore is 1. If LCDmutex is 1, it means the LCD is free. If LCDmutex is 0, it means the LCD is busy and no thread is waiting. In this chapter, a thread that calls `OS_Wait` on a semaphore already 0 will wait until the semaphore becomes greater than 0. For a spinlock semaphore in this application, the possible values are only 0 (busy) or 1 (free). A semaphore that can only be 0 or 1 is called a ____binary semaphore____.


--
--

###2.5.2. Thread communication between two threads using a mailbox

[Thread communication](https://youtu.be/9Ex6y-lcvDw)

The objective of this example is to communicate between two main threads using a mailbox. In this first implementation both the producer and consumer are main threads, which are scheduled by the round robin thread scheduler (Program 2.22). The producer first generates data, and then it calls SendMail(). Consumer first calls `RecvMail()`, and then it processes the data. `Mail` is a shared global variable that is written by a producer thread and read by a consumer thread. In this way, data flows from the producer to the consumer. The `Send` semaphore allows the producer to tell the consumer that new mail is available. The `Ack` semaphore is a mechanism for the consumer to tell the producer, the mail was received. If `Send` is 0, it means the shared global does not have valid data. If `Send` is 1, it means the shared global does have valid data. If `Ack` is 0, it means the consumer has not yet read the global. If `Ack` is 1, it means the consumer has read the global. The sequence of operation depends on which thread arrives first. Initially, semaphores `Send` and `Ack` are both 0. Consider the case where the producer executes first.

    Execution                Mail    Send Ack  Comments
    Initially                none    0    0
    Producer sets Mail       valid   0    0    Producer gets here first
    Producer signals Send    valid   1    0
    Producer waits on Ack    valid   1    0    Producer spins because Ack =0
    Consumer waits on Send   valid   0    0    Returns immediately because Send was 1
    Consumer reads Mail      none    0    0    Reading once means Mail not valid
    Consumer signals Ack     none    0    1    Consumer continues to execute
    Producer finishes wait   none    0    0    Producer continues to execute

Consider the case where the consumer executes first.

    Execution                Mail    Send Ack  Comments
    Initially                none    0    0
    Consumer waits on send   none    0    0    Consumer spins because Send =0
    Producer sets Mail       valid   0    0    Producer gets here second
    Producer signals Send    valid   1    0
    Producer waits on Ack    valid   1    0    Producer spins because Ack =0
    Consumer finishes wait   valid   0    0    Consumer continues to execute
    Consumer reads Mail      none    0    0    Reading once means Mail not valid
    Consumer signals Ack     none    0    1    Consumer continues to execute
    Producer finishes wait   none    0    0    Producer continues to execute

```c
uint32_t Mail;  // shared data
int32_t Send=0; // semaphore
int32_t Ack=0;  // semaphore
```

```c
void SendMail(uint32_t data){
  Mail = data;
  OS_Signal(&Send);
  OS_Wait(&Ack);
}
void Producer(void){ 
  Init1();
  while(1){ uint32_t int myData;
    myData = MakeData();
    SendMail(myData);
    Unrelated1();
  }
}
```

```c
uint32_t RecvMail(void){ 
uint32_t theData;
  OS_Wait(&Send);
  theData = Mail; // read mail
  OS_Signal(&Ack);
  return theData;
} 
void Consumer(void){ 
  Init2();
  while(1){ uint32_t thisData;
    thisData = RecvMail();
    Unrelated2();
  }
}
```

*Program 2.22. Semaphores used to implement a mailbox. Both Producer and Consumer are main threads.*

Remember that only main threads can call `OS_Wait`, so the above implementation works only if both the producer and consumer are main threads. If producer is an event thread, it cannot call `OS_Wait`. For this scenario, we must remove the `Ack` semaphore and only use the `Send` semaphore (Program 2.24). Initially, the `Send` semaphore is 0. If `Send` is already 1 at the beginning of the producer, it means there is already unread data in the mailbox. In this situation, data will be lost. In this implementation, the error count, `Lost`, is incremented every time the producer calls `SendMail()` whenever the mailbox is already full.



```c
uint32_t Lost=0;
void SendMail(uint32_t data){
  Mail = data;
  if(Send){
    Lost++;
  }else{
    OS_Signal(&Send);
  }
}
void Producer(void){ 
  Init1();
  while(1){ uint32_t int myData;
    myData = MakeData();
    SendMail(myData);
    Unrelated1();
  }
}
```

```c
uint32_t RecvMail(void){ 
  OS_Wait(&Send);
  return Mail; // read mail
}


void Consumer(void){ 
  Init2();
  while(1){ uint32_t thisData;
    thisData = RecvMail();
    Unrelated2();
  }
}
```
*Program 2.24. Semaphores used to implement a mailbox. Producer is an event thread and Consumer is a main thread.*


####CHECKPOINT 2.14

There are many possible ways to handle the case where data is lost in Program 2.24. The code as written will destroy the old data, and the consumer will skip processing the old lost data. Modify Program 2.24 such that the system destroys the new data, and the consumer will skip processing the new data.
```c
Notice this function discards the new data on error
void SendMail(uint32_t int data){
  if(Send){
    Lost++; // discard new data
  }else{
    Mail = data;
    OS_Signal(&Send);
  }
}
```

A mailbox forces the producer and consumer to execute lock-step {producer, consumer, producer, consumer,…}. It also suffers from the potential to lose data. Both of these limitations will motivate the ___first in first out (FIFO) queue___ presented in the next chapter.

--
--

###About Lab 2

[Introduction to Lab 2](https://youtu.be/qKLcdf3tcDQ)

###OBJECTIVES
* Develop debugging skills using the Keil debugger and TExaS logic analyzer
* Understand the two existing round robin preemptive schedulers presented in Chapter 2
* Appreciate the distinction between real-time and non-real time tasks
* Develop a scheduler that runs two periodic event threads and four main threads
* Implement spin-lock semaphores and a mailbox

###OVERVIEW
We want you to understand how an RTOS works and demonstrate your understanding by completing a set of activities. The Lab 2 starter project using the LaunchPad and the Educational BoosterPack MKII (BOOSTXL-EDUMKII) is again a fitness device. However, the starter project will not execute until you implement a very simple RTOS. The user code inputs from the microphone, accelerometer, temperature sensor and switches. It performs some simple measurements and calculations of steps, sound intensity, and temperature. It outputs data to the LCD and it generates simple beeping sounds. Figure Lab2.1 shows the data flow graph of Lab 2. Your assignment is to first understand the concepts of the chapter in general and the projects RTOS_xxx and RoundRobin_xxx in specific. Your RTOS will run two periodic threads and four main threads. Sections 2.3.1 – 2.3.6 develops an RTOS that runs three main threads and your system must run four main threads. Section 2.3.7 explains how to extend the Scheduler() function so that it also runs periodic tasks. Section 2.4.2 explains how to implement spinlock semaphores.

![Figure 2.1](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/d026608aab917085868883d1f4280d84/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Lab2.1dataFlow.jpg)
*Figure 2.1. Data flow graph of Lab 2.*

This simple fitness device has six tasks: two periodic and four main threads. Since we have two periodic threads to schedule, we could have used interrupts on two hardware timers to run the real-time periodic threads. However, Lab 2 will run with just SysTick interrupts to run two the periodic threads and to switch between the four main threads. These are the six tasks:

However, Lab 2 will run with just SysTick interrupts to run two the periodic threads and to switch between the four main threads. These are the six tasks:

* Task0: event thread samples microphone input at 1000 Hz
* Task1: event thread samples acceleration input at 10 Hz
* Task2: main thread detecting steps and plotting at on LCD, runs about 10 Hz
* Task3: main thread inputs from switches, outputs to buzzer
* Task4: main thread measures temperature, runs about 1 Hz
* Task5: main thread output numerical data to LCD, runs about 1 Hz

Your RTOS manages these six tasks. We will use the same metrics as described as used in Lab 1, except jitter and error are only relevant for the two real-time event tasks:

* Minj = minimum ΔTj for Task j, j=0 to 5
* Maxj = maximum ΔTj for Task j, j=0 to 5
* Jitterj = Maxj - Minj for Task j, j=0 to 1
* Avej = Average ΔTj for Task j, j=0 to 5
* Errj = 100*( Avej - Δtj)/ Δtj for Task j, j=0 to 1

In addition to the above quantitative measures, you will be able to visualize the execution profile of the system using a logic analyzer. Each task in Lab 2 toggles both the virtual logic analyzer and a real logic analyzer when it starts. For example, Task0 calls `TExaS_Task0()`. The first parameter to the function `TExaS_Init()` will be GRADER or LOGICANALYZER. Just like Lab 1, calling `TExaS_Task0()` in grader mode performs the lab grading. However in logic analyzer mode, these calls implement the virtual logic analyzer and can be viewed with ___TExaSdisplay___. The TExaS logic analyzer should be used during debugging.


--
--
###Debugging Lab 2

####SPECIFICATIONS

A real-time system is one that guarantees the jitters are less than a desired threshold, and the averages are close to desired values. Now that we are using interrupts we expect the jitter for the two event tasks to be quite low. For the four main threads, you will be graded only on minimum, maximum, and average time between execution of tasks. Your assignment is implement the OS functions in OS.c and write the SysTick interrupt service routine in osasm.s. We do not expect you to edit the user code in Lab2.c, the board support package in BSP.c, or the interface specifications in profile.h, Texas.h, BSP.h, or OS.h. More specifically, we are asking you to develop and debug a real-time operating system, such that

* Task0: jitter between executions should be less than or equal to 15us
* Task1: jitter between executions should be less than or equal to 30us
* Task2: average time between executions should be 100 ms within 5%
* Task3: average time between executions should be less than 50 ms
* Task4: average time between executions should be less than 1.2 s
* Task5: average time between executions should be 1.0 s within 5%

![Figure 2.2](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/8b196f6e2907988092fc31fe87130085/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Lab2_1.jpg)
Figure 2.2. TExaS window showing a solution to Lab 2.

###APPROACH

Before you begin editing, downloading and debugging, we encourage you to first open up and run a couple of projects. The first project we recommend is ___RTOS_xxx___. This project implements a very simple real-time operating system as described in Sections 2.3.1 – 2.3.5. Make sure you understand function pointers and each line of the SysTick ISR.

[repeat of RTOS video in 2.3.6](https://youtu.be/323Y4JUbREM)

--

###Running round robin

Next, we encourage you should open up the project RoundRobin_xxx. This project extends the simple real time operating system so the `scheduler` is implemented in C as described in Section 2.3.6. Make sure you understand how the assembly code calls a C function. You will need to understand how the assembly code accesses the shared global, RunPt. Remember this approach will only work if the time to execute `Scheduler()` is very short compared to the time between SysTick interrupt triggers.

[Running round robin](https://youtu.be/QGdlkiuc0Cw)

--

###Running Lab 2 with the TExaS Logic Analyzer

[Running Lab 2 with the TExaS Logic Analyzer](https://youtu.be/76jcGUhfRTs)


--

###Running Lab2 with a real logic analyzer

[Running Lab2 with a real logic analyzer] (https://youtu.be/Q5Y-O8YFp4A)

Third, we encourage you should open up the project ___Lab2_xxx___ and fully understand the system from the user perspective by reading through Lab2.c. 
Lab2 requires both the ___LaunchPad___ and the ___Educational BoosterPack MKII___. Next, read through ___OS.c___ and OS.h to learn how your operating 
system will support the user system. Since this is a class on operating systems, and not personal fitness devices, we do not envision you modifying 
Lab2.c at all. Rather you are asked to implement the RTOS by writing code in the ___osasm.s___ and ___OS.c___ files.

To activate the logic analyzer, initialize TExaS with `TExaS_Init(LOGICANALYZER,1000)`; Do not worry about the number 1000; you will fill in a valid 
number once you are done with ___Lab 2___.

To activate the grader, initialize TExaS with `TExaS_Init(GRADER,1000)`; When you run the starter code in grading mode, you should see this output on 
___TExaSdisplay___. Note the numbers on the MSP432 running at 48 MHz will be slightly different than the numbers generated by the TM4C123 running at 80 MHz.

1. ___Step 1)___ Implement the three spin lock semaphore functions as defined in ___OS.c___ and ___OS.h___. For more information on semaphores review 
Section 2.4. Create a simple main program to test the functions.

```c
int32_t s1,s2;
int main(void){
  OS_InitSemaphore(&s1,0);
  OS_InitSemaphore(&s2,1);
  while(1){
    OS_Wait(&s2);   //now s1=0, s2=0
    OS_Signal(&s1); //now s1=1, s2=0
    OS_Signal(&s2); //now s1=1, s2=1
    OS_Signal(&s1); //now s1=2, s2=1
    OS_Wait(&s1);   //now s1=1, s2=1
    OS_Wait(&s1);   //now s1=0, s2=1
  }
}
```

2. ___Step 2)___ Implement the three mailbox functions as defined in OS.c and OS.h. Task1 is an event thread that calls `OS_MailBox_Send`. 
Therefore, your implementation of send cannot spin. In other words if Task1 sends data to the mailbox and the mailbox is already full 
that data will be lost. Create a simple main program to test the functions.

```c
uint32_t Out;
int main(void){ uint32_t in=0;
  OS_MailBox_Init);
  while(1){
    OS_MailBox_Send(in); 
    Out = OS_MailBox_Recv();
    in++;
  }
}
```

3. Step 3) The minimal set of functions you need to write to get the system running is
  * `SysTick_Handler` (without calling the C function and without running periodic threads)
  * `StartOS` 
  * `OS_Init`
  * `OS_AddThreads` (with just 3 threads for now)
  * `OS_Launch` 


Use this minimum OS to run ___Task3___ ___Task4___ and ___Task5___. In other words, for now we will not run Task0, Task1, and Task 2. To get it to compile you 
will have to change the prototype of `OS_AddThreads` to match the implementation in OS.c and the call in `main()`. In other words, this minimal OS 
runs three main threads and your Lab 2 will eventually run four main threads. Task5 will stall in `OS_Wait` because it has no data. However 
___Task3___ should respond to button pushes and ___Task4___ should measure temperature. You should hear the buzzer when you press a switch. You should 
be able to see these three tasks running on the TExaS logic analyzer, and you should be able to see global variables `PlotState` change with 
buttons, and you should notice that `TemperatureData` is set by Task4.

![Figure 2.3](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/2afa7ae7dc0f2653b1f39cbcac3bedfb/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Lab2_2.jpg)
*Figure 2.3. TExaS window showing a step 3 output. Notice only Tasks 3 and 4 will run.*


![Figure 2.4](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/601e8bc777d7b0bdb2d8b241b3718ab5/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Lab2_3.jpg)
*Figure 2.4. TExaS logic analyzer trace for a step 3 output. Notice only Tasks 3 and 4 will run. Task5 is running but it is stuck in OS_Wait.*

Step 4) Modify the system so the OS takes and runs four main threads and use it to run Task2 Task3 Task4 and Task5. Modify the SysTick ISR 
so it calls a C function and implement the round robin scheduler in C. You will add periodic threads in the next step. The behavior of 
Task3 Task4 and Task3 will be similar to step 3. In addition, Task2 will stall in the OS_Wait inside of OS_MailBox_Recv. The logic analyzer 
trace for step 4 will be similar to Figure 2.4, except Task 3 runs a little slower because the scheduler is running tasks 2,3,4,5.

![Figure 2.5](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/028a454260b881256490b41168014ba2/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Lab2_4.jpg)
*Figure 2.5. TExaS window showing a step 4 output. Tasks 2 and 5 are running but they are stuck in OS_Wait.*

5. Step 5) Modify the system to execute one of the periodic tasks. If you run ___Task0___, then ___Task5 will___ now run. If you 
run ___Task1___, then ___Task2___ will run.

![Figure 2.6](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/1e2c06c827fb3cf8702a51662e064478/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Lab2_5.jpg)
*Figure 2.6. TExaS window showing a step 5 output. Task 2 is running but it is stuck in OS_Wait.*

![Figure 2.7](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/32cecc62ad44ebee441b2c75c6421261/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Lab2_6.jpg)
*Figure 2.7. TExaS logic analyzer trace for a step 5 output. Only one periodic task is running (just Task 0 and not Task1)*

Hint: If I wished to run `Myfunction()` every 49ms, I could do this

```c
uint32_t TheTime=0;
void Scheduler(void){ // every 1ms time slice
  TheTime++;
  if(TheTime == 49){
    MyFunction(); // every 49ms
    TheTime = 0;
  }
  RunPt = RunPt->next; // Round Robin
}
```

6. Step 6) Modify the system to execute both periodic tasks.


--
--

###Lab 2 Grader

Grading your lab solution does require a ___LaunchPad___ development board. Your assignment is to implement a very simple RTOS that runs two 
event threads and four main threads. In particular, we are asking you to modify the main program, such that

* Task0 runs exactly every 1ms (jitter less than 15us)
* Task1 runs exactly every 100ms (jitter less than 30us)
* Task2 runs every 100ms (average within 5%)
* Task3 runs at least every 50ms (average less than 50ms)
* Task4 runs approximately every 1s (average less than 1.2s)
* Task5 runs every 1.0s (average within 5%)

We do expect you to execute Tasks 0 and 1 during the execution of the ___SysTick ISR___ (inside of Scheduler), so we expect the jitter on these two tasks to be small.

___Step 1___ Enter the `6554` number into your ___Lab2.c___ source code as the second parameter to `TExaS_Init`. In particular, make the call 
in `main()` look like:
```c
TExaS_Init(GRADER, 6554);
```
___Step 2___ Compile (build) your project in Keil, and download the code.

___Step 3___ Start TExaSdisplay and open the COM port.

___Step 4___ Start your software with the debugger or by hitting the reset on the LaunchPad. It takes about 10 seconds to collect the 
task profile data the grader needs. The logic analyzer is not available during grading. Wait until grading is finished. Any score 
above 70 will be considered a passing grade. If you are not satisfied with your score you are allowed multiple submissions.






