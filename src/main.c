#include <avr/interrupt.h>

#include "clock.h"
#include "comms_interface.h"
#include "control.h"
#include "escs_interface.h"
#include "imu_interface.h"
#include "settings.h"
#include "debug.h"

int main() {
  measured_state_t measured_state;
  desired_state_t desired_state;
  rotor_speeds_t rotor_speeds;

  init_debug_uart0();
  clock_init();
  imu_init();
  escs_init();

  // Enable interrupts.
  sei();

  while (1) {
    // Communicate with IMU.
    imu_read(&measured_state);

    // Run control algorithm.
    control_cycle(&measured_state, &desired_state, &rotor_speeds);

    // Update ESC duty cycles.
    escs_update(&rotor_speeds);
  }
}
