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

#endif
