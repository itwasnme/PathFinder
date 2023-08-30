/* DriverLib Includes */
#include <driverlib.h>
#include <msp.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Project Includes */
#include "Bump.h"

// Define statements for bump switch pins
#define BUMP_PORT   GPIO_PORT_P4
#define BUMP0       GPIO_PIN0   // P4.0
#define BUMP1       GPIO_PIN2   // P4.2
#define BUMP2       GPIO_PIN3   // P4.3
#define BUMP3       GPIO_PIN5   // P4.5
#define BUMP4       GPIO_PIN6   // P4.6
#define BUMP5       GPIO_PIN7   // P4.7
#define BUMP_PINS   (BUMP0 | BUMP1 | BUMP2| BUMP3| BUMP4 | BUMP5)

// Initialize the bump switch pins as GPIO inputs with pull up resistors
// Switches are active low (* Port 4, Pins 0, 2, 3, 5, 6, and 7)
void Bump_Init(void){
   // The six Bump switches are initialized with pull up resistors:
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN0);
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN2);
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN3);
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN5);
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN6);
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN7);
}

// reads values of bump switches
//gives result with positive logic
uint8_t Bump_Read(void){
    // Get input from each pin, since we want to store all data in a single variable, we push bits from further bump switches to the left (towards most significant bits).

    //Bit data of P4.0 is the LSB ( xxxx xxx() )
    uint8_t lowest = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN0);
    uint8_t lowes = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN2);
    //Push bit data of P4.2 one place ( xxxx xx()x )
    lowes = lowes<<1;
    uint8_t mi = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN3);
    //Push bit data of P4.3 two places ( xxxx x()xx )
    mi = mi<<2;
    uint8_t mid = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN5);
    //Push bit data of P4.5 three places ( xxxx ()xxx )
    mid = mid<<3;
    uint8_t highes = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN6);
    //Push bit data of P4.6 four places ( xxx() xxxx )
    highes = highes<<4;
    uint8_t highest = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN7);
    //Push bit data of P4.7 four places ( xx()x xxxx )
    highest = highest<<5;
    //Add all variables holding bump switches data to store all info in a single 8 bit variable.
    uint8_t datum1 = lowest+lowes+mi+mid+highes+highest;
    //Use Bitwise not operator (recall bump switches are initialized with pull up resistors)
    //But we want a 1 to represent a bump being pressed, so we flip bits...
    datum1 = ~datum1;
    //Since we only have 6 bumps, we want to set bit 6 and 7 to 0. So we use a bitwise AND with (0011 1111) so set those to 0 and let the other bits unchanged (mask it).
    datum1 = datum1 & 0x3F;
    return datum1;
}

#define BUMP_INTERRUPT_PRIORITY 0
void (*BumpTask)(uint8_t bumpData); // function pointer for user task when bump interrupt is detected

// Initializes the bump switch pins and enables Port 4 GPIO interrupts
void Bump_Interrupt_Init(void(*task)(uint8_t)){
    //Write this for Interrupt Module Race-Day
    // set BumpTask to be then user function to be called in ISR,
    BumpTask = task;
    // initialize bump pins as GPIO inputs with pull up resistors
    Bump_Init();
    // configure falling edge interrupts on bump pins (bump switches have pull-up resistors).
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN0, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN2, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN3, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN7, GPIO_HIGH_TO_LOW_TRANSITION);
    // clear interrupt flags on bump pins
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN7);
    // enable interrupts with GPIO on the bump pins
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN7);
    // enable the P4 interrupts in the NVIC
    Interrupt_enableInterrupt(INT_PORT4);
    // set the bump interrupts to the desired priority (remember to shift it to the correct location)
    Interrupt_setPriority(INT_PORT4,BUMP_INTERRUPT_PRIORITY<<5); //shift the desired priority level to get it into the highest 3 bits
}

// ISR for bump interrupts
// clear interrupt flag, read bump switches, and call user function for handling a collision
// there is only one line of code for you to add to this function, that is the call to the DL function 
// that clears the interrupt flag.
void PORT4_IRQHandler(void){
    uint8_t bumpData;
    //write this for Interrupt Module Race-Day
    // read bump switch data to be passed to the bump task
    bumpData = Bump_Read();
    // Call the bump task with the bumpData
    BumpTask(bumpData);
    // clear interrupt flags
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN0 || GPIO_PIN6 || GPIO_PIN5 || GPIO_PIN3 || GPIO_PIN2 || GPIO_PIN7 );
}
