###Objectives

The objectives of this chapter are to:

Introduce communication systems for embedded systems
Present fundamentals of Bluetooth Low Energy (BLE)
Overview the features of the CC2650 Microcontroller
Define the network processor/application processor model
Implement communication between the embedded object and a smart phone

[Introduction to Communication Systems](https://youtu.be/HM7D9Pdyt28)

--
--

###6.1.1. Network Model

[Network Model](https://youtu.be/ti_keMbN5Dc)

A network is a collection of interfaces that share a physical medium and a data protocol. A network allows software tasks in one computer to communicate and synchronize with software tasks running on another computer. For an embedded system, the network provides a means for distributed computing, which is a means to combine multiple computers to solve a common problem. The topology of a network defines how the components are interconnected. ZigBee is a multi-hop wireless Personal Area Network (PAN) as shown in Figure 6.1. Notice that there can be multiple paths with which to route packets from source to destination. With Bluetooth we will pair two nodes and communicate data between the two.

![Figure 6.1](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/f3830a2b636d836304ad52fd45bff248/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_01_Zigbeenodes.jpg)
*Figure 6.1. ZigBee wireless networks communicate by hopping between nodes.*

The network provides the transfer of information as well as the mechanisms for process synchronization. It is convenient to visualize the network in a layered fashion. Figure 6.2 shows the structure of a controller area network (CAN) used in conjunction with a real-time operating system. At the highest level, we consider communication between users or tasks. At the lowest level, frames are transferred between I/O ports of the two (or more) computers along the physical link or hardware channel. At the next logical level, the operating system (OS) of one computer sends messages or packets to the OS on the other computer. The message protocol will specify the types and formats of these messages. Later in the chapter we will present the network protocol interface (NPI) that we will use to perform Bluetooth Low Energy (BLE) communication between two nodes.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/78b3ceea15825aaca77f976bec75ef56/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_02NetworkBlockDiagram.jpg)
*Figure 6.2. A layered approach to communication systems.*

Most networks provide an abstraction that hides low-level details from high-level operations. This abstraction is often described as layers. The International Standards Organization (ISO) defines a 7-layer model called the Open Systems Interconnection (OSI), as shown in Figure 6.3. It provides a standard way to classify network components and operations. The Physical layer includes connectors, bit formats, and a means to transfer energy. Examples include RS232, controller area network (CAN), modem V.35, T1, 10BASE-T, 100BASE-TX, DSL, and 802.11a/b/g/n PHY. The Data link layer includes error detection and control across a single link (single hop). Examples include 802.3 (Ethernet), 802.11a/b/g/n MAC/LLC, PPP, and Token Ring. The Network layer defines end-to-end multi-hop data communication. The Transport layer provides connections and may optimize network resources. The Session layer provides services for end-user applications such as data grouping and check points. The Presentation layer includes data formats, transformation services. The Application layer provides an interface between network and end-user programs.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/58e64a83dbc0012b7b625a6fad251bc1/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_03_OSIlayers.jpg)
*Figure 6.3. The Open Systems Interconnection model has seven layers.*

Observation: Communication systems often specify bandwidth in total bits/sec, but the important parameter is the data transfer rate.

Observation: Often the bandwidth is limited by the software and not the hardware channel.

Many embedded systems require the communication of command or data information to other modules at either a near or a remote location. A full duplex channel allows data to transfer in both directions at the same time. Ethernet, SPI, and UART implement full duplex communication. In a half duplex system, data can transfer in both directions but only in one direction at a time. Half duplex is popular because it is less expensive and allows the addition of more devices on the channel without change to the existing nodes. CAN, I2C, and most wireless protocols implement half-duplex communication. A simplex channel allows data to flow in only one direction.


####CHECKPOINT 6.1
####In which manner to most people communicate: simplex, half duplex or full duplex?
Most people communicate in half-duplex. Normally, when we are talking, the sound of our voice overwhelms our ears, so we usually cannot listen while we are talking.


--
--

###6.1.2. Physical Channel

[6.1.2. Physical Channel](https://youtu.be/NWo3JGqqHAI)

Information, such as text, sound, pictures and movies, can be encoded in digital form and transmitted across a channel, as shown in Figure 6.4. Channel capacity is defined as the maximum information per second it can transmit. In order to improve the effective bandwidth many communication systems will compress the information at the source, transmit the compressed version, and then decompress the data at the destination. Compression essentially removes redundant information in such a way that the decompressed data is identical (lossless) or slightly altered but similar enough (lossy). For example, a 400 pixels/inch photo compressed using the JPEG algorithm will be 5 to 30 times smaller than the original. A guided medium focuses the transmission energy into a well-defined path, such as current flowing along copper wire of a twisted pair cable, or light traveling along a fiber optic cable. Conversely, an unguided medium has no focus, and the energy field diffuses as in propagates, such as sound or EM fields in air or water. In general, for communication to occur, the transmitter must encode information as energy, the channel must allow the energy to move from transmitter to receiver, and the receiver must decode the energy back into the information, see Figure 6.4. In an analog communication system, energy can vary continuously in amplitude and time. A digital communication signal exists at a finite number of energy levels for discrete amounts of time. Along the way, the energy may be lost due to attenuation. For example, a simple V=I*R voltage drop is in actuality a loss of energy as electrical energy converted to thermal energy. A second example of attenuation is an RF cable splitter. For each splitter, there will be 50% attenuation, where half the energy goes left and the other half goes right through the splitter. Unguided media will have attenuation as the energy propagates in multiple directions. Attenuation causes the received energy to be lower in amplitude than the transmitted energy.

A second problem is distortion. The transfer gain and phase in the channel may be function of frequency, time, or amplitude. Distortion causes the received energy to be different shape than the transmitted energy.

A third problem is noise. The noise energy is combined with the information energy to create a new signal. White noise is an inherent or internally generated noise caused by thermal fluctuations. EM field noise is externally generated and is coupled or added into the system. Crosstalk is a problem where energy in one wire causes noise in an adjacent wire. We quantify noise with signal-to-noise ratio (SNR), which is the ratio of the information signal power to noise power.

![formu](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/2edf5c11281437005ac176394b6653bf/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/SNR.jpg)

####CHECKPOINT 6.2
####Why do we measure SNR as power and not voltage?
Since information is encoded as energy, and data is transferred at a fixed rate, each energy packet will exist for a finite time. Energy per time is power.

####CHECKPOINT 6.3
####Why do we always have a ratio of two signals whenever we use the log10 to calculate the amplitude of a signal?
If the units of a signal x is something like volts or watts, we cannot take the log10(x), because the units of log10(x) is not defined. Whenever we use the log10 to calculate the amplitude of a signal, we always perform the logarithm on a value without dimensions. In other words, we always perform the logarithm on a ratio of one signal to another.

Observation: Whenever we use the log10 to calculate the amplitude of a signal, we multiply by 10 if we have a ratio of two power signals or energy signals, and we multiply by 20 if we have a ratio of two voltage signals or current signals.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/cda0cbba7e477ea52fb1d8e06e5b2db8/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_04communication.jpg)
*Figure 6.4. Information is encoded as energy, but errors can occur during transmission.*

We can make an interesting analogy between time and space. A communication system allows us transfer information from position A to position B. A digital storage system allows us transfer information from time A to time B. Many of the concepts (encoding/decoding information as energy, signal to noise ratio, error detection/correction, security, and compression) apply in an analogous manner to both types of systems.

####CHECKPOINT 6.4
####We measure the performance of a communication system as bandwidth in bits/sec. What is the analogous performance measure of a digital storage system?
The performance measure for a storage system is information density in bits/cm3.

Errors can occur when communicating through a channel with attenuation, distortion and added noise. If the receiver detects an error, it can send a negative acknowledgement so the transmitter will retransmit the data. The CAN, ZigBee, and Bluetooth protocols handle this detection-retransmission process automatically. Networks based on the UARTs could define and implement error detection. For example, we can add an additional bit to the serial frame for the purpose of detecting errors. With even parity, the sum of the data bits plus the parity bit will be an even number. The framing error in the UART can also be used to signify the data may be corrupted. The CAN network sends a longitudinal redundancy check, LRC, which is the exclusive or of the bytes in the frame. The ZigBee network adds a checksum, which is the sum of all the data. The Network Processor Interface (NPI) that we will use in this chapter uses LRC.

