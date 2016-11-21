###Objectives

The objectives of this chapter are to:

Review real-time applications that require priority
* Overview edge-triggered interrupts
* Use the operating system to debounce switches
* Implement a priority scheduler
* Review of other real-time operating systems

To motivate the need for priority we will discuss some classic real-time system scenarios like Data Acquisition systems, Digital Signal Processing (DSP), and Real-Time Control systems. The level of detail provided here is not needed for the course, but we believe it will give you a context for the kinds of systems you may encounter as a practitioner in the RTOS domain.

--

###4.1.1. Data Acquisition Systems

[Data Acquisition Systems](https://youtu.be/lRU5RDkY-qg)


Figure 4.1 illustrates the integrated approach to data acquisition systems. In this section, we begin with the clear understanding of the problem. We can use the definitions in this section to clarify the design parameters as well as to report the performance specifications.

![Figure 4.1](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/5fb35d719fe2cbadf17e0708ffa74ed2/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig04_01DAS.jpg)
*Figure 4.1. Individual components are integrated into a data acquisition system.*

The measurand is the physical quantity, property, or condition that the instrument measures. See Figure 4.2. The measurand can be inherent to the object (like position, mass, or color), located on the surface of the object (like the human EKG, or surface temperature), located within the object (e.g., fluid pressure, or internal temperature), or separated from the object (like emitted radiation.) In general, a transducer converts one energy type into another. In the context of this section, the transducer converts the measurand into an electrical signal that can be processed by the microcontroller-based instrument. Typically, a transducer has a primary sensing element and a variable conversion element. The primary sensing element interfaces directly to the object and converts the measurand into a more convenient energy form. The output of the variable conversion element is an electrical signal that depends on the measurand. For example, the primary sensing element of a pressure transducer is the diaphragm, which converts pressure into a displacement of a plunger. The variable conversion element is a strain gauge that converts the plunger displacement into a change in electrical resistance. If the strain gauge is placed in a bridge circuit, the voltage output is directly proportional to the pressure. Some transducers perform a direct conversion without having a separate primary sensing element and variable conversion element. The system contains signal processing, which manipulates the transducer signal output to select, enhance, or translate the signal to perform the desired function, usually in the presence of disturbing factors. The signal processing can be divided into stages. The analog signal processing consists of instrumentation electronics, isolation amplifiers, amplifiers, analog filters, and analog calculations. The first analog processing involves calibration signals and preamplification. Calibration is necessary to produce accurate results. An example of a calibration signal is the reference junction of a thermocouple. The second stage of the analog signal processing includes filtering and range conversion. The analog signal range should match the ADC analog input range. Examples of analog calculations include: RMS calculation, integration, differentiation, peak detection, threshold detection, phase lock loops, AM FM modulation/demodulation, and the arithmetic calculations of addition, subtraction, multiplication, division, and square root. When period, pulse width, or frequency measurement is used, we typically use an analog comparator to create a digital logic signal to measure. Whereas the Figure 4.1 outlined design components, Figure 4.2 shows the data flow graph for a data acquisition system or control system. The control system uses an actuator to drive a parameter in the real world to a desired value while the data acquisition system has no actuator because it simply measures the parameter in a nonintrusive manner.

![Figure 4.2](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/f918f9a4a306a12c48b96f2258893e23/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig04_02DAS.jpg)
Figure 4.2. Signal paths for a data acquisition system without an actuator; the control system includes an actuator so the system can use feedback to drive the real-world parameter to a desired state.

The data conversion element performs the conversion between the analog and digital domains. This part of the instrument includes: hardware and software computer interfaces, ADC, DAC, and calibration references. The analog to digital converter (ADC) converts the analog signal into a digital number. The digital to analog converter (DAC) converts a digital number to an analog output.

In many systems the input could be digital rather than analog. For these systems measuring period, pulse width, and/or frequency provides a low-cost high-precision alternative to the traditional ADC. Similarly the output of the system could be digital. The pulse width modulator (PWM) is a digital output with a constant period, but variable duty cycle. The software can adjust the output of the actuator by setting the duty cycle of the PWM output.

The digital signal processing includes: data acquisition (sampling the signal at a fixed rate), data formatting (scaling, calibration), data processing (filtering, curve fitting, FFT, event detection, decision making, analysis), control algorithms (open or closed loop). The human interface includes the input and output which is available to the human operator. The advantage of computer-based instrumentation is that, devices that are sophisticated but easy to use and understand are possible. The inputs to the instrument can be audio (voice), visual (light pens, cameras), or tactile (keyboards, touch screens, buttons, switches, joysticks, roller balls). The outputs from the instrument can be numeric displays, CRT screens, graphs, buzzers, bells, lights, and voice. If the system can deliver energy to the real world then it is classified as a control system.

--
--

###4.1.2. Real-time Digital Signal Processing

[Real-time Digital Signal Processing](https://youtu.be/UTv4_xq1plo)

