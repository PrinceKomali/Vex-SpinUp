#include "main.h"
#include "color.h"
#include "chassis.h"
#include "ports.h"
using namespace pros;
using namespace pros::c;

#define DRIVE_SPEED 110 
#define TURN_SPEED  80
#define SWING_SPEED 90

#define piston_up() adi_digital_write(Pneumatic, 0);
#define piston_down() adi_digital_write(Pneumatic, 5);
#define raiser_up() adi_digital_write(AngleRaiser, 0);;
#define raiser_down() adi_digital_write(AngleRaiser, 5);
 #define conv_start() motor_move_velocity(Intake, -600); 
 #define conv_stop() motor_move_velocity(Intake, 0); 
#define fly_spin() motor_move_voltage(Fly1, 9000);
void default_constants() {
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

#define SENSITIVITY 20
void shoot(int d, int s = 12500) {
    int current_volt_a = s;
    int cutoff_timer = 0;
    int cutoff_bool = 0;
    while (1) {
        if (cutoff_timer >= d)
            break;
        int r = -1 * rotation_get_velocity(ROTATION);
        int error = s - r;
        if (r > s)
            current_volt_a /= 2;
        else
            current_volt_a = 12000;
        if (error < SENSITIVITY) {
            cutoff_bool = 1;
            // motor_move_velocity(Intake, -75);
        }
        motor_move_voltage(Fly1, current_volt_a);
        delay(20);
        if (cutoff_bool) {
            if(r + 1000 < s) {
                motor_move_velocity(Intake, 0);
            } else {
                motor_move_velocity(Intake, -200);
                cutoff_timer += 20;
            }
        }
    }
    cutoff_bool = 0;
    cutoff_timer = 0;
    motor_move_velocity(Intake, 0);
    motor_move_voltage(Fly1, 0);
}

void auton_skills() {
    // preloads
    piston_up();
    raiser_down();
    fly_spin();
    chassis.set_turn_pid(5.9, TURN_SPEED);
    chassis.wait_drive();
    shoot(1500, 14600);
    chassis.set_turn_pid(0, TURN_SPEED);
    chassis.wait_drive();

    // roller 1
    chassis.set_drive_pid(-38, 110, true);
    chassis.wait_drive();
    chassis.set_turn_pid(90, TURN_SPEED);
    chassis.wait_drive();
    backup_roller(1);

    // 3 line 1
    chassis.set_drive_pid(6, 110, true);
    chassis.wait_drive();
    chassis.set_turn_pid(220.5, TURN_SPEED);
    chassis.wait_drive();
    conv_start();
    piston_down();
    fly_spin();
    chassis.set_drive_pid(-62, 80, true);
    chassis.wait_drive();
    chassis.set_turn_pid(315.5, TURN_SPEED);
    chassis.wait_drive();
    conv_stop();
    piston_up();
    shoot(2000, 14300);

    // 3 stack 1
    piston_down();
    chassis.set_turn_pid(225, TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(-37, 110, true);
    chassis.wait_drive();
    pros::delay(200);
    conv_start();
    chassis.set_drive_pid(-30, 80, true);
    chassis.wait_drive();
    chassis.set_turn_pid(270, TURN_SPEED);
    chassis.wait_drive();
    conv_stop();
    raiser_up();
    fly_spin();
    chassis.set_drive_pid(47, 110, true);
    chassis.wait_drive();
    piston_up();
    conv_start();
    chassis.set_turn_pid(266, TURN_SPEED);
    chassis.wait_drive();
    shoot(2000, 14000);

    // roller 2
    raiser_down();
    // chassis.set_turn_pid(272, TURN_SPEED);
    // chassis.wait_drive();
    chassis.set_drive_pid(-49, 110, true);
    chassis.wait_drive();
    chassis.set_turn_pid(180, TURN_SPEED);
    chassis.wait_drive();
    backup_roller(0);

    // roller 3
    chassis.set_drive_pid(24, 110, true);
    chassis.wait_drive();
    chassis.set_turn_pid(270, TURN_SPEED);
    chassis.set_drive_pid(-12, 75, true);
    chassis.wait_drive();



    
}
void auton_right() {
    fly_spin();
    chassis.set_turn_pid(35, TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(18, 110, true);
    chassis.wait_drive();
    chassis.set_turn_pid(20, TURN_SPEED);
    chassis.wait_drive();
    shoot(2000, 16000);
    chassis.set_turn_pid(-45, TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(-16, 110, true);
    chassis.wait_drive();
    chassis.set_turn_pid(0, TURN_SPEED);
    chassis.wait_drive();
    backup_roller(1);
    chassis.set_drive_pid(4, 110, true);
    chassis.wait_drive();
    chassis.set_turn_pid(135, TURN_SPEED);
    chassis.wait_drive();
    conv_start();
    piston_down();
    fly_spin();
    chassis.set_drive_pid(-65, 110, true);
    chassis.wait_drive();
    pros::delay(200);
    conv_stop();
    chassis.set_turn_pid(47, TURN_SPEED);
    chassis.wait_drive();
    piston_up();
    shoot(3000, 16000);
}
void auton_test() {
    backup_roller(0, 1);
}
void auton_left() {
    // backup_roller(0);
    raiser_down();
    piston_up();
    chassis.set_turn_pid(-4.9, TURN_SPEED);
    chassis.wait_drive();
    shoot(2000, 17200);
    chassis.set_turn_pid(0, TURN_SPEED);
    chassis.wait_drive();
    backup_roller(0);
    chassis.set_drive_pid(8, 100, true);
    chassis.wait_drive();
    chassis.set_turn_pid(-135, TURN_SPEED);
    chassis.wait_drive();
    piston_down();
    chassis.set_drive_pid(-24, 100, true);
    chassis.wait_drive();
    conv_stop();
    // pros::delay(500);
    conv_start();
    chassis.set_drive_pid(-24, 50, true);
    chassis.wait_drive();
    chassis.set_turn_pid(332, TURN_SPEED);
    chassis.wait_drive();
    conv_stop();
    piston_up();
    shoot(3000, 16000);
}


// void autonomous() { auton_test(); }
void autonomous() { auton_skills(); }
// #include "mode.txt"


void tug (int attempts) {
  for (int i=0; i<attempts-1; i++) {
    // Attempt to drive backwards
    printf("i - %i", i);
    chassis.set_drive_pid(-12, 127);
    chassis.wait_drive();

    // If failsafed...
    if (chassis.interfered) {
      chassis.reset_drive_sensor();
      chassis.set_drive_pid(-2, 20);
      pros::delay(1000);
    }
    // If robot successfully drove back, return
    else {
      return;
    }
  }
}
// If there is no interference, robot will drive forward and turn 90 degrees. 
// If interfered, robot will drive forward and then attempt to drive backwards. 
void interfered_example() {
 chassis.set_drive_pid(24, DRIVE_SPEED, true);
 chassis.wait_drive();

 if (chassis.interfered) {
   tug(3);
   return;
 }

 chassis.set_turn_pid(90, TURN_SPEED);
 chassis.wait_drive();
}

