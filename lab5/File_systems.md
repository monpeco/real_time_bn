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

--
--

###5.2.2. Linked allocation

[Linked allocation](https://youtu.be/Tqo78RhMbGY)

Linked allocation places a sector pointer in each data sector containing the address of the next sector in the file, as shown in Figure 5.6. Each directory entry contains a file name and the sector number of the first sector. There needs to be a way to tell the end of a file. The directory could contain the file size, each sector could have a counter, or there could be an end-of-file marker in the data itself. Sometimes, there is also a pointer to the last sector, making it faster to add to the end of the file. Assuming the directory is in memory and the file is stored in N sectors, it takes on average N/2 disk-sector reads to access any random piece of data on the disk. Sequential reading and writing are efficient, and it also will be easy to append data at the end of the file.


![Figure 5.6](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/350643f5fd629d3f767a9628ac488898/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_06FileAllocation.jpg)
*Figure 5.6. A simple file system with linked allocation.*

####CHECKPOINT 5.6

If the disk holds 2 Gibibytes of data broken into 512-byte sectors, how many bits would it take to store the sector address?

2 Gibibytes is 2^31 bytes. 512 bytes is 2^9 bytes. 31-9 = 22, so it would take 22 bits to store the block number.

####CHECKPOINT 5.7

If the disk holds 2 Gibibytes of data broken into 32k-byte sectors, how many bits would it take to store the sector address?

2 Gibibytes is 2^31 bytes. 32k bytes is 2^15 bytes. 31-15 = 16, so it would take 16 bits to store the block number.

####CHECKPOINT 5.8

The disk in Figure 5.6 has 32 sectors with the directory occupying sector 0. The disk-sector size is 512 bytes. What is the largest new file that can be created? Is there external fragmentation?

There are 16 free blocks, they can all be linked together to create one new file. This means there is no external fragmentation.

####CHECKPOINT 5.9

How would you handle the situation where the number of bytes stored in a file is not an integer multiple of the number of data bytes that can be stored in each sector?

There are many answers. One answer is you could store a byte count in the directory. Another answer is you could store a byte count in each block. Example: the sector size is 512 bytes, and the file contains 1000 bytes. Option 1) We store the count=1000 in the directory. Now, we know the first sector has 512 bytes and the second sector has 488 bytes. In this case there are 24 bytes of wasted space, which we call internal fragmentation. Option 2) We store up 0 to 510 bytes of data in each sector plus a 2-byte count. The first sector has count=510 plus 510 bytes of data, the second sector has count=490 and 490 bytes of data. In this case there are 20 bytes of wasted space, which we call internal fragmentation.

We can also use the links to manage the free space, as shown in Figure 5.7. If the directory were lost, then all file information except the filenames could be recovered. Putting the number of the last sector into the directory with double-linked pointers improves recoverability. If one data sector were damaged, then remaining data sectors could be rechained, limiting the loss of information to the one damaged sector.

![Figure 5.7](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/10cf97d0ab8f07739ddae67670091c4b/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_07FileAllocation.jpg)
*Figure 5.7. A simple file system with linked allocation and free space management.*


--
--

###5.2.3. Indexed allocation

