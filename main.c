#include "msp.h"
#include "driverlib.h"
#include <stdint.h>
#include "Clock.h"
#include "FSM.h"
#include "PWM.h"
#include "Motor.h"
#include "Nokia5110.h"

#include "opt3001.h"
#include "bmi160_support.h"
#include "i2c_driver.h"

#define FALSE 0
#define TRUE 1


/////////////////////////////////////////////////////////////////////////////

// BMI160/BMM150
BMI160_RETURN_FUNCTION_TYPE returnValue;
struct bmi160_gyro_t        s_gyroXYZ;
struct bmi160_accel_t       s_accelXYZ;
struct bmi160_mag_xyz_s32_t s_magcompXYZ;


// OPT3001
uint16_t rawData;
float    convertedLux;

//Calibration off-sets
int8_t accel_off_x;
int8_t accel_off_y;
int8_t accel_off_z;
s16 gyro_off_x;
s16 gyro_off_y;
s16 gyro_off_z;

/////////////////////////////////////////////////////////////////////////////


// Global variables
uint8_t datum = 0; //Data from sensor line (location relative to line)
uint8_t bumpData = 0; //Data from bump sensors
short acc1 = 16400; //Data from accelerometer
float light = 110;  //Data from Light Sensor
int stopC = 0; //Counter

void Task(void){ //User-defined function for ISR
    stopC++; //On the remaining interrupts, the ISR should do nothing, except perhaps toggle an LED or !INCREMENT A COUNTER!.

    if((stopC-1)%10==0){ //On every tenth interrupt, starting with the first, the ISR should call Reflectance_Start().
        Reflectance_Start();
        }
    if((stopC-2)%10==0){ //On every tenth interrupt, starting with the second, the ISR should call Reflectance_End() and store the data in a shared global variable.
        datum = Reflectance_End();
        }
    if((stopC-3)%10==0){ //On every tenth interrupt, starting with the third, the ISR should call Bump_Read() and store the data in a shared global variable.
        bumpData = Bump_Read();
    }
    if((stopC-4)%10==0){ //On every fourth interrupt, starting with the third, store the light sensor reading in light for FSM use
        light = convertedLux;
    }
    if((stopC-5)%10==0){ //On every fifth interrupt, starting with the third, store the z-axis accelerometer reading in acc1 for FMS use
        acc1 = s_accelXYZ.z;
    }
}

void initializeLedSignals(void){
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_PIN5);
    GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_PIN7);
    GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_PIN0);
    GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_PIN6);
    GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN6);
}

void main(void)
{
    FSMType lineControler;
    Clock_Init(); //Initialize Clock
    Bump_Init(); //Initialize bump switches
    Motor_Init(); //Initialize motor hardware
    initializeLedSignals(); // Initialize LED hardware (LED outputs for FSM)
    Nokia5110_Init(); //Initialize LCD Screen
    InitializeFSM(&lineControler); // Initialize Finite State Machine (FSM) state variables.
    TimerA1_Init(&Task, 1000); //Initialize timer to count until 1000 since (1000/1MHz = 0.001 seconds)

    initI2C(); //Set up hardware for I2C Protocol
    bmi160_initialize_sensor(); //Initialize accelerometer sensor
    sensorOpt3001Init(); //Initialize Light sensor
    sensorOpt3001Enable(true); //Enable light sensor


    while (TRUE) {
        //Go to next state based on global variables and current state
        lineControler.CurrentState = NextStateFunction(&lineControler,datum,bumpData,light,acc1);
        //Output according to state
        OutputFunction(&lineControler,datum,bumpData);
        readSensors();
    }

}
//Function to read light and accelerometer sensors
void readSensors(){
    sensorOpt3001Read(&rawData); //Read flux raw data
    sensorOpt3001Convert(rawData, &convertedLux); //Convert data
    bmi160_read_accel_xyz(&s_accelXYZ); //Read accelerometer XYZ
    bmi160_initialize_sensor(); //Initialize bmi160 sensor (avoid timer issues)
}


///////////////////BMI160 FUNCTIONS////////////////////////////////////////

void startWakeUpTimerA(uint16_t ulClockMS)
{
    ulClockMS = (ulClockMS * 32768)/1000;

    /* TimerA UpMode Configuration Parameter */
    Timer_A_UpModeConfig upConfig =
    {
            TIMER_A_CLOCKSOURCE_ACLK,              // ACLK Clock Source
            TIMER_A_CLOCKSOURCE_DIVIDER_1,         // ACLK/1 = 32KHz
            ulClockMS,                             // tick period
            TIMER_A_TAIE_INTERRUPT_DISABLE,        // Disable Timer interrupt
            TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,   // Enable CCR0 interrupt
            TIMER_A_SKIP_CLEAR                     // Clear value
    };

    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
    MAP_Timer_A_enableCaptureCompareInterrupt(TIMER_A0_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_0);

    MAP_Interrupt_enableInterrupt(INT_TA0_0);
    MAP_Timer_A_startCounter(TIMER_A0_BASE,TIMER_A_UP_MODE);
}

/***********************************************************
  Function:
 */
void stopWakeUpTimerA(void)
{
    MAP_Interrupt_disableInterrupt(INT_TA0_0);
    MAP_Timer_A_stopTimer(TIMER_A0_BASE);
}

/***********************************************************
  Function: TA0_0_IRQHandler
 */
void TA0_0_IRQHandler(void)
{
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_0);

#ifdef USE_LPM
    MAP_Interrupt_disableSleepOnIsrExit();
#endif
}

/***********************************************************
  Function: WDT_A_IRQHandler
 */
void WDT_A_IRQHandler(void)
{
    //MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    // Waking up from LMP3 take us to PCM_AM_LDO_VCORE0 instead of PCM_AM_LF_VCORE0
//  MAP_PCM_setPowerState(PCM_AM_LDO_VCORE0);
//    MAP_PCM_setCoreVoltageLevel(PCM_AM_DCDC_VCORE0);

#ifdef USE_LPM
    MAP_Interrupt_disableSleepOnIsrExit();
#endif
}

/***********************************************************
  Function: PORT1_IRQHandler
 */
void PORT1_IRQHandler(void)
{
    uint32_t debounce;
    uint32_t status;

    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);

    if(status & GPIO_PIN1)
    {
    }

    /* Delay for switch debounce */
    for(debounce = 0; debounce < 10000; debounce++)
        MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
}
