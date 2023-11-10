#include <Arduino.h>
#include <Chassis.h>
#include <IRSensor.h>
#include <PIDController.h>
#include <Romi32U4.h>
#include <SonarSensor.h>

// Mode of operation
// #define IR_SENSOR_TESTING
// #define SONAR_SENSOR_TESTING
#define WALL_FOLLOWING

// Sanity checker
#if (defined(IR_SENSOR_TESTING) + defined(SONAR_SENSOR_TESTING) + \
         defined(WALL_FOLLOWING) !=                               \
     1)
#error "Please select exactly one mode of operation"
#endif

// Wall following constants
#define DIST_PID_UPDATE_INTERVAL 50  // ms
#define TARGET_DISTANCE 30.0f        // mm
#define MAX_PID_OUTPUT 50.0f         // mm/s
#define DRIVE_SPEED 75.0f            // mm/s

enum ROBOT_STATE { IDLE, DRIVING };
ROBOT_STATE robot_state = IDLE;
uint32_t lastPIDUpdate = 0;

Romi32U4ButtonA buttonA;
Chassis chassis;
#ifdef IR_SENSOR_TESTING
IRSensor sensor;
#elif defined(SONAR_SENSOR_TESTING)
SonarSensor sensor;
#elif defined(WALL_FOLLOWING)
IRSensor sensor;
PIDController wallFollowPID(4.0f, 0.0f, 0.0f);
#endif

void setup() {
  sensor.init();

  Serial.begin(9600);
}

void loop() {
#ifdef IR_SENSOR_TESTING
  Serial.print("Voltage:");
  Serial.println(sensor.getRawVoltage(), 8);
  delay(150);

#elif defined(SONAR_SENSOR_TESTING)
  Serial.print("Echo duration:");
  Serial.println(sensor.getDuration());
  delay(100);

#elif defined(WALL_FOLLOWING)
  switch (robot_state) {
    case IDLE:
      if (buttonA.getSingleDebouncedRelease()) {
        robot_state = DRIVING;
        chassis.resetDrivePID();
        wallFollowPID.reset();
        sensor.resetDistAvg();
      }
      break;

    case DRIVING:

      // Get the average distance
      // Also updates the average distance if needed
      float avgDist = sensor.getAvgDistance();

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

      // E-stop
      if (buttonA.getSingleDebouncedRelease()) {
        chassis.stop();
        robot_state = IDLE;
      }
      break;
  }
#endif
}