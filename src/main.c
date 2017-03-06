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
  imu_init();
  escs_init();

  while (1) {
    // Communicate with IMU.
    imu_read(&measured_state);

    // Run control algorithm.
    control_cycle(&measured_state, &desired_state, &duty_cycles);

    // Update ESC duty cycles.
    escs_update(&duty_cycles);
  }
}
