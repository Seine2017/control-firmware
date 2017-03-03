#include "clock.h"
#include "control.h"

int main() {
  clock_init();
  control_init();

  while (1) {
    control_cycle();
  }
}
