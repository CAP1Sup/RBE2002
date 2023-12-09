#include <Arduino.h>
#include <Chassis.h>
#include <IRSensor.h>
#include <PIDController.h>
#include <Romi32U4.h>
#include <SonarSensor.h>

// Mode of operation
// #define Zombie
#define Runner

// Sanity checker
#if (defined(Zombie) + defined(Runner) != 1)
#error "Please select exactly one mode of operation"
#endif

// Wall following constants
#define DIST_PID_UPDATE_INTERVAL 50 // ms
#define TARGET_DISTANCE 30.0f       // mm
#define MAX_PID_OUTPUT 50.0f        // mm/s
#define DRIVE_SPEED 75.0f           // mm/s
#define CHASED_TIME_INTERVAL 5000   // ms

uint32_t lastPIDUpdate = 0;
uint32_t chasedTime = 0;
Romi32U4ButtonA buttonA;
Chassis chassis;
#ifdef Zombie
IRSensor IR_Left;
IRSensor IR_Right;
SonarSensor sonar;
PIDController wallFollowPID(4.0f, 0.0f, 0.0f);
#elif defined(Runner)
enum ROBOT_STATE { Idle, Lost, Chased };
ROBOT_STATE robot_state = Idle;
SonarSensor sonar;
IRSensor IR;
PIDController wallFollowPID(4.0f, 0.0f, 0.0f);
PIDController chasedWallFollowPID(3.0f, 1.0f, 0.0f);
#endif

void setup() {
  sensor.init();

  Serial.begin(9600);
}

void loop() {

#ifdef Runner
  switch (robot_state) {
  case Idle:
    if (buttonA.getSingleDebouncedRelease()) {
      robot_state = Lost;
      chassis.resetDrivePID();
      wallFollowPID.reset();
      IR.resetDistAvg();
    }
    break;

  case Lost:
    // Get the average distance
    // Also updates the average distance if needed
    float avgDist = IR.getAvgDistance();

    // Calculate the wall distance PID output if needed
    if (millis() - lastPIDUpdate >= DIST_PID_UPDATE_INTERVAL) {
      lastPIDUpdate = millis();
      float pidOut = wallFollowPID.calculate(TARGET_DISTANCE - avgDist);
      Serial.print("Avg dist: ");
      Serial.println(avgDist);

      // Constrain the output
      pidOut = constrain(pidOut, -MAX_PID_OUTPUT, MAX_PID_OUTPUT);

      // Set the target speeds
      chassis.setTargetSpeeds(DRIVE_SPEED + pidOut, DRIVE_SPEED - pidOut);
    }

    // Update the chassis PID (if needed)
    chassis.updateMotorPID();

    if (tag.id == 0 || tag.id == 1 || tag.id == 2 || tag.id == 3) {
      robot_state = Chased;
      chasedTime = millis();
      chassis.resetDrivePID();
      wallFollowPID.reset();
      IR.resetDistAvg();
    }

    if (buttonA.getSingleDebouncedRelease()) {
      chassis.stop();
      robot_state = IDLE;
    }
    break;

  case Chased:

    float avgDist = IR.getAvgDistance();

    // Calculate the wall distance PID output if needed
    if (millis() - lastPIDUpdate >= DIST_PID_UPDATE_INTERVAL) {
      lastPIDUpdate = millis();
      float pidOut = chasedWallFollowPID.calculate(TARGET_DISTANCE - avgDist);
      Serial.print("Avg dist: ");
      Serial.println(avgDist);

      // Constrain the output
      pidOut = constrain(pidOut, -MAX_PID_OUTPUT, MAX_PID_OUTPUT);

      // Set the target speeds
      chassis.setTargetSpeeds((DRIVE_SPEED + pidOut) * 1.5f,
                              (DRIVE_SPEED - pidOut) * 1.5f);
    }

    // Update the chassis PID (if needed)
    chassis.updateMotorPID();

    if (millis() - updateTime >= CHASED_TIME_INTERVAL) {
      robot_state = Chased;
      chassis.resetDrivePID();
      wallFollowPID.reset();
      IR.resetDistAvg();
    }

    if (tag.w >= 100) {
      robot_state = IDLE;
      chassis.stop();
      wallFollowPID.reset();
      IR.resetDistAvg();
    }
  }
}

#elif defined(Zombie)
  switch (robot_state) {
  case Lost:
    if (buttonA.getSingleDebouncedRelease()) {
      robot_state = DRIVING;
      chassis.resetDrivePID();
      wallFollowPID.reset();
      sensor.resetDistAvg();
    }
    break;
  }
#endif
}