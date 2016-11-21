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

Digital signal processing (DSP) is beyond the scope of this class. However, we would like to introduce the field of DSP in order to better understand how real-time operating systems will be deployed to process digital signals. In particular, sampling data will be an important task, so we will take special care to sample accurately and with low jitter.

Back in the last chapter we presented FIFO queues as a solution to the data flow problem with producers and consumers. We defined a producer as a thread that created data and stored it in the FIFO. If this data is created at a regular rate (e.g., in Lab 3 we sample the ADC at a fixed frequency of 1000 Hz) we can consider the data as a sampled digital signal. We defined a consumer as a thread that removed data from the FIFO and processed it. With DSP we will modify the data structure to allow access to multiple elements, using a multiple-access-circular-queue (MACQ), as shown in Figure 4.3.

![Figure 4.3](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/8fea0db4bfb6eb2aee6e302666bbec77/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig04_03_MACQdataflow.jpg)
*Figure 4.3. Data flow for digital signal processing.*

We consider the sampled data as sequence of digital numbers. Let xc(t) be the continuous analog signal to be digitized. xc(t) is the analog input to the ADC converter. If fs is the sample rate, then the computer samples the ADC every T seconds. (T = 1/fs). Let x(0),x(1),...,x(n),... be the ADC output sequence, where

```
    x(n) = xc(nT) with -∞ < n < +∞. 
```

There are two types of approximations associated with the sampling process. Because of the finite precision of the ADC, amplitude errors occur when the continuous signal, xc(t), is sampled to obtain the digital sequence, x(n). The second type of error occurs because of the finite sampling frequency. The Nyquist Theorem states that the digital sequence, x(n), properly represents the DC to ½fs frequency components of the original signal, x(t). There are two important assumptions that are necessary to make when using digital signal processing:

We assume the signal has been sampled at a fixed and known rate, fs
We assume aliasing has not occurred.
We can guarantee the first assumption by using a hardware clock to start the ADC at a fixed and known rate. A less expensive but not as reliable method is to implement the sampling routine as a high priority periodic interrupt process. If the time jitter is Δt then we can estimate the voltage error by multiplying the time jitter by the slew rate of the input, dV/dt*Δt. By establishing a high priority of the interrupt handler, we can place an upper bound on the interrupt latency, guaranteeing that ADC sampling is occurring at an almost fixed and known rate. We can observe the ADC input with a spectrum analyzer to prove there are no significant signal components above ½fs. “No significant signal components” is defined as having an ADC input voltage |Z| less than the ADC resolution, Δz,

```
    |Z| ≤ Δz for all f ≥ ½fs
```

A causal digital filter calculates y(n) from y(n-1), y(n-2),... and x(n), x(n-1), x(n 2),... Simply put, a causal filter cannot have a nonzero output until it is given a nonzero input. The output of a causal filter, y(n), cannot depend on future data (e.g., y(n+1), x(n+1) etc.)

A linear filter is constructed from a linear equation. A nonlinear filter is constructed from a nonlinear equation. An example of a nonlinear filter is the median. To calculate the median of three numbers, one first sorts the numbers according to magnitude, then chooses the middle value. Other simple nonlinear filters include maximum, minimum, and square. A finite impulse response filter (FIR) relates y(n) only in terms of x(n), x(n-1), x(n 2),... If the sampling rate is 360 Hz, this simple FIR filter will remove 60 Hz noise:

```
    y(n) = (x(n)+x(n-3))/2
```

An infinite impulse response filter (IIR) relates y(n) in terms of both x(n), x(n-1),..., and y(n 1), y(n-2),... This simple IIR filter has averaging or low-pass behavior:

```
    y(n) = (x(n)+y(n-1))/2
```

The step signal represents a sharp change (like an edge in a photograph). We will analyze three digital filters. 
* The FIR is y(n) = (x(n)+x(n-1))/2. 
* The IIR is y(n) = (x(n)+y(n-1))/2. 
* The nonlinear filter is y(n) = median(x(n), x(n-1), x(n-2)).

The median can be performed on any odd number of data points by sorting the data and selecting the middle value. The median filter can be performed recursively or nonrecursively. A nonrecursive 3-wide median filter is implemented in Program 4.1.

