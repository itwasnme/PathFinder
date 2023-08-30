# include "FSM.h"
#include "driverlib.h"

#define FALSE 0
#define TRUE 1



#define speed 23
#define highSpeed 25
#define lowSpeed 20
#define lightIntensity 20


//

//Set initial state...
void InitializeFSM(FSMType *FSM)
{
    FSM->CurrentState = init;
}

//Set next state based on data from sensor, bump switches, light sensor reading, and accelerometer sensor reading...
FSMState NextStateFunction(FSMType *FSM, uint8_t data, uint8_t bumpData, float light, short acc1)
{
    FSMState NextState = FSM->CurrentState;

    switch (FSM->CurrentState) {
    case init:
        if ((data == 24 || data == 60 || data == 126) && bumpData==0 && light>lightIntensity) {
            NextState = center;
        }
        else if ((data == 16 || data == 48 || data == 32 || data == 56 || data == 252 || data == 248 || data == 124 || data == 120 || data == 127) && bumpData==0 && light>lightIntensity){
            NextState = right;
        }
        else if ((data == 96 || data == 64 || data == 192 || data == 128 || data == 240 || data == 112 || data == 224 || data == 120 || data == 127) && bumpData==0 && light>lightIntensity){
            NextState = rightright;
        }
        else if ((data == 8 || data == 12 || data == 4 || data == 28 || data == 30 || data == 31 || data == 63 || data == 61 || data == 62 || data == 254) && bumpData==0 && light>lightIntensity){
            NextState = left;
        }
        else if ((data == 6 || data == 2 || data == 3 || data == 1 || data == 15 || data == 7 || data == 14 || data == 254) && bumpData==0 && light>lightIntensity){
            NextState = leftleft;
        }
        else if (bumpData>0 && light>lightIntensity){
            NextState = bump;
        }
        else if ((light<=lightIntensity) || data == 153){
            NextState = stop;
        }
        else{
            NextState = lostTurningRight;
        }
        break;
    case center:
        if (acc1>18500 || acc1<15000) {
            NextState = picked;
        }
        else if ((data == 24 || data == 60 || data == 126) && bumpData==0 && light>lightIntensity) {
            NextState = center;
        }
        else if ((data == 16 || data == 48 || data == 32 || data == 56 || data == 252 || data == 248 || data == 124 || data == 120 || data == 127) && bumpData==0 && light>lightIntensity){
            NextState = right;
        }
        else if ((data == 8 || data == 12 || data == 4 || data == 28 || data == 30 || data == 31 || data == 63 || data == 61 || data == 62 || data == 254) && bumpData==0 && light>lightIntensity){
            NextState = left;
        }
        else if (bumpData>0 && light>lightIntensity){
            NextState = bump;
        }
        else if ((light<=lightIntensity) || data == 153){
            NextState = stop;
        }
        else{
            NextState = lostTurningLeft;
        }
        break;
    case right:
        if (acc1>18500 || acc1<15000) {
            NextState = picked;
        }
        else if ((data == 24 || data == 60 || data == 126) && bumpData==0 && light>lightIntensity) {
            NextState = center;
        }
        else if ((data == 16 || data == 48 || data == 32 || data == 56 || data == 252 || data == 248 || data == 124 || data == 120 || data == 127) && bumpData==0 && light>lightIntensity){
            NextState = right;
        }
        else if ((data == 96 || data == 64 || data == 192 || data == 128 || data == 240 || data == 112 || data == 224 || data == 120 || data == 127) && bumpData==0 && light>lightIntensity){
            NextState = rightright;
        }
        else if ((data == 8 || data == 12 || data == 4 || data == 28 || data == 30 || data == 31 || data == 63 || data == 61 || data == 62 || data == 254) && bumpData==0 && light>lightIntensity){
            NextState = left;
        }
        else if (bumpData>0 && light>lightIntensity){
            NextState = bump;
        }
        else if ((light<=lightIntensity) || data == 153){
            NextState = stop;
        }
        else{
            NextState = lostTurningLeft;
        }
        break;
    case rightright:
        if (acc1>18500 || acc1<15000) {
            NextState = picked;
        }
        else if ((data == 16 || data == 48 || data == 32 || data == 56 || data == 252 || data == 248 || data == 124 || data == 120 || data == 127) && bumpData==0 && light>lightIntensity){
            NextState = right;
        }
        else if ((data == 96 || data == 64 || data == 192 || data == 128 || data == 240 || data == 112 || data == 224 || data == 120 || data == 127) && bumpData==0 && light>lightIntensity){
            NextState = rightright;
        }
        else if (bumpData>0 && light>lightIntensity){
            NextState = bump;
        }
        else if ((light<=lightIntensity) || data == 153){
            NextState = stop;
        }
        else{
            NextState = lostTurningLeft;
        }
        break;
    case left:
        if (acc1>18500 || acc1<15000) {
            NextState = picked;
        }
        else if ((data == 24 || data == 60 || data == 126) && bumpData==0 && light>lightIntensity) {
            NextState = center;
        }
        else if ((data == 16 || data == 48 || data == 32 || data == 56 || data == 252 || data == 248 || data == 124 || data == 120 || data == 127) && bumpData==0 && light>lightIntensity){
            NextState = right;
        }
        else if ((data == 8 || data == 12 || data == 4 || data == 28 || data == 30 || data == 31 || data == 63 || data == 61 || data == 62 || data == 254) && bumpData==0 && light>lightIntensity){
            NextState = left;
        }
        else if ((data == 6 || data == 2 || data == 3 || data == 1 || data == 15 || data == 7 || data == 14 || data == 254) && bumpData==0 && light>lightIntensity){
            NextState = leftleft;
        }
        else if (bumpData>0 && light>lightIntensity){
            NextState = bump;
        }
        else if ((light<=lightIntensity) || data == 153){
            NextState = stop;
        }
        else{
            NextState = lostTurningRight;
        }
        break;
    case leftleft:
        if (acc1>18500 || acc1<15000) {
            NextState = picked;
        }
        else if ((data == 8 || data == 12 || data == 4 || data == 28 || data == 30 || data == 31 || data == 63 || data == 61 || data == 62 || data == 254) && bumpData==0 && light>lightIntensity){
            NextState = left;
        }
        else if ((data == 6 || data == 2 || data == 3 || data == 1 || data == 15 || data == 7 || data == 14 || data == 254) && bumpData==0 && light>lightIntensity){
            NextState = leftleft;
        }
        else if (bumpData>0 && light>lightIntensity){
            NextState = bump;
        }
        else if ((light<=lightIntensity) || data == 153){
            NextState = stop;
        }
        else{
            NextState = lostTurningRight;
        }
        break;
    case lostTurningRight:
        if (acc1>18500 || acc1<15000) {
            NextState = picked;
        }
        else if ((data == 24 || data == 60 || data == 126) && bumpData==0 && light>lightIntensity) {
            NextState = center;
        }
        else if ((data == 16 || data == 48 || data == 32 || data == 56 || data == 252 || data == 248 || data == 124 || data == 120 || data == 127) && bumpData==0 && light>lightIntensity){
            NextState = right;
        }
        else if ((data == 96 || data == 64 || data == 192 || data == 128 || data == 240 || data == 112 || data == 224 || data == 120 || data == 127) && bumpData==0 && light>lightIntensity){
            NextState = rightright;
        }
        else if ((data == 8 || data == 12 || data == 4 || data == 28 || data == 30 || data == 31 || data == 63 || data == 61 || data == 62 || data == 254) && bumpData==0 && light>lightIntensity){
            NextState = left;
        }
        else if ((data == 6 || data == 2 || data == 3 || data == 1 || data == 15 || data == 7 || data == 14 || data == 254) && bumpData==0 && light>lightIntensity){
            NextState = leftleft;
        }
        else if (bumpData>0 && light>lightIntensity){
            NextState = bump;
        }
        else if ((light<=lightIntensity) || data == 153){
            NextState = stop;
        }
        else{
            NextState = lostTurningRight;
        }
        break;
    case lostTurningLeft:
        if (acc1>18500 || acc1<15000) {
            NextState = picked;
        }
        else if ((data == 24 || data == 60 || data == 126) && bumpData==0 && light>lightIntensity) {
            NextState = center;
        }
        else if ((data == 16 || data == 48 || data == 32 || data == 56 || data == 252 || data == 248 || data == 124 || data == 120 || data == 127) && bumpData==0 && light>lightIntensity){
            NextState = right;
        }
        else if ((data == 96 || data == 64 || data == 192 || data == 128 || data == 240 || data == 112 || data == 224 || data == 120 || data == 127) && bumpData==0 && light>lightIntensity){
            NextState = rightright;
        }
        else if ((data == 8 || data == 12 || data == 4 || data == 28 || data == 30 || data == 31 || data == 63 || data == 61 || data == 62 || data == 254) && bumpData==0 && light>lightIntensity){
            NextState = left;
        }
        else if ((data == 6 || data == 2 || data == 3 || data == 1 || data == 15 || data == 7 || data == 14 || data == 254) && bumpData==0 && light>lightIntensity){
            NextState = leftleft;
        }
        else if (bumpData>0 && light>lightIntensity){
            NextState = bump;
        }
        else if ((light<=lightIntensity) || data == 153){
            NextState = stop;
        }
        else{
            NextState = lostTurningLeft;
        }
        break;
    case stop:
        if ((light > lightIntensity) && data != 153) {
            NextState = init;
        }
        else{
            NextState = stop;
        }
        break;
    case picked:
        if (light > lightIntensity && data != 255) {
            NextState = init;
        }
        else{
            NextState = picked;
        }
        break;
    case bump:
        if (acc1>18500 || acc1<15000) {
            NextState = picked;
        }
        else if ((data == 24 || data == 60 || data == 126) && bumpData==0 && light>lightIntensity) {
            NextState = center;
        }
        else if ((data == 16 || data == 48 || data == 32 || data == 56 || data == 252 || data == 248 || data == 124 || data == 120 || data == 127) && bumpData==0 && light>lightIntensity){
            NextState = right;
        }
        else if ((data == 96 || data == 64 || data == 192 || data == 128 || data == 240 || data == 112 || data == 224 || data == 120 || data == 127) && bumpData==0 && light>lightIntensity){
            NextState = rightright;
        }
        else if ((data == 8 || data == 12 || data == 4 || data == 28 || data == 30 || data == 31 || data == 63 || data == 61 || data == 62 || data == 254) && bumpData==0 && light>lightIntensity){
            NextState = left;
        }
        else if ((data == 6 || data == 2 || data == 3 || data == 1 || data == 15 || data == 7 || data == 14 || data == 254) && bumpData==0 && light>lightIntensity){
            NextState = leftleft;
        }
        else if (bumpData>0 && light>lightIntensity){
            NextState = bump;
        }
        else if ((light<=lightIntensity) || data == 153){
            NextState = stop;
        }
        else{
            NextState = lostTurningRight;
        }
        break;
    }
    return NextState;
}

