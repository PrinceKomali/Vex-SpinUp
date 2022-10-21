#include "opcontrol.h"
#include "main.h"
#include "ports.h"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/screen.h"
#include <stdio.h>

#include "ctrl.h"

#define FLY_SPEED 8700

int a_toggle = 0;
int pressed_a = 0;
int y_toggle = 0;
int pressed_y = 0;
int p_bool = 0;
void opcontrol() {
	while (1) {
		char buf[100];
		snprintf(buf, 100, "%.3f      %.3f", motor_get_actual_velocity(Fly1), motor_get_actual_velocity(Fly2)); 
		
		controller_set_text(E_CONTROLLER_MASTER, 1, 1, buf);
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
		if(Y && !pressed_y) {
			pressed_y = 1;
			y_toggle = !y_toggle;
			p_bool = !p_bool;
		} 
		if(!Y) pressed_y = 0;
		
		adi_digital_write(Pneumatic, y_toggle * 5);
		motor_move_voltage(Fly1, B * -(FLY_SPEED));
		motor_move_voltage(Fly2, B * (FLY_SPEED));

		motor_move_velocity(RightFront, (X2 + (X1 - Y1)));
		motor_move_velocity(LeftFront, (-X2 - (X1 + Y1)) );
		motor_move_velocity(RightBack, ((-X2 + (X1 + Y1))) * -1);
        motor_move_velocity(LeftBack, ((X2 - (X1 - Y1))) * -1);
	}
}