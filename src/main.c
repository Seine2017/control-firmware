#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "clock.h"
#include "comms_interface.h"
#include "control.h"
#include "escs_interface.h"
#include "imu_interface.h"
#include "settings.h"
#include "debug.h"

static void convert_logg_packet(measured_state_t* measured_state){
  logg_data_packet.roll.value = measured_state->roll;
  logg_data_packet.pitch.value = measured_state->pitch;
  logg_data_packet.yaw_vel.value = measured_state->yaw_vel;
}

static void convert_rc_packet(desired_state_t *desired_state) {
  desired_state->z_vel = rc_data_packet.channel_0;
  desired_state->roll = rc_data_packet.channel_1;
  desired_state->pitch = rc_data_packet.channel_2;
  desired_state->yaw_vel = rc_data_packet.channel_3;
}

int main() {
  measured_state_t measured_state;
  desired_state_t desired_state;
  rotor_speeds_t rotor_speeds;

  // Disable interrupts during setup.
  cli();

  // Disable watchdog timer.
  wdt_disable();

  // Initialise modules.
  init_debug_uart0();
  clock_init();
  imu_init();
  escs_init();
  comms_slave_init();

  // Enable interrupts.
  sei();

  while (1) {
    // Communicate with IMU.
    imu_read(&measured_state);
    convert_logg_packet(&measured_state);

    // Run control algorithm.
    convert_rc_packet(&desired_state);
    control_cycle(&measured_state, &desired_state, &rotor_speeds);

    // Update ESC duty cycles.
    escs_update(&rotor_speeds);
  }
}
