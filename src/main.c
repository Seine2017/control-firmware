#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdio.h>
#include <util/delay.h>

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
  // TODO: fix this conversion!
  desired_state->z_vel = ((float) rc_data_packet.channel_0) / 128.0 - 1.0;
  desired_state->roll = ((float) rc_data_packet.channel_1) / 128.0 - 1.0;
  desired_state->pitch = ((float) rc_data_packet.channel_2) / 128.0 - 1.0;
  desired_state->yaw_vel = ((float) rc_data_packet.channel_3) / 128.0 - 1.0;
}

int main() {
  measured_state_t measured_state;
  desired_state_t desired_state;
  rotor_speeds_t rotor_speeds;

  rotor_speeds.a = 0.0;
  rotor_speeds.b = 0.0;
  rotor_speeds.c = 0.0;
  rotor_speeds.d = 0.0;

  desired_state.z_vel = 0.0;
  desired_state.roll = 0.0;
  desired_state.pitch = 0.0;
  desired_state.yaw_vel = 0.0;

  printf("Initialising...\n");

  // Disable interrupts during setup.
  cli();

  // Disable watchdog timer.
  wdt_disable();

  // Initialise modules.
  init_debug_uart0();
  clock_init();
  escs_init();
  escs_update(&rotor_speeds);
  imu_init();
  comms_slave_init();

  printf("Ready!\n");

  // Enable interrupts.
  sei();

  // Keep ESC signal at 0 for a while.
  _delay_ms(1000);

  printf("Starting loop.\n");

  while (1) {
    // Communicate with IMU.
    imu_read(&measured_state);
    convert_logg_packet(&measured_state);

    //printf("%f,%f,%f\n", measured_state.roll, measured_state.pitch, measured_state.yaw_vel);

    // Run control algorithm.
    convert_rc_packet(&desired_state);
    control_cycle(&measured_state, &desired_state, &rotor_speeds);

    // Update ESC duty cycles.
    escs_update(&rotor_speeds);
  }
}