There are many ways to improve transmission in the channel, reducing the probability of errors. The first design choice is the selection of the interface driver. For example RS422 is less likely to exhibit errors than RS232. Of course having a driver will be more reliable than not having a driver. The second consideration is the cable. Proper shielding can improve SNR. For example, Cat6 Ethernet cables have a separator between the four pairs of twisted wire, which reduce the crosstalk between lines as compared to Cat5e cable. If we can separate or eliminate the source of added noise, the SNR will improve. Reducing the distance and reducing the bandwidth often will reduce the probability of error. If we must transmit long distances, we can use a repeater, which accepts the input and retransmits the data again.

--
--

###6.1.3. Controller Area Network

[6.1.3. Controller Area Network](https://youtu.be/Q_bZWOvIGs8)

In this section, we will design and implement a Controller Area Network (CAN). CAN is a high-integrity serial data communications bus that is used for real-time applications, like automobiles and robots. It can operate at data rates of up to 1 Mbits/second, having excellent error detection and confinement capabilities. The CAN was originally developed by Robert Bosch for use in automobiles, and is now extensively used in industrial automation and control applications. The CAN protocol has been developed into an international standard for serial data communication, specifically the ISO 11989. Figure 6.5 shows the block diagram of a CAN system, which can have up to 112 nodes. There are four components of a CAN system. The first part is the CAN bus consisting of two wires (CANH, CANL) with 120Ω termination resistors on each end. Topologically, a CAN network consists of single 2-wire cable, with no branching. Each node taps into that cable. The second part is the Transceiver, which handles the voltage levels and interfacing the separate receive (RxD) and transmit (TxD) signals onto the CAN bus. The third part is the CAN controller, which is hardware built into the microcontroller, and it handles message timing, priority, error detection, and retransmission. The last part is software that handles the high-level functions of generating data to transmit and processing data received from other nodes.
    
![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/7ef11a38d5978e8cc2cd0d2ff872bad3/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_05_CANdiagram.jpg)
*Figure 6.5. Circuit Diagram of a TM4C123-Based CAN communication system.*

Each node consists of a microcontroller (with an internal CAN controller), and a transceiver that interfaces the CAN controller to the CAN bus. A transceiver is a device capable of transmitting and receiving on the same channel. The CAN is based on the "broadcast communication mechanism", which follows a message-based transmission protocol rather than an address-based protocol. The CAN provides two communication services: the sending of a message (data frame transmission) and the requesting of a message (remote transmission request). All other services such as error signaling, automatic retransmission of erroneous frames are user-transparent, which implies that the CAN interface automatically performs these functions. Some microcontrollers have an integrated CAN interface (e.g., the TM4C123 and TM4C1294 both have two CAN channels). The physical channel consists of two wires containing in differential mode one digital logic bit. Because multiple outputs are connected together, there must be a mechanism to resolve simultaneous requests for transmission. In a manner similar to open collector logic, there are dominant and recessive states on the transmitter, as shown in Figure 6.6. The outputs follow a wired-and mechanism in such a way that if one or more nodes are sending a dominant state, it will override any nodes attempting to send a recessive state.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/65a17468b08618d94648b2dcf071fb62/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_06_CANsignals.jpg)
*Figure 6.6. Voltage specifications for the recessive and dominant states.*

####CHECKPOINT 6.5
####Open collector outputs have two states: low and off. To create digital logic, one normally adds a passive resistor pullup so the signals are high (passive) and low (active). With open collector we can connect multiple outputs together. What are the dominant and recessive states in open collector logic?
If multiple open collector outputs are connected together, the low will dominate over HiZ. The signal will be low if any output is low, and the signal will be high only if all outputs are off.

The CAN transceiver is a high-speed, fault-tolerant device that serves as the interface between a CAN protocol controller (located in the microcontroller) and the physical bus. The transceiver is capable of driving the large current needed for the CAN bus and has electrical protection against defective stations. Typically each CAN node must have a device to convert the digital signals generated by a CAN controller to signals suitable for transmission over the bus cabling. The transceiver also provides a buffer between the CAN controller and the high-voltage spikes than can be generated on the CAN bus by outside sources. Examples of CAN transceiver chips include the Texas Instruments SN65HVD1050D, AMIS-30660 high speed CAN transceiver, ST Microelectronics L9615 transceiver, Philips Semiconductors AN96116 transceiver, and the Microchip MCP2551 transceiver. These transceivers have similar characteristics and would be equally suitable for implementing a CAN system.

In a CAN system, messages are identified by their contents rather by addresses. Each message sent on the bus has a unique identifier, which defines both the content and the priority of the message. This feature is especially important when several stations compete for bus access, a process called bus arbitration. As a result of the content-oriented addressing scheme, a high degree of system and configuration flexibility is achieved. It is easy to add stations to an existing CAN network.

Four message types or frames can be sent on a CAN bus. These include the Data Frame, the Remote Frame, the Error Frame, and the Overload Frame. This section will focus on the Data Frame, where the parts in standard format are shown in Figure 6.7. The Arbitration Field determines the priority of the message when two or more nodes are contending for the bus. For the Standard CAN 2.0A, it consists of an 11-bit identifier. For the Extended CAN 2.0B, there is a 29-bit Identifier. The identifier defines the type of data. Remote transmission request (RTR) bit will be dominant (0) for data frames and recessive (1) for remote request frames. The Control Field contains the DLC, which specifies the number of data bytes. The Data Field contains zero to eight bytes of data. The CRC Field contains a 15-bit checksum used for error detection. Any CAN controller that has been able to correctly receive this message sends an Acknowledgement bit at the end of each message. This bit is stored in the Acknowledge slot in the CAN data frame. The transmitter checks for the presence of this bit and if no acknowledge is received, the message is retransmitted. To transmit a message, the software must set the 11-bit Identifier, set the 4-bit DLC, and give the 0 to 8 bytes of data. The receivers can define filters on the identifier field, so only certain message types will be accepted. When a message is received the software can read the identifier, length, and data.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/e075118bbfbc8ad675d78905ad0c1301/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_07_CANframe.jpg)
*Figure 6.7. CAN Standard Format Data Frame.*

The Intermission Frame Space (IFS) separates one frame from the next. There are two factors that affect the number of bits in a CAN message frame. The ID (11 or 29 bits) and the Data fields (0, 8, 16, 24, 32, 40, 48, 56, or 64 bits) have variable length. The remaining components (36 bits) of the frame have fixed length including SOF (1), RTR (1), IDE/r1 (1), r0 (1), DLC (4), CRC (15), and ACK/EOF/intermission (13). For example, a Standard CAN 2.0A frame with two data bytes has 11+16+36 = 63 bits. Similarly, an Extended CAN 2.0B frame with four data bytes has 29+32+36 = 97 bits.

If a long sequence of 0’s or a long sequence of 1's is being transferred, the data line will be devoid of edges that the receiver needs to synchronize its clock to the transmitter. In this case, measures must be taken to ensure that the maximum permissible interval between two signal edges is not exceeded. Bit Stuffing can be utilized by inserting a complementary bit after five bits of equal value. Some CAN systems add stuff bits, where the number of stuff bits depends on the data transmitted. Assuming n is the number of data bytes (0 to 8), CAN 2.0A may add 3+n stuff bits and a CAN 2.0B may add 5+n stuff bits. Of course, the receiver has to un-stuff these bits to obtain the original data.

The cyclic redundancy check (CRC) is used for error detection. The CRC value is based on the remainder of a polynomial division of the contents.

The urgency of messages to be transmitted over the CAN network can vary greatly in a real-time system. Typically there are one or two activities that require high transmission rates or quick responses. Both bandwidth and response time are affected by message priority. Low priority messages may have to wait for the bus to be idle. There are two priorities occurring as the CANs transmit messages. The first priority is the 11-bit identifier, which is used by all the CAN controllers wishing to transmit a message on the bus. Message identifiers are specified during system design and cannot be altered dynamically. The 11-bit identifier with the lowest binary number has the highest priority. In order to resolve a bus access conflict, each node in the network observes the bus level bit by bit, a process known as bit-wise arbitration. In accordance with the wired-and-mechanism, the dominant state overwrites the recessive state. All nodes with recessive transmission but dominant observation immediately lose the competition for bus access and become receivers of the message with the higher priority, see Figure 6.8. They do not attempt transmission until the bus is available again. Transmission requests are hence handled according to their importance for the system as a whole. The second priority occurs locally, within each CAN node. When a node has multiple messages ready to be sent, it will send the highest priority messages first. Nodes can use filters to allow messages that are important and to block messages that are not important, see Figure 6.9.

