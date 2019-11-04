#Interrupt Timer
##Description
The interrupt timer makes use the builtin interrupts on the PIC to create accurate delays with a resolution of 2&mu;s.
##Theory
The diagram below shows the clock input into the pre-scalar which scales the frequency into the counter. The scaled clock input increments the counter and when the counter overflows the interrupt flag is set.
![](images\interrupt_diagram.jpg)
[Diagram Source](https://www.google.com/url?sa=i&source=images&cd=&ved=2ahUKEwjBoNTSssnlAhX1D2MBHRT_ACkQjRx6BAgBEAQ&url=https%3A%2F%2Fwww.brainkart.com%2Farticle%2FTimer-counters_7661%2F&psig=AOvVaw2rNMrI7EoopRcUHrTBbYCY&ust=1572710858173270)
##Implementation
TMR0 on the PIC16F877A chip has an 8-bit counter. This means that the counter value can reach 255 before it overflows. The counter is both readable and writable which is imporant to have control of the timings. TMR0 can also accept external clock input however this will not be used. The internal clock has frequency of 4MHz.

TMR0 has the following pre-scalers available: 1:2, 1:4, 1:8, 1:16, 1:32, 1:64, 1:128, 1:256.
These are set in the first three bits of INTCON<0:2>

\(T_{interrupt}\) - Time until interrupt will be triggered.
\(f_{interrupt}\) - Frequency the interrupt will be triggered.
\(f_{clock}\) - Frequency of the onboard timer (4MHz)
\(t_{0}\) - Initial value of the TMR0 register
\(C_{i}\) - Number of times the interrupt is going to be triggered before it is acted upon (this is introduced within the code and not part of the chip).
\(s\) - This is the pre-scalar.

The frequency at which the overflow occurs is:
\[f_{interrupt} = \frac{f_{clock}}{4*s*(256-t_{0})*C_{i}}\]
The time for this to occur can be given by:
\[T_{interrupt} = \frac{1}{f_{interrupt}}\]
These can be rearragned to get the time directly from:
\[T_{interrupt} = \frac{4*s*(256-t_{0})*C_{i}}{f_{clock}}\]
To calculate the t0 value the following formula can be used:
\[t_{0} = 1024sC_{i}-T_{interrupt}f_{clock}\]

###Example
Using the 1:2 pre-scaler allows the timer to have a resolution of 2&mu;s. Which means that one interrupt cycle will last 512&mu;s if we start with a \(t_{0}\) value of 0. If the \(t_{0}\) value is set to 255 then accurate timing down to 2&mu;s can be carried out. By adjusting the pre-scaler the following methods will be implemented:

```c
inline void DelayMicroseconds(const unsigned int delayLength) const;
inline void DelayMiliseconds(const unsigned int delayLength) const;
inline void DelaySeconds(const unsigned char delayLength) const;
```

##Resources
[TMR0](http://www.microcontrollerboard.com/pic-timer0-tutorial.html)