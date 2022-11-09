#include "opcontrol.h"
#include "main.h"
#include "ports.h"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/screen.h"
#include <stdio.h>
#include "ctrl.h"

#define FLY_MAX 12000
#define FLY_MIN 2000


#define STAGE_4 10500
#define STAGE_3 9000
#define STAGE_2 8500
#define STAGE_1 8000




int FLY_SPEED = 9000;
int STAGE_NUM = 0;

int a_toggle = 0;
int pressed_a = 0;
int y_toggle = 0;
int pressed_y = 0;
int p_bool = 0;

int pressed_up = 0;
int pressed_down = 0;
int pressed_right = 0;
int pressed_left = 0;

void set_stage() {
	FLY_SPEED = ((int[]){STAGE_1, STAGE_2, STAGE_3, STAGE_4})[STAGE_NUM];
}
int get_stage() {
	return ((int[]){STAGE_1, STAGE_2, STAGE_3, STAGE_4})[STAGE_NUM];
}
void opcontrol() {
	while (1) {
		char buf[100];
		snprintf(buf, 100, "%d | STG = %d      ", FLY_SPEED/100, STAGE_NUM + 1); 
		
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
		if(ZL && !X) {
			a_toggle = 0;
			motor_move_voltage(TopIntake, 9000);
			motor_move_voltage(BottomIntake, -9000);
		}
		if(Y && !pressed_y) {
			pressed_y = 1;
			y_toggle = !y_toggle;
			p_bool = !p_bool;
		} 
		if(!Y) pressed_y = 0;

		if(D_UP && !pressed_up) {
			pressed_up = 1;
			if(FLY_SPEED < FLY_MAX) FLY_SPEED += 500;
		}
		if(!D_UP) pressed_up = 0;

		if(D_DOWN && !pressed_down) {
			pressed_down = 1;
			if(FLY_SPEED > FLY_MIN) FLY_SPEED -= 500;
		}
		if(!D_DOWN) pressed_down = 0; 

		if(D_RIGHT && !pressed_right) {
			
			pressed_right = 1;
			STAGE_NUM = (STAGE_NUM + 1) % 4;
			if(FLY_SPEED == STAGE_1 || FLY_SPEED == STAGE_2 || FLY_SPEED == STAGE_3 || FLY_SPEED == STAGE_4) set_stage();
		}
		if(!D_RIGHT) pressed_right = 0;
		if(D_LEFT && !pressed_left) {
			pressed_left = 1;
			STAGE_NUM = STAGE_NUM == 0 ? 3 : STAGE_NUM - 1;
			if(FLY_SPEED == STAGE_1 || FLY_SPEED == STAGE_2 || FLY_SPEED == STAGE_3 || FLY_SPEED == STAGE_4) set_stage();
		}
		if(!D_LEFT) pressed_left = 0; 
		if(X) FLY_SPEED = get_stage();
		
		adi_digital_write(Launcher, ZL * X  * 5);
		adi_digital_write(Pneumatic, y_toggle * 5);
		motor_move_voltage(Fly1, B * -(FLY_SPEED));
		motor_move_voltage(Fly2, B * (FLY_SPEED));

		motor_move_velocity(RightFront, (X2 + (X1 - Y1)));
		motor_move_velocity(LeftFront, (-X2 - (X1 + Y1)) );
		motor_move_velocity(RightBack, ((-X2 + (X1 + Y1))) * -1);
        motor_move_velocity(LeftBack, ((X2 - (X1 - Y1))) * -1);
	}
}