Observation: It is confusing when designing systems that use a sophisticated I/O interface like the CAN to understand the difference between those activities automatically handled by the CAN hardware module and those activities your software must perform. The solution to this problem is to look at software examples to see exactly the kinds of tasks your software must perform.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/c01ea747f9f696cfdc18c45928af19e5/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_08_CANarbitration.gif)
*Figure 6.8. Arbitration where three nodes simultaneously attempt to send a frame. Node 1 sends ID=0x67F, Node 2 sends ID=0x659, and Node 3 sends ID=0x659. Node 2 has highest priority because it is sending the lowest ID and has RTR low. At bit 5, Node 1 recognizes it has lost arbitration and switches to listen mode. At the RTR, Node 3 recognizes it has lost arbitration and switches to listen mode. The remainder of the CAN message is only driven by Node 1.*

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/f6494c72a8c584e6a6f24dbeef4b37c9/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_09_CANbroadcast.gif)
*Figure 6.9. CAN network with Node 2 as the transmitter and other three nodes are listeners. Node 2 sends a CAN frame. The filters are set so Nodes 1 and 4 will receive the message and Node 3 will not receive it.*

For more information search the CAN in Automation web site http://www.can-cia.org/can-knowledge/

--
--

###6.1.4. Wireless Communication

[6.1.4. Wireless Communication](https://youtu.be/Yj0EJerf1i4)

The details of exactly how wireless communication operates are beyond the scope of this course. Nevertheless, the interfacing techniques presented in this book are sufficient to implement wireless communication by selecting a wireless module and interfacing it to the microcontroller. In general, one considers bandwidth, distance, topology and security when designing a wireless link. Bandwidth is the fundamental performance measure for a communication system. In this book, we define bandwidth of the system as the information transfer rate. However, when characterizing the physical channel, bandwidth can have many definitions. In general, the bandwidth of a channel is the range of frequencies passed by the channel (Communication Networks by Leon-Garcia). Let Gx(f) be the gain versus frequency of the channel. When considering EM fields transmitted across space, we can define absolute bandwidth as the frequency interval that contains all of the signal's frequencies. Half-power bandwidth is the interval between frequencies at which Gx(f) has dropped to half power ( 3dB). Let fc be the carrier frequency, and Px be the total signal power over all frequencies. The equivalent rectangular bandwidth is Px/Gx(fc). The null-to-null bandwidth is the frequency interval between first two nulls of Gx(f). The FCC defines fractional power containment bandwidth as the bandwidth with 0.5% of signal power above and below the band. The bounded power spectral density is the band defined so that everywhere outside Gx(f) must have fallen to a given level. The purpose of this list is to demonstrate to the reader that, when quoting performance data, we must give both definition of the parameter and the data. If we know the channel bandwidth W in Hz and the SNR, we can use the Shannon–Hartley Channel Capacity Theorem to estimate the maximum data transfer rate C in bits/s:

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/81efc08c8abb4f3367374a40c54c45d8/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/ShannonSNR.jpg)

For example, consider a telephone line with a bandwidth W of 3.4 kHz and SNR of 38 dB. The dimensionless SNR = 10(38/10) = 6310. Using the Channel Capacity Theorem, we calculate C = 3.4 kHz * log2(1 + 6310) = 43 kbits/s.



--
--

###6.1.5. Radio

[6.1.5. Radio](https://youtu.be/wOOiHYsT55M)

Figure 6.10 shows a rough image of various electromagnetic waves that exist from radio waves to gamma rays. Visible light constitutes a very small fraction, ranging from 430–770 THz. Bluetooth LE uses an even narrower range from 2.40 to 2.48 GHz, which exists in the microwave spectrum.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/e392067b757e48a9aa35b8e062ce0314/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_10_frequencySpectrum.jpg)
*Figure 6.10. Bluetooth communication occurs in the microwave band at about 2.4 GHz.*

Table 6.1 shows some general descriptions of the three major communication standards operating in this 2.4 GHz band. In this chapter we will focus on BLE.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/07677225e1e2f08b4be0ad3da237b6c7/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/table6_1.jpg)
*Table 6.1. Comparison between Wifi, Bluetooth, and ZigBee.*

Bluetooth LE could use any of the 40 narrow bands (LL 0 to 39) at 2.4 GHz; these bands are drawn as bumps in Figure 6.11. This figure also shows the Wifi channels, which exist as three wide bands of frequencies, called channel 1, 6 and 11. Because BLE coexists with regular Bluetooth and WiFi, BLE will avoid the frequencies used by other communication devices. LL channels 37, 38 and 39 are used to advertise, and LL channels 9-10, 21-23 and 33-36 are used for BLE communication. BLE has good performance in congested/noisy environments because it can hop from one frequency to another. Frequency Hopping Spread Spectrum (FHSS) rapidly switches the carrier among many frequency channels, using a pseudorandom sequence known to both transmitter and receiver. This way, interference will only affect some but not all communication.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/0db6a109b44f7d1febf89b1fc5d2b4bd/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_11_wifiChannels.jpg)
*Figure 6.11. The 2.4 GHz spectrum is divided into 40 narrow bands, numbered LL 0 to 39. Each band is ±1 MHz.*

Figure 6.12 illustrated the inverted F shape of the 2.4 GHz antenna used on the CC2650 LaunchPad. For more information on antenna layout, see http://www.ti.com/lit/an/swra351a/swra351a.pdf

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/9e9604d681a3ecf2659c954253049869/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_12_invertedF_antenna.jpg)
*Figure 6.12. One possible layout of the 2.4 GHz antenna.*

--
--

###CC2650 Solutions

###Introduction

[Introduction](https://youtu.be/SIH0b97UniY)

Bluetooth is wireless medium and a data protocol that connects devices together over a short distance. Examples of Bluetooth connectivity include headset to phone, speaker to computer, and fitness device to phone/computer. Bluetooth is an important component of billions of products on the market today. Bluetooth operates from 1 to 100 meters, depending on the strength of the radio. Most Bluetooth devices operate up to a maximum of 10 meters. However, in order to improve battery life, many devices reduce the strength of the radio, and therefore save power by operating across distances shorter than 10 meters. If the computer or phone provides a bridge to the internet, a Bluetooth-connected device becomes part of the Internet of Things (IoT).

Bluetooth is classified as a personal area network (PAN) because it implements communication within the range of an individual person. Alternatively, devices within a Bluetooth network are usually owned or controlled by one person. When two devices on the network are connected, we often say the devices are paired.

At the highest level, we see Bluetooth devices implement profiles. A profile is a suite of functionalities that support a certain type of communication. See https://en.wikipedia.org/wiki/List_of_Bluetooth_profiles. For example, the Advanced Audio Distribution Profile (A2DP) can be used to stream data. The Health Device Profile (HDP) is a standard profile for medical devices. There are profiles for remote controls, images, printers, cordless telephones, health devices, hands free devices, and intercoms. The profile we will use in this chapter is the generic attribute protocol (GATT). Within the GATT there can be once or more services. Table 6.2 shows some of the services that have been developed.

![Table 6.2](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/1e9839a91bb34605243267cc9f9a3da2/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/table6_2.jpg)
*Table 6.2. Adopted GATT services, https://www.bluetooth.com/specifications/gatt/services*

Within a service there may be one or more characteristics. A characteristic is user or application data that is transmitted from one device to another across the network. One of the attributes of a characteristic is whether it is readable, writeable, or both. We will use the notify indication to stream data from the embedded object to the smart phone. Characteristics have a universally unique identifier (UUID), which is a 128-bit (16-byte) number that is unique. BLE can use either 16-bit or 32-bit UUIDs. A specific UUID is used within the network to identify a specific characteristic. Often a characteristic has one or more descriptors. Descriptors may be information like its name and its units. We will also see handles, which are a mechanism to identify characteristics within the device. A handle is a pointer to an internal data structure within the GATT that contains all the information about that characteristic. Handles are not passed across the Bluetooth network; rather, handles are used by the host and controller to keep track of characteristics. UUIDs are passed across the network. Figure 6.13 shows a GATT service with seven characteristics.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/4a249ac17964a21bf54d26daf05b2788/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_13_GATTservice.jpg)
*Figure 6.13. A GATT profile implements services, and a service has one or more characteristics.*

