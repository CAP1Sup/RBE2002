#include "zombieRomi.h"
#include <Romi32U4.h>

// Mode of operation
#define Zombie
// #define Runner

// Sanity checker
#if (defined(Zombie) + defined(Runner) != 1)
#error "Please select exactly one mode of operation"
#endif

#ifdef Zombie
// PIDController wallFollowPID(4.0f, 0.0f, 0.0f);
Chassis chassis;
Romi32U4ButtonA buttonA;
zombieRomi romi = zombieRomi(&chassis);
enum ROBOSTATE { IDLE, SEEKING, CHASING, STOP };
ROBOSTATE state = IDLE;

#elif defined(Runner)
IRSensor IR;
SonarSensor sonar;
#endif

void setup() {
#ifdef Zombie
#endif
  Serial.begin(9600);
}

void loop() {
#ifdef Zombie
  switch (state) {
  case IDLE:
    if (buttonA.getSingleDebouncedRelease()) {
      state = SEEKING;
    }
    break;
  case SEEKING:
    romi.receiveTargetCoordinates(0, 0); // Add time delay here
    romi.moveToLastKnownLocation();
    if (romi.survivorFound()) {
      state = CHASING;
    }
    break;
  case CHASING:
    romi.pursueSurvivor();
    if (romi.survivorInfected()) {
      state = STOP;
    } else if (!romi.survivorFound()) {
      state = SEEKING;
    }
    break;
  case STOP:
    romi.stop();
    break;
  }

#elif defined(Runner)
#endif
}
