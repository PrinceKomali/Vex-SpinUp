#include "auton.h"
#include "ctrl.h"
#include "main.h"
#include "ports.h"
#include "pros/adi.h"
#include "pros/rtos.h"

#define AUTON_FLY_SPEED 9700
#define INTAKE_SPEED 600

#define forwards(b,a) \
    motor_move_velocity(RightFront, -a); \
    motor_move_velocity(RightBack, -a); \
    motor_move_velocity(LeftFront, -b); \
    motor_move_velocity(LeftBack, -b);

#define fly_start \
    motor_move_voltage(Fly1, -(AUTON_FLY_SPEED)); \
	motor_move_voltage(Fly2, (AUTON_FLY_SPEED));

#define fly_stop \
    motor_move_voltage(Fly1, 0); \
	motor_move_voltage(Fly2, 0);

#define conv_start \
    motor_move_velocity(TopIntake, -INTAKE_SPEED); \
	motor_move_velocity(BottomIntake, INTAKE_SPEED);

#define conv_reverse \
    motor_move_velocity(TopIntake, INTAKE_SPEED); \
	motor_move_velocity(BottomIntake, -INTAKE_SPEED);

#define conv_stop \
    motor_move_velocity(TopIntake, 0); \
	motor_move_velocity(BottomIntake, 0);

#define piston_up adi_digital_write(Pneumatic, 0);
#define piston_down adi_digital_write(Pneumatic, 5);

#define strafe(v) \
    motor_move_velocity(RightFront, v); \
    motor_move_velocity(RightBack, -v); \
    motor_move_velocity(LeftFront, -v); \
    motor_move_velocity(LeftBack, v);

void autonomous() {
    fly_start;
    piston_down;
    forwards(14, 200);
    delay(500);

    forwards(190, 30);
    delay(800);
    forwards(100, 100);
    delay(110);
    forwards(0, 0)
    
    delay(1300);
    piston_up;
    conv_start;

	delay(2000);
    conv_stop;
    fly_stop;
    forwards(-200,200)
    delay(350);
    forwards(-200,-200);
    delay(750);
    forwards(0, -200);
    delay(500);
    forwards(-30, -30);
    delay(1000);
    conv_reverse;
    delay(500);
    forwards(0, 0);
    conv_stop;
}