--
--

###6.2.1. Bluetooth Protocol Stack

[Bluetooth Protocol Stack](https://youtu.be/uwk3gKY9DfA)

The BLE protocol stack includes a controller and a host, as shown in Figure 6.14. Bluetooth BR (basic rate), Bluetooth EDR (enhanced data rate), and Bluetooth LE (low energy) all separate the controller and host as different layers and are often implemented separately. The user application and operating system sit on top of the host layer. This section is a brief overview of BLE. For more information on HCI, www.ti.com/ble-wiki and www.ti.com/ble-stack.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/842adf807b71b6926c296346dd5ee2f0/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_14_BLEstack.jpg)
*Figure 6.14. The BLE stack. These layers are implemented inside the CC2650. The physical layer includes the antenna, which is outside the CC2650.*

The physical layer (PHY) is a 1Mbps adaptive frequency-hopping GFSK (Gaussian Frequency-Shift Keying) radio operating in the unlicensed 2.4 GHz ISM (Industrial, Scientific, and Medical) band.

The link layer (LL) controls the radiofrequency state of the device. The device can be in one of five states: standby, advertising, scanning, initiating, or connected. Advertisers transmit data without being in a connection, while scanners listen for advertisers. An Initiator is a device that is responding to an Advertiser with a connection request. If the Advertiser accepts, both the advertiser and initiator will enter a connected state. When a device is in a connection, it will be connected in one of two roles master or slave. The device that initiated the connection becomes the master, and the device that accepted the request becomes the slave. In Lab 6, the embedded system will be an advertiser and the smart phone will be the initiator.

The host control interface (HCI) layer provides a means of communication between the host and controller via a standardized interface. Standard HCI commands and events are specified in the Bluetooth Core Spec. The HCI layer is a thin layer which transports commands and events between the host and controller. In Lab 6, the HCI is implemented has function calls and callbacks within the CC2650 controller.

The link logical control and adaption protocol (L2CAP) layer provides data encapsulation services to the upper layers, allowing for logical end-to-end communication of data. The security manager (SM) layer defines the methods for pairing and key distribution, and provides functions for the other layers of the protocol stack to securely connect and exchange data with another device. The generic access protocol (GAP) layer handles the connection and security. In Lab 6, you will configure the GAP to setup and initiate advertisement. We will use the GAP to connect our embedded system to a smart phone.

The overriding theme of Bluetooth communication is the exchange of data between paired devices. A service is a mechanism to exchange data. A collection of services is a profile. The generic attribute profile (GATT) handles services and profiles. The attribute protocol (ATT) layer protocol allows a device to expose “attributes” to other devices. All data communications that occur between two devices in a BLE connection are handled through the GATT.

The first step for our embedded device to perform is to configure and start advertisement, see Figure 6.15. In advertisement mode the device sends out periodic notifications of its existence and its willingness to connect. Another device, such as a smart phone, scans the area for possible devices. If desired this device can request a connection. If the advertiser accepts, both devices enter a connected phase, where the embedded device will be the slave (server) and the initiator becomes the master (client).

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/7d7c6306d5ac01a2798dc447cb38aff9/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_15_BLEconnection.jpg)
*Figure 6.15. BLE connection steps.*

In order to save power, the device spends most the time sleeping. The master sends out periodic requests to communicate. If the slave wishes to communicate, the master and slave will exchange data during this connection event. Figure 6.16 plots the device current verses time. This graph shows most of the current draw occurs during the connection events. The embedded device can save power by reducing the period of the connection events or by choosing not to participate in all the events.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/a14ed1dd3c88f1f48a4c9842567dab8b/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_16_BLEconnectionEvents.jpg)
*Figure 6.16. CC2650 current verses time, showing the connection events.*

For example, you will see the advertising interval parameter in the NPI_StartAdvertisement message. In particular, the example projects set the advertising interval to 62.5ms.

--
--

###6.2.2. Client-server Paradigm

