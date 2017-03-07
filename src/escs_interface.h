// This file specifies the interface between the ESC driver code and the control
// code (written by Kier).

#ifndef __ESCS_INTERFACE_H
#define __ESCS_INTERFACE_H

// A structure to hold the speeds for each of the four rotors. All
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
} rotor_speeds_t;

// Initialise the PWM outputs.
void escs_init();

// Set the new duty cycles for the ESCs.
void escs_update(rotor_speeds_t *rotor_speeds);

#endif
