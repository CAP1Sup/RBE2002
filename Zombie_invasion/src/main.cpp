#include "main.h"

// Mode of operation
#define Zombie
// #define Runner

// Sanity checker
#if (defined(Zombie) + defined(Runner) != 1)
#error "Please select exactly one mode of operation"
#endif

#ifdef Zombie
zombieRomi romi = zombieRomi();
PIDController wallFollowPID(4.0f, 0.0f, 0.0f);
Romi32U4ButtonA buttonA;

enum ROBOSTATE { IDLE, SEEKING, CHASING, TURNING, STOPPED };
ROBOSTATE state = IDLE;

#elif defined(Runner)
IRSensor IR;
SonarSensor sonar;
#endif

void setup() { Serial.begin(9600); }

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
    if (romi.getOnLastKnownPosition()) {
      state = CHASING;
    }
    break;
  }
#elif defined(Runner)
#endif
}