[Client-server](https://youtu.be/9tf9W2od5zc)

The client-server paradigm is the dominant communication pattern for network protocols, see Figure 16.17. In Lab 6, the embedded system will be the server, and the smart phone will be the client. The client can request information from the server, or the client can send data to the server. With Bluetooth this exchange of data is managed by the services and profiles, discussed in the next section. There are four main profile types.

A peripheral device has sensors and actuators. On startup it advertises as connectable, and once connected it acts as a slave. The embedded device in Lab 6 will be a peripheral.

A central device has intelligence to manage the system. On startup it scans for advertisements and initiates connections. Once connected it acts as the master. The smart phone in Lab 6 will be a central device.

A broadcaster has sensors collecting information that is generally relevant. On startup it advertises but is not connectable. Other devices in the vicinity can read this information even though they cannot connect to the broadcaster. An example is a thermometer.

An observer can scan for advertisements but cannot initiate a connection. An example is a temperature display device that shows temperatures measured by broadcasters.

![Figure 6.17](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/7f6386d673079e7a0c956a1939ca95a5/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_17_clientServer.jpg)
*Figure 6.17. Client-server Paradigm.*

Read indication. When the client wishes to know the value of a characteristic, it will issue a read indication. Inside the request will be a universally unique identifier (UUID) that specifies which characteristic is desired. The server will respond with the value by returning a read confirmation. The data may be one or more bytes. For large amounts of data, the response could be broken into multiple messages. In the example projects, the data will be 1, 2 or 4 bytes long. The size of the data is determined during initialization as the characteristic is configured.

Write indication. When the client wishes to set the value of a characteristic, it will issue a write indication. This request will include data. The request will also include a UUID that specifies to which characteristic the data should be written. The server will respond with an acknowledgement, called a write confirmation.

Notify request. When the client wishes to keep up to data on a certain value in the server, it will issue a notify request. The request includes a UUID. The server will respond with an acknowledgement, and then the server will stream data. This streaming could occur periodically, or it could occur whenever the value changes. In the example projects, notify indication messages are sent from server to client periodically. The client can start notification (listen command on the phone) or stop notifications.

--
--

###6.3.1. CC2650 Microcontroller

[CC2650](https://youtu.be/i6BRqb2SCyo)

There are three controllers on the CC2650: a main CPU, an RF core, and a sensor controller. Together, these combine to create a one-chip solution for Bluetooth applications. The main CPU includes 128kB of flash, 20kB of SRAM, and a full range of peripherals. Typically, the ARM Cortex-M3 processor handles the application layer and BLE protocol stack. However, in this chapter, we will place the application layer on another processor and use the CC2560 just to implement Bluetooth.

The RF Core contains an ARM Cortex-M0 processor that interfaces the analog RF and base-band circuitries, handles data to and from the system side, and assembles the information bits in a given packet structure. The RF core offers a high level, command-based API to the main CPU. The RF core is capable of autonomously handling the time-critical aspects of the radio protocols (802.15.4 RF4CE and ZigBee, Bluetooth Low Energy) thus offloading the main CPU and leaving more resources for the user application. The RF core has its own RAM and ROM. The ARM Cortex-M0 ROM is not programmable by customers. The basic circuit implementing the 2.4 GHz antenna is shown in Figure 6.18.

The Sensor Controller block provides additional flexibility by allowing autonomous data acquisition and control independent of the main CPU, further extending the low-power capabilities of the CC2650. The Sensor Controller is set up using a PC-based configuration tool, called Sensor Controller Studio, and example interfaces include:

* Analog sensors using integrated ADC
* Digital sensors using GPIOs, bit-banged I2C, and SPI
* UART communication for sensor reading or debugging
* Capacitive sensing
* Waveform generation
* Pulse counting
* Keyboard scan
* Quadrature decoder for polling rotation sensors
* Oscillator calibration

![Figure 6.18](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/52729bb44fa458e47dec16cd12003b05/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_18_CC2650_BlockDiagram.jpg)
*Figure 6.18. The CC2650 includes a main CPU, a suite of I/O devices, an RF core, and a sensor controller.*

The CC2650 uses a radio-frequency (RF) link to implement Bluetooth Low Energy (BLE). As illustrated in Figure 6.19, the CC2650 can be used as a bridge between any microcontroller and Bluetooth. It is a transceiver, meaning data can flow across the link in both directions.

![Figure 6.19](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/610802ef952b6a9332209430e9fd064c/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_19_CC2650_networks.jpg)
*Figure 6.19. Block diagram of a wireless link between two microcontroller systems.*

Figure 6.20 shows a CC2650 BoosterPack. This board comes preprogrammed with the simple network processor described in the next section. With a JTAG debugger, other programs can be loaded onto this CC2650. For more information see
http://www.ti.com/tool/boostxl-cc2650ma

![Figure 6.20](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/8682ac32abd2d64a4102e8f9f1b6a6cf/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_20_CC2650BoosterPhoto.jpg)
*Figure 6.20. CC2650 BoosterPack (BOOSTXL-CC2650MA).*

Figure 6.21 shows a CC2650 LaunchPad. The top part of the PCB is the debugger and the bottom part implements the CC2650 target system. Figure 6.22 shows the pin connections to the booster pack headers. More details on the connections we will use are given in Lab 6. For more information, see
http://www.ti.com/ww/en/launchpad/launchpads-connected-launchxl-cc2650.html

![Figure 6.21](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/c68a89c777db78b04a4b80bb37c8ca81/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_21_CC2650LaunchPadPicture.jpg)
Figure 6.21. CC2650 LaunchPad (LAUNCHXL-CC2650).

![Figure 6.22](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/12b0dde156a89691c01b31f1f731a4be/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_22_CC2650LaunchPadPins.jpg)
Figure 6.22. CC2650 LaunchPad pin connections.

--
--

###6.3.2. Single Chip Solution, CC2650 LaunchPad

The CC2650 microcontroller is a complete System-on-Chip (SoC) Bluetooth solution, as shown in Figure 6.23. One could deploy the application, the Bluetooth stack, and the RF radio onto the CC2650.

![Figure 6.23](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/5c8b734fbd5eb86b1ce0f76d48a84a16/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_23_CC2500singlechipNetwork.jpg)
*Figure 6.23. Block diagram of a wireless link between two single-chip embedded systems.*

Figure 6.24 shows a CC2650 module, which could be used in a Bluetooth enabled device.

![Figure 6.24](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/217c70322f12cce4bf92c2a611e39f9f/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_24_CC2650Module.jpg)
*Figure 6.24. CC2650 module. See http://www.ti.com/tool/boostxl-cc2650ma*

####Programming with CCS Clould

[CCS Clould](https://youtu.be/gOjPnEpikY4)

1. Start with http://www.ti.com/ and log into TI
2. Go to https://dev.ti.com/ and click CCS cloud. Bookmark this page, it is where you will develop code.
3. Open another tab at following site to get more information on this process  project0_resources
4. Go to the following site and import the Project 0 CCS cloud (click the cloud icon) Project Zero code
5. Connect the CC2650 LaunchPad, and run the ProjectZeroStack

--
--

###6.4. Network Processor Interface

###6.4.1. Overview

[Overview](https://youtu.be/Jr_DLcP2usI)

Simple Network Processor (SNP) is TI’s name for the application that runs on the CC2650 when using the CC2650 with another microcontroller such as the MSP432 or TM4C123. In this configuration the controller and host are implemented together on the CC2650, while the profiles and application are implemented on an external MCU. The application and profiles communicate with the CC2650 via the Application Programming Interface (API) that simplifies the management of the BLE network processor. The SNP API communicates with the BLE device using the Network Protocol Interface (NPI) over a serial (SPI or UART) connection. In this chapter, we will use a UART interface as shown in Figure 6.25. This configuration is useful for applications that wish to add Bluetooth functionality to an existing device. In this paradigm, the application runs on the existing microcontroller, and BLE runs on the CC2650. For a description of the Simple Network Processor, refer to

* SNP http://processors.wiki.ti.com/index.php/CC2640_BLE_Network_Processor
* Developer guide http://www.ti.com/lit/ug/swru393c/swru393c.pdf 
* TI wiki page http://processors.wiki.ti.com/index.php/NPI

In this chapter, our TM4C123/MSP432 LaunchPad will be the application processor (AP) and the CC2650 will be the network processor (NP). There are 7 wires between the AP and the NP. Two wires are power and ground, one wire is a negative logic reset, two wires are handshake lines, and two wires are UART transmit and receive.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/939ef529afdfc2e48d7b7d59349cca47/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_25_NPIcircuit.jpg)
*Figure 6.25. Hardware interface between the LaunchPad AP and the CC2650 NP.*

To initialize Bluetooth, the master (AP) first resets the slave (NP). The reset line is a GPIO output of the AP and is the hardware reset line on the NP. There are two handshake lines: master ready and slave ready. Master ready (MRDY) is a GPIO output of the AP and a GPIO input to the NP. Slave ready (SRDY) is a GPIO output of the NP and a GPIO input of the AP. If the AP wishes to reset the NP, it sets MRDY high and pulses reset low for 10 ms, Figure 6.26. Normally, the reset operation occurs once, and thereafter the reset line should remain high.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/52f7b6a3b8b3792e4b89112d917efc2a/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_26_NPI_reset.jpg)
*Figure 6.26. The LaunchPad AP can reset the CC2650 NP.*

There are two types of communication. Messages can be sent from master to slave, or from slave to master. If the master (AP) wishes to send a message to the slave (NP), it follows 5 steps, Figure 6.27. First, the master sets MRDY low (Master: “I wish to send”). Second, the slave responds with SRDY low (Slave: “ok, I am ready”). The communication is handshaked because the master will wait for SRDY to go low. Third, the master will transmit a message on its UART output (Rx input to slave). The format of this message will be described later. Fourth, after the message has been sent, the master pulls MRDY high (Master: “I am done”). Fifth, the slave pulls its SRDY high (Slave: “ok”). Again, the handshaking requires the master to wait for SRDY to go high.

![Figure 6.27](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/8ff57c49fffd139c42903df983eeec63/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_27_NPI_mastrSendhandshake.jpg)
*Figure 6.27. The LaunchPad AP can send a message to the CC2650 NP. Handshake means the steps 1 – 5 always occur in this sequence.*

If the slave (NP) wishes to send a message to the master (AP), there are also 5 steps, Figure 6.28. First, the slave sets SRDY low (Slave: “I wish to send”). Second, the master responds with MRDY low (Master: “ok, I am ready”). You will notice in the example projects that the master will periodically check to see if the SRDY line has gone low, and if so it will receive a message. Third, the slave will transmit a message on its UART output (Tx output from slave). The format of this message will be the same for all messages. Fourth, after the message has been sent, the slave pulls SRDY high (Slave: “I am done”). The master will wait for SRDY to go high. Fifth, the master pulls its MRDY high (Master: “ok”).

![Figure 6.28](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/699252ea39d3d88222e7e616a69de90d/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_28_NPI_slaveSendhandshake.jpg)
*Figure 6.28. The CC2650 NP can send a message to the LaunchPad AP. Handshake means the steps 1 – 5 always occur in this sequence.*

The format of the message is shown in Figure 6.29. The boxes in the figure represent UART frames. Each UART frame contains 1 start bit, 8 data bits, and 1 stop bit, sent at 115,200 bits/sec. All messages begin with a start of frame (SOF), which is a 254 (0xFE). The next two bytes are the payload length in little endian format. Since all the payloads in this chapter are less than 256 bytes, the second byte is the length, L, and the third byte is 0. The fourth and fifth bytes are the command. Most commands have a payload, which contains the parameters of the command. Some commands do not have a payload. All messages end with a frame check sequence (FCS). The FCS is the 8-bit exclusive or of all the data, not including the SOF and the FCS itself.

![Figure 6.29](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/a8c4e1334d06e6f9633dc794391728c5/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_29_NPI_messageformat.jpg)
*Figure 6.29. The format of an NPI message.*

The following steps occur in this order

1. Initialize GATT (add services, characteristics, CCCD's);
2. Initialize GAP (advertisement data, connection parameters);
3. Advertise and optionally wait for a connection; 
4. Respond to GATT requests and send notifications / indications as desired.

--
--

###6.4.2. Services and Characteristics

[Services and Characteristics](https://youtu.be/ci9r_qG-Pk4)

After the CC2650 is reset, the next step is to services and characteristics. In the example projects we will define one service with multiple characteristics. To create a service, the master first issues an Add Service command (0x35,0x81). For each characteristic, the master sends an Add Characteristic Value (0x35,0x82) and an Add Characteristic Description (0x35,0x83) message. Once all the characteristics are defined, the master sends a Register Service command (0x35,0x84). Each of the commands has an acknowledgement response. The debugger output for a service with one characteristic is shown in Figure 6.30. The detailed syntax of these messages can be found in the TI CC2640 Bluetooth low energy Simple Network Processor API Guide.


![Figure 6.30](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/d78fb6607690b51c3c60c8c13dc62994/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_30_AddService.jpg)
*Figure 6.30. TExaSdisplay output as the device sets up a service with one characteristic.*

These data were collected running the VerySimpleApplicationProcessor_xxx project. Figures 6.31 through 6.34 show the four messages used to define a service with one characteristic. The add service creates a service. The add characteristic value declaration defines the read/write/notify properties of a characteristic in that service. The response to this message includes the handle. The add characteristic description declaration defines the name of the characteristic. When we create services with multiple characteristics, we simply repeat the add characteristic value and add characteristic description declarations for each. The register service makes that service active.


![Figure 6.31](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/9c96c5f75b4a5d3a14afaa033ed044da/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_31_AddService2.jpg)
*Figure 6.31. Add service message from the VerySimpleApplicationProcessor_xxx project.*


![Figure 6.32](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/337f749a825ef865fc5782220403601e/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_32_AddCharValue2.jpg)
*Figure 6.32. Add characteristic value declaration message from the VerySimpleApplicationProcessor_xxx project.*


![Figure 6.33](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/4b62a2cf57a91470709f95c7ee0081e6/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_33_AddCharDesc2.jpg)
*Figure 6.33. Add characteristic declaration message from the VerySimpleApplicationProcessor_xxx project.*


![Figure 6.34](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/b83c145a89f5fcc54904be1aff67723d/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_34_RegisterService2.jpg)
*Figure 6.34. Register service message from the VerySimpleApplicationProcessor_xxx project.*


--
--


###6.4.3. Advertising

[6.4.3. Advertising](https://youtu.be/MNFVXFdWcZ8)


After all the services and characteristics are defined, the master will setup and initiate advertising. The master will send four messages to set up advertising. The debugger output for advertising is shown in Figure 6.35. Each message will be acknowledged by the NP. A 0x35,0x85 message will set the device name. There are two 0x55,0x43 messages to configure the parameters of the advertising. The 0x55,0x42 message will start advertising. Again, detailed syntax of these messages can be found in the TI CC2640 Bluetooth low energy Simple Network Processor API Guide. Figure 6.36 shows the C code to define a Set Device Name message.


![Figure 6.35](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/2679757038b186d7ad8679f234d451e9/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_35_AdvertisingOutput2.jpg)
*Figure 6.35. TExaSdisplay output as the device sets up advertising. These data were collected running the VerySimpleApplicationProcessor_xxx project.*


![Figure 6.36](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/3cd9e07d6d7ea1068b1f87029e533439/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_36_SetGATTparameter2.jpg)
*Figure 6.36. A set device name message from the VerySimpleApplicationProcessor_xxx project.*

--
--

###6.4.4. Read and Write Indications

[6.4.4. Read and Write Indications](https://youtu.be/8dBcgSfCBeE)

Figure 6.37 shows the message exchange when the client issues a read request. The NP sends a read indication to the AP, containing the connection and handle of the characteristic. The AP responds with a read confirmation containing status, connection, handle, and the data.


![Figure 6.37](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/8f794a850b650adc92945056f806d50d/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_37_read.jpg)
*Figure 6.37. TExaSdisplay output occurring when the client issues a read request. These data were collected running the VerySimpleApplicationProcessor_xxx project.*

Figure 6.38 shows the message exchange when the client issues a write request. The NP sends a write indication to the AP, containing the connection, handle of the characteristic, and the data to be written. The AP responds with a write confirmation containing status, connection, and handle.


![Figure 6.38](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/875482dce124a9b12047f4e10e23e938/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_38_write.jpg)
*Figure 6.38. TExaSdisplay output occurring when the client issues a write request. These data were collected running the VerySimpleApplicationProcessor_xxx project.*

####References

* [SNP_API_Updated.pdf](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/4d03a377b88972197ede4cf38a512f35/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/SNP_API_Updated.pdf)
* [CC2640_Simple_Network_Processer_API_Guide.pdf](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/8db181df94eb13033011730aedd74b01/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/CC2640_Simple_Network_Processer_API_Guide.pdf)
* 


--
--


###6.4.5. Example Projects

We recommend you run the VerySimpleApplicationProcessor project first. The first video in the section shows this project connected to an iPhone running LightBlue. It sets up one service and 4 characteristics.

[Example Projects](https://youtu.be/VzR-0tOLb3A)

The next video shows the VerySimpleApplicationProcessor project connected to an Android running BLE Scanner. It sets up one service and 4 characteristics.

[VerySimpleApplicationProcessor on an Android](https://youtu.be/49x6RRB3yv4)

The second project we recommend is the ApplicationProcessor. The next shows this project controlled by an iPhone. It sets up one service and five characteristics.

[ApplicationProcessor](https://youtu.be/Jc2L0eAp7Pc)


--
--

###6.5.1. CoAP

The Constrained Application Protocol (CoAP) was specifically developed to allow resource-constrained devices to communicate over the Internet using UDP instead of TCP. In particular, many embedded devices have limited memory, processing power, and energy storage. Developers can interact with any CoAP-enabled device the same way they would with a device using a traditional Representational state transfer (REST) based API like HTTP. CoAP is particularly useful for communicating with low-power sensors and devices that need to be controlled via the Internet.

CoAP is a simple request/response protocol very similar to HTTP, that follows a traditional client/server model. Clients can make GET, PUT, POST, and DELETE requests to resources. CoAP packets use bitfields to maximize memory efficiency, and they make extensive usage of mappings from strings to integers to keep the data packets small enough to transport and interpret on-device. A CoAP message header is only 4-bytes long with most control messages being just that length. Most optional fields in the message format are in binary with the payload restricted in size so all CoAP messages fit inside a UDP datagram. TCP is a connection oriented protocol, which means the server, or a client, will open a socket and establish a connection with the server. And the communication is done over a connection. For the duration of the communication, the connection is on. Whereas, COAP works on UDP, which means that it's connectionless. And it allows what we call as a disconnected operation, which means that the client and the server are not connected to each other. And therefore, they can act asynchronously.

Aside from the extremely small packet size, another major advantage of CoAP is its usage of UDP; using datagrams allows for CoAP to be run on top of packet-based technologies like SMS. There is a one-to-one mapping between CoAP and HTTP effectively providing a bridge between the all popular HTTP protocol to the emerging CoAP protocol.

All CoAP messages can be marked as either “confirmable” or “nonconfirmable,” serving as an application-level Quality of Service (QoS) to provide reliability. While SSL/TLS encryption isn’t available over UDP, CoAP makes use of Datagram Transport Layer Security (DTLS), which is analogous to the TCP version of TLS. The default level of encryption is equivalent to a 3,072-bit RSA key. Even with all of this, CoAP is designed to work on microcontrollers with as little as 10KB of RAM.

One of the downsides of CoAP: It's a one-to-one protocol. Though extensions that make group broadcasts possible are available, broadcast capabilities are not inherent to the protocol. Arguably, an even more important disadvantage is the need for both devices to be simultaneously powered, so when one sends a UDP, the other can receive it. In summary, the highlights of CoAP include:

Small 4-byte header
Option fields in binary
Messages fit into one UDP datagram (no fragmentation)
Works with SMS (text messaging)
Connectionless
Needs less than 10 kB of RAM

Reference:
http://www.infoworld.com/article/2972143/internet-of-things/real-time-protocols-for-iot-apps.html

--
--

###6.5.2 MQTT

[MQTT](https://youtu.be/iO4SKlomgmI)


Message Queue Telemetry Transport (MQTT) is a publish-subscribe messaging protocol, abbreviated as pub-sub. The MQTT name was inherited from a project at IBM. Similar to CoAP, it was built with resource-constrained devices in mind. MQTT has a lightweight packet structure designed to conserve both memory usage and power. A connected device subscribes to a topic hosted on an MQTT broker. Every time another device or service publishes data to a topic, all of the devices subscribed to it will automatically get the updated information.

Figure 6.39 shows the basic idea of the pub-sub model. MQTT uses an intermediary, which is called a broker. There are clients, or publishers, which produce data. The MQTT protocol calls this data a topic, and each topic must have a unique identifier. The figure shows a temperature sensor, which is an embedded device with a sensor attached, and it periodically publishes the topic “temperature”. To publish a topic means to send data to the broker. The broker keeps track of all the published information. Subscribers are devices consumers, which are interested in the data. What the subscribers do is they express their interest in a topic by sending a subscription message. In this figure we have two devices that have subscribed to the topic “temperature”. Whenever new data is available, the broker will serve it to both subscribers.


![Figure 6.39](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/b4ea03a1ea98cd303051d05dc7fc648f/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig06_39_MQTT.jpg)
*Figure 6.39. With MQTT, the broker acts as an intermediary between producers and consumers.*

The fundamental advantage of a pub/sub model for communication in contrast with a client-server model is the decoupling of the communicating entities in space, time and synchronization. That is, the publisher and subscribed do not need to know each other, they do not run at the same time and they can act asynchronously. Other advantages of MQTT are the use of a publish-subscribe message queue and the many-to-many broadcast capabilities. Using a long-lived outgoing TCP connection to the MQTT broker, sending messages of limited bandwidth back and forth is simple and straightforward.

The downside of having an always-on connection is that it limits the amount of time the devices can be put to sleep. If the device mostly sleeps, then another MQTT protocol can be used: MQTT-SN, which is an extension of MQTT for sensor networks, originally designed to support ZigBee. MQTT-S is another extension that allows the use of UDP instead of TCP as the transport protocol, with support for peer-to-peer messaging and multicasting.

Another disadvantage of MQTT is the lack of encryption in the base protocol. MQTT was designed to be a lightweight protocol, and incorporating encryption would add a significant amount of overhead to the connection. One can however, use Transport Layer Security(TLS) extensions to TCP, or add custom security at the application level.

References:

http://www.hivemq.com/blog/mqtt-essentials/
http://www.infoworld.com/article/2972143/internet-of-things/real-time-protocols-for-iot-apps.html


--
--

###About Lab 6

The objectives of Lab 6 are:

* Interface the 2650 BLE module to the LaunchPad
* Develop a set of NPI message packets to support BLE communication
* Connect the fitness device to a cell phone
* Understand the concepts of service, characteristic, and advertising

[Running a lab 6 Solution](https://youtu.be/Dyv5dGaNCg8)

Chapter 6 provides a short introduction to the fundamentals of Bluetooth. The two starter applications VerySimpleApplicationProcessor_xxx and ApplicationProcessor_xxx provide simple solutions using Bluetooth Low Energy (BLE) to pair an embedded system with a smart phone using BLE implemented on the CC2650. The goal of these two examples is to create the simplest BLE solution that exposes the concepts of service, characteristic, and advertising. In this lab, you are asked to fully understand the syntax and functionality of the NPI protocol used to communicate between the TM4C123/MSP432 LaunchPad and the CC2650 BLE module. Your LaunchPad implements the fitness device, derived from either Lab 1 or Lab 3, and the CC2650 implements the BLE stack in form of the Simple Network Processor (SNP). More specifically, in this lab you will implement functions that build 11 different types of NPI messages. There are three possible hardware configurations to complete Lab 6 described in the following sections. The three options listed here correspond to the Option 1, Option 2, and Option 3 as described in the GPIO.c file used for the BLE applications.

[Four Options for using the CC2650](https://youtu.be/ER6VZquSGbc)

The first option for Lab 6 is to use the BOOSTXL-CC2650MA BoosterPack programmed with a default version of SNP just as it is shipped by the manufacturer. The MRDY/SRDY pins in this option correspond to the documentation provided with the CC2650 BoosterPack, see Figure 6.1. To use the CC2650 BoosterPack for Lab 6 in this option you do not have to program the CC2650. However, you do have to connect seven wires between the TM4C123/MSP432 LaunchPad and the CC2650 BoosterPack.


![Figure 6.1](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/13558a878d9e86b916a9791a5d07fb42/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Lab06_01_CC2650BP.jpg)
*Figure 6.1. Connections between TM4C123/MSP432 LaunchPad and the CC2650 BoosterPack (this option does not require programming the CC2650, but the boards are connected with 7 wires and CANNOT be stacked)*

In the file GPIO.h, line 8, comment out (do not define DEFAULT).

//#define DEFAULT 1

[CC2650 BoosterPack](https://youtu.be/b9TJSAx0CzE)

The second option for Lab 6 is to use the BOOSTXL-CC2650MA BoosterPack programmed with a special version of SNP called simple_np_cc2650bp_uart_pm_xsbl_mooc_custom.hex. To use the CC2650 BoosterPack for Lab 6 stacking it with the MKII as shown in Figure 6.2, it must be programmed.

![Figure 6.2](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/b886f7ef5fc062488ac2f874eb021324/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Lab06_02_CC2650BP.jpg)
*Figure 6.2. Connections between the MKII, the TM4C123/MSP432 LaunchPad and the CC2650 BoosterPack. This option requires programming the CC2650BP, but allows the three boards to be stacked.*

Within the list of available boards for this class there are two possible boards that can be used to program the CC2650 BoosterPack. The first programming option is the CC2650 LaunchPad. To configure a CC2650 LaunchPad as a stand-alone debugger: 1) Remove the five left-most jumpers on the CC2650 LaunchPad (RESET, TDI, TDO, TCK, and TMS). This disconnects the target CC2650 from the debugger; 2) Connect the JTAG cable from the CC2650 LaunchPad to the CC2650 BoosterPack. Now you can connect the CC2650 LaunchPad to the computer and use the SmartRF programmer to reflash the CC2650 BoosterPack. Program the hex files with bp (not lp) in the name. The hex file needed for this class has mooc in its name.

Another way to reprogram the CC2650 BoosterPack is the red MSP432 LaunchPad (not the black one). 1) Remove the five left-most jumpers on the red MSP432 LaunchPad (RESET, TDI, TDO, TCK, and TMS). This disconnects the target MSP432 from the debugger; 2) Connect the JTAG cable from the red MSP432 LaunchPad (XDS110 Out) to the CC2650 BoosterPack. Now you can connect the red MSP432 LaunchPad to the computer and use the SmartRF programmer to reflash the CC2650 BoosterPack.

Step 0) Create an account on my.ti.com and log in.

Step 1) Search TI.com for “Smartrf flash programmer”. Download and unzip a file called flash-programmer-2-1.7.4.zip. In administrator mode, install the application, Setup_SmartRF_Flash_Programmer_2-1.7.4.exe

Step 2) Download and unzip hex files from this web [link ble_2_02_simple_np_setup.exe](http://software-dl.ti.com/dsps/forms/self_cert_export.html?prod_no=ble_2_02_simple_np_setup.exe&ref_url=http://software-dl.ti.com/lprf/BLE-Simple-Network-Processor-Hex-Files)  These hex files (object code) implement the BLE stack in the form of the simple network processor (SNP). This download creates two directories: one with files for the BoosterPack (cc2650bp) and one with files for the LaunchPad (cc2650lp).

Step 3) Find this hex file on your computer: simple_np_cc2650bp_uart_pm_xsbl_mooc.hex 
Notice the letters bp (for BoosterPack) and mooc.

Step 4) Use the Flash Programmer to burn this hex file onto your CC2650 BoosterPack

Step 5) In the file GPIO.h, line 8, comment out (do not define DEFAULT)

//#define DEFAULT 1

There is a mistake in the comments of option 3 of GPIO.c. MRDY in this configuration is connected to the CC2650 DIO8 pin.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/3b2a4bce69bfa0135e660fc3c5fa1951/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Lab6fix.jpg)

The third option for Lab 6 is to use the LAUNCHXL-CC2650 LaunchPad programmed with a special version of SNP called simple_np_cc2650lp_uart_pm_xsbl_mooc_custom.hex.

![Figure 6.3](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/dc9687d065721b37e0a3df71b528db70/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Lab06_03_CC2650LP.jpg)
*Figure 6.3. Connections between the MKII, the TM4C123/MSP432 LaunchPad and the CC2650 LaunchPad. This option requires programming the CC2650LP, but allows the three boards to be stacked.*

To use the CC2650 LaunchPad for Lab 6 it must be programmed using the instructions outlined in the following video.

[Programming the CC2650 LaunchPad](https://youtu.be/Ov7gAtLUmKk)

Step 0) Create an account on my.ti.com and log in.

Step 1) Search TI.com for “Smartrf flash programmer”. Download and unzip a file called flash-programmer-2-1.7.4.zip. In administrator mode, install the application, Setup_SmartRF_Flash_Programmer_2-1.7.4.exe

Step 2) Download and unzip hex files from this web link ble_2_02_simple_np_setup.exe These hex files (object code) implement the BLE stack in the form of the simple network processor (SNP). This download creates two directories: one with files for the BoosterPack (cc2650bp) and one with files for the LaunchPad (cc2650lp).

Step 3) Find this hex file on your computer: simple_np_cc2650lp_uart_pm_xsbl_mooc.hex Notice the letters lp (for LaunchPad) and mooc.

