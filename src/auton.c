#include "auton.h"
#include "ctrl.h"
#include "main.h"
#include "ports.h"
#include "pros/adi.h"
#include "pros/imu.h"
#include "pros/motors.h"
#include "pros/rtos.h"
#include "pros/vision.h"

#define AUTON_FLY_SPEED 9950
#define AUTON_FLY_SPEED_2 9500 

#define INTAKE_SPEED 600

#define forwards(b,a) \
    motor_move_velocity(RightFront, -a); \
    motor_move_velocity(RightBack, -a); \
    motor_move_velocity(LeftFront, -b); \
    motor_move_velocity(LeftBack, -b);
#define stop \
    motor_brake(RightFront); \
    motor_brake(RightBack); \
    motor_brake(LeftFront); \
    motor_brake(LeftBack);

#define fly_start \
    motor_move_voltage(Fly1, -(AUTON_FLY_SPEED)); \
	motor_move_voltage(Fly2, (AUTON_FLY_SPEED));

#define fly_start_2 \
    motor_move_voltage(Fly1, -(AUTON_FLY_SPEED_2)); \
	motor_move_voltage(Fly2, (AUTON_FLY_SPEED_2));
    // why

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


#define TARGET -4.9
#define TARGET_2 5
#define sign(n) ((int)n/abs((int)n))

double s;
double diff = 360; 
void point_in_dir(double tn, double freq /* why did i call it this */, double mins) {
    diff = 999999;
    
    
     while(fabs(diff) > freq) {
        double rot = imu_get_heading(GYRO);
        
        double n = rot > 180 ? rot - 360 : rot;
        double t = tn > 180 ? 360 - tn : tn; 
        
        diff = t - n;
        s = fabs(diff) > 10 ? 8000 : fmax(fabs(diff)*500, mins);
        s *= sign(diff);
        motor_move_voltage(RightFront,s);
        motor_move_voltage(LeftFront,-s);
        motor_move_voltage(RightBack, s);
        motor_move_voltage(LeftBack, -s);
    }
    forwards(0,0);
    delay(500);
}
void auton_short() {
    diff = 999999;
    imu_reset(GYRO);
    // forwards(-50, -50);
    // conv_start;
    // delay(200);
    // conv_stop;
    // forwards(100,100);
    // delay(200);
    // forwards(0, 0);
    while(imu_get_heading(GYRO) > 360) {}
    point_in_dir(TARGET, 0.7, 2200);
    forwards(0,0);
    fly_start;
    delay(3000);
    conv_start;
    fly_start;
    delay(500);
    conv_stop;
    fly_start;
    delay(3000);
    conv_start;
    delay(3000);
    conv_stop;
    fly_stop;
    forwards(40,-50);
    delay(500);
    forwards(-100, -100);
    delay(600);
    forwards(0, 0);
    // forwards(-50, -50);
    conv_reverse;
    delay(700);
    conv_stop;
    forwards(0, 0);
}
void print_fn() {
    while(1) {
        char buf[100];
        snprintf(buf, 100, "     %d               ", diff );
        controller_set_text(E_CONTROLLER_MASTER, 0, 1, buf);
    
        delay(1);
    }
}
void auton_long() {
    task_create(print_fn, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "My Task");
    imu_reset(GYRO);
    while(imu_get_heading(GYRO) > 360) {} // 0.7    
    point_in_dir(TARGET, 0.7, 2300);
    forwards(0,0);
    fly_start;
    delay(3000);
    conv_start;
    fly_start;
    delay(500);
    conv_stop;
    fly_start;
    delay(3000);
    conv_start;/**/
    delay(3000);
    conv_stop;
    fly_stop;
    point_in_dir(0, .5, 2200);
    forwards(0,0);
    // diff = 99999;
    // imu_reset(GYRO);
    // while(imu_get_heading(GYRO) > 360) {}
    delay(500);
    forwards(100,100);
    delay(700);
    // forwards(80, -80);
    // delay(1000);
    // forwards(0,0);
    // delay(500);

    // point_in_dir(90, .4, 2200);

    forwards(0,0);
    forwards(51, -51);
    delay(2000);
    forwards(-100,-100);
    delay(1300);
    forwards(-20, -20);
    conv_reverse;
    delay(250);
    conv_stop;
    forwards(100,100);
    delay(1080);
    forwards(-51, 51);
    delay(2000);
    forwards(-100,-100);
    delay(1000);
    forwards(0,0);
    // forwards(-20, -20);
    conv_reverse;
    delay(230);
    conv_stop;
    forwards(100,100);
    delay(500);
    forwards(0,0);
    strafe(-100);
    delay(500);
    forwards(150, -150);
    delay(300);
    forwards(100,100);
    delay(300);
    forwards(0,0);
    stop;

    adi_digital_write(Launcher, 5);

    return; 
}
#include "mode.txt"

void _autonomous() {
    while(1) {
    vision_signature_s_t red_sig = vision_signature_from_utility(1, 7555, 9499, 8526, -1087, -263, -674, 3.000, 0);
    vision_object_s_t rtn = vision_get_by_size(1, 0);
    char buf[100];
    snprintf(buf, 100, "%d    %d      ", rtn.x_middle_coord, rtn.y_middle_coord); 
		
	controller_set_text(E_CONTROLLER_MASTER, 1, 1, buf);
    delay(100);
    }
}