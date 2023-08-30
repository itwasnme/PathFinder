/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>

/* Project Includes */
#include "PWM.h"


/* Timer_A UpDown Configuration Parameter */
Timer_A_UpDownModeConfig upDownConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,          // SMCLK/1 = 12MHz
        1000,                                   // 1000 tick period (1000/12000000 = 0.0000833 seconds)
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,    // Disable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};


/* Timer_A Compare Configuration Parameter  (PWMR) */
Timer_A_CompareModeConfig compareConfig_PWMRIGHT =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_3,          // Use CCR3
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_TOGGLE_SET,              // Toggle output bit
};

/* Timer_A Compare Configuration Parameter (PWML) */
Timer_A_CompareModeConfig compareConfig_PWMLEFT =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_4,          // Use CCR4
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_TOGGLE_SET,              // Toggle output bit
};

void PWM_Init(uint16_t period, uint16_t duty3, uint16_t duty4){

    //Set timer A0period based on parameter
    upDownConfig.timerPeriod = period;

    // converts Duty cycle from 0-100 to a CCR value
    duty3 = (((100-duty3)*1000)/100);
    duty4 = (((100-duty4)*1000)/100);

    // Set the desired duty cycle on P2.6 (TA0 CCR3) and P2.7 (TA0 CCR4)
    compareConfig_PWMRIGHT.compareValue = duty3;
    compareConfig_PWMLEFT.compareValue = duty4;

    // Set Motor pins for CCR Output
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2,
                GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    // Configure Timer_A0 for UpDown Mode
    Timer_A_configureUpDownMode(TIMER_A0_BASE, &upDownConfig);

    // Start TimerA0 in UpDown Mode
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UPDOWN_MODE);

    // Initialize compare registers to generate PWMRIGHT
    Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWMRIGHT);

    // Initialize compare registers to generate PWMLEFT
    Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWMLEFT);
}

void PWM_Duty_Right(uint16_t duty1){
    // converts Duty cycle from 0-100 to a CCR value
    duty1 = (((100-duty1)*1000)/100);
    // Set the desired duty cycle on P2.6 (TA0 CCR3)
    compareConfig_PWMRIGHT.compareValue = duty1;
    // Initialize compare registers to generate PWMRIGHT
    Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWMRIGHT);
}

void PWM_Duty_Left(uint16_t duty4){
    // converts Duty cycle from 0-100 to a CCR value
    duty4 = (((100-duty4)*1000)/100);
    // Set the desired duty cycle on P2.7 (TA0 CCR4)
    compareConfig_PWMLEFT.compareValue = duty4;
    // Initialize compare registers to generate PWMLEFT
    Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWMLEFT);
}