Step 4) Use the Flash Programmer to burn this hex file onto your CC2650 LaunchPad

Step 5) In the file GPIO.h, line 8, comment out (do not define DEFAULT)

//#define DEFAULT 1

The reason we reprogrammed the CC2650 BoosterPack in option 2 was there is a pin conflict between the standard configuration of the CC2650 and MKII BoosterPacks. If you have a BLE project that does not use the MKII, then you can use the CC2650 BoosterPack without reprogramming the CC2650. Since Lab 6 uses the MKII, this option cannot be used for Lab 6, but could be used for other projects that do not require the MKII I/O BoosterPack.


![Figure 6.4](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/05e4f5cad91e39189d5e21d5388bb9c1/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Lab06_04_CC2650BP.jpg)
*Figure 6.4. Connections between the TM4C123/MSP432 LaunchPad and the CC2650 BoosterPack. This option does not require programming the CC2650BLP, but cannot be used with the MKII BoosterPack.*

In this approach, in the file GPIO.h, activate the define statement in line 8 out (do define DEFAULT).

#define DEFAULT 1

There is a bug in the lab 6 grader for testing BuildSetAdvertisementDataMsg. When the grader calls your function with “Yerraballi” as the name, it incorrectly looks for this message:

FE,15,00,55,43,00,0A,09,59,65,72,72,61,62,61,6C,6C,69,05,12,50,00,20,03,02,0A,5B

