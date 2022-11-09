#ifndef _ctrl
#define _ctrl

#define X1 (int)((float)controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_X)*0.787*2)
#define Y1 (int)((float)controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_Y)*0.787*2)
#define X2 (int)((float)controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_X)*0.787*2)
#define Y2 (int)((float)controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_Y)*0.787*2)
#define A controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_A)
#define B controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_B)
#define Y controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_Y)
#define X controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)
#define D_UP controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_UP)
#define D_DOWN controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_DOWN)
#define D_RIGHT controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_RIGHT)
#define D_LEFT controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_LEFT)
#define ZL controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_L2)

#endif
