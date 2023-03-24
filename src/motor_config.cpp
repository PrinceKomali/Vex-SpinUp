#include "motor_config.h"
#include "code_from_arizona.h"
#include "ports.h"
#include "main.h"
void drive_set_coast() {
    motor_config(L1, COAST, GREEN, true);
    motor_config(L2, COAST, GREEN, true);
    motor_config(L3, COAST, GREEN, true);
    motor_config(R1, COAST, GREEN, true);
    motor_config(R2, COAST, GREEN, true);
    motor_config(R3, COAST, GREEN, true);
}

void drive_set_hold() {
    motor_config(L1, HOLD, GREEN, true);
    motor_config(L2, HOLD, GREEN, true);
    motor_config(L3, HOLD, GREEN, true);
    motor_config(R1, HOLD, GREEN, true);
    motor_config(R2, HOLD, GREEN, true);
    motor_config(R3, HOLD, GREEN, true);
}
