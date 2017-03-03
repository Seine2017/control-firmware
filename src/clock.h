// clock.h declares the functions and variables defined in clock.c.

// clock.c contains routines to manage a monotonic time source; in other words,
// a way of measuring how much time has passed since the processor was switched
// on. This is used by the control algorithm to perform calculus operations
// (integration and differentiation).

#ifndef __CLOCK_H
#define __CLOCK_H

#include <stdint.h>
#include <avr/io.h>

// The frequency at which ticks occur.
// Hence, each tick represents 100 microseconds.
#define clock_time_tICK_FREQUENCY 10000

// A type representing the number of clock 'ticks' since the processor was last
// reset or powered on.
typedef uint16_t clock_time_t;

// A type representing a time interval (the number of 'ticks' between two
// distinct time measurements).
typedef int16_t clock_interval_t;

// The actual number of ticks that have passed since power on.
extern volatile clock_time_t clock_now;

// Initialise the clock.
static void clock_init() {
  // Set up timer 0:
  //   COM0A = 00 (OC0A disconnected)
  //   COM0B = 00 (OC0B disconnected)
  //   WGM0 = 010 (clear timer on compare match)
  //   CS0 = 010 (prescaler = 8)
  //   OCIE0A = 0 (no interrupt on output compare match A)
  //   OCIE0B = 0 (no interrupt on output compare match B)
  //   TOIE0 = 1 (interrupt on timer overflow)
  TCCR0A = _BV(WGM01);
  TCCR0B = _BV(CS01);
  TIMSK0 = _BV(TOIE0);

  //    F_OVERFLOW = F_CPU / (PRESCALER * (1 + OCR0A))
  // => OCR0A = F_CPU / (PRESCALER * F_OVERFLOW) - 1
  OCR0A = (uint8_t) (((uint32_t) F_CPU) / (((uint32_t) 8) * ((uint32_t) clock_time_tICK_FREQUENCY)) - 1);
}

// Check the current time.
static clock_time_t clock_get_time() {
  return clock_now;
}

// Find the interval of time that passed between two distinct time measurements.
static clock_interval_t clock_diff(clock_time_t start, clock_time_t end) {
  return (clock_interval_t) (end - start);
}

#endif
