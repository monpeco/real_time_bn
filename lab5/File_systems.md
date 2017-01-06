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





