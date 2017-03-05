// This file declares the functions and variables defined in control.c.

#ifndef __CONTROL_H
#define __CONTROL_H

#include "comms_interface.h"
#include "escs_interface.h"
#include "imu_interface.h"

// Perform a full cycle of the control algorithm.
void control_cycle(measured_state_t *measured_state,
                   desired_state_t *desired_state,
                   duty_cycles_t *duty_cycles);

#endif
