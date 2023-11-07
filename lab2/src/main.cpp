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

#define TARGET_DISTANCE 30.0f
#define MAX_PID_OUTPUT 50.0f
#define DRIVE_SPEED 75.0f

enum ROBOT_STATE { ROBOT_IDLE, ROBOT_DRIVING };
ROBOT_STATE robot_state = ROBOT_IDLE;

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
    case ROBOT_IDLE:
      if (buttonA.getSingleDebouncedRelease()) {
        robot_state = ROBOT_DRIVING;
        chassis.beginDriving();
        wallFollowPID.reset();
        sensor.resetDistAvg();
      }
      break;

    case ROBOT_DRIVING:

      // Calculate the PID output
      float avgDist = sensor.getAvgDistance();
      float pidOut = wallFollowPID.calculate(TARGET_DISTANCE - avgDist);
      Serial.print("Avg dist: ");
      Serial.println(avgDist);

      // Constrain the output
      pidOut = constrain(pidOut, -MAX_PID_OUTPUT, MAX_PID_OUTPUT);

      // Set the target speeds
      chassis.setTargetSpeeds(DRIVE_SPEED + pidOut, DRIVE_SPEED - pidOut);

      // Update the PID (if needed)
      chassis.updateMotorPID();

      // E-stop
      if (buttonA.getSingleDebouncedRelease()) {
        chassis.stop();
        robot_state = ROBOT_IDLE;
      }
      break;
  }
#endif
}