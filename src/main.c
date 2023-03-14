#include "main.h"
#include "auton.h"
#include "code_from_arizona.h"
#include "opcontrol.h"
#include "ports.h"
#include "pros/imu.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */

void on_center_button() {
    static bool pressed = false;
    pressed = !pressed;
    if (pressed) {
        lcd_set_text(2, "I was pressed!");
    } else {
        lcd_clear_line(2);
    }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    adi_pin_mode(Pneumatic, OUTPUT);
    adi_pin_mode(Launcher, OUTPUT);
    motor_config(L1, COAST, GREEN, true);
    motor_config(L2, COAST, GREEN, true);
    motor_config(L3, COAST, GREEN, true);
    motor_config(R1, COAST, GREEN, true);
    motor_config(R2, COAST, GREEN, true);
    motor_config(R3, COAST, GREEN, true);
    motor_config(Intake, COAST, BLUE, false);
    // motor_config(TopIntake, COAST, BLUE, false);
    motor_config(Fly1, COAST, BLUE, false);
    // motor_config(Fly2, COAST, BLUE, false);
    lcd_initialize();
    lcd_register_btn0_cb(on_center_button);
    optical_set_led_pwm(COLOR, 100);
    // imu_reset(GYRO);
    // while(imu_get_heading(GYRO) > 360) {}
    controller_set_text(E_CONTROLLER_MASTER, 0, 1, "IMU CALIBRATED`");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
