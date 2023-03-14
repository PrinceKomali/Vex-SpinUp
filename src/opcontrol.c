#include "opcontrol.h"
#include "ctrl.h"
#include "main.h"
#include "ports.h"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/screen.h"
#include "pros/vision.h"
#include <stdio.h>

#define sign(n) ((int)n / abs((int)n))

#define FLY_MAX 18600
#define FLY_MIN 2000

#define STAGE_4 16000
#define STAGE_3 13800
#define STAGE_2 12800
#define STAGE_1 12300

#define INTAKE_SPEED STAGE_1

#define TARGET -5

int FLY_TARGET = STAGE_1;
int STAGE_NUM = 0;

int a_toggle = 0;
int pressed_a = 0;
int y_toggle = 0;
int pressed_y = 0;
int pressed_r = 0;
int r_toggle = 0;

int pressed_up = 0;
int pressed_down = 0;
int pressed_right = 0;
int pressed_left = 0;

int exp_n = 0;

void set_stage() {
    FLY_TARGET = ((int[]){STAGE_1, STAGE_2, STAGE_3, STAGE_4})[STAGE_NUM];
}
int get_stage() {
    return ((int[]){STAGE_1, STAGE_2, STAGE_3, STAGE_4})[STAGE_NUM];
}

int vision_check = 0;



int current_volt = 0;
void set_fly_speed() {
    // I HATE BANGBANG
    while (1) {
        if (competition_is_autonomous() || competition_is_disabled()) return;
        if (!B) {
            motor_move_voltage(Fly1, -0);
            current_volt = 12000;
            continue;
        }

        int r = rotation_get_velocity(ROTATION);
        int error = FLY_TARGET - 10;
        if (r > FLY_TARGET) current_volt = 0;
        else
            current_volt = 12000;
        motor_move_voltage(Fly1, current_volt);
        delay(20);
    }
}

void controller_print_msg() {
    while(1) {
        int cdps = rotation_get_velocity(ROTATION);
        // Controller Display
        char buf[100];
        snprintf(buf, 100, "FLY SPEED = %d            ", FLY_TARGET / 100);
        controller_set_text(E_CONTROLLER_MASTER, 0, 1, buf);
        snprintf(buf, 100, "FLY STAGE = %d            ", STAGE_NUM + 1);
        controller_set_text(E_CONTROLLER_MASTER, 1, 1, buf);
        snprintf(buf, 100, "V=%d S=%d            ", current_volt, cdps);
        controller_set_text(E_CONTROLLER_MASTER, 2, 1, buf);
        delay(80);
    }
}

void opcontrol() {
    //  while(1) {
    //     optical_rgb_s_t rgb = optical_get_rgb(COLOR);
    //     double red = rgb.red;
    //     double blue = rgb.blue;
    //     if(red + blue < 10) continue;
    //     if(blue - 5 > red) break;
    //     motor_move_velocity(Intake, 200);
    //     // ((int)red << 16) + ((int)rgb.green << 8) + ((int)blue)
    //     // screen_set_pen(red > blue ? 0xff0000 : 0x0000ff);
    //     // screen_fill_rect(0,0,100,100);
    //     // char buf[100]; 
    //     // snprintf(buf, 100, "%.3f %.3f", red,blue);
    //     // controller_set_text(E_CONTROLLER_MASTER, 0, 1, buf);
        
    // }
    // motor_brake(Intake);
    // return;
    controller_clear(E_CONTROLLER_MASTER);
    task_create(set_fly_speed, NULL, TASK_PRIORITY_DEFAULT,
                TASK_STACK_DEPTH_DEFAULT, "My Task");
    task_create(controller_print_msg, NULL, TASK_PRIORITY_DEFAULT,
                TASK_STACK_DEPTH_DEFAULT, "My Task 2");
    
    while (1) {
        
        // Toggles
        if (A && !pressed_a) {
            pressed_a = 1;
            a_toggle = !a_toggle;
        }
        if (!A) pressed_a = 0;
        if (R && !pressed_r) {
            pressed_r = 1;
            r_toggle = !r_toggle;
        }
        if (!R) pressed_r = 0;
        if (Y && !pressed_y) {
            pressed_y = 1;
            y_toggle = !y_toggle;
        }
        if (!Y) pressed_y = 0;

        // Flywheel Staging - TODO: rework with cdps
        if (D_UP && !pressed_up) {
            pressed_up = 1;
            if (FLY_TARGET < FLY_MAX) FLY_TARGET += 500;
        }
        if (!D_UP) pressed_up = 0;

        if (D_DOWN && !pressed_down) {
            pressed_down = 1;
            if (FLY_TARGET > FLY_MIN) FLY_TARGET -= 500;
        }
        if (!D_DOWN) pressed_down = 0;

        if (D_RIGHT && !pressed_right) {

            pressed_right = 1;
            STAGE_NUM = (STAGE_NUM + 1) % 4;
            if (FLY_TARGET == STAGE_1 || FLY_TARGET == STAGE_2 ||
                FLY_TARGET == STAGE_3 || FLY_TARGET == STAGE_4)
                set_stage();
        }
        if (!D_RIGHT) pressed_right = 0;
        if (D_LEFT && !pressed_left) {
            pressed_left = 1;
            STAGE_NUM = STAGE_NUM == 0 ? 3 : STAGE_NUM - 1;
            if (FLY_TARGET == STAGE_1 || FLY_TARGET == STAGE_2 ||
                FLY_TARGET == STAGE_3 || FLY_TARGET == STAGE_4)
                set_stage();
        }
        if (!D_LEFT) pressed_left = 0;
        if (X) FLY_TARGET = get_stage();

        if (L) {
            imu_reset(GYRO);
        }

        // Intake
        if (a_toggle) {
            motor_move_velocity(Intake, -INTAKE_SPEED);
        } else {
            motor_brake(Intake);
        }
        if (ZL && !X) {
            a_toggle = 0;
            motor_move_voltage(Intake, 9000);
        }

        // Endgame
        if (ZL && X) exp_n = 1;
        adi_digital_write(Launcher, exp_n * 5);
        adi_digital_write(Pneumatic, y_toggle * 5);

        // Driving
        int y1 = Y1 * 2;
        int x2 = sign(X2) * exp((double)abs(X2) * 0.040) * 2; 
        motor_move_velocity(L1, -(y1 - x2));
        motor_move_velocity(L2, (y1 - x2));
        motor_move_velocity(L3, -(y1 - x2));

        motor_move_velocity(R1, (y1 + x2));
        motor_move_velocity(R2, -(y1 + x2));
        motor_move_velocity(R3, (y1 + x2));
    }
}