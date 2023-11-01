#include <Arduino.h>
#include <Romi32U4.h>

#include "Chassis.h"
#include "IRSensor.h"
#include "PIDController.h"
#include "SonarSensor.h"

// Mode of operation
#define IR_SENSOR_TESTING
// #define SONAR_SENSOR_TESTING
//  #define WALL_FOLLOWING

// Sanity checker
#if (defined(IR_SENSOR_TESTING) + defined(SONAR_SENSOR_TESTING) +              \
         defined(WALL_FOLLOWING) !=                                            \
     1)
#error "Please select exactly one mode of operation"
#endif

#define TARGET_DISTANCE 10.0f // TODO: TUNE LATER
#define MAX_PID_OUTPUT 25.0f  // TODO: TUNE LATER

enum ROBOT_STATE { ROBOT_IDLE, ROBOT_DRIVING };
ROBOT_STATE robot_state = ROBOT_IDLE;

Romi32U4ButtonA buttonA;
Chassis chassis;
#ifdef IR_SENSOR_TESTING
IRSensor sensor;
#elif defined(SONAR_SENSOR_TESTING)
SonarSensor sensor;
#endif

PIDController wallFollowPID(7.0f, 2.0f, 0);

void setup() {
  sensor.init();

#if defined(IR_SENSOR_TESTING) || defined(SONAR_SENSOR_TESTING)
  Serial.begin(9600);
#endif
}

void loop() {
#ifdef IR_SENSOR_TESTING
  Serial.print("Voltage:");
  Serial.println(sensor.getRawVoltage(), 8);
  delay(100);

#elif defined(SONAR_SENSOR_TESTING)
  Serial.print("Echo duration:");
  Serial.println(sensor.getDuration());
  delay(100);

#elif defined(WALL_FOLLOWING)
  switch (robot_state) {
  case ROBOT_IDLE:
    if (buttonA.getSingleDebouncedRelease())
      robot_state = ROBOT_DRIVING;
    chassis.beginDriving();
    wallFollowPID.reset();
    break;

  case ROBOT_DRIVING:
    // uncomment this line to check whether the speed controller is
    // operational on your robot PIcontroller.Process(50,50);

    // uncomment this line of code, once you are done with assignments 1 and 2
    // to demonstrate that your robot is capable of following a wall
    // autonomously. int speed = PDcontroller.Start(30); //distance in [cm]
    // PIcontroller.Start(50+speed,50-speed); //speed in [[mm/s]]

    // Calculate the PID output
    float pidOut =
        wallFollowPID.calculate(TARGET_DISTANCE - sensor.getDistance());

    // Constrain the output
    pidOut = constrain(pidOut, -MAX_PID_OUTPUT, MAX_PID_OUTPUT);

    // Set the target speeds
    chassis.setTargetSpeeds(50 + pidOut, 50 - pidOut);

    // E-stop
    if (buttonA.getSingleDebouncedRelease()) {
      chassis.stop();
      robot_state = ROBOT_IDLE;
    }
    break;
  }
#endif
}