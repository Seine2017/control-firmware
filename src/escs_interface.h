// This file specifies the interface between the ESC driver code and the control
// code (written by Kier).

#ifndef __ESCS_INTERFACE_H
#define __ESCS_INTERFACE_H

// A structure to hold the PWM duty cycles for each of the four rotors. All
// values are floats between 0.0 and 1.0.
typedef struct {
  // Rotor A (front, left).
  float a;

  // Rotor B (front, right).
  float b;

  // Rotor C (rear, right).
  float c;

  // Rotor D (rear, left).
  float d;
} duty_cycles_t;

// Set the new duty cycles for the ESCs.
void update_escs(duty_cycles_t *duty_cycle);

#endif
