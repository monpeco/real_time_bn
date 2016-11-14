###Objectives

[Objectives](https://youtu.be/cdjWLQebQQE)

The objectives of Chapter 3 are implement:

Cooperation using suspend
* Blocking semaphores
* Data flow with first in first out queues
* Thread sleeping
* Periodic interrupts to manage periodic tasks

An important aspect of real-time systems is managing time, more specifically minimizing wastage of 
time through an idle busy-wait. Such busy-wait operations were used in our simple implementation 
of semaphores in the last chapter. In this chapter we will see how we can recover this wasted time.

###3.1.1. Spin-lock semaphore implementation with cooperation

[Spin lock with cooperation](https://youtu.be/ONJoOHG5cWs)

Sometimes a thread knows it can no longer make progress. If a thread wishes to cooperatively release 
control of the processor it can call ___OS_Suspend___, which will halt this thread and run another thread. 
Because all the threads work together to solve a single problem, adding cooperation at strategic 
places allows the system designer to greatly improve performance. When threads wish to suspend 
themselves, they call ___OS_Suspend___. Again, the SysTick ISR must be configured as a priority 7 interrupt 
so that it does not attempt to suspend any hardware ISRs that may be running. ___OS_Suspend___ can only be 
called by a main thread. Note that it is possible to force a SysTick interrupt by bypassing the normal 
"count to zero" event that causes it. To do this, we write a 1 to bit 26 of the ___INTCTRL___ register, 
which causes the SysTick interrupt. Writing zeros to the other bits of this register has no effect. 
This operation will set the ___Count___ flag in SysTick and the ISR will suspend the current thread, runs 
the ___SysTick_Handler___ (which calls the scheduler), and then launch another thread. In this first 
implementation, we will not reset the SysTick timer from interrupting normally (count to zero). 
Rather we simply inject another execution of the ISR. If we were 75% through the 1-ms time slice 
when OS_Suspend is called, this operation will suspend the current thread and grant the remaining 
0.25-ms time to the next thread. 

```c
void OS_Suspend(void){ 
  INTCTRL = 0x04000000; // trigger SysTick, but not reset timer
}
```

One way to make a spin-lock semaphore more efficient is to place a thread switch in the while loop 
as it is spinning, as shown on the right of Figure 3.1 and as Program 3.1. This way, if the semaphore 
is not available, the thread stops running. If there are n other running threads and the time slice 
is Δt, then the semaphore is checked every n*Δt, and very little processor time is wasted on the 
thread which cannot run. One way to suspend a thread is to trigger a SysTick interrupt.

![Figure 3.1](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/1fd9e4dabf2b6ead2efa9e44c01fec1a/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig03_01CooperativeSemaphore.jpg)
*Figure 3.1. Regular and efficient implementations of spinlock wait.*

```c
void OS_Suspend(void){ 
  INTCTRL = 0x04000000; // trigger SysTick
}
void OS_Wait(int32_t *s){
  DisableInterrupts();
  while((*s) == 0){
    EnableInterrupts();
    OS_Suspend(); // run thread switcher
    DisableInterrupts();
  }
  (*s) = (*s) - 1;
  EnableInterrupts(); }
```

*Program 3.1. A cooperative thread switch will occur if the software explicitly triggers a thread switch.*

####CHECKPOINT 3.1

Assume the thread scheduler switches threads every 1 ms without cooperation, and there are 5 total threads in the scheduler. If a thread needs to wait 1 second for its semaphore to be incremented, how much time will spinlock implementation waste, spinning in OS_Wait doing no useful work?

Each thread runs for 1ms, so each thread runs every 5ms. The spinning thread will be run 200 times, wasting 200ms while it waits for its semaphore to be signaled. This is a 20% waste of processor time.

####CHECKPOINT 3.2

Assume the thread scheduler switches threads every 1 ms, one thread is spinning in `OS_Wait` because its semaphore is 0, and there are 4 other running threads that are not spinning. Assuming `OS_Wait` is implemented like Program 3.1 with cooperation, how often is the loop in `OS_Wait` run?

Other threads run for 1 ms each, the semaphore is checked every 4 ms. However, the amount of time wasted will be quite small because the spinning thread will go through the loop once and suspend. Obviously, once the semaphore goes above 0, the `OS_Wait` will return.

The implementation in Program 3.1 did not reset the SysTick counter on a cooperative thread switch. So it is a little unfair for the thread that happens to be run next. However, in this implementation, since SysTick interrupts are still triggered every 1 ms, SysTick can be used to perform periodic tasks. Once we shift the running of periodic tasks to another timer ISR, we will be able to add a more fair implementation of suspend:

```c
void OS_Suspend(void){ 
  STCURRENT = 0;        // reset counter
  INTCTRL = 0x04000000; // trigger SysTick
}
```

Using this version of suspend, if we are 75% through the 1-ms time slice when `OS_Suspend` is called, this operation will suspend the current thread and grant a full 1-ms time to the next thread. We will be able to use this version of suspend once we move the periodic event threads away from SysTick and onto another periodic interrupt.

In particular, periodic event threads will be handled in Lab 3 using `BSP_PeriodicTask_Ini`t. This means the accurate running of event threads will not be disturbed by resetting the SysTick timer. Although you could use either version of `OS_Suspend` in Lab 3, resetting the counter will be fairer.


--
--

###3.1.2. Cooperative Scheduler


[Cooperative scheduler, show in cooperative project](https://youtu.be/EsFqmac7Esk)

In this section we will develop a 3-thread cooperative round-robin scheduler by letting the tasks suspend themselves by triggering a `SysTick` interrupt.

You can find this cooperative OS as `Cooperative_xxx`, where xxx refers to the specific microcontroller on which the example was tested, Program 3.2. Figure 3.2 shows a profile of this OS. We can estimate the thread switch time to be about 1 µs, because of the gap between the last edge on one pin to the first edge on the next pin. In this case, because the thread switch occurs every 1.3 µs, the 1-µs thread-switch overhead is significant. Even though SysTick interrupts are armed, the SysTick hardware never triggers an interrupt. Instead, each thread voluntarily suspends itself before the 1-ms interval.

```c
void Task0(void){
  Count0 = 0;
  while(1){
    Count0++;
    Profile_Toggle0(); // toggle bit
    OS_Suspend();
  }
}
void Task1(void){
  Count1 = 0;
  while(1){
    Count1++;
    Profile_Toggle1(); // toggle bit
    OS_Suspend();
  }
}
void Task2(void){
  Count2 = 0;
  while(1){
    Count2++;
    Profile_Toggle2(); // toggle bit
    OS_Suspend();
  }
}
```

*Program 3.2. User threads that use a cooperative scheduler.*

![Figure 3.2](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/614517dd9b26f393bf7b7413201552fe/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig03_02LogicAnalyzer.jpg)

Figure 3.2. The OS runs three threads; each thread volunteers to suspend running in simulation mode on the TM4C123. The three profile pins from Program 3.2 are measured versus time using a logic analyzer.

We did not add cooperation to Lab 2 because it would have disturbed the ability of SysTick to run periodic tasks. In other words, in Lab 2 we had to maintain SysTick periodic interrupts at exactly 1000 Hz or one every 1 ms. However, we will be able to add cooperation in Lab 3, once we schedule periodic tasks using a separate hardware timer generating its own high-priority interrupts using `BSP_PeriodicTask_Init` and/or `BSP_PeriodicTask_Init16`.

--
--

###3.2.1. The need for blocking

[Need for blocking *redo](https://youtu.be/a618ssg6zHg)

The basic idea of a ___blocking semaphore___ will be to prevent a thread from running (we say the thread is blocked) when the thread needs a resource that is unavailable. There are three reasons we will replace spin-lock semaphores with blocking semaphores. The first reason is an obvious ___inefficiency___ in having threads spin while there is nothing for them to do. Blocking semaphores will be a means to recapture this lost processing time. Essentially, with blocking semaphores, a thread will not run unless it has useful work it can accomplish. Even with spinlock/cooperation it is wasteful to launch a thread you know can’t run, only to suspend itself 10 μs later.

The second problem with spin-lock semaphores is a ___fairness___ issue. Consider the case with threads 1 2 3 running in round robin order. Assume thread 1 is the one calling Signal, and threads 2 and 3 call Wait. If threads 2 and 3 are both spinning waiting on the semaphore, and then thread 1 signals the semaphore, which thread (2 or 3) will be allowed to run? Because of its position in the 1 2 3 cycle, thread 2 will always capture the semaphore ahead of thread 3. It seems fair when the status of a resource goes from busy to available, that all threads waiting on the resource get equal chance. A similar problem exists in non-computing scenarios where fairness is achieved by issuing numbered tickets, creating queues, or having the customers sign a log when they enter the business looking for service. E.g., when waiting for a checkout clerk at the grocery store, we know to get in line, and we think it is unfair for pushy people to cut in line in front of us. We define bounded waiting as the condition where once a thread begins to wait on a resource (the call to OS_Wait does not return right away), there are a finite number of threads that will be allowed to proceed before this thread is allowed to proceed. Bounded waiting does not guarantee a minimum time before OS_Wait will return, it just guarantees a finite number of other threads will go before this thread. For example, it is holiday time, I want to mail a package to my mom, I walk into the post office and take a number, the number on the ticket is 251, I look up at the counter and the display shows 212, and I know there are 39 people ahead of me in line. We could implement bounded waiting with blocking semaphores by placing the blocked threads on a list, which is sorted by the order in which they blocked. When we wake up a thread off the blocked list, we wake up the one that has been waiting the longest. Note: none of the labs in this class will require you to implement bounded waiting. We introduce the concept of bounded waiting because it is a feature available in most commercial operating systems.

The third reason to develop blocking semaphores will be the desire to implement a ___priority thread scheduler___. In Labs 2 and 3, you implemented a round-robin scheduler and assumed each thread had equal importance. In Lab 4 you will create a priority scheduler that will run the highest priority thread that is ready to run. For example, if we have one high priority thread that is ready, we will run it over and over regardless of whether or not there are any lower priority threads ready. We will discuss the issues of starvation, aging, inversion and inheritance in Chapter 4. A priority scheduler will require the use of blocking semaphores. I.e., we cannot use a priority scheduler with spin-lock semaphores.

--
--

###3.2.2. The blocked state

[The blocked state *redo](https://youtu.be/dNCeNaXhE4k)


A thread is in the blocked state when it is waiting for some external event like input/output (keyboard input 
available, printer ready, I/O device available.) We will use semaphores to implement communication and 
synchronization, and it is semaphore function `OS_Wait` that will block a thread if it needs to wait. 
For example, if a thread communicates with other threads then it can be blocked waiting for an input 
message or waiting for another thread to be ready to accept its output message. If a thread wishes to 
output to the display, but another thread is currently outputting, then it will block. If a thread needs 
information from a FIFO (calls Get), then it will be blocked if the FIFO is empty (because it cannot 
retrieve any information.) Also, if a thread outputs information to a FIFO (calls Put), then it will 
be blocked if the FIFO is full (because it cannot save its information.) The semaphore function `OS_Signal` 
will be called when it is appropriate for the blocked thread to continue. For example, if a thread 
is blocked because it wanted to print and the printer was busy, it will be signaled when the printer 
is free. If a thread is blocked waiting on a message, it will be signaled when a message is available. 
Similarly, if a thread is blocked waiting on an empty FIFO, it will be signaled when new data are put 
into the FIFO. If a thread is blocked because it wanted to put into a FIFO and the FIFO was full, it 
will be signaled when another thread calls Get, freeing up space in the FIFO.

Figure 3.3 shows five threads. In this simple implementation of blocking we add a third field, called `blocked`, 
to the TCB structure, defining the status of the thread. The `RunPt` points to the TCB of the thread that 
is currently running. The next field is a pointer chaining all five TCBs into a circular linked list. 
Each TCB has a `StackPt` field. Recall that, if the thread is running it is using the real SP for its 
stack pointer. However, the other threads have their stack pointers saved in this field. The third field 
is a `blocked` field. If the blocked field is null, there are no resources preventing the thread from 
running. On the other hand, if a thread is blocked, the `blocked` field contains a pointer to the 
semaphore on which this thread is blocked. In Figure 3.3, we see threads 2 and 4 are blocked waiting 
for the resource (semaphore free). All five threads are in the circular linked list although only three 
of them will be run.

![Figure 3.3](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/0db84b5ad1383ba00b98db276ff9df4b/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/BlockingAnimation.gif)
*Figure 3.3. Threads 0, 1 and 3 are being run by the scheduler. Threads 2 and 4 are blocked on free and will not run until some thread signals free.*

In this simple approach, a main thread can only be blocked on one resource. In other words, when a thread calls `OS_Wait` on a semaphore with value 0 or less, that thread is blocked and stops running. Therefore, once blocked on one semaphore, it cannot block on a second semaphore. Figure 3.3 shows just one semaphore, but even when there are multiple semaphores, we need only one blocked field in the TCB. Since C considers zero as false and nonzero as true, the blocked field can also be considered as a Boolean, specifying whether or not the thread is blocked. This simple solution is adequate for systems with a small number of threads (e.g., less than 20).

Notice in this simple implementation we do not maintain a separate linked list of threads blocked on a specific semaphore. In particular, in Figure 3.3 we know threads 2 and 5 are blocked on the semaphore `free`, but we do not know which thread blocked first. The advantage of this implementation using one circular linked list data structure to hold the TCBs of all the threads will be speed and simplicity. Note that, we need to add threads to the TCB list only when created, and remove them from the TCB list if the thread kills itself. If a thread cannot run (blocked) we can signify this event by setting its blocked field like Figure 3.3 to point to the semaphore on which the thread is blocked.

In order to implement bounded waiting, we would have to create a separate blocked linked list for each reason why the thread cannot execute. For example, we could have one blocked list for threads waiting for the output display to be free, one list for threads waiting because a FIFO is full, and one lists for threads waiting because another FIFO is empty. In general, we will have one blocked list with each reason a thread might not be able to run. This approach will be efficient for systems with many threads (e.g., more than 20). These linked lists contain threads sorted in order of how long they have been waiting. To implement bounded waiting, when we signal a semaphore, we wake up the thread that has been waiting the longest. Note: none of the labs in this class will require you to implement bounded waiting.

In this more complex implementation, we unchain a TCB from the ready circular linked list when it is blocked. In this way a blocked thread will never run. We place the blocked TCBs on a linear linked list associated with the semaphore (the reason it was blocked). We can implement bounded waiting by putting blocked TCBs at the end of the list and waking up threads from the front of the list. There will be a separate linked list for every semaphore. This method is efficient when there are many threads that will be blocked at one time. The thread switching will be faster because the scheduler will only see threads that could run, and not have to look at blocked threads in the circular linked list. We do not expect you to unchain threads when blocked and rechain them when they wake up in any of the labs. We discuss it because this is how most commercial operating systems implement blocking.

--
--

###3.2.3. Implementation

[Implementation of blocking](https://youtu.be/AU0nLB7OaAk)

We will present the simple approach for implementing blocking semaphores, and we suggest you use this approach for Lab 3. Notice in Figure 3.4 that wait always decrements and signal always increments. This means the semaphore can become negative. In the example of using a semaphore to implement mutual exclusion, if free is 1, it means the resource is free. If free is 0, it means the resource is being used. If free is -1, it means one thread is using the resource and a second thread is blocked, waiting to use it. If free is -2, it means one thread is using the resource and two other threads are blocked, waiting to use it. In this simple implementation, the semaphore is a signed integer.

![Figure 3.4](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/1c80d8c0536698922593e5d691d0940d/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig03_04FlowchartBlockingSemaphore.jpg)
Figure 3.4. Flowcharts of a blocking counting semaphore.

This simple implementation of blocking is appropriate for systems with less than 20 threads. In this implementation, a blocked field is added to the TCB. The type of this field is a pointer to a semaphore. The semaphore itself remains a signed integer. If blocked is null, the thread is not blocked. If the blocked field contains a semaphore pointer, it is blocked on that semaphore. The “Block this thread” operation will set the blocked field to point to the semaphore, then suspend the thread.

```c
void OS_Wait(int32_t *s){
  DisableInterrupts();
  (*s) = (*s) - 1;
  if((*s) < 0){
    RunPt->blocked = s; // reason it is blocked
    EnableInterrupts();
    OS_Suspend();       // run thread switcher
  }
  EnableInterrupts();
}
```

The "Wakeup one thread" operation will be to search all the TCBs for first one that has a blocked field equal to the semaphore and wake it up by setting its blocked field to zero

```c
void OS_Signal(int32_t *s){
  tcbType *pt;
  DisableInterrupts();
  (*s) = (*s) + 1;
  if((*s) <= 0){
    pt = RunPt->next;   // search for a thread blocked on this semaphore
    while(pt->blocked != s){
      pt = pt->next;
    }
    pt->blocked = 0;    // wakeup this one
  }
  EnableInterrupts();
}
```

Notice in this implementation, calling the signal will not invoke a thread switch. During the thread switch, the OS searches the circular linked-list for a thread with a blocked field equal to zero (the woken up thread is a possible candidate). This simple implementation will not allow you to implement bounded waiting. You do not need to implement bounded waiting in any of the labs.

```c
void Scheduler(void){
  RunPt = RunPt->next;    // run next thread not blocked
  while(RunPt->blocked){  // skip if blocked
    RunPt = RunPt->next;
  } 
}
```

####CHECKPOINT 3.3

Assume the RTOS is running with a preemptive thread switch every 1 ms. Assume there are 8 threads in the TCB circular list, and 5 of the threads are blocked. Assume the while loop in the above Scheduler function takes 12 assembly instructions or 150ns to execute each time through the loop. What is the maximum time wasted in the scheduler looking at threads that are blocked? In other words, how much time could be saved by unchaining blocked threads from the TCB list?

The worst case is you must look at all 5 blocked threads, so the while loop executes 5 times. This is a waste of 5*150 = 750ns. Since the scheduler runs every 1 ms, this waste is 0.075% of processor time.

--
--

###3.2.4. Thread synchronization or rendezvous

The objective of this example is to synchronize Threads 1 and 2 (Program 3.3). In other words, whichever thread gets to this part of the code first will wait for the other. Initially semaphores S1 and S2 are both 0. The two threads are said to rendezvous at the code following the signal and wait calls. The rendezvous will cause thread 1 to execute Stuff1 at the same time (concurrently) as thread 2 executes its Stuff2. There are three scenarios the semaphores may experience and their significance is listed below:

   S1   S2    Meaning
   0    0     Neither thread has arrived at the rendezvous or both have passed
   -1   +1    Thread 2 arrived first and Thread 2 is blocked waiting for Thread 1
   +1   -1    Thread 1 arrived first and Thread 1 is blocked waiting for Thread 2


![Program 3.3](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/8aa1c7372c98d8dab1446013be67246f/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Program03_03.jpg) 
*Program 3.3. Semaphores used to implement rendezvous.*

--
--

###3.3.1. Producer/Consumer problem using a FIFO

First in first out (FIFO) queue![](https://youtu.be/nPk_qDLw7Z0)
*First in first out (FIFO) queue*

A common scenario in operating systems is where producer generates data and a consumer consumes/processes data. To decouple the producer and consumer from having to work in lock-step a buffer is used to store the data, so the producer thread can produce when it runs and as long as there is room in the buffer and the consumer thread can process data when it runs, as long as the buffer is non-empty. A common implementation of such a buffer is a FIFO which preserves the order of data, so that the first piece of data generated in the first consumed.

The first in first out circular queue (___FIFO___) is quite useful for implementing a buffered I/O interface (Figure 3.5). The function `Put` will store data in the FIFO, and the function `Get` will remove data. It operates in a first in first out manner, meaning the `Get` function will return/remove the oldest data. It can be used for both buffered input and buffered output. This order-preserving data structure temporarily saves data created by the source (producer) before it is processed by the sink (consumer). The class of FIFOs studied in this section will be statically allocated global structures. Because they are global variables, it means they will exist permanently and can be carefully shared by more than one program. The advantage of using a FIFO structure for a data flow problem is that we can decouple the producer and consumer threads. Without the FIFO we would have to produce one piece of data, then process it, produce another piece of data, then process it. With the FIFO, the producer thread can continue to produce data without having to wait for the consumer to finish processing the previous data. This decoupling can significantly improve system performance.

![Figure 3.5](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/fe52a6a9e4816a25722ddf175f615df3/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig03_05_FifoFlowDiagramb.jpg)
*Figure 3.5. The FIFO is used to buffer data between the producer and consumer. The number of data stored in the FIFO varies dynamically, where Put adds one data element and Get removes/returns one data element.*

You have probably already experienced the convenience of FIFOs. For example, a FIFO is used while streaming audio from the Internet. As sound data are received from the Internet they are stored (calls `Put`) into a FIFO. When the sound board needs data it calls `Get`. As long as the FIFO never becomes full or empty, the sound is played in a continuous manner. A FIFO is also used when you ask the computer to print a file. Rather than waiting for the actual printing to occur character by character, the print command will put the data in a FIFO. Whenever the printer is free, it will get data from the FIFO. The advantage of the FIFO is it allows you to continue to use your computer while the printing occurs in the background. To implement this magic, our RTOS must be able to manage FIFOs. There are many producer/consumer applications, as we previously listed in Table 2.1, where the processes on the left are producers that create or input data, while the processes on the right are consumers which process or output data.

FIFOs can be statically allocated, where the buffer size is fixed at compile time, Figure 3.6. This means the maximum number of elements that can be stored in the FIFO at any one time is determined at design time. Alternately, FIFOs can be dynamically allocated, where the OS allows the buffer to grow and shrink in size dynamically. To allow a buffer to grow and shrink, the system needs a memory manager or ___heap___. A heap allows the system to allocate, deallocate, and reallocate buffers in RAM dynamically. There are many memory managers (heaps), but the usual one available in C has these three functions. The function `malloc` creates a new buffer of a given size. The function `free` deallocates a buffer that is no longer needed. The function `realloc` allocates a new buffer, copies data from a previous buffer into the new buffer of different size, and then deallocates the previous buffer. realloc is the function needed to increase or decrease the allocated space for the FIFO statically-allocated FIFOs might result in lost data or reduced bandwidth compared to dynamic allocation.

![Figure 3.6](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/2ae4f4f355ed772965e93c4fb7822dca/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/FIFOallocation.gif)
*Figure 3.6. With static allocation, the maximum number of elements stored in the FIFO is fixed at compile time. With dynamic allocation, the system can call realloc when the FIFO is almost full to grow the size of the FIFO dynamically. Similarly, if the FIFO is almost empty, it can shrink the size freeing up memory.*

A system is considered to be ___deterministic___ if when the system is run with the same set of inputs, it produces identical responses. Most real-time systems often require deterministic behavior, because testing can be used to certify performance. Dynamically-allocated FIFOs cause the behavior of one subsystem (that might allocate large amounts of RAM from the heap) to affect behavior in another unrelated subsystem (our FIFO that wishes to increase buffer size). It is better for real-time systems to be reliable and verifiable than to have higher performance. As the heap runs, it can become fragmented; meaning the free memory in the heap has many little pieces, rather than a few big pieces. Since the time to reallocate a buffer can vary tremendously, depending on the fragmentation of the heap, it will be difficult to predict execution time for the FIFO functions. Since a statically allocated FIFO is simple, we will be able to predict execution behavior. For these reasons, we will restrict FIFO construction to static allocation. In other words, you should not use malloc and free in your RTOS.

There are many ways to implement a statically-allocated FIFO. We can use either two pointers or two indices to access the data in the FIFO. We can either use or not use a counter that specifies how many entries are currently stored in the FIFO. There are even hardware implementations. In this section we will present three implementations using semaphores.








