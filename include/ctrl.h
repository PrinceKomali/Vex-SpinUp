#ifndef _ctrl
#define _ctrl

#define X1 (int)((float)pros::c::controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_X)*0.787)
#define Y1 (int)((float)pros::c::controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_Y)*0.787)
#define X2 (int)((float)pros::c::controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_X)*0.787)
#define Y2 (int)((float)pros::c::controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_Y)*0.787)
#define A pros::c::controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_A)
#define B pros::c::controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_B)
#define Y pros::c::controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_Y)
#define X pros::c::controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_X)
#define D_UP pros::c::controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_UP)
#define D_DOWN pros::c::controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_DOWN)
#define D_RIGHT pros::c::controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_RIGHT)
#define D_LEFT pros::c::controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_LEFT)
#define ZL pros::c::controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_L2)
#define ZR pros::c::controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_R2)
#define L pros::c::controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_L1)
#define R pros::c::controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_R1)

#endif
