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



