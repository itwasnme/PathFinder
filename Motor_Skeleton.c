#include "driverlib.h"
#include "PWM.h"
#include "Motor.h"

//   Left Motor:                Right Motor:
//    - Direction: P5.4         - Direction: P5.5
//    - PWM:       P2.7         - PWM        P2.6
//    - Enable:    P3.7         - Enable     P3.6


// initialize PWM outputs to 0% duty cycle on the two motor PWM pins (P2.6 and P2.7)
// initialize motor enable and direction pins as GPIO outputs
// set motors to sleep mode
// set motor direction to forward initially
void Motor_Init(void){
    PWM_Init(1000, 0, 0); // initialize PWM outputs to 0% duty cycle on the two motor PWM pins (P2.6 and P2.7)
    // initialize motor enable and direction pins as GPIO outputs
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN4|GPIO_PIN5); //pin 5.4-5.5
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6|GPIO_PIN7); //pin 3.6-3.7
    // set motor direction to forward initially
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4|GPIO_PIN5); //Forward... (low)
    // set motors to sleep mode
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6|GPIO_PIN7); //Sleep mode... (Low)
}

// Drive both motors forwards at the given duty cycles
void Motor_Forward(uint16_t leftDuty, uint16_t rightDuty){
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4|GPIO_PIN5); //R and L Forward... (low)
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6|GPIO_PIN7); //Enable mode... (High)
    PWM_Init(1000, rightDuty, leftDuty); // initialize PWM outputs based on parameters
}


// Drive both motors backwards at the given duty cycles
void Motor_Backward(uint16_t leftDuty, uint16_t rightDuty){
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN4|GPIO_PIN5); //R and L Backwards... (High)
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6|GPIO_PIN7); //Enable mode... (High)
    PWM_Init(1000, rightDuty, leftDuty); // initialize PWM outputs based on parameters
}



// Drive the right motor forwards and the left motor backwards at the given duty cycles
void Motor_Left(uint16_t leftDuty, uint16_t rightDuty){
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN5);  //R Forward... (low)
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN4); //L Backwards... (High)
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6|GPIO_PIN7); //Enable mode... (High)
    PWM_Init(1000, rightDuty, leftDuty); // initialize PWM outputs based on parameters
}


// Drive the right motor backwards and the left motor forwards at the given duty cycles
void Motor_Right(uint16_t leftDuty, uint16_t rightDuty){
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN5); //R Backwards... (High)
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4); //L Forward... (low)
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6|GPIO_PIN7);//Enable mode... (High)
    PWM_Init(1000, rightDuty, leftDuty); // initialize PWM outputs based on parameters
}


// Stop the motors and enter sleep mode
void Motor_Stop(void){
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6|GPIO_PIN7); //Sleep mode... (Low)
}
