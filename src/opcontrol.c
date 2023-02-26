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

#define BANGBANG

#ifdef PD_TEST
#define KP 0.1
#define KD 0.1
int current_volt = 0;
int last_error = 0;
int error_before_last = 0;
void set_fly_speed() { // todo: put in loop and run in thread\

    while (1) {
        if (!B) {
            motor_move_voltage(Fly1, -0);
            motor_move_voltage(Fly2, 0);
            return;
        }
        int current = rotation_get_velocity(ROTATION);
        int error = FLY_TARGET - current;
        current_volt = current_volt + (KP * (last_error - error)) +
                       (KD * ((error - (2 * last_error) + error_before_last)) /
                        10); // delay will be 10
        motor_move_voltage(Fly1, -current_volt);
        motor_move_voltage(Fly2, current_volt);

        error_before_last = last_error;
        last_error = error;
        delay(10);
    }
}
#endif

#ifdef P_TEST
#define KMOD 50
int current_volt = 8000;
void set_fly_speed() {
    while (1) {
        if (!B) {
            motor_move_voltage(Fly1, -0);
            motor_move_voltage(Fly2, 0);
            current_volt = 12000;
            // return;
            continue;
        }

        int r = rotation_get_velocity(ROTATION);
        int error = FLY_TARGET - 10;
        if (r > FLY_TARGET) current_volt -= KMOD;
        else
            current_volt += KMOD;
        motor_move_voltage(Fly1, -current_volt);
        motor_move_voltage(Fly2, current_volt);
        delay(10);
    }
}
#endif
#ifdef BANGBANG
// #define KMOD 50
int current_volt = 0;
void set_fly_speed() {
    while (1) {
        if (competition_is_autonomous() || competition_is_disabled()) return;
        if (!B) {
            motor_move_voltage(Fly1, -0);
            motor_move_voltage(Fly2, 0);
            current_volt = 12000;
            // return;
            continue;
        }

        int r = rotation_get_velocity(ROTATION);
        int error = FLY_TARGET - 10;
        if (r > FLY_TARGET) current_volt = 0;
        else
            current_volt = 12000;
        motor_move_voltage(Fly1, -current_volt);
        motor_move_voltage(Fly2, current_volt);
        delay(20);
    }
}
#endif
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
        // set_fly_speed();

        // motor_move_voltage(Fly1, B * -(FLY_TARGET)); // R
        // motor_move_voltage(Fly2, B * (FLY_TARGET)); // REMOVE ME!

        if (L) {
            imu_reset(GYRO);
        }

        // Intake
        if (a_toggle) {
            motor_move_velocity(TopIntake, -INTAKE_SPEED);
            motor_move_velocity(BottomIntake, INTAKE_SPEED);
        } else {
            motor_brake(TopIntake);
            motor_brake(BottomIntake);
        }
        if (ZL && !X) {
            a_toggle = 0;
            motor_move_voltage(TopIntake, 9000);
            motor_move_voltage(BottomIntake, -9000);
        }

        // Endgame
        if (ZL && X) exp_n = 1;
        adi_digital_write(Launcher, exp_n * 5);
        adi_digital_write(Pneumatic, y_toggle * 5);

        // Driving
        motor_move_velocity(RightFront, (X2 + (X1 - Y1)));
        motor_move_velocity(LeftFront, (-X2 - (X1 + Y1)));
        motor_move_velocity(RightBack, ((-X2 + (X1 + Y1))) * -1);
        motor_move_velocity(LeftBack, ((X2 - (X1 - Y1))) * -1);
    }
}