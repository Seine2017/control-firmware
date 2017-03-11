#ifndef __SETTINGS_H
#define __SETTINGS_H

#include <avr/io.h>

// The frequency at which system clock ticks occur.
// Hence, each tick represents 100 microseconds.
#define CLOCK_TICKS_PER_SECOND 10000
#define SECONDS_PER_CLOCK_TICK (1.0 / CLOCK_TICKS_PER_SECOND)

// The frequency/period at which software PWM ticks occur.
#define US_PER_PWM_TICK 10
#define US_PER_PWM_REP 20000
#define PWM_TICKS_PER_REP (US_PER_PWM_REP / US_PER_PWM_TICK)
#define PWM_TICKS_PER_SECOND (1000000 / US_PER_PWM_TICK)
#define PWM_TICKS_PER_MS (1000 / US_PER_PWM_TICK)

// Z-factor PID parameters.
// Iteration 1: 0.002548/0.0004406/0.003685 (PID disabled)
// Iteration 2: 1.07/21.38/0.0005811 (PID disabled)
// Iteration 3: -0.05018/-0.0364/0
// Iteration 4: 1.013/192.4/6.746e-5 (bad)
// Start again
// Iteration 1 (z3-2.txt): 0.1506/0.03667/0.1546
#define PID_GAIN_Z_P 0.1506
#define PID_GAIN_Z_I 0.03667
#define PID_GAIN_Z_D 0.1546
#define MIN_INTEGRAL_Z -1.0
#define MAX_INTEGRAL_Z 1.0

// Roll-factor PID parameters.
#define PID_GAIN_ROLL_P 0.0
#define PID_GAIN_ROLL_I 0.0
#define PID_GAIN_ROLL_D 0.0
#define MIN_INTEGRAL_ROLL -1.0
#define MAX_INTEGRAL_ROLL 1.0

// Pitch-factor PID parameters.
// Iteration 1: 0.01338/0.005628/0.007947
// Iteration 2: -0.4264/-2.777/-0.0006686
// Fixed axis
// Iteration 3: 4.82/545.7/0.0003971
#define PID_GAIN_PITCH_P 0.0
#define PID_GAIN_PITCH_I 0.0
#define PID_GAIN_PITCH_D 0.0
#define MIN_INTEGRAL_PITCH (-0.3/PID_GAIN_PITCH_I)
#define MAX_INTEGRAL_PITCH (-0.3/PID_GAIN_PITCH_I)

// Yaw-factor PID parameters.
#define PID_GAIN_YAW_P 0.0
#define PID_GAIN_YAW_I 0.0
#define PID_GAIN_YAW_D 0.0
#define MIN_INTEGRAL_YAW -1.0
#define MAX_INTEGRAL_YAW 1.0

// Accelerometer/gyroscope "who am I" ID.
#define IMU_ID 0x71 // Grove IMU
//#define IMU_ID 0x73 // Pawel's IMU

// IMU sensor sensitivities.
#define GYRO_SENSITIVITY 65.5    // = 65.5 LSB/degrees/sec
#define ACCEL_SENSITIVITY 4096 // = 4096 LSB/g (8g range)

// Allowable limits for rotor speeds (0.0 = 1ms pulse, 1.0 = 2ms pulse).
#define MIN_ROTOR_SPEED 0.0
#define MAX_ROTOR_SPEED 0.6

// Rotor speeds are filtered to reduce transients. The higher this parameter is,
// the more filtering is applied (but the slower the rotors are to respond to
// changes).
// Range: 0.0 to 1.0
#define ROTOR_SPEED_FILTERING 0.98

// Specification of which pins on the microcontroller are connected to the PWM
// inputs on the ESCS.
#if defined(BOARD_uno)
  #define ESCS_DDR DDRC
  #define ESCS_PORT PORTC
  #define ESCS_BIT_A PC0 // Arduino pin A0
  #define ESCS_BIT_B PC1 // Arduino pin A1
  #define ESCS_BIT_C PC2 // Arduino pin A2
  #define ESCS_BIT_D PC3 // Arduino pin A3
#elif defined(BOARD_promicro)
  #define ESCS_DDR DDRF
  #define ESCS_PORT PORTF
  #define ESCS_BIT_A PF7 // Arduino pin A0
  #define ESCS_BIT_B PF6 // Arduino pin A1
  #define ESCS_BIT_C PF5 // Arduino pin A2
  #define ESCS_BIT_D PF4 // Arduino pin A3
#elif defined(BOARD_nano)
  #define ESCS_DDR DDRD
  #define ESCS_PORT PORTD
  #define ESCS_BIT_A PD4 // Arduino pin 4
  #define ESCS_BIT_B PD5 // Arduino pin 5
  #define ESCS_BIT_C PD6 // Arduino pin 6
  #define ESCS_BIT_D PD7 // Arduino pin 7
#else
  #error "Don't know how to set up pin definitions on the target device. Please update settings.h."
#endif


#endif
