#include "clock.h"
#include "control.h"

int main() {
  control_state_t state;

  clock_init();
  control_init(&state);

  while (1) {
    control_cycle(&state);
  }
}
