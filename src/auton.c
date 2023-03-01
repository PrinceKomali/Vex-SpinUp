#include "auton.h"
#include "ctrl.h"
#include "main.h"
#include "ports.h"
#include "pros/adi.h"
#include "pros/imu.h"
#include "pros/motors.h"
#include "pros/rtos.h"
#include "pros/vision.h"
#include <math.h>


#define AUTON_FLY_SPEED 10000   // 9950 before
#define AUTON_FLY_SPEED_2 10100 // 9500 before
#define AUTON_FLY_SPEED_3 9000

#define INTAKE_SPEED 600
#define MARGIN_ERROR 0.7

#define forwards(b, a)                                                         \
    motor_move_velocity(RightFront, -a);                                       \
    motor_move_velocity(RightBack, -a);                                        \
    motor_move_velocity(LeftFront, -b);                                        \
    motor_move_velocity(LeftBack, -b);
#define stop                                                                   \
    motor_brake(RightFront);                                                   \
    motor_brake(RightBack);                                                    \
    motor_brake(LeftFront);                                                    \
    motor_brake(LeftBack);

#define fly_start                                                              \
    motor_move_voltage(Fly1, -(AUTON_FLY_SPEED));                              \
    motor_move_voltage(Fly2, (AUTON_FLY_SPEED));

#define fly_start_2                                                            \
    motor_move_voltage(Fly1, -(AUTON_FLY_SPEED_2));                            \
    motor_move_voltage(Fly2, (AUTON_FLY_SPEED_2));
// why
#define fly_start_3                                                            \
    motor_move_voltage(Fly1, -(AUTON_FLY_SPEED_3));                            \
    motor_move_voltage(Fly2, (AUTON_FLY_SPEED_3));

#define fly_stop                                                               \
    motor_move_voltage(Fly1, 0);                                               \
    motor_move_voltage(Fly2, 0);

#define conv_start                                                             \
    motor_move_velocity(TopIntake, -INTAKE_SPEED);                             \
    motor_move_velocity(BottomIntake, INTAKE_SPEED);
#define conv_start_2                                                           \
    motor_move_velocity(TopIntake, -(INTAKE_SPEED - 100));                     \
    motor_move_velocity(BottomIntake, (INTAKE_SPEED - 100));

#define conv_reverse                                                           \
    motor_move_velocity(TopIntake, INTAKE_SPEED);                              \
    motor_move_velocity(BottomIntake, -INTAKE_SPEED);

#define conv_stop                                                              \
    motor_move_velocity(TopIntake, 0);                                         \
    motor_move_velocity(BottomIntake, 0);

#define piston_up adi_digital_write(Pneumatic, 0);
#define piston_down adi_digital_write(Pneumatic, 5);

#define strafe(v)                                                              \
    motor_move_velocity(RightFront, v);                                        \
    motor_move_velocity(RightBack, -v);                                        \
    motor_move_velocity(LeftFront, -v);                                        \
    motor_move_velocity(LeftBack, v);

#define TARGET -4.9
#define TARGET_2 5
#define sign(n) ((int)n / abs((int)n))

// int sign_of_rotation(int current, int target) {
//     int targets[3] = { target - 360, target, target + 360 };
//     int closest = 10000;
//     for(int i = 0; i < 3; i++) {
//         if(abs(current - targets[i]) < abs(closest)) closest = current - targets[i];
//     }
//     return (closest < 0) * 2 - 1;

// }