The correct message the grader should have been looking for is:

FE,16,00,55,43,00,0B,09,59,65,72,72,61,62,61,6C,6C,69,05,12,50,00,20,03,02,0A,00,59

We fixed this Lab6 grader bug and created new texas.o files. Download a new texas.o and replace the existing one in your Lab6 folder (either Lab6 with Lab1 or Lab6 with Lab3). This TM4C123 version should be placed in the TM4C123 Lab 6 folder http://users.ece.utexas.edu/~valvano/edX/TM4C123/texas.o. This MSP432 version should be placed in the MSP432 Lab 6 folder http://users.ece.utexas.edu/~valvano/edX/MSP432/texas.o This new grader will accept either the old version with the bug, or the new correct version.


--
--

###Debugging Lab 6

Step 1) The first step for completing Lab 6 is to configure your system using one of the three options listed in the previous section.

Step 2) The next step is to compile, download, and run these two projects

VerySimpleApplicationProcessor_xxx
ApplicationProcessor_xxx
We encourage you to rewatch the three videos in edX Section 6.4.5, and run these two projects on your hardware.

Step 3) You have two software options for Lab 6. The Lab6wLab1_xxx project allows you to complete Lab 6 using the simple fitness device from Lab 1. The Lab6wLab3_xxx project allows you to complete Lab 6 using the RTOS from Lab 3. The next step is to open one of these two Lab6 projects and paste in either your Lab 1 or your Lab 3 solution. The code you have to write will be placed in the AP_Lab6.c file. In this file there are 11 empty functions into which you will develop your Lab 6 solution. Basically, these 11 functions are used to formulate NPI messages that are sent from your LaunchPad to the CC2650 (which is running SNP). Details of these functions can be found in the AP_Lab6.c file.

