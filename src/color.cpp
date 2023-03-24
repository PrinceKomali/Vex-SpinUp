#include "chassis.h"
#include "color.h"
#include "ports.h"
#include "pros/motors.h"
#include "pros/optical.h"
#include "pros/misc.h"
#include "pros/screen.h"
#include "pros/rtos.h"
#include <stdio.h>

using namespace pros::c;


void backup_roller(int lr) {
    optical_set_led_pwm(COLOR_R,100);
    optical_set_led_pwm(COLOR_L,100);
    int sensor = lr ? COLOR_R : COLOR_L;
    int running = 1;
    while(1) {
        if(running) chassis.set_drive_pid(-2, 30, true);
        optical_rgb_s_t rgb = optical_get_rgb(COLOR_L);
        double red = rgb.red;
        double blue = rgb.blue;
        // if(red + blue < 10) continue;
        if(blue - 5 > red) break;
        motor_move_velocity(Intake, 600);
        // if(running && motor_get_efficiency(Intake) < 85) {
        //     running = 0;
        //     forwards(0,0);
        // }
        screen_set_pen(((int)red << 16) + ((int)rgb.green << 8) + ((int)blue));
        screen_fill_rect(0,0,100,100);
        
    }
    motor_brake(Intake);
    // forwards(0,0);
    delay(500);
    // stop;
    return;
}