#ifndef _AZ
#define AZ 
#define motor_config(port, brake_mode, gearing, reversed)                      \
    pros::c::motor_set_brake_mode(port, pros::E_MOTOR_BRAKE_##brake_mode);                    \
    pros::c::motor_set_gearing(port, pros::E_MOTOR_GEAR_##gearing);                           \
    pros::c::motor_set_reversed(port, reversed);                                        \
    pros::c::motor_set_encoder_units(port, pros::E_MOTOR_ENCODER_DEGREES);                    \
    pros::c::motor_tare_position(port);  
#endif