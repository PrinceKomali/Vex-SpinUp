#define motor_config(port, brake_mode, gearing, reversed) \
motor_set_brake_mode(port, E_MOTOR_BRAKE_##brake_mode); \
motor_set_gearing(port, E_MOTOR_GEAR_##gearing); \
motor_set_reversed(port, reversed); \
motor_set_encoder_units(port, E_MOTOR_ENCODER_DEGREES); \
motor_tare_position(port);
