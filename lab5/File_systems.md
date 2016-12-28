###Objectives


[Objectives](https://youtu.be/0NQGypSxUpU)

The objectives of chapter 5 are:

* Discuss how a real-time time systems uses a file system
* Define performance metrics
* Present the fundamentals of a file system:
    1. Directory
    2. Allocation
    3. Free space
* Discussion internal and external fragmentation
* Create a solid state disk by reading and writing sectors of flash memory
* Present software to erase and write into the internal flash EEPROM

We will begin this chapter with an introduction of file systems. In particular, we briefly present what is a file system, discuss how it will be used, develop performance metrics, present fundamental concepts, and then conclude with a couple of simple examples. In Lab 5 you will implement one of these simple file systems. In particular, you will design a very simple write-once file system

Embedded applications that might require disk storage include data acquisition, database systems, and signal generation systems. You can also use a disk in an embedded system to log debugging information.

---
---

###5.1.1. Usage

[File System Usage]()

A file system allows the software to store data and to retrieve previously stored data. Typically the size of the stored data exceeds available memory of the computer. Each file will have a name, with which we will use to access that file. In general, we can organize files into directories. However in this chapter we will restrict our file system implementations to a single directory containing all files. In general, file systems allow for these operations:

* Create a new file
* Write data to the file
    * Append to the end
    * Insert at an arbitrary location
* Read data from the file
    * Sequentially read from the beginning to the end
    * Read from an arbitrary location
* Erase the file

![Figure 5.1](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/9d9919ba71372e39f2f2da4be4baf592/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_01_FileSystem.jpg)
*Figure 5.1. A file system is used to store data.*

When designing a file system, it is important to know how it will be used. We must know if files will be erased. In particular, we can simplify how the disk is organized if we know files, once created, will never be destroyed.

For example, when recording and playing back sound and images, the data will be written and read in a sequential manner. We call this use pattern as sequential access. If we are logging recorded data, then we will need to append data at the end of a file but never change any data once logged. Conversely, an editor produces more of a random access pattern for data reading and writing. Furthermore, an editor requires data insertion and removal anywhere from the beginning to the end of a file. If the file is used as a data base, then the positions in the file where we read will be random (random access reading).

The reliability of the storage medium and the cost of lost information will also affect the design of a file system. For an embedded system we can improve reliability by selecting a more reliable storage medium or by deploying redundancy. For example we could write the same data into three independent disks, and when reading we read all three and return the median of the three data values.

So in general, we should first study the use cases in our system before choosing or designing the file system. In Lab 5, we will record fitness data onto the file system. Both writing and reading will be done sequentially, and files will never be deleted.

<!-- language: lang-cpp -->