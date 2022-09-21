#include "opcontrol.h"
#include "main.h"
#include "ports.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/screen.h"

#define X1 controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_X)
#define Y1 controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_Y)
#define X2 controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_X)
#define Y2 controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_Y)
#define A controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_A)
#define B controller_get_digital(CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_B)

#define FLY_SPEED 85

int a_toggle = 0;
int pressed_a = 0;
void opcontrol() {
	while (1) {
		if(A && !pressed_a) {
			pressed_a = 1;
			a_toggle = !a_toggle;
		} 
		if(!A) pressed_a = 0;
		if(a_toggle) {
			motor_move_velocity(TopIntake, -600);
			motor_move_velocity(BottomIntake, 600);
		}
		else {
			motor_brake(TopIntake);
			motor_brake(BottomIntake);
		}

		motor_move_velocity(Fly1, B * -(Fly1 * FLY_SPEED));
		motor_move_velocity(Fly2, B * -(Fly1 * FLY_SPEED));

		motor_move_velocity(RightFront, (X2 + (X1 - Y1)));
		motor_move_velocity(LeftFront, (-X2 - (X1 + Y1)));
		motor_move_velocity(RightBack, ((-X2 + (X1 + Y1)))*-1);
        motor_move_velocity(LeftBack, ((X2 - (X1 - Y1)))*-1);
	}
}