#include "main.h"

// Mode of operation
#define Zombie
// #define Runner

// Sanity checker
#if (defined(Zombie) + defined(Runner) != 1)
#error "Please select exactly one mode of operation"
#endif

#ifdef Zombie
PIDController wallFollowPID(4.0f, 0.0f, 0.0f);
Romi32U4ButtonA buttonA;
zombieRomi romi = zombieRomi();
/* IRSensor leftIR;
IRSensor rightIR;
SonarSensor sonar;
LineSensor lineSensor;
Chassis chassis;
zombieRomi romi = zombieRomi(&chassis, &lineSensor, &leftIR, &rightIR, &sonar);
*/
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
    romi.followLine();
    romi.detectSurvivorPosition();
    if (romi.survivorFound()) {
      state = CHASING;
    }
    break;
  case CHASING:
    romi.pursueSurvivor();
    if (romi.survivorInfected()) {
      state = STOP;
    }
    break;
  case STOP:
    romi.stop();
    break;
  }

#elif defined(Runner)
#endif
}
