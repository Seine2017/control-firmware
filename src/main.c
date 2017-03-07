#include <avr/interrupt.h>
#include <util/delay.h>

#include "clock.h"
#include "comms_interface.h"
#include "control.h"
#include "escs_interface.h"
#include "imu_interface.h"
#include "settings.h"

int main() {
  rotor_speeds_t rotor_speeds;
  int t = 0;

  escs_init();

  // Enable interrupts
  sei();

  while (1) {
    rotor_speeds.a = ((float) t) * 0.1;
    rotor_speeds.b = ((float) t) * 0.1 + 0.05;
    rotor_speeds.c = ((float) t) * 0.1;
    rotor_speeds.d = ((float) t) * 0.1 + 0.05;
    escs_update(&rotor_speeds);
    _delay_ms(1000);
    t++;
    if (t == 6) {
      t = 0;
    }
  }
}