```c
uint8_t Median(uint8_t u1,uint8_t u2,uint8_t u3){ 
uint8_t result;
  if(u1>u2)
    if(u2>u3) result = u2; // u1>u2,u2>u3 u1>u2>u3
    else
    if(u1>u3) result = u3; // u1>u2,u3>u2,u1>u3 u1>u3>u2
    else      result = u1; // u1>u2,u3>u2,u3>u1 u3>u1>u2
  else 
    if(u3>u2) result = u2; // u2>u1,u3>u2 u3>u2>u1
    else
    if(u1>u3) result = u1; // u2>u1,u2>u3,u1>u3 u2>u1>u3
    else      result = u3; // u2>u1,u2>u3,u3>u1 u2>u3>u1
  return(result); 
}
```
*Program 4.1: The median filter is an example of a nonlinear filter.*

For a nonrecursive median filter, the original data points are not modified. For example, a 5-wide nonrecursive median filter takes as the filter output the median of {x(n), x(n-1), x(n-2), x(n–3), x(n-4)} On the other hand, a recursive median filter replaces the sample point with the filter output. For example, a 5-wide recursive median filter takes as the filter output the median of {x(n), y(n-1), y(n-2), y(n-3), y(n-4)} where y(n-1), y(n-2),... are the previous filter outputs. A median filter can be applied in systems that have impulse or speckle noise. For example the noise every once in a while causes one sample to be very different than the rest (like a speck on a piece of paper) then the median filter will completely eliminate the noise. Except for the delay, the median filter passes a step without error. The step responses of the three filters are (Figure 4.4):

    FIR ..., 0, 0, 0, 0.5, 1, 1, 1, ...
    IIR ..., 0, 0, 0, 0.5, 0.75, 0.88, 0.94, 0.97, 0.98, 0.99, ...
    median ..., 0, 0, 0, 0, 1, 1, 1, 1, 1, ...
    
![Figure 4.4](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/eff89375a14f4fadb6453d581e9fd15c/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig04_04stepResponse.jpg)
*Figure 4.4. Step response of three simple digital filters.*

The impulse represents a noise spike (like spots on a Xerox copy). The impulse response of a filter is defined as h(n). The median filter completely removes the impulse. The impulse responses of the three filters are (Figure 4.5):

    FIR ..., 0, 0, 0, 0.5, 0.5, 0, 0, 0, ...
    IIR ..., 0, 0, 0, 0.5, 0.25, 0.13, 0.06, 0.03, 0.02, 0.01, ...
    median ..., 0, 0, 0, 0, 0, 0, 0, 0, ...
    
![*Figure 4.5](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/02c369ea9ce70badec41eabfc12dd00b/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig04_05ImpulseResponse.jpg)
*Figure 4.5. Impulse response of three simple digital filters. Note that the median filter preserves the sharp edges and removes the spike or impulsive noise.*


Note that the median filter preserves the sharp edges and removes the spike or impulsive noise. Impulse and Step responses are important aspects in control theory as they give us the behavior of the output signal (in time) when presented with a sudden input (impulse) or when the inputs suddenly change from zero to 1 (step) respectively.


###4.1.3. Real-time signal processing

