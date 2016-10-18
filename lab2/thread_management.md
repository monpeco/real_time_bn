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




