If you hold Button 1 down and start the software it will run the Lab 6 grader and pause while you are holding the button. The results of the grader can be observed on TExaSdisplay. When you release the button, the fitness device will run.

[Running the Starter Project](https://youtu.be/HaIeMaJdq0U)



--
--

###Lab 6 Grader

[Getting your grade in Lab 6](https://youtu.be/WOG0oIPxSz4)

Lab 6
90.0 puntos posibles (calificado)
Grading your Lab 6 does require a LaunchPad development board and one of the CC2650 modules. There are two aspects of Lab 6 that are graded. First, you must implement the following 11 functions that produce NPI messages. These functions do not send the messages, they just create the message with the proper syntax. Prior to launching the fitness device, the grader will evaluate these functions. If you hold button 1 down when the software is started it will pause after this initial grading so you can see the results.

BuildGetStatusMsg
BuildGetVersionMsg
BuildAddServiceMsg
BuildRegisterServiceMsg
BuildAddCharValueMsg
BuildAddCharDescriptorMsg
BuildAddNotifyCharDescriptorMsg
BuildSetDeviceNameMsg
BuildSetAdvertisementData1Msg
BuildSetAdvertisementDataMsg
BuildStartAdvertisementMsg
The second part of the grading involves Bluetooth communication.

Step 1 Start TExaSdisplay and open the COM port. Download and run your solution.

Step 2 Connect your fitness device to your smart phone, open the edXNum characteristic on the phone, and write the 5549 number to this characteristic. We understand the number here on edX is decimal, and the 4-digit number entered on the phone is hexadecimal. You do not have to convert decimal to hex, just enter the four digits into the phone. The grader will handle the hex to decimal conversion in an appropriate manner.

Step 3 The Lab 6 score and 8-character code will be observable in the TExaSdisplay window. Any score above 70 will be considered a passing grade. If you are not satisfied with your score you are allowed multiple submissions.

Enter the CopyThisToEdX code from the Lab 6 grading engine:

On the Android, we use BLE Scanner

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/718517bee09c4c616ea0a5254a731dcb/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Android01.JPG)

You will find all the characteristics under Custom Service

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/f227a2ebae9413be45e5cfdf047d6134/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Android02.JPG)

When using BLE Scanner on the Android, you select services by the UUID. For example, the edXNum has a UUID of FFF6. On the Android, you write to the characteristic by hitting the W.

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/04731954bf16be176574fcadcc9fcfe3/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Android03.JPG)

The data protocol is byte array. To get a grade, change the format to byte array

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/9a4a3d4d279ea7a942db69dab97be36c/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Android04.JPG)

 and simply type in the 4-digit edXNum listed in the above section (no spaces, no commas, no 0x, no converting decimal to hex, no converting hex to decimal). 
 
![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/a9fae1ec782cc36c7a8a5bec544cfbc2/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Android05.JPG)

Notice the 1234 typed into the phone maps to the 1234 in TExaSdisplay

![](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/fd0c5c33c2cea0765a4106fa12813b2c/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/AndroidTExaS.jpg)
--
--
###References

Reprinted with approval from these five books:

Embedded Systems: Introduction to ARM Cortex-M Microcontrollers, ISBN: 978-1477508992, Jonathan Valvano, http://users.ece.utexas.edu/~valvano/arm/outline1.htm

Embedded Systems: Introduction to the MSP432 Microcontroller, ISBN: 978-1512185676, Jonathan Valvano, http://users.ece.utexas.edu/~valvano/arm/msp432.htm

Embedded Systems: Real-Time Interfacing to the MSP432 Microcontroller, ISBN: 978-1514676585, Jonathan Valvano, http://users.ece.utexas.edu/~valvano/arm/msp432.htm

Embedded Systems: Real-Time Interfacing to ARM Cortex-M Microcontrollers, ISBN: 978-1463590154, Jonathan Valvano, http://users.ece.utexas.edu/~valvano/arm/outline.htm

Embedded Systems: Real-Time Operating Systems for ARM Cortex-M Microcontrollers, ISBN: 978-1466468863, Jonathan Valvano, http://users.ece.utexas.edu/~valvano/arm/outline3.htm
