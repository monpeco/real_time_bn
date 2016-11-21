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

A control system is a collection of mechanical and electrical devices connected for the purpose of commanding, directing, or regulating a physical plant (see Figure 4.9). The real state variables are the properties of the physical plant that are to be controlled. The sensor and state estimator comprise a data acquisition system. The goal of this data acquisition system is to estimate the state variables. A closed-loop control system uses the output of the state estimator in a feedback loop to drive the errors to zero. The control system compares these estimated state variables, X'(t), to the desired state variables, X*(t), in order to decide appropriate action, U(t). The actuator is a transducer that converts the control system commands, U(t), into driving forces, V(t), that are applied to the physical plant. In general, the goal of the control system is to drive the real state variables to equal the desired state variables. In actuality though, the controller attempts to drive the estimated state variables to equal the desired state variables. It is important to have an accurate state estimator, because any differences between the estimated state variables and the real state variables will translate directly into controller errors. If we define the error as the difference between the desired and estimated state variables:

```
       e(t) = X*(t)- X’(t)
```

then the control system will attempt to drive e(t) to zero. In general control theory, X(t), X’(t), X*(t), U(t), V(t) and e(t) refer to vectors, but the examples in this chapter control only a single parameter. Even though this chapter shows one-dimensional systems, and it should be straight-forward to apply standard multivariate control theory to more complex problems. We usually evaluate the effectiveness of a control system by determining three properties: steady state controller error, transient response, and stability. The steady state controller error is the average value of e(t). The transient response is how long does the system take to reach 99% of the final output after X* is changed. A system is stable if steady state (smooth constant output) is achieved. The error is small and bounded on a stable system. An unstable system oscillates, or it may saturate.


![Figure 4.9](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/858758a0ed1521d5b75bb579adb0cd79/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig04_09_ControlSystem.jpg)
*Figure 4.9. Block diagram of a microcomputer-based closed-loop control system.*

An open-loop control system does not include a state estimator. It is called open loop because there is no feedback path providing information about the state variable to the controller. It will be difficult to use open-loop with the plant that is complex because the disturbing forces will have a significant effect on controller error. On the other hand, if the plant is well-defined and the disturbing forces have little effect, then an open-loop approach may be feasible. Because an open-loop control system does not know the current values of the state variables, large errors can occur. Stepper motors are often used in open loop fashion.

In order to make a fast and accurate system, we can use linear control theory to develop the digital controller. There are three components of a proportional integral derivative PID controller.

```
U(t)=KpE(t)+∫0tKiE(τ)dτ+KddE(t)dt
```

The error, E(t), is defined as the present set-point, X*(t), minus the measured value of the controlled variable, X’(t). The PID controller calculates its output by summing three terms. The first term is proportional to the error. The second is proportional to the integral of the error over time, and the third is proportional to the rate of change (first derivative) of the error term. The values of Kp, Ki and Kd are design parameters and must be properly chosen in order for the control system to operate properly. The proportional term of the PID equation contributes an amount to the control output that is directly proportional to the current process error. The gain term Kp adjusts exactly how much the control output response should change in response to a given error level. The larger the value of Kp, the greater the system reaction to differences between the set-point and the actual state variable. However, if Kp is too large, the response may exhibit an undesirable degree of oscillation or even become unstable. On the other hand, if Kp is too small, the system will be slow or unresponsive. An inherent disadvantage of proportional-only control is its inability to eliminate the steady state errors (offsets) that occur after a set-point change or a sustained load disturbance.

The integral term converts the first order proportional controller into a second order system capable of tracking process disturbances. It adds to the controller output a factor that takes corrective action for any changes in the load level of the system. This integral term is scaled to the sum of all previous process errors in the system. As long as there is a process error, the integral term will add more amplitude to the controller output until the sum of all previous errors is zero. Theoretically, as long as the sign of Ki is correct, any value of Ki will eliminate offset errors. But, for extremely small values of Ki, the controlled variables will return to the set-point very slowly after a load upset or set-point change occurs. On the other hand, if Ki is too large, it tends to produce oscillatory response of the controlled process and reduces system stability. The undesirable effects of too much integral action can be avoided by proper tuning (adjusting) the controller or by including derivative action which tends to counteract the destabilizing effects.

The derivative action of a PID controller adds a term to the controller output scaled to the slope (rate of change) of the error term. The derivative term “anticipates” the error, providing a greater control response when the error term is changing in the wrong direction and a dampening response when the error term is changing in the correct direction. The derivative term tends to improve the dynamic response of the controlled variable by decreasing the process setting time, the time it takes the process to reach steady state. But if the process measurement is noisy, that is, if it contains high-frequency random fluctuations, then the derivative of the measured (controlled) variable will change wildly, and derivative action will amplify the noise unless the measurement is filtered.


We can also use just some of the terms. For example a proportional/integrator (PI) controller drops the derivative term. We will analyze the digital control system in the frequency domain, see Figure 4.10. Let X(s) be the Laplace transform of the state variable x(t). Let X*(s) be the Laplace transform of the desired state variable x*(t). Let E(s) be the Laplace transform of the error

