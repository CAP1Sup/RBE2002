#include "main.h"

#ifdef ZOMBIE
#include "Zombie.h"
Zombie zombie = Zombie();
#elif defined(SURVIVOR)
#include "Survivor.h"
Survivor survivor = Survivor();
#endif

void setup() {
  // No need to call init methods, constructors will do that
  // Serial.begin(115200);
  Serial.begin(9600);
#ifdef ZOMBIE
  zombie.init();
#elif defined(SURVIVOR)
  survivor.init();
#endif
}

void loop() {
#ifdef ZOMBIE
  zombie.run();
#elif defined(SURVIVOR)
  survivor.run();
#endif
  delay(20);
}