int q;
void point_in_dir(double tn, double freq /* why did i call it this */,
                  double mins) {
    imu_set_heading(GYRO, 360 - tn);
    while (1) {
        double rot = imu_get_heading(GYRO);
        double d = fabs(rot > 180 ? rot - 360 : rot);
        double s = fmax(d * 300, mins) * (rot > 180 ? 1 : -1);
        if (d < freq)
            break;
        motor_move_voltage(RightFront, s);
        motor_move_voltage(LeftFront, -s);
        motor_move_voltage(RightBack, s);
        motor_move_voltage(LeftBack, -s);
    }
    forwards(0, 0);
    // delay(300);
}
int current_volt_a = 12000;
int FLY_TARGET_A = 12300;
int cutoff_timer = 0;
int cutoff_bool = 0;
void bangbang(int d) {
    while (1) {
        if (cutoff_timer >= d)
            break;
        int r = rotation_get_velocity(ROTATION);
        int error = FLY_TARGET_A - r;
        if (r > FLY_TARGET_A)
            current_volt_a = 0;
        else
            current_volt_a = 12000;
        if (error < 3) {
            cutoff_bool = 1;
            conv_start;
        }
        motor_move_voltage(Fly1, -current_volt_a);
        motor_move_voltage(Fly2, current_volt_a);
        delay(20);
        if (cutoff_bool)
            cutoff_timer += 20;
    }
    cutoff_bool = 0;
    cutoff_timer = 0;
    conv_stop;
    fly_stop;
}
void auton_shoot(int d) {
    conv_start;
    delay(600);
    conv_stop;
    delay(d);
    conv_start_2;
    delay(1200);
    conv_stop;
    fly_stop;
}
void auton_short() {
    piston_up;
    fly_start;
    point_in_dir(360 - 4.6, 0.7, 2450);
    forwards(0, 0);
    delay(2500);
    auton_shoot(1000);
    point_in_dir(4.6, 0.7, 2450);
    delay(200);
    forwards(-100, -100);
    delay(300);
    forwards(0, 0);
    // forwards(-50, -50);
    conv_reverse;
    delay(200); // 150
    conv_stop;
    forwards(0, 0);
}
void auton_short_right() {
    fly_start_2;
    piston_up;
    point_in_dir(25, 0.4, 2300);
    forwards(0, 0);
    delay(2000);
    auton_shoot(3000);
    point_in_dir(335, 0.4, 2300);
    strafe(100);
    delay(1300);
    conv_reverse;
    forwards(-100, -100);
    delay(300);
    // conv_reverse;
    delay(250); // 200
    conv_stop;
    stop;
}
void auton_long() {
    // start
    piston_up;
    point_in_dir(10, 0.4, 2600);
    bangbang(2000);
    point_in_dir(350, 0.2, 2400);
    delay(200);

    // to 1st roller
    forwards(-100, -100);
    delay(2600);
    point_in_dir(90, 0.2, 2400);
    delay(200);

    // turn 1st roller
    forwards(-100, -100);
    conv_start;
    delay(300);
    stop;
    delay(225);
    conv_stop;

    // to 2nd roller
    forwards(100, 100);
    delay(1300);
    point_in_dir(270, 0.2, 2400);
    delay(200);

    // turn 2nd roller
    forwards(-100, -100);
    delay(600);
    conv_start;
    delay(100);
    stop;
    delay(200);
    conv_stop;

    // wall slam :)
    forwards(100, 100);
    // point_in_dir(270, 0.2, 2400);
    delay(500);
    strafe(100);
    delay(900);
    forwards(-200, -200);
    delay(800);
    stop;
    do {
        imu_reset_blocking(GYRO);
    } while (fabs(imu_get_heading(GYRO) - 360) < 0.5);
    forwards(100, 100);
    delay(1700);
    forwards(1,1);
    delay(300);
    point_in_dir(80, 0.2, 2300); // ???
    delay(200);
    forwards(100,100);
    delay(720);

    // get discs
    point_in_dir(135, 0.2, 2300);
    conv_start;
    piston_down;
    forwards(-100,-100);
    delay(3000);
    stop;
    delay(500);
    point_in_dir(310, 0.2, 2300);
    conv_stop;
    delay(200);
    forwards(100,100);
    delay(650);
    stop;
    piston_up;
    cutoff_bool = 0;
    bangbang(3000);
    fly_stop;
    
    // roller 3 
    conv_start; // pick up annoying disc
    forwards(-100, -100);
    delay(950);
    fly_start_3;
    delay(1200);
    stop;
    point_in_dir(100, 0.2, 2300);
    forwards(-100, -100);
    delay(1150);
    stop;
    delay(50);
    conv_stop;
    fly_stop;
    
    // to 4th roller
    forwards(100, 100);
    delay(500);
    strafe(100);
    delay(500);
    forwards(100, 100);
    delay(200);
    conv_start;
    point_in_dir(270, 0.2, 2400);
    delay(200);

    // turn 4th roller
    forwards(-100, -100);
    delay(750);
    stop;
    delay(150);
    conv_stop;

    // endgame
    forwards(100, 100);
    delay(500);
    strafe(-100);
    delay(500);
    point_in_dir(45, 0.2, 2400);
    delay(200);
    forwards(100,100);
    delay(200);
    stop;
    adi_digital_write(Launcher, 5);

    return;
}

#include "mode.txt"

void _autonomous() {
    while (1) {
        vision_signature_s_t red_sig = vision_signature_from_utility(
            1, 7555, 9499, 8526, -1087, -263, -674, 3.000, 0);
        vision_object_s_t rtn = vision_get_by_size(1, 0);
        char buf[100];
        snprintf(buf, 100, "%d    %d      ", rtn.x_middle_coord,
                 rtn.y_middle_coord);

        controller_set_text(E_CONTROLLER_MASTER, 1, 1, buf);
        delay(100);
    }
}