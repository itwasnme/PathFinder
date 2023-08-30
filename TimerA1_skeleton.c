/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>


/* Project Includes */
#include "TimerA1.h"

    Timer_A_UpModeConfig configuration =  // "up" counts from 0 to the value in ccr0
    {
     TIMER_A_CLOCKSOURCE_SMCLK, // Sets clock for our timer to use (4 options). SMCLK uses the HSMCLK clock resource selection for
     //its clock resource.
     TIMER_A_CLOCKSOURCE_DIVIDER_12, //Sets the clock divider (The larger, the smaller the frequency so the ticks go slooooow (less ticks per second)). 12MHz / 12 = 1 MHz
     //65535, //Largest decimal value for unsigned 16 bits (timer register size)
     1, //SET PERIOD, will change after calling TimerA1_Init
     TIMER_A_TAIE_INTERRUPT_DISABLE, //disable Timer_A interrupt
     TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE, //Enable Timer_A CCR0 captureCompare interrupt
     TIMER_A_DO_CLEAR // Clock divider, count direction, count need to be reset...
    };

// Pointer to user function that gets called on timer interrupt
void (*TaskPointer)(void);

// Initializes Timer A1 in up mode and triggers a periodic interrupt at a desired frequency
void TimerA1_Init(void(*task)(void), uint16_t period){
    configuration.timerPeriod = period; //Set config period based on parameter
    TaskPointer = task;
    Timer_A_configureUpMode(TIMER_A1_BASE, &configuration); //Configure TimerA1 base module in Up mode
    Interrupt_enableInterrupt(INT_TA1_0);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE); //Starts Timer_A counter
}

// Stops Timer A1
void TimerA1_Stop(void){
    //MODE IS SET IN REGISTER TAxCTL (TA1CTL IN OUR CASE)...
    //UP MODE CONTROL: bits 5-4 -> b01
    //STOP MODE CONTROL: bits 5-4 -> b00
    //USE 0XFFE7 = 1111 1111 1100 1111 TO MASK REGISTER
    TA1CTL = TA1CTL & 0xFFCF; //MASK IT USING BITWISE AND TO SET STOP MODE!
}

// ISR function for Timer A1 periodic interrupt
void TA1_0_IRQHandler(void){
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0); //Clears the capture-compare interrupt flag
    TaskPointer(); // Call the user-defined function using a function pointer
}
