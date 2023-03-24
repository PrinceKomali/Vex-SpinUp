#include "main.h"
#include "ports.h"
#include "chassis.h"

Drive chassis (
  {-R1, R2, -R3}
 ,{L1, -L2, L3}
  // IMU Port
  ,GYRO
  // Wheel Diameter (Remember, 4" wheels are actually 4.125!)
  //    (or tracking wheel diameter)
  ,4.125
  ,600
  // External Gear Ratio (MUST BE DECIMAL)
  //    (or gear ratio of tracking wheel)
  // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
  // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
  ,2.0
);
void chassis_init() { chassis.initialize(); }