```
E(s) = X*(s) - X(s)
```

Let G(s) be the transfer equation of the PID linear controller. PID controllers are unique in this aspect. In other words we cannot write a transfer equation for a bang-bang, incremental or fuzzy logic controller.

```
G(s)=Kp+Kds+Kis
```

Let H(s) be the transfer equation of the physical plant. If we assume the physical plant (e.g., a DC motor) has a simple single pole behavior, then we can specify its response in the frequency domain with two parameters. m is the DC gain and τ is its time constant. The transfer function of this simple motor is

```
H(s)=m1+τs
```

The overall gain of the control system is

X(s)X∗(s)=G(s)H(s)1+G(s)H(s)


![Figure 4.10](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/9c5689daca170abb9f967354c62f73d9/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig04_10_ControlSystemLaplace.jpg)
*Figure 4.10. Block diagram of a linear control system in the frequency domain.*

Theoretically we can choose controller constants, Kp Ki and Kd, to create the desired controller response. Unfortunately it can be difficult to estimate m and τ. If a load is applied to the motor, then m and τ will change.

To simplify the PID controller implementation, we break the controller equation into separate proportion, integral and derivative terms. I.e., let

```
U(t) = P(t) + I(t) + D(t)
```

where U(t) is the actuator output, and P(t), I(t) and D(t) are the proportional, integral and derivative components respectively. The proportional term makes the actuator output linearly related to the error. Using a proportional term creates a control system that applies more energy to the plant when the error is large. To implement the proportional term we simply convert it to discrete time.

```
P(t)=KpE(t)⟶P(n)=KpE(n)
```

where the index "n" refers to the discrete time input of E(n) and output of P(n).

Observation: In order to develop digital signal processing equations, it is imperative that the control system be executed on a regular and periodic rate.

Common error: If the sampling rate varies, then controller errors will occur.

The integral term makes the actuator output related to the integral of the error. Using an integral term often will improve the steady state error of the control system. If a small error accumulates for a long time, this term can get large. Some control systems put upper and lower bounds on this term, called anti-reset-windup, to prevent it from dominating the other terms. The implementation of the integral term requires the use of a discrete integral or sum. If I(n) is the current control output, and I(n-1) is the previous calculation, the integral term is simply

```
I(t)=∫0tKiE(τ)dτ⟶I(n)=∑m=1m=nKiE(i)Δt⟶I(n)=I(n−1)+KiE(n)Δt
```

where ∆t is the sampling rate of E(n).

The derivative term makes the actuator output related to the derivative of the error. This term is usually combined with either the proportional and/or integral term to improve the transient response of the control system. The proper value of Kd will provide for a quick response to changes in either the set point or loads on the physical plant. An incorrect value may create an overdamped (very slow response) or an underdamped (unstable oscillations) response. There are a couple of ways to implement the discrete time derivative. The simple approach is

```
D(t)=KddE(t)dt⟶D(n)=KdE(n)−E(n−1)Δt
```

In practice, this first order equation is quite susceptible to noise. Figure 4.11 shows a sequence of E(n) with some added noise. Notice that huge errors occur when the above equation is used to calculate derivative.


![Figure 4.11](https://d37djvu3ytnwxt.cloudfront.net/assets/courseware/v1/f6fbc126722f5f425dd0331d0a555b7f/asset-v1:UTAustinX+UT.RTBN.12.01x+3T2016+type@asset+block/Fig04_11_Derivative.jpg)
Figure 4.11. Illustration of the effect noise plays on the calculation of discrete derivative.

In most practical control systems, the derivative is calculated using the average of two derivatives calculated across different time spans. For example

```
D(n)=Kd(12E(n)−E(n−3)3Δt+12E(n−1)−E(n−2)Δt)
```

that simplifies to

```
D(n)=Kd(E(n)+3E(n−1)−3E(n−2)−E(n−3)6Δt)
```

Linear regression through multiple points can yield the slope and yet be immune to noise.


The goal of this example is to spin a DC motor at a constant speed. A tachometer is used to measure the current speed in rotations per minute (Speed). The operator selects the desired speed, Xstar also in rpm. The motor time constant is defined as the time it takes the motor to reach 63% of the final speed after the delivered power is changed. Typically we run the controller ten times faster than its time constant. For this motor, the time constant is 100 ms, so we run the digital control loop every 10 ms.

```c
void PIControlLoop(void){ // event thread
  E = Xstar-Speed; 
  P = (5250*E)/1000;   // Kp = 5.250
  I = I+(158*E)/1000;  // KiDt = 0.158
  if(I < -500) I=-500; // anti-reset windup
  if(I > 40000) I=40000;
  U = P+I;             // PI controller
  if(U < 100) U=100;   // Constrain output
  if(U>39900) U=39900; // 100 to 39900
  Actuator(U);         // output 
}
```

*Program 4.4. Proportional-integral motor controller.*

