#include <PciManager.h>
#include <SoftTimer.h>
#include <Task.h>
#include <Debouncer.h>
#include <FrequencyTask.h>

#define BEEPER_PIN  10

FrequencyTask f1(BEEPER_PIN, 261.63);
FrequencyTask f2(BEEPER_PIN, 329.63);
FrequencyTask f3(BEEPER_PIN, 392.00);

// -- This task will switch on/off the frequency tasks.
Task stateStepTask(3000, stateStep);

int state = 0;

void setup() {
  SoftTimer::instance().add(&stateStepTask);
}

void stateStep(Task* task) {
  state += 1;
  if(state == 1) {
    // -- turn ON f1
    SoftTimer::instance().add(&f1);
  }
  else if(state == 2) {
    // -- turn ON f2
    SoftTimer::instance().add(&f2);
  }
  else if(state == 3) {
    // -- turn ON f3
    SoftTimer::instance().add(&f3);
  }
  else if(state == 4) {
    // -- turn OFF f3
    SoftTimer::instance().remove(&f3);
  }
  else if(state == 5) {
    // -- turn OFF f2
    SoftTimer::instance().remove(&f2);
  } else {
    // -- turn OFF f1 (silence)
    SoftTimer::instance().remove(&f1);
    state = 0;
  }
}