// Output based on FSM state (VALOR 00111000?)-
void OutputFunction(FSMType *FSM, uint8_t data, uint8_t bumpData)
{
    switch (FSM->CurrentState) {
    case init: //Do nothing
        break;
    case center: //If the robot is centered, both front indicator lights should be on (brake lights off) and Robot goes straight... Display Status on LCD
        Motor_Forward(speed,speed);
        GPIO_setOutputHighOnPin (GPIO_PORT_P8, GPIO_PIN5);
        GPIO_setOutputHighOnPin (GPIO_PORT_P8, GPIO_PIN0);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN6);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN7);
        Nokia5110_Clear(); //Clear LCD dispaly
        Nokia5110_OutString("  "); //Format...
        Nokia5110_OutChar((data & 0x80 ? '1' : '0')); //Display sensor data (8 bits).
        Nokia5110_OutChar((data & 0x40 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x20 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x10 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x08 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x04 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x02 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x01 ? '1' : '0'));
        Nokia5110_OutString("                 center   RMS:13%     LMS:13%  "); //Display current State and Motors speed.
        Clock_Delay1us(150);
        break;
    case right: //If the robot is right (close) of center, only the right indicator is on and Robot goes slightly left... Display Status on LCD
        Motor_Forward(speed,highSpeed);
        GPIO_setOutputHighOnPin (GPIO_PORT_P8, GPIO_PIN5);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN0);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN6);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN7);
        Nokia5110_Clear();
        Nokia5110_OutString("  ");
        Nokia5110_OutChar((data & 0x80 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x40 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x20 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x10 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x08 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x04 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x02 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x01 ? '1' : '0'));
        Nokia5110_OutString("                 right    RMS:20%     LMS:13%  ");
        Clock_Delay1us(150);
        break;
    case rightright: //If the robot is right (far) of center, only the right indicator is on and Robot goes left... Display Status on LCD
        Motor_Forward(3,highSpeed-5);
        GPIO_setOutputHighOnPin (GPIO_PORT_P8, GPIO_PIN5);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN0);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN6);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN7);
        Nokia5110_Clear();
        Nokia5110_OutString("  ");
        Nokia5110_OutChar((data & 0x80 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x40 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x20 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x10 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x08 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x04 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x02 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x01 ? '1' : '0'));
        Nokia5110_OutString("               rightright RMS:20%     LMS:5%  ");
        Clock_Delay1us(150);
        break;
    case stop: //If convertedLux is under lightIntensity, turn off the motors and the led lights... Display Status on LCD
        Motor_Stop();
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN5);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN0);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN6);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN7);
        Nokia5110_Clear();
        Nokia5110_OutString("  ");
        Nokia5110_OutChar((data & 0x80 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x40 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x20 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x10 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x08 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x04 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x02 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x01 ? '1' : '0'));
        Nokia5110_OutString("      stop    RMS:0%      LMS:0%  ");
        Clock_Delay1us(5000);
        break;
    case left: //If the robot is left (close) of center, only the left indicator is on and Robot goes slightly right... Display Status on LCD
        Motor_Forward(highSpeed,speed);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN5);
        GPIO_setOutputHighOnPin (GPIO_PORT_P8, GPIO_PIN0);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN6);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN7);
        Nokia5110_Clear();
        Nokia5110_OutString("  ");
        Nokia5110_OutChar((data & 0x80 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x40 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x20 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x10 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x08 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x04 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x02 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x01 ? '1' : '0'));
        Nokia5110_OutString("                 left     RMS:13%     LMS:20%  ");
        Clock_Delay1us(150);
        break;
    case leftleft: //If the robot is left (far) of center, only the left indicator is on and Robot goes left... Display Status on LCD
        Motor_Forward(highSpeed-5,3);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN5);
        GPIO_setOutputHighOnPin (GPIO_PORT_P8, GPIO_PIN0);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN6);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN7);
        Nokia5110_Clear();
        Nokia5110_OutString("  ");
        Nokia5110_OutChar((data & 0x80 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x40 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x20 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x10 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x08 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x04 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x02 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x01 ? '1' : '0'));
        Nokia5110_OutString("                leftleft  RMS:5%      LMS:20%  ");
        Clock_Delay1us(150);
        break;
    case lostTurningRight: //If the robot is lost (off the line completely), the brake lights are on and the indicators are off. Since the robot comes from a right turn, the robot rotates right... Display Status on LCD
        Motor_Right(speed,speed);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN5);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN0);
        GPIO_setOutputHighOnPin (GPIO_PORT_P8, GPIO_PIN6);
        GPIO_setOutputHighOnPin (GPIO_PORT_P8, GPIO_PIN7);
        Nokia5110_Clear();
        Nokia5110_OutString("  ");
        Nokia5110_OutChar((data & 0x80 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x40 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x20 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x10 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x08 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x04 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x02 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x01 ? '1' : '0'));
        Nokia5110_OutString("              lostTurningRRMS:13% (B) LMS:13%  ");
        Clock_Delay1us(500);
        break;
    case lostTurningLeft: //If the robot is lost (off the line completely), the brake lights are on and the indicators are off. Since the robot comes from a left turn, the robot rotates left... Display Status on LCD
        Motor_Left(speed,speed);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN5);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN0);
        GPIO_setOutputHighOnPin (GPIO_PORT_P8, GPIO_PIN6);
        GPIO_setOutputHighOnPin (GPIO_PORT_P8, GPIO_PIN7);
        Nokia5110_Clear();
        Nokia5110_OutString("  ");
        Nokia5110_OutChar((data & 0x80 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x40 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x20 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x10 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x08 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x04 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x02 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x01 ? '1' : '0'));
        Nokia5110_OutString("              lostTurningLRMS:13%     LMS:13% (B)  ");
        Clock_Delay1us(500);
        break;
    case bump: //If any bump sensor is activated, all four lights are on and the motors stop... Display Status on LCD
        Motor_Stop();
        GPIO_setOutputHighOnPin (GPIO_PORT_P8, GPIO_PIN5);
        GPIO_setOutputHighOnPin (GPIO_PORT_P8, GPIO_PIN0);
        GPIO_setOutputHighOnPin (GPIO_PORT_P8, GPIO_PIN6);
        GPIO_setOutputHighOnPin (GPIO_PORT_P8, GPIO_PIN7);
        Nokia5110_Clear();
        Nokia5110_OutString("  ");
        Nokia5110_OutChar((data & 0x80 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x40 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x20 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x10 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x08 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x04 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x02 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x01 ? '1' : '0'));
        Nokia5110_OutString("                  bump    RMS:0%      LMS:0%  ");
        Clock_Delay1us(500);
        break;
    case picked: // If the accelerometer changes in the Z-axis, stop motor and turn off Led lights... Display Status on LCD
        Motor_Stop();
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN5);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN0);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN6);
        GPIO_setOutputLowOnPin (GPIO_PORT_P8, GPIO_PIN7);
        Nokia5110_Clear();
        Nokia5110_OutString("  ");
        Nokia5110_OutChar((data & 0x80 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x40 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x20 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x10 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x08 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x04 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x02 ? '1' : '0'));
        Nokia5110_OutChar((data & 0x01 ? '1' : '0'));
        Nokia5110_OutString("                picked    RMS:0%      LMS:0%  ");
        Clock_Delay1us(500);
        break;
    }
}
