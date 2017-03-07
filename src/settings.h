#ifndef __SETTINGS_H
#define __SETTINGS_H

// The frequency at which system clock ticks occur.
// Hence, each tick represents 100 microseconds.
#define TICKS_PER_SECOND 10000
#define SECONDS_PER_TICK (1.0 / TICKS_PER_SECOND)

// The frequency at which we communicate with the comms processor.
#define COMMUNICATIONS_PER_SECOND 10
#define TICKS_PER_COMMUNICATION (TICKS_PER_SECOND / COMMUNICATIONS_PER_SECOND)

// Y-factor PID parameters.
#define PID_GAIN_Z_P 0
#define PID_GAIN_Z_I 0
#define PID_GAIN_Z_D 0

// Roll-factor PID parameters.
#define PID_GAIN_ROLL_P 0
#define PID_GAIN_ROLL_I 0
#define PID_GAIN_ROLL_D 0

// Pitch-factor PID parameters.
#define PID_GAIN_PITCH_P 0
#define PID_GAIN_PITCH_I 0
#define PID_GAIN_PITCH_D 0

// Yaw-factor PID parameters.
#define PID_GAIN_YAW_P 0
#define PID_GAIN_YAW_I 0
#define PID_GAIN_YAW_D 0

// IMU sensor sensitivities.
#define GYRO_SENSITIVITY 131    // = 131 LSB/degrees/sec
#define ACCEL_SENSITIVITY 16384 // = 16384 LSB/g

// Allowable limits for rotor speeds (0.0 = 1ms pulse, 1.0 = 2ms pulse).
#define MIN_ROTOR_SPEED 0.0
#define MAX_ROTOR_SPEED 0.7

// Specification of which pins on the microcontroller are connected to the PWM
// inputs on the ESCS.
#define ESCS_DDR DDRB
#define ESCS_PORT PORTB
#define ESCS_BIT_A PB0
#define ESCS_BIT_B PB1
#define ESCS_BIT_C PB2
#define ESCS_BIT_D PB3

#endif
