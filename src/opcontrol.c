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

#define sign(n) ((int)n/abs((int)n))

#define FLY_MAX 12000
#define FLY_MIN 2000

#define STAGE_4 10500
#define STAGE_3 9000
#define STAGE_2 8500
#define STAGE_1 8000

#define INTAKE_SPEED 600

#define TARGET -5


int FLY_SPEED = 8000;
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

int exp_n = 0;

void set_stage() {
    FLY_SPEED = ((int[]){STAGE_1, STAGE_2, STAGE_3, STAGE_4})[STAGE_NUM];
}
int get_stage() {
    return ((int[]){STAGE_1, STAGE_2, STAGE_3, STAGE_4})[STAGE_NUM];
}

int vision_check = 0;

double target_speed = 360;
void set_fly_speed() {
    int r = rotation_get_velocity(0); // or whatever port it is
    if(r > target_speed) {
        motor_move_voltage(Fly1, 0);
        motor_move_voltage(Fly2, 0);
    } else {
        motor_move_voltage(Fly1, B * -12000);
        motor_move_voltage(Fly2, B * 12000);
    }
}




void opcontrol() {
    vision_signature_s_t red_sig = vision_signature_from_utility(
            1, 7555, 9499, 8526, -1087, -263, -674, 3.000, 0);
    controller_clear(E_CONTROLLER_MASTER);
    double diff = 0.0;
    int s = 0;
    while (1) {
        double rot = imu_get_heading(GYRO);
        double n = rot > 180 ? rot - 360 : rot;
        if(R) {
            
            double t = TARGET > 180 ? 360 - TARGET : TARGET; 
            /*

            // 350
            //  n = 10
            // t = -5
            // t - n == -5 - 10 == -15 
            fabs(-5) - fabs(10) == -5

            n = 350
            t = -5 
            t - n == -5 - 350 == -355

            350 - -5 


             double diff = 360 - ;
            // 
            // -10 | 30 
            // 30 - -10 -> +

            // -10 | -70 
            -70 - -10 -> -

            // -70 | -20
            -20 - -70 -> +

            -100 | 30
            30 - -100 -> +

            TARGET - ANGLE
            
            

               X        X
            -       |       +
                X
            
            */


            // if(diff < 0) diff += 360.0;
            // diff -= 180.0;

            // if(diff < 0) diff += 360.0; 

             diff = t - n;
            s = fabs(diff) > 10 ? 12000 : fmax(fabs(diff)*500, 1900);
            s *= sign(diff);
            // if(t - n)
            
            // if(n > 2) {
                motor_move_voltage(RightFront,s);
                motor_move_voltage(LeftFront,-s);
                motor_move_voltage(RightBack, s);
                motor_move_voltage(LeftBack, -s);
            // }
        }
            vision_object_s_t rtn = vision_get_by_size(1, 0);
        
        char buf[100];
        snprintf(buf, 100, "%d          %d        ", s,
                 rtn.height < 1);
        controller_set_text(E_CONTROLLER_MASTER, 0, 1, buf);
        snprintf(buf, 100, "%d | STG = %d      ", FLY_SPEED/100, STAGE_NUM + 1); 

        controller_set_text(E_CONTROLLER_MASTER, 1, 1, buf);
        char b2[100];
        snprintf(b2, 100, "%.3f           %.3f    ", rot, n);
        controller_set_text(E_CONTROLLER_MASTER, 2, 1, b2);
        if(L) {
            imu_reset(GYRO);
        }
        if (0) {
            int x = rtn.x_middle_coord;
            if(rtn.width + rtn.height < 1) {
                // do nothing
            }
            else if (x < 100) {
                x = x/2;
                // motor_move_velocity(RightFront, -(100-x));
                // motor_move_velocity(LeftFront, (100-x));
                // motor_move_velocity(RightBack, -(100-x));
                // motor_move_velocity(LeftBack, (100-x));
                motor_move_velocity(RightFront, -50);
                motor_move_velocity(LeftFront,50);
                motor_move_velocity(RightBack, -50);
                motor_move_velocity(LeftBack, 50);
            }
            else {
                x = x/2;
                // motor_move_velocity(RightFront, x-100);
                // motor_move_velocity(LeftFront, -(x-100));
                // motor_move_velocity(RightBack, x-100);
                // motor_move_velocity(LeftBack, -(x-100));
                motor_move_velocity(RightFront, 50);
                motor_move_velocity(LeftFront, -50);
                motor_move_velocity(RightBack, 50);
                motor_move_velocity(LeftBack, -50);
            }
            // vision_clear_led(1);
            vision_check = x;
        } else {
            // char buf[100];
            // snprintf(buf, 100, "%d | STG = %d      ", FLY_SPEED/100,
            // STAGE_NUM + 1);

            // controller_set_text(E_CONTROLLER_MASTER, 1, 1, buf);
            if (A && !pressed_a) {
                pressed_a = 1;
                a_toggle = !a_toggle;
            }
            if (!A)
                pressed_a = 0;
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
            if (Y && !pressed_y) {
                pressed_y = 1;
                y_toggle = !y_toggle;
                p_bool = !p_bool;
            }
            if (!Y)
                pressed_y = 0;

            if (D_UP && !pressed_up) {
                pressed_up = 1;
                if (FLY_SPEED < FLY_MAX)
                    FLY_SPEED += 500;
            }
            if (!D_UP)
                pressed_up = 0;

            if (D_DOWN && !pressed_down) {
                pressed_down = 1;
                if (FLY_SPEED > FLY_MIN)
                    FLY_SPEED -= 500;
            }
            if (!D_DOWN)
                pressed_down = 0;

            if (D_RIGHT && !pressed_right) {

                pressed_right = 1;
                STAGE_NUM = (STAGE_NUM + 1) % 4;
                if (FLY_SPEED == STAGE_1 || FLY_SPEED == STAGE_2 ||
                    FLY_SPEED == STAGE_3 || FLY_SPEED == STAGE_4)
                    set_stage();
            }
            if (!D_RIGHT)
                pressed_right = 0;
            if (D_LEFT && !pressed_left) {
                pressed_left = 1;
                STAGE_NUM = STAGE_NUM == 0 ? 3 : STAGE_NUM - 1;
                if (FLY_SPEED == STAGE_1 || FLY_SPEED == STAGE_2 ||
                    FLY_SPEED == STAGE_3 || FLY_SPEED == STAGE_4)
                    set_stage();
            }
            if (!D_LEFT)
                pressed_left = 0;
            if (X)
                FLY_SPEED = get_stage();
            if(ZL && X) exp_n = 1;

            adi_digital_write(Launcher, exp_n * 5);
            adi_digital_write(Pneumatic, y_toggle * 5);
            motor_move_voltage(Fly1, B * -(FLY_SPEED));
            motor_move_voltage(Fly2, B * (FLY_SPEED));

            motor_move_velocity(RightFront, (X2 + (X1 - Y1)));
            motor_move_velocity(LeftFront, (-X2 - (X1 + Y1)));
            motor_move_velocity(RightBack, ((-X2 + (X1 + Y1))) * -1);
            motor_move_velocity(LeftBack, ((X2 - (X1 - Y1))) * -1);
        }
    }
}