[Indexed allocation](https://youtu.be/-tLlyH2VJuc)

Indexed allocation uses an index table to keep track of which sectors are assigned to which files. Each directory entry contains a file name, an index for the first sector, and the total number of sectors, as shown in Figure 5.8. One implementation of indexed allocation places all pointers for all files on the disk together in one index table. Another implementation allocates a separate index table for each file. Often, this table is so large it is stored in several disk sectors. For example, if the sector number is a 16-bit number and the disk sector size is 512 bytes, then only 256 index values can be stored in one sector. Also for reliability, we can store multiple copies of the index on the disk. Typically, the entire index table is loaded into memory while the disk is in use. The RAM version of the table is stored onto the disk periodically and when the system is shut down. Indexed allocation is faster than linked allocation if we employ random access. If the index table is in RAM, then any data within the file can be found with just one sector read. One way to improve reliability is to employ both indexed and linked allocation. The indexed scheme is used for fast access, and the links can be used to rebuild the file structure after a disk failure.

![Figure 5.8](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/8590b3b862ad6b5f48af215dc3246de6/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_08FileAllocation.jpg)
*Figure 5.8. A simple file system with indexed allocation.*

####CHECKPOINT 5.10

If the sector number is a 16-bit number and the sector size is 512 bytes, what is the maximum disk size?

16+9=25. 2^25 is 32 Mebibytes, which is the largest possible disk.

####CHECKPOINT 5.11

A disk with indexed allocation has 2 GiB of storage. Each file has a separate index table, and that index occupies just one sector. The disk sector size is 1024 bytes. What is the largest file that can be created? Give two ways to change the file system to support larger files.

There are 2^31/2^10=2^21 blocks, so the 21-bit block address will be stored as a 32-bit number. One can store 1024/4=256 index entries in one 1024-byte block. So the maximum file size is 256*1024 = 2^8*2^10 = 2^18 = 256 kibibytes. You can increase the block size or store the index in multiple blocks.

####CHECKPOINT 5.12

This disk in Figure 5.8 has 32 sectors with the directory occupying sector 0 and the index table in sector 1. The disk-sector size is 512 bytes. What is the largest new file that can be created? Is there external fragmentation?

There are 15 free blocks, and they can create an index table using all the free blocks to create one new file. This means there is no external fragmentation.

--
--

###5.2.4. File allocation table (FAT)

[File allocation table (FAT)](https://youtu.be/LcvPoijFwS0)

The file allocation table (FAT) is a mixture of indexed and linked allocation, as shown in Figure 5.5. Each directory entry contains a file name and the sector number of the first sector. The FAT is just a table containing a linked list of sectors for each file. Figure 5.9 shows file A in sectors 10, 3, and 12. The directory has sector 10, which is the initial sector. The FAT contents at index 10 is a 3, so 3 is the second sector. The FAT contents at index 3 is a 12, so 12 is the third sector. The FAT contents at index 12 is a NULL, so there are no more sectors in file A.


![Figure 5.9](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/9537075c9f2d9499c4ca4a12ba19903f/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_09FileAllocation.jpg)
*Figure 5.9. A simple file system with a file allocation table.*

Many scientists classify FAT as a “linked” scheme, because it has links. However, other scientists call it an "indexed" scheme, because it has the speed advantage of an "indexed" scheme when the table for the entire disk is kept in main memory. If the directory and FAT are in memory, it takes just one disk read to access any data in a file. If the disk is very large, the FAT may be too large to fit in main memory. If the FAT is stored on the disk, then it will take 2 or 3 disk accesses to find an element within the file. The - in Figure 5.9 represent free sectors. In Figure 5.10, we can chain them together in the FAT to manage free space. Note, even though we will implement a FAT in Lab 5, we will not chain together the free sectors.

####CHECKPOINT 5.13

This disk in Figure 5.10 has 32 sectors with the directory occupying sector 0 and the FAT in sector 1. The disk sector size is 512 bytes. What is the largest new file that can be created? Is there an external fragmentation? 

There are 15 free blocks, they can create FAT using all the free blocks to create one new file. Each block is 512 bytes, so the largest file is 15 time 512 bytes; there is no external fragmentation.

![Figure 5.10](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/7c25c9036ab34bb286844fbfdd13b9df/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_10FileAllocation.jpg)
*Figure 5.10. The simple file system with a file allocation table showing the free-space management.*

In this section we use 0 to mean null pointer. Later in the chapter and in lab we will use 255 to mean null pointer. We use 0 in this section because this discussion is similar to the standard FAT16. However, in lab we need to use 255 because 255 is the value that occurs when the flash memory is erased.


--
--

###5.3.1. Flash memory

[Flash memory](https://youtu.be/zFn41DD8d9I)

In general, we can divide memory into volatile and nonvolatile categories. Volatile means it loses its data when power is removed and restored. Nonvolatile means it retains its data when power is removed and restored. There are many types of memory, but here are four of them

Volatile memory
Static random access memory, SRAM
Dynamic random access memory, DRAM
Nonvolatile memory
Flash electrically erasable programmable read only memory, EEPROM
Ferroelectric random access memory, FRAM
As you know data and the stack are allocated in RAM. DRAM has fewer transistors/bit compared to SRAM because it does require periodic refreshing. Most Cortex M microcontrollers use SRAM because of its simple technology and ability to operate on a wide range of bus frequencies. For random access memories, there is a size above which DRAM is more cost effective than SRAM. Dynamic random access memory (DRAM) is the type of memory found in most personal computers. Embedded devices like the Beaglebone and Raspberry Pi use DRAM.

Ferroelectric RAM (FRAM) is a random access memory similar to DRAM but uses a ferroelectric layer instead of a dielectric layer. The ferroelectric layer provides the non-volatility needed for program storage. Some new lines of microcontrollers use FRAM instead of flash EEPROM for their non-volatile storage. The MSP430FRxx microcontrollers from Texas Instruments use FRAM to store programs and data in one shared memory object. Other companies that produce FRAM microcontrollers include Fujitsu and Silicon Labs. FRAM requires less power usage, has a faster write, and provides a greater maximum number of write-erase cycles when compared to flash. When compared to flash, FRAMs have lower storage densities, smaller sizes, and higher cost.

Solid-state disks can be made from any nonvolatile memory, such as battery-backed RAM, FRAM, or flash EEPROM. Personal computers typically use disks made with magnetic storage media and moving parts. While this magnetic-media technology is acceptable for the personal computer because of its large storage size (greater than 1 Tibibyte) and low cost (less than $100 OEM), it is not appropriate for an embedded system because of its physical dimensions, electrical power requirements, noise, sensitivity to motion, and weight. Secure digital (SD) cards use Flash EEPROM together with interface logic to read and write data. For an embedded system we could create a file system using an SD card or using the internal flash of the microcontroller itself. SD cards are an effective approach when file storage needs exceed 128 kibibytes, because of the low cost and simple synchronous serial interface. However, in this chapter we will develop a very simple file system using the internal flash of the microcontroller. Other than the microcontroller itself, there will be no additional costs to developing this file system.

Smart phones, tablets, and cameras currently employ solid-state disks because of their small physical size and low power requirements. Unfortunately, solid-state disks have smaller storage sizes and higher cost/bit than the traditional magnetic storage disk. A typical 64-Gibibyte SD card costs less than $20. The cost/bit is therefore about $300/Tibibyte. In contrast, an 8-Tibibyte hard drive costs about $200 or $25/Tibibyte. The cost/bit of flash storage is expensive as compared to a traditional hard drive. However, there is a size point (e.g., below 128 Gibibyte), below which the overall cost of flash will be less than a traditional magnetic/motorized drive.

A flash memory cell uses two transistors; the gates of the two transistors are positioned gate to gate separated by an insulation layer as shown in Figure 5.11. Because each flash bit has only two transistors, the microcontroller can pack more flash bits into the chip as compared to SRAM or FRAM bits. A normal transistor has an input gate that is used to control conductance between the source and drain. However in a flash memory cell, one of the gates is floating, which means it is not connected to anything. If we trap charge on this floating gate, we define this state as value 0. If there is no trapped charge, we define the state as a 1. There are three operations we can perform on the cell.

If we place a large voltage on the control gate (Vcg), we can get any trapped charge to flow from the floating gate to the source below, hence erasing the cell, making its value equal to 1.

Conversely if we place a large voltage of the opposite polarity on the control gate, we can add charge to the floating gate, programming its value equal to 0. On the TM4C123 the smallest granularity with which we can erase is 1024 bytes. On the MSP432 we erase flash in blocks of 4096 bytes. However we can program individual words on most flash memories including the TM4C123 and MSP432. Once erased to a 1 or programmed to a 0, the charge or lack of charge remains on the floating gate even if power is removed from the system. Hence, this memory is nonvolatile. Data in the TM4C123 and MSP432 flash memories will remain valid for 20 years, and the memory will operate up to 100,000 erase/program cycles. Erasing and programming operations take a very long time compared to writing static RAM (SRAM). For example, it takes 8 to 15 ms to erase an entire 1024-byte page on the TM4C123. In contrast, writing 256 words in RAM on an 80-MHz Cortex-M takes 5 cycles/loop, which adds up to 1280 cycles or 16 µs.


![Figure 5.11](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/dda17a381ed2ed2639f0bba698120849/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_11_flash.jpg)
*Figure 5.11. The floating gate in a flash memory cell creates the storage.*

To read the value from flash, the control gate is activated. There is a threshold voltage for the control gate at which source-drain current (Id) flows if the bit is 0 and will not flow if the bit is 1. The threshold voltage is depicted as the dotted line in Figure 5.12.


![Figure 5.12](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/b7a50a77d6dc458b65e44bfd16d2c3d3/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_12_flash2.jpg)
*Figure 5.12. The trapped charge in the floating gate affects the relationship between control gate voltage and drain current.*

For more information on flash see http://computer.howstuffworks.com/flash-memory.htm For information on RAM memory see http://computer.howstuffworks.com/ram.htm

In summary:

* Flash memory cells have two transistors, so it is has very high density
* Nonvolatile behavior implemented as trapped/no charge on the floating gate
* We can erase an entire block (1k or 4k), making all bits 1
* We can program individual bytes/words, making bits 0 as needed
* Both erasing and programming are very slow compared to reading


--
--

###5.3.2. Flash device driver

[Flash device driver](https://youtu.be/HjaD5EhMFJ8)

In this chapter we will develop a file system using the internal flash storage of the microcontroller. Both the TM4C123 and MSP432 have 256 kibibytes of internal flash, existing from addresses 0 to 0x0003FFFF. Normally, we use the internal flash to save the machine code of our software. However, in this chapter we will allocate half of the flash, which is 128 kibibytes, to create a solid state disk. We divide the disk into sectors and operate on a sector by sector basis. Typically the sector size is a power of 2; let each sector be 2p bytes. This means we will partition the 217-byte disk into 2m sectors, where m+p=17. In general, there are three operations: we can erase (set bits to 1), program (set bits to 0), and read. The physical layer functions provide these basic operations. Program 5.1 shows the prototypes for the TM4C123. We do not need physical layer functions to read the flash, because once erased and programmed, software simply reads from the memory address in the usual manner. The TM4C123 is optimized for programming up to 128-byte (32-word) aligned “mass writes” or “fast writes”. The MSP432 implements this feature for up to 64-byte (16-word) arrays. The smallest block that we can erase on the TM4C123 is 1024 bytes. On the MSP432 we erase flash in blocks of 4096 bytes.

    <!-- language: lang-c -->
    //------------Flash_Erase------------
    // Erase 1 KB block of flash.
    // Input: addr 1-KB aligned flash memory address to erase
    // Output: 0 if successful, 1 if fail 
    int Flash_Erase(uint32_t addr);
    
    //------------Flash_Write------------
    // Write 32-bit data to flash at given address.
    // Input: addr 4-byte aligned flash memory address to write
    //        data 32-bit data
    // Output: 0 if successful, 1 if fail 
    int Flash_Write(uint32_t addr, uint32_t data);
    
    //------------Flash_WriteArray------------
    // Write an array of 32-bit data to flash starting at given address.
    // Input: source pointer to array of 32-bit data
    //        addr 4-byte aligned flash memory address to start writing
    //        count number of 32-bit writes
    // Output: number of successful writes; return value == count if successful
    // Note: at 80 MHz, it takes 678 usec to write 10 words
    int Flash_WriteArray(uint32_t *source, uint32_t addr, uint16_t count);

*Program 5.1. Prototypes for the physical layer functions to manage the flash (4-k erase for MSP432).*


--
--

###5.3.3. eDisk device driver

[eDisk device driver](https://youtu.be/U53iC5EEhNY)

We will add an abstraction level above the physical layer to create an object that behaves like a disk. In particular, we will use 128 kibibytes of flash at addresses 0x00020000 to 0x0003FFFF to create the solid state disk and partition the disk into 512-byte sectors. This abstraction will allow us to modify the physical layer without modifying the file system code. For example, we might change the physical layer to a secure digital card, to a battery-backed RAM, to an FRAM, or even to network storage.

On most disks, there is physical partitioning of the storage into blocks in order to optimize for speed. For example, the smallest block on the MSP432 that we can erase is 4 kibibytes, and on the TM4C123 the block size is 1 kibibyte. We will use the term block to mean a physical partition created by the hardware, and use the term sector (which can be 1 or more blocks) as a logical partition defined by the operating system. In a file system, we will partition the disk into sectors and allocate whole sectors to a single file. In other words, we will not store data from two files into the same sector. This all or nothing allocation scheme is used by most file systems, because it simplifies implementation.

If we were to implement a file system that allows users to erase, move, insert (grow) or remove (shrink) data in the files, then we would need to erase blocks dynamically. Because the smallest block on the MSP432 that we can erase is 4096 bytes, we would have to choose a sector size that is an integer multiple of 4k. On the TM4C123 smallest sector size would be 1k. Unfortunately, a disk made from the 128k of the flash with 4k-sectors would only have 32 sectors. 32 is such a small number the file system would be quite constrained.

The philosophy of this course has been to implement the simplest system that still exposes the fundamental concepts. Therefore, in this course we will develop a simple file system that does not allow the user to delete, move, grow, or shrink data in the files. It does however allow users to create files and write data to a file in increments of sectors. More specifically, when writing we will always append data to the end of the file. We call this simple approach as a write-once file system. We will erase the 128k flash once, and then program 0’s into the flash memory dynamically as it runs. Data logging and storage of debug information are applications of a write-once file system. For this simple file system, we can choose the sector size to be any size, because the flash is erased only once, and data is programmed as the user creates and writes sectors to the file. The size of the disk is 128 kibibytes, i.e., 217 bytes. If the sector size is 2n, then there will be 217-n sectors. For this system, if we were to use the fast write capabilities of the TM4C123 we could partition the 128 kibibyte disk as 1024 sectors with 128 bytes in each sector. Conversely, if we use the regular write function (Flash_WriteArray) then we could choose any sector size. In Lab 5 we will partition the disk into 256 sectors with 512 bytes per sector creating a file system where the sector address is an 8-bit number.

Program 5.2 shows the prototypes of the disk-level functions. You will implement these three functions in Lab 5. eDisk_Init() has no operations to perform in this system. It was added because other disks, like the SD card, will need initialization. For Lab 5, you could have eDisk_Init return zero if the drive parameter is 0 and return 1 if the drive parameter is not zero. Reading a sector requires an address translation. The function eDisk_ReadSector will copy 512 bytes from flash to RAM. The start of the disk is at flash address 0x00020000. Each sector is 512 bytes long, so the starting address of the sector is simply

     0x00020000 + 512*sector

Writing a sector requires the same address translation. The function eDisk_WriteSector will program 512 bytes from RAM into flash. In particular, it will do the address translation and call the function Flash_WriteArray. 512 bytes is 128 words, so the count parameter will be 128.

    <!-- language: lang-c -->
    //*************** eDisk_Init ***********
    // Initialize the interface between microcontroller and disk
    // Inputs: drive number (only drive 0 is supported)
    // Outputs: status
    //  RES_OK    0: Successful 
    //  RES_ERROR 1: Drive not initialized
    enum DRESULT eDisk_Init(uint32_t drive);
    
    //*************** eDisk_ReadSector ***********
    // Read 1 sector of 512 bytes from the disk, data goes to RAM
    // Inputs: pointer to an empty RAM buffer
    // sector number of disk to read: 0,1,2,...255
    // Outputs: result
    //  RES_OK     0: Successful 
    //  RES_ERROR  1: R/W Error 
    //  RES_WRPRT  2: Write Protected 
    //  RES_NOTRDY 3: Not Ready 
    //  RES_PARERR 4: Invalid Parameter 
    enum DRESULT eDisk_ReadSector(
            uint8_t *buff, // Pointer to a RAM buffer into which to store
            uint8_t sector); // sector number to read from
    
    //*************** eDisk_WriteSector ***********
    // Write 1 sector of 512 bytes of data to the disk, data comes from RAM
    // Inputs: pointer to RAM buffer with information
    // sector number of disk to write: 0,1,2,...,255
    // Outputs: result
    //  RES_OK     0: Successful 
    //  RES_ERROR  1: R/W Error 
    //  RES_WRPRT  2: Write Protected 
    //  RES_NOTRDY 3: Not Ready 
    //  RES_PARERR 4: Invalid Parameter 
    enum DRESULT eDisk_WriteSector(
            const uint8_t *buff, // Pointer to the data to be written 
            uint8_t sector); // sector number

*Program 5.2. Header file for the solid state disk device driver.*

--
--

###5.4.1. Directory

Although the file system described in this section is simple, this section is not the way we intend you to do Lab 5. The approach to Lab 5 will be presented in Section 5.5.

In this section, we develop a file system that would be appropriate for implementation with an SD card used for storage. In order to implement this file system, you would need to have physical layer eDisk driver functions for the SD card. There are a couple of projects for the TM4C123 that have implementations for this physical layer. The second example includes both a low-level eDisk and a high-level FAT16 file system for the SD card.
   http://users.ece.utexas.edu/~valvano/arm/SDC_4C123.zip   
   http://users.ece.utexas.edu/~valvano/arm/SDCFile_4C123.zip

The first component of the file system is the directory, as shown in Figure 5.13. In this system, the sector size is 512 bytes. In order to support disks larger than 32 Mebibytes, 32-bit sector pointers will be used. The directory contains a mapping between the symbolic filename and the physical address of the data. Specific information contained in the directory might include the filename, the number of the first sector containing data, and the total number of bytes stored in the file. One possible implementation places the directory in sector 0. In this simple system, all files are listed in this one directory (there are no subdirectories). There is one fixed-size directory entry for each file. A filename is stored as an ASCII string in an 8-byte array. A null-string (first byte 0) means no file. Since the directory itself is located in sector 0, zero can be used as a null-sector pointer. In this simple scheme, the entire directory must fit into sector 0, the maximum number of files can be calculated by dividing the sector size by the number of bytes used for each directory entry. In Figure 5.9, each directory entry is 16 bytes, so there can be up to 512/16 = 32 files. We will need one directory entry to manage the free space on the disk, so this disk format can have up to 31 files.


![Figure 5.13](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/8d6f7caae64cc701227f04d1881a2039/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/fig05_13_Simple.jpg)
*Figure 5.13. Directory for a linked file allocation with 512-byte sectors. Each file in the directory points to its first sector.*

Other information that one often finds in a directory entry includes a pointer to the last sector of the file, access rights, date of creation, date of last modification, and file type.

--
--

###5.4.2. Allocation

The second component of the file system is the logical-to-physical address translation. Logically, the data in the file are addressed in a simple linear fashion. The logical address ranges from the first to the last. There are many algorithms one could use to keep track of where all the data for a file belongs. This simple file system uses linked allocation as illustrated in Figure 5.6. Recall that the directory contains the sector number of the first sector containing data for the file. The start of every sector contains a link (the sector number) of the next sector, and a byte count (the number of data bytes in this sector). If the link is zero, this is last sector of the file. If the byte count is zero, this sector is empty (contains no data). Once the sector is full, the file must request a free sector (empty and not used by another file) to store more data. Linked allocation is effective for systems that employ sequential access. Sequential read access involves two functions similar to a magnetic tape: rewind (start at beginning) and read the next data. Sequential write access simply involves appending data to the end of the file. Figure 5.13 assumes the sector size is 512 bytes and the filename has up to 7 characters. The null-terminated ASCII string is allocated 8 bytes regardless of the size of the string. The sector pointer and the size entry (e.g., file ‘Ramesh’ has 519 bytes) each require 4 bytes (32 bits). Since each data sector has a 4-byte link and a 2-byte counter, each sector can store up to 506 bytes of data.

--
--

###5.4.3. Free space management

The third component of the file system is free-space management. Initially, all sectors except the one used for the directory are free and available for files to store data. To store data into a file, sectors must be allocated to the file. When a file is deleted, its sectors must be made available again. One simple free-space management technique uses linked allocation, similar to the way data is stored. Assume there are N sectors numbered from 0 to N-1. An empty file system is shown in Figure 5.14. Sector 0 contains the directory, and sectors 1 to N-1 are free. You could assign the last directory entry for free-space management. This entry is hidden from the user. E.g., this free-space file cannot be opened, printed, or deleted. It doesn't use any of the byte count fields, but it does use the links to access all of the free sectors. Initially, all of the sectors (except the directory itself) are linked together, with the special directory entry pointing to the first one and the last one having a null pointer.

![Figure 5.14](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/17d3da373b6aabf0446a0008992822be/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/fig05_14FAT.jpghttps://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/17d3da373b6aabf0446a0008992822be/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/fig05_14FAT.jpg)
*Figure 5.14. Free-space management.*

When a file requests a sector, it is unlinked from the free space and linked to the file. When a file is deleted, all of its sectors are linked to the free space again.


####CHECKPOINT 5.14

If the directory shown in Figure 5.13 allocated 6 bytes for the filename instead of 10, how many files could it support?

Each directory entry now requires 10 bytes. You could have 50 files, leaving some space for the free space management.


--
--

###5.5.1. Usage

####The material in Section 5.5 is how we intend you to implement Lab 5.

[Restrictions](https://youtu.be/yA-b0rLRB34)

Even though the previous approaches were indeed simple, we can simplify the file system even more if we make the following usage restrictions/specifications:

* The 128k flash memory is erased only once at the time of downloading the project;
* The act of erasing the entire flash is equivalent to "formatting" the disk;
* The disk is partitioned into 256 sectors of 512 bytes/sector;
* We can append data to a file but cannot delete data or files;
* We append data to a file in chunks of 512 bytes;
* We will read data in a sequential fashion;
* We assign file names as single 8-bit numbers (0 to 254);
* We limit the file system to a maximum of 255 files;
* We will mount (initialize the driver) the file system on startup;
* We will call flush (backup to disk) the file system before powering down.
* One sector will be reserved for the operating system to manage the directory and allocation scheme and the other 255 sectors will contain data. When the program is loaded into flash, the entire flash is erased. This erase event will serve to "format" the disk. All 255 data sectors will be free and the file system will have no files.

While using this disk we could have 255 individual files, each with one sector. We could have 51 files each with 5 sectors. Alternately, we could have one file with 255 sectors. Any combination is possible where the number of files is less than or equal to 255, and the total allocated sectors is also less than or equal to 255.

There will be a function, OS_File_New, which will return the file number of an empty file. This function will fail if there are no more files left, because there are already 254 files created, or if there are no free sectors, because the disk is full.

```c
//********OS_File_New*************
// Returns a file number of a new file for writing
// Inputs: none
// Outputs: number of a new file
// Errors: return 255 on failure or disk full
uint8_t OS_File_New(void);
```

To check the status of a file, we can call OS_File_Size. This function returns the number of sectors allocated to this file. If the size is zero, this is an empty file.

```c
//********OS_File_Size*************
// Check the size of this file
// Inputs: num, 8-bit file number, 0 to 254
// Outputs: 0 if empty, otherwise the number of sectors
// Errors: none
uint8_t OS_File_Size(uint8_t num);
```

To write data to an existing file we need to specify the file number into which we will store the data. The write data function will allocate another sector to the file and append 512 bytes of new data to the file. The input parameters to OS_File_Append are the file number and a sector of 512 bytes of data to write. This function will fail if there are no free sectors, because the disk is full.

```c
//********OS_File_Append*************
// Save 512 bytes into the file
// Inputs: num, 8-bit file number, 0 to 254
//         buf, pointer to 512 bytes of data
// Outputs: 0 if successful
// Errors: 255 on failure or disk full
uint8_t OS_File_Append(uint8_t num, uint8_t buf[512]);
```

To read data from a file we call OS_File_Read. The three parameters to this function are the file number, the location, and a pointer to RAM. The location parameter defines the logical address of the data in a file. Location 0 will access the first sector of the file. For example, if a file has 5 sectors, the location parameter could be 0, 1, 2, 3, or 4. The read data function will copy 512 bytes of data from the file into the RAM buffer. This function will fail if this file does not have data at this location.

```c
//********OS_File_Read*************
// Read 512 bytes from the file
// Inputs: num, 8-bit file number, 0 to 254
//         location, logical address, 0 to 254
//         buf, pointer to 512 empty spaces in RAM
// Outputs: 0 if successful
// Errors: 255 on failure because no data
uint8_t OS_File_Read(uint8_t num, uint8_t location,
      uint8_t buf[512]);
```

We will load into RAM versions of the directory and the FAT when the system starts. When we call OS_File_Flush the RAM versions will be stored onto the disk. Notice that due to the nature of how this file system is designed, bits in the directory and FAT never switch from 0 to 1. We can either call this function periodically or call it once just before the system is shut down.

```c
//********OS_File_Flush*************
// Update working buffers onto the disk
// Power can be removed after calling flush
// Inputs:  none
// Outputs: 0 if success
// Errors:  255 on disk write failure
uint8_t OS_File_Flush(void);
```

During the software download, the flash is erased. When the flash is erased, the disk in essence is formatted, because we defined the all ones state as empty. However, if one wishes to erase the entire disk removing all data and all files, one could call OS_File_Format. This function will erase the flash from 0x00020000 to 0x0003FFFF. You will not need to implement this function in Lab 5. However, on the TM4C123, it could be implemented by erasing all blocks from 0x00020000 to 0x0003FFFF. Notice that this implementation skips the eDisk layer and directly calls the physical layer.

```c
//********OS_File_Format*************
// Erase all files and all data
// Inputs: none
// Outputs: 0 if success
// Errors: 255 on disk write failure 
uint8_t OS_File_Format(void){
uint32_t address;
  address = 0x00020000; // start of disk
  while(address <= 0x00040000){
    Flash_Erase(address); // erase 1k block
    address = address+1024;
  }
}
```

####CHECKPOINT 5.15

The physical block size on the MSP432 is 4096 bytes. How would you modify OS_File_Format for the MSP432?

Change the 1024 to 4096.

--
--

###5.5.2. Allocation

[Allocation](https://youtu.be/vKp04oQHz54)

There are many possible solutions, but we choose FAT allocation because it supports appending to an existing file. FAT supports many small files or one large file. Because there are 256 sectors we will use 8-bit sector addresses. Because we will define a completely erased flash as “formatted”, we will use the sector address 255=0xFF to mean null-pointer, and use sector number 255 as the directory. To implement a FAT with this disk, we would need only 255 bytes. Since the sector is 512 bytes we can use 256 bytes for the directory and the other 256 bytes for the FAT. Notice that sectors are allocated to files, but never released. This means we can update the FAT multiple times because bits are all initially one (erased) and programmed to 0 once, and never need to be erased again.

Since the files are identified by number and not name, the directory need not store the name. Rather the directory is a simple list of 255 8-bit numbers, containing the sector number of its first sector. Notice there is exactly one directory entry for each possible file. If this sector number is 255, this file is empty. Similarly, the FAT is another simple list of 255 8-bit numbers. However, a 255 in the FAT may mean a free sector or the last sector of a file. Notice there is one entry in the FAT for each data sector on the disk. Figure 5.15 shows the disk after formatting. Each rectangle in the disk figure represents a 512-byte data sector. The directory and FAT are both stored in sector number 255.


![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/0e33335052841d37bcde483802a2fa1f/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_15WriteOnce.jpg)
*Figure 5.15. Empty disk on the write-once file system.*

If we ask for a new file, the system will return a number from 0 to 254 of a file that has not been written. In other words, OS_File_New will return the number of an empty file. If we execute the following when the disk is empty, n will be OS_File_New will return a 0 (n=0), and the eight calls to OS_File_Append will store eight sectors on the disk. In this example the variables n,m,p are simple global variables containing the file numbers we are using. The parameters buf0-buf9, dat0-dat4, arr0-2 represent RAM buffers with 512 bytes of data. Having 18 buffers we not to imply we needed a separate RAM buffer for every sector on the disk, but rather to differentiate where data is stored on the disk. In other words, the use of 18 different RAM buffers was meant to associate the 18 calls to OS_File_Append with the corresponding 18 sectors used on the disk. You will see in Lab 5, that we will only use just one or two RAM buffers.

```c
n = OS_File_New();
OS_File_Append(n,buf0);
OS_File_Append(n,buf1);
OS_File_Append(n,buf2);
OS_File_Append(n,buf3);
OS_File_Append(n,buf4);
OS_File_Append(n,buf5);
OS_File_Append(n,buf6);
OS_File_Append(n,buf7);
```

![Figure 5.16](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/f4481fdc6e9b235b6a76536ba5aaef8f/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_16WriteOnce.jpg)
*Figure 5.16. A disk with one file, this file has 8 sectors.*

If we were to continue this example and execute the following, there would now be 3 files on the disk occupying 18 sectors.

```c
m = OS_File_New();
OS_File_Append(m,dat0);
OS_File_Append(m,dat1);
OS_File_Append(m,dat2);
OS_File_Append(m,dat3);
p = OS_File_New();
OS_File_Append(p,arr0);
OS_File_Append(p,arr1);
OS_File_Append(n,buf8);
OS_File_Append(n,buf9);
OS_File_Append(p,arr2);
OS_File_Append(m,dat4);
```

![Figure 5.17](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/c59407259a6d6472b5efff8e9f516f78/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_17WriteOnce.jpg)
*Figure 5.17. A disk with three files, file 0 has 10 sectors, file 1 has 5 sectors and file 2 has 3 sectors.*

Notice that we limit usage to adding data to the disk is chunks of 512 bytes. As mentioned earlier we will never delete a file, nor will we delete parts of a file previously written. Furthermore, we always append to the end of a file, which means we never move data of a file from one place on the disk to another.


--
--


###5.5.3. Directory

[Directory](https://youtu.be/P6jgX_dJaLI)

We will read the directory/FAT into RAM on startup. We need to be able to write the directory to the disk multiple times. We will write the directory/FAT each time we close a file and before removing power. Figure 5.18 shows one possible implementation of the process to create a new file. This function will return the file number (0 to 254) of a file not yet written to.


![Figure 5.18](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/942bef1037fdae3e3dd913f3dc018c47/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig05_18_writeonce.jpg)
*Figure 5.18. Software flowchart for OS_File_New. Returning with a 255 means fail because the disk already has 254 files. The only way for this function to fail is if the disk has 254 files, and each file is one sector.*

This simple file system assumes you append some data after you create a new file and before you create a second new file. The following shows a proper use case of creating multiple files:

```c
n = OS_File_New();        // create a new file
OS_File_Append(n,stuff);  // add to n
m = OS_File_New();        // second file
OS_File_Append(m,other);  // add to m
```

If you violate this assumption and execute the following code, then files n and m will be one file. I.e., n will equal m.

```c
n = OS_File_New();        // create a new file
m = OS_File_New();        // second file
OS_File_Append(n,stuff);  // add to n
OS_File_Append(m,other);  // add to m
```






