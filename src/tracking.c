#include "tracking.h"
#include "ports.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rotation.h"
#include "pros/rtos.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI 
    #define M_PI 3.14 
    // get rid of vscode warning
#endif


#define MIN_VOLT 1700
#define TURN_SPEED 6
// #define sign(n) ((int)n / fabs((int)n))
#define forwards(b, a)                                                         \
    motor_move_voltage(R1, a * -1);                                       \
    motor_move_voltage(R2, a);                                       \
    motor_move_voltage(R3, a * -1);                                        \
    motor_move_voltage(L1, b);                                        \
    motor_move_voltage(L2, b * -1);                                        \
    motor_move_voltage(L3, b);
#define brake()                                                         \
    motor_brake(R1);                                       \
    motor_brake(R2);                                       \
    motor_brake(R3);                                        \
    motor_brake(L1);                                        \
    motor_brake(L2);                                        \
    motor_brake(L3);

void a_opcontrol() {
    float dist_from_center = (17.5 / 2 - 2.6) * 25.4;
    float wheel_circum = (4 * 25.4) * M_PI;
    float tracking_circum = 2.75 * 25.4* M_PI;

    // 360° rotation
    float tracking_circle_circum = (dist_from_center * M_PI * 2);

    float num_rot_per_360 = tracking_circle_circum / tracking_circum;
    float num_rot_cd = num_rot_per_360 * 36000;

    int target = (int)num_rot_cd / 4;
    int error = 12345678;
    // rotation_reset_position(TRACKING_WHEEL);
    while(fabs(error) > 25) {
        int c = rotation_get_position(TRACKING_WHEEL);
        error = target - c;
        char buf[100];
        snprintf(buf, 100, "%d   %d         ", error);
        controller_set_text(E_CONTROLLER_MASTER, 0, 1, buf);
        int r = fmax(fabs(error) / TURN_SPEED, MIN_VOLT);
        if(error < 0) {
            forwards(r, -r);
        }
        else {
            forwards(-r, r);
        }
        delay(10);
    } ;
    brake();
    delay(500);
    forwards(0,0);
    
}