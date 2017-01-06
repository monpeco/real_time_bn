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

--
--

###5.1.2. Specifications

[Specifications](https://youtu.be/6wfwU8VIaeo)

There are many organizational approaches when designing a file system. As we make design decisions, it is appropriate to consider both quantitative and qualitative parameters. We can measure the effectiveness of a file system by

* Maximum file size
* Maximum number of files
* Speed to read data at a random position in the file
* Speed to read data in a sequential fashion
* Speed to write data into the file

--
--

###5.1.3. Fragmentation

[Fragmentation](https://youtu.be/uVGChlkK7No)

Internal fragmentation is storage that is allocated for the convenience of the operating system but contains no information. This space is wasted. Often this space is wasted in order to improve speed or to provide for a simpler implementation. The fragmentation is called "internal" because the wasted storage is inside the allocated region. In most file systems, whole sectors (or even clusters of sectors) are allocated to individual files, because this simplifies organization and makes it easier to grow files. Any space left over between the last byte of the file and the first byte of the next sector is a form of internal fragmentation called file slack or slack space. A file holding 26 bytes is allocated an entire sector capable of storing n bytes of data. However, only 26 of those locations contained data, so the remaining n-26 bytes can be considered internal fragmentation. The pointers and counters used by the OS to manage the file are not considered internal fragmentation, because even though the locations do not contain data, the space is not wasted. Whether or not to count the OS pointers and counters as internal fragmentation is a matter of debate. As is the case with most definitions, it is appropriate to document your working definition of internal fragmentation whenever presenting performance specifications to your customers.


![Figure 5.2](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/0964fe97053061ecdc23ac7800399c71/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_02_Fragmentation.jpg)
*Figure 5.2. The large block is the entire disk. There are four files on this disk. The rectangle on the left represents one file. Within the allocated space for this file there is data and there is some space in the allocated area that is not data. The space within the allocated area not used for data is internal fragmentation.*

Many compilers will align variables on a 32-bit boundary. If the size of a data structure is not divisible by 32 bits, it will skip memory bytes so the next variable is aligned onto a 32-bit boundary. This wasted space is also internal fragmentation.

####CHECKPOINT 5.1

If the sector size is n and the size of the files is randomly distributed, what is the average internal fragmentation?

On average, each file wastes ½ n bytes. Since this is inside the file, this wasted space is classified as internal fragmentation.


External fragmentation exists when the largest file that can be allocated is less than the total amount of free space on the disk. External fragmentation occurs in systems that require contiguous allocation, like a memory manager. External fragmentation would occur within a file system that allocated disk space in contiguous sectors. Over time, free storage becomes divided into many small pieces. It is a particular problem when an application allocates and deallocates regions of storage of varying sizes. The result is that, although free storage is available, it is effectively unusable because it is divided into pieces that are too small to satisfy the demands of the application. The term "external" refers to the fact that the unusable storage is outside the allocated regions.


![Figure 5.3](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/60bd1cc52294b777d970e457b367ba5d/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_03_Fragmentation.jpg)
*Figure 5.3. There are four files on this disk, and there are five sections of free space. The largest free space is less than the total free space, which is defined as external fragmentation.*

For example, assume we have a file system employing contiguous allocation. A new file with five sectors might be requested, but the largest contiguous chunk of free disk space is only three sectors long. Even if there are ten free sectors, those free sectors may be separated by allocated files, one still cannot allocate the requested file with five sectors, and the allocation request will fail. This is external fragmentation because there are ten free sectors but the largest file that can be allocated is three sectors.

####CHECKPOINT 5.2

Consider this analogy. You are given a piece of wood that is 10 meters long, and you are asked to cut it because you need one piece that is 2 meters long. What is the best way to cut the wood so there is no external fragmentation? Think of another way the wood could have been cut so the largest piece of free wood is smaller than the total free wood, creating external fragmentation?

The best way to cut the wood is obviously at the 2-meter spot, generating the 2-meter piece and leaving 8 meters free. If you were to cut at the 4-meter and 6-meter spots, you would indeed have the 2-meter piece as needed, but this cutting would leave you two 4-meter leftover pieces. The largest available piece now is 4 meters, but the total amount free would be 8 meters. This condition is classified as external fragmentation.

--
--

###5.2.1. Contiguous allocation

There are three components of the file system: the directory, allocation, and free-space management. This section introduces fundamental concepts and the next two sections present simple file systems. In this chapter, we define sector as a unit of storage. Whole sectors will be allocated to a file. In other words, we will not combine data from multiple files into a single sector.

We consider information in a file as a simple linear array of bytes. The "logical" address is considered as the index into this array. However, data must be placed at a "physical" location on the disk. An important task of the file system is to translate the logical address to the physical address.

![Figure 5.4](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/f2a73dc46b4d0899d35aeb089dc6b5c3/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_04_Fragmentation.jpg)
*Figure 5.4. Logical to physical address.*

[Contiguous Allocation](https://youtu.be/aNv6coHqNN4)

Contiguous allocation places the data for each file at consecutive sectors on the disk, as shown in Figure 5.5. Each directory entry contains the file name, the sector number of the first sector, the length in sectors. This method has similar problems as a memory manager. You could choose first-fit, best-fit, or worst-fit algorithms to manage storage. If the file can increase in size, either you can leave no extra space, and copy the file elsewhere if it expands, or you can leave extra space when creating a new file. Assuming the directory is in memory, it takes only one disk sector read to access any data in the file. A disadvantage of this method is you need to know the maximum file size when a file is created, and it will be difficult to grow the file size beyond its initial allocation.

![Figure 5.5](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/d102491b08953dcac62a4156bed63ede/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_05FileAllocation.jpg)
*Figure 5.5. A simple file system with contiguous allocation. Notice all the sectors of a file are physically next to each other.*

####CHECKPOINT 5.3

The disk in Figure 5.5 has 32 sectors with the directory occupying sector 0. The disk sector size is 512 bytes. What is the largest new file that can be created?

The largest contiguous part of the disk is 8 blocks. So the largest new file can have 8*512 bytes of data (4096 bytes). This is less than the available 16 free blocks, therefore there is external fragmentation.

####CHECKPOINT 5.4

You wish to allocate a new file requiring 1 sector on the disk in Figure 5.5. Using first-fit allocation, where would you put the file? Using best-fit allocation, where would you put the file? Using worst- fit allocation, where would you put the file?

First fit would put the file in block 1 (block 0 has the directory). Best fit would put the file in block 10, because it is the smallest free space that is big enough. Worst fit would put it in block 14, because it is the largest free space.

One of the tasks the file system must manage is free space. One simple scheme for free space management is a bit table. If the disk has n sectors, then we will maintain a table with n bits, assigning one bit for each sector. If the bit is 1, the corresponding sector is free, and if the bit is 0, the sector is used. Figure 5.5 shows a simple disk with 32 sectors. For this disk we could manage free space with one 32-bit number.

####CHECKPOINT 5.5

Assume the sector size is 4096 bytes and the disk is one gibibytes. How many bytes would it take to maintain a bit table for the free space?

A gibibyte is 2^30 bytes. Each sector is 2^12 bytes, so there are 2^18 sectors. So you need 2^18 bits in the table, one for each sector. There are 2^3 bits in a byte, so the table should be 2^15 (32768) bytes long.





