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
void default_constants() {
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}


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
        if (error < 2) {
            cutoff_bool = 1;
            motor_move_velocity(Intake, -75);
        }
        motor_move_voltage(Fly1, current_volt_a);
        delay(20);
        if (cutoff_bool)
            cutoff_timer += 20;
    }
    cutoff_bool = 0;
    cutoff_timer = 0;
    motor_move_velocity(Intake, 0);
    motor_move_voltage(Fly1, 0);
}


void auton_long() {
    // first shots
    piston_up();
    raiser_down();
    // raiser_up();
    chassis.set_turn_pid(14, TURN_SPEED);
    chassis.wait_drive();
    shoot(2000, 14200);
    chassis.set_turn_pid(0, TURN_SPEED);
    chassis.wait_drive();
    // raiser_down();
    pros::delay(200);
    chassis.set_drive_pid(-48, 100, true);
    chassis.wait_drive();
    chassis.set_turn_pid(90, TURN_SPEED);
    chassis.wait_drive();
    backup_roller(1);

    chassis.set_drive_pid(28, 100, true);
    chassis.wait_drive();
    chassis.set_turn_pid(0, TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(-24, 100, true);
    chassis.wait_drive();

    // backup roller
    chassis.set_drive_pid(28, 100, true);
    chassis.wait_drive();
    chassis.set_turn_pid(225, TURN_SPEED);
    chassis.wait_drive();

    piston_down();
    conv_start();
    chassis.set_drive_pid(-60, 40, true);
    chassis.wait_drive();
    conv_stop();

    chassis.set_drive_pid(84, 100, true);
    chassis.wait_drive();
    chassis.set_turn_pid(90, TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(60, 100, true);
    chassis.wait_drive();
    chassis.set_turn_pid(105, TURN_SPEED);
    chassis.wait_drive();
    piston_up();
    shoot(2000, 15000);
    chassis.set_turn_pid(135, TURN_SPEED);
    chassis.wait_drive();
    chassis.set_drive_pid(-24, 110, true);
    chassis.wait_drive();
    pros::delay(700);
    piston_down();
    conv_start();
    chassis.set_drive_pid(-24, 20, true);
    chassis.wait_drive();
    pros::delay(600);
    conv_stop();
    chassis.set_drive_pid(48, 100, true);
    chassis.wait_drive();
    chassis.set_turn_pid(135, TURN_SPEED);
    chassis.wait_drive();
    piston_up();
    shoot(2000, 15000);
    

    

    



}



// void autonomous() { auton_long(); }
#include "mode.txt"


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
