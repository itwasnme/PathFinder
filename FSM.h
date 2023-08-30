#ifndef FSM_H_
#define FSM_H_

#include <stdint.h>

// Type Definitions
typedef enum {
    init, center, left, leftleft, right, rightright, lostTurningRight, lostTurningLeft, bump, stop, picked
} FSMState;

typedef struct {
    FSMState CurrentState;      // Current state of the FSM
} FSMType;

// Function Prototypes
void InitializeFSM(FSMType *FSM);
FSMState NextStateFunction(FSMType *FSM, uint8_t data, uint8_t bumpData, float light, short acc1);
void OutputFunction(FSMType *FSM, uint8_t data, uint8_t bumpData);

#endif /* FSM_H_ */