[Real-time signal processing](https://youtu.be/7luf0-_NNIk)

In this section we will develop a simple example that samples an analog signal and calculates the derivative of that signal in real time. A transducer is used to convert the measurand into an electrical signal, a circuit is used to convert the input into the voltage range of the ADC, and the ADC is sampled at 1000 Hz. A periodic event task samples the ADC and stores the input into a multiple access circular queue (MACQ). A MACQ is a fixed length order preserving data structure, see Figure 4.6. The source process (ADC sampling software) places information into the MACQ. Once initialized, the MACQ is always full. The oldest data is discarded when the newest data is Put into a MACQ. The consumer process can read any of the data from the MACQ. The Read function is non-destructive. This means that the MACQ is not changed by the Read operation. In this MACQ, the newest sample, x(n) is stored in element x[0], x(n-1) is stored in element x[1], x(n-2) is stored in element x[2], and x(n-3) is stored in element x[3].

![Figure 4.6](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/6fb11810c404cd1680af972c07988794/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig04_06_MACQdataflow.jpg)
*Figure 4.6. Data is sampled at 1 kHz with an event thread. MACQ contains the most recent four samples. A mailbox-like synchronization is used to pass data from producer to consumer.*

To Put data into this MACQ, four steps are followed, as shown in Figure 4.7. First, the data is shifted down (steps 1, 2, 3), and then the new data is entered into the x[0] position (step 4).

![Figure 4.7](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/ad940405ff7fa8a048c8758f73ee2e80/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig04_07_MACQ.jpg)
*Figure 4.7. When data is put into a multiple access circular queue, the oldest data is lost.*

The drawing in Figure 4.7 shows the position in memory of x(n), x(n-1),… does not move when one data sample is added. Notice however, the data itself does move. As time passes the data gets older, the data moves down in the MACQ. In this example the ADC sampling is triggered every 1 ms. x(n) will refer to the current sample, and x(n-1) will be the sample 1 ms ago. There are a couple of ways to implement a discrete time derivative. The simple approach is

```
    d(n) = (x(n) – x(n-1))/Δt
```

In practice, this first order equation is quite susceptible to noise. An approach generating less noise calculates the derivative using a higher order equation like

```
    d(n) = (x(n) + 3x(n-1) – 3x(n-2) – x(n-3))/(6Δt)
```

The C implementation of this discrete derivative uses a MACQ (Program 4.2). Since ∆t is 1 ms, we simply consider the derivative to have units 6mV/ms and not actually execute the divide by 6∆t operation. Signed arithmetic is used because the slope may be negative.

```c
int32_t x[4];        // MACQ (mV)
int32_t d;           // derivative(V/s)
int32_t DataReady;   // semaphore, initially 0
int32_t Derivative(void){ // called every 1 ms
  return x[0]+3*x[1]-3*x[2]-x[3]; // in 6V/s
}
void MACQ_Put(int32_t in){
  x[3] = x[2]; // shift data
  x[2] = x[1]; // units of mV
  x[1] = x[0];
  x[0] = in;
}
void RealTimeSampling(void){ // event thread at 1 kHz, every 1 ms
int32_t sample; // 0 to 1023
int32_t mV;     // -1650 to 1650 mV
  BSP_Microphone_Input(&sample); // sample is 0 to 1023
  mV = 1650*(sample-512)/512;    // in mV
  MACQ_Put(mV);                  // save in MACQ
  OS_Signal(&DataReady);
}
void DigitalSignalProcesing(void){ // main thread
  while(1){
    OS_Wait(&DataReady);
    d = Derivative();
  }
}

```
*Program 4.2. Software implementation of first derivative using a multiple access circular queue.*

When the MACQ holds many data points it can be implemented using a pointer or index to the newest data. In this way, the data need not be shifted each time a new sample is added. The disadvantage of this approach is that address calculation is required during the Read access. For example, we could implement a 16-element averaging filter. More specifically, we will calculate the average of the last 16 samples, see Program 4.3.

Entering data into this MACQ is a three step process (Figure 4.8). First, the pointer is decremented. If necessary, the pointer is wrapped such that it is always pointing to elements x[0] through x[15]. Second, new data is stored into the location of the pointer. Third, a second copy of the new data is stored 16 elements down from the pointer.

Because the pointer is maintained within the first 16 elements, *Pt to *(Pt+15) will always point to valid data within the MACQ. Let m be an integer from 0 to 15. In this MACQ, the data element x(n-m) can be found using *(Pt+m).

![Figure 4.8](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/33295b3561251e943a6e2b3f987a4ec9/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig04_08_MACQ16.jpg)
*Figure 4.8. When data is put into a multiple access circular queue, the oldest data is lost.*

The drawing in Figure 4.8 shows the labels x(n), x(n-1),… moving from before to after. Notice however, the data itself does not move. What moves is the significance (or meaning) of the data. The data grows older as time passes. The passage of time is produced by decrementing the pointer.

Observation: It is possible to implement a pointer-based MACQ that keeps just one copy of the data. Time to access data would be slower, but half as much storage would be needed.

```c
uint16_t x[32]; // two copies 
uint16_t *Pt;   // pointer to current
uint16_t Sum;   // sum of the last 16 samples
void LPF_Init(void){ 
  Pt = &x[0]; Sum = 0;
}
// calculate one filter output
// called at sampling rate
// Input: new ADC data
// Output: filter output, DAC data
uint16_t LPF_Calc(uint16_t newdata){ 
  Sum = Sum - *(Pt+16); // subtract the one 16 samples ago
  if(Pt == &x[0]){
    Pt = &x[15];        // wrap, always within 0,1,...,15
  } else{
    Pt--;               // make room for data
  }
  *Pt = *(Pt+16) = newdata; // two copies of the new data
  return Sum/16;
}
```

*Program 4.3. Digital low pass filter implemented by averaging the previous 16 samples (cutoff = fs/32).*

####CHECKPOINT 4.4

__Assume you wished to implement a digital filter that averaged the last 100 samples. How would you change Program 4.3? Will this new filter take longer to execute?__

You would change the 32 to 200 (double copy of each data), and change all the 16’s to 100. Change 15 to 99. Notice this new filter executes in the same amount of time as the smaller filter.


--
--

###4.1.4. Real-time control systems

[Real-time control systems](https://youtu.be/mijSiCD_9Dc)