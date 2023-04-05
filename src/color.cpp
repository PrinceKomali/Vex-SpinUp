#include "chassis.h"
#include "color.h"
#include "ports.h"
#include "pros/motors.h"
#include "pros/optical.h"
#include "pros/misc.h"
#include "pros/screen.h"
#include "pros/rtos.h"
#include <stdio.h>


#include "color.txt"
using namespace pros::c;


void backup_roller(int lr, int reverse) {
    int sensor = lr ? COLOR_R : COLOR_L;
    optical_set_led_pwm(sensor,100);
    // /* REMOVE ME */ chassis.set_drive_pid(-10, 20, false);
    // /* REMOVE ME */ chassis.wait_drive();
    // optical_set_led_pwm(COLOR_L,100);
    int running = 1;
    while(1) {
        if(running) chassis.set_drive_pid(-2, 30, true);
        optical_rgb_s_t rgb = optical_get_rgb(sensor);
        double red = rgb.red;
        double blue = rgb.blue;
        screen_set_pen(((int)red << 16) + ((int)rgb.green << 8) + ((int)blue));
        screen_fill_rect(0,0,100,100);
#ifdef LOOKING_BLUE
        if(red + blue < 450) continue;
        if(red - 30 > blue) break;
#endif
#ifdef LOOKING_RED
        if(red + blue < 10) continue;
        if(blue - 5 > red) break;
#endif
        // if(blue - 5 > red) break;
        motor_move_velocity(Intake, reverse ? -600 : 600 );
        // if(running && motor_get_efficiency(Intake) < 85) {
        //     running = 0;
        //     forwards(0,0);
        // }
       
        
    }

    motor_brake(Intake);
    // forwards(0,0);
    chassis.set_drive_pid(0, 30, true);
    delay(200);
    // stop;
    optical_set_led_pwm(sensor,0);
    return;
}
void backup_roller(int a) { backup_roller(a, 0); }