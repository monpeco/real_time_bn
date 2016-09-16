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


