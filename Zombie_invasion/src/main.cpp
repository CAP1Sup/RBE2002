#include "main.h"

// Mode of operation
// #define Zombie
#define Runner

// Sanity checker
#if (defined(Zombie) + defined(Runner) != 1)
#error "Please select exactly one mode of operation"
#endif

#ifdef Zombie
IRSensor IR_Left;
IRSensor IR_Right;
SonarSensor sonar;
PIDController wallFollowPID(4.0f, 0.0f, 0.0f);
#elif defined(Runner)

#endif

void setup() {
  sonar.init();
  IR.init();

  Serial.begin(9600);
}

void loop() {}

#elif defined(Zombie)
switch (robot_state) {
case LOST:
  if (buttonA.getSingleDebouncedRelease()) {
    robot_state = DRIVING;
    chassis.resetDrivePID();
    wallFollowPID.reset();
    sensor.resetDistAvg();
  }
  break;
}
#endif