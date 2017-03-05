#include "clock.h"
#include "comms_interface.h"
#include "control.h"
#include "escs_interface.h"
#include "imu_interface.h"
#include "settings.h"

int main() {
  measured_state_t measured_state;
  desired_state_t desired_state;
  duty_cycles_t duty_cycles;

  clock_init();

  // Keep track of the time at which we last talked to the comms processor.
  clock_time_t last_communicated = clock_get_time();

  while (1) {
    // Communicate with IMU.
    read_sensors(&measured_state);

    // Run control algorithm.
    control_cycle(&measured_state, &desired_state, &duty_cycles);

    // Update ESC duty cycles.
    escs_update(&duty_cycles);

    clock_time_t now = clock_get_time();
    if (clock_diff(last_communicated, now) > TICKS_PER_COMMUNICATION) {
      // Communicate with comms processor.
      communicate(&desired_state);
      last_communicated = now;
    }
  }
}

// This function isn't implemented yet, so temporarily create an emptyfunction
// to allow the code to still compile.
void read_sensors(measured_state_t *destination) {}

// This function isn't implemented yet, so temporarily create an emptyfunction
// to allow the code to still compile.
void communicate(desired_state_t *desired_state) {}
