#ifndef _ctrl
#define _ctrl

#define X1 (int)((float)controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_X)*0.787*2)
#define Y1 (int)((float)controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_Y)*0.787*2)
#define X2 (int)((float)controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_X)*0.787*2)
#define Y2 (int)((float)controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_Y)*0.787*2)
#define A controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_A)
#define B controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_B)
#define Y controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_Y)
#endif
