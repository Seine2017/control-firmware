#include "clock.h"
#include "comms_interface.h"
#include "control.h"
#include "escs_interface.h"
#include "imu_interface.h"

int main() {
  measured_state_t measured_state;
  desired_state_t desired_state;
  duty_cycles_t duty_cycles;

  clock_init();

  while (1) {
    // Communicate with IMU.
    read_sensors(&measured_state);

    // Run control algorithm.
    control_cycle(&measured_state, &desired_state, &duty_cycles);

    // Update ESC duty cycles.
    update_escs(&duty_cycles);

    // Communicate with comms processor.
    communicate(&desired_state);
  }
}

// This function isn't implemented yet, so temporarily create an emptyfunction
// to allow the code to still compile.
void read_sensors(measured_state_t *destination) {}

// This function isn't implemented yet, so temporarily create an emptyfunction
// to allow the code to still compile.
void communicate(desired_state_t *desired_state) {}

// This function isn't implemented yet, so temporarily create an emptyfunction
// to allow the code to still compile.
void update_escs(duty_cycles_t *duty_cycle) {}
