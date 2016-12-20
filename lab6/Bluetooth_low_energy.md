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