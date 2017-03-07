#include <avr/interrupt.h>
#include <avr/io.h>

#include "escs_interface.h"
#include "settings.h"

#define ESCS_TICK_PERIOD_US 10
#define ESCS_REP_PERIOD_US 20000
#define ESCS_TICKS_PER_REP (ESCS_REP_PERIOD_US / ESCS_TICK_PERIOD_US)
#define ESCS_TICKS_PER_SECOND (1000000 / ESCS_TICK_PERIOD_US)
#define ESCS_TICKS_PER_MS (1000 / ESCS_TICK_PERIOD_US)

#define ESCS_MASK_A _BV(ESCS_BIT_A)
#define ESCS_MASK_B _BV(ESCS_BIT_B)
#define ESCS_MASK_C _BV(ESCS_BIT_C)
#define ESCS_MASK_D _BV(ESCS_BIT_D)
#define ESCS_MASK_ALL (ESCS_MASK_A | ESCS_MASK_B | ESCS_MASK_C | ESCS_MASK_D)

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
  #define ESCS_TIMER_INTERRUPT TIMER2_COMPA_vect
  static void escs_init_timer() {
    // Set up timer 0:
    //   COM0A = 00 (OC0A disconnected)
    //   COM0B = 00 (OC0B disconnected)
    //   WGM0 = 010 (clear timer on compare match)
    //   CS0 = 001 (no prescaler)
    //   OCIE0A = 1 (interrupt on output compare match A)
    //   OCIE0B = 0 (no interrupt on output compare match B)
    //   TOIE0 = 0 (no interrupt on timer overflow)
    TCCR0A = _BV(WGM01);
    TCCR0B = _BV(CS00);
    TIMSK0 = _BV(OCIE0A);

    //    F_OVERFLOW = F_CPU / (PRESCALER * (1 + OCR0A))
    // => OCR0A = F_CPU / (PRESCALER * F_OVERFLOW) - 1
    OCR0A = (uint8_t) (((uint32_t) F_CPU) / (((uint32_t) 1) * ((uint32_t) ESCS_TICKS_PER_SECOND)) - 1);
  }
#elif defined(__AVR_ATmega32U4__)
  #define ESCS_TIMER_INTERRUPT TIMER3_COMPA_vect
  static void escs_init_timer() {
    // Set up timer 0:
    //   COM0A = 00 (OC0A disconnected)
    //   COM0B = 00 (OC0B disconnected)
    //   WGM0 = 010 (clear timer on compare match)
    //   CS0 = 001 (no prescaler)
    //   OCIE0A = 1 (interrupt on output compare match A)
    //   OCIE0B = 0 (no interrupt on output compare match B)
    //   TOIE0 = 0 (no interrupt on timer overflow)
    TCCR0A = _BV(WGM01);
    TCCR0B = _BV(CS00);
    TIMSK0 = _BV(OCIE0A);

    //    F_OVERFLOW = F_CPU / (PRESCALER * (1 + OCR0A))
    // => OCR0A = F_CPU / (PRESCALER * F_OVERFLOW) - 1
    OCR0A = (uint16_t) (((uint32_t) F_CPU) / (((uint32_t) 1) * ((uint32_t) ESCS_TICKS_PER_SECOND)) - 1);
  }
#else
  #error "Don't know how to set up timer on the target device. Please update escs.c."
#endif

volatile uint16_t escs_ticks = 0;
volatile struct {
  uint16_t a, b, c, d;
} escs_levels;

ISR(ESCS_TIMER_INTERRUPT) {
  uint16_t ticks = escs_ticks;
  uint8_t port_reg = ESCS_PORT;
  if (ticks == 0) {
    // Set all outputs to high.
    port_reg |= ESCS_MASK_ALL;
  }
  if (ticks >= escs_levels.a) {
    // Set output A to low.
    port_reg &= ~ESCS_MASK_A;
  }
  if (ticks >= escs_levels.b) {
    // Set output B to low.
    port_reg &= ~ESCS_MASK_B;
  }
  if (ticks >= escs_levels.c) {
    // Set output C to low.
    port_reg &= ~ESCS_MASK_C;
  }
  if (ticks >= escs_levels.d) {
    // Set output D to low.
    port_reg &= ~ESCS_MASK_D;
  }
  ticks++;
  if (ticks == ESCS_TICKS_PER_REP) {
    ticks = 0;
  }
  escs_ticks = ticks;
  ESCS_PORT = port_reg;
}

void escs_init() {
  // Reset variables.
  escs_ticks = 0;
  escs_levels.a = 0;
  escs_levels.b = 0;
  escs_levels.c = 0;
  escs_levels.d = 0;

  // Set pins to be outputs.
  ESCS_DDR |= ESCS_MASK_ALL;
  // Set outputs to low.
  ESCS_PORT &= ~ESCS_MASK_ALL;

  escs_init_timer();
}

static float clamp(float x, float min, float max) {
  if (x < min) {
    return min;
  }
  if (x > max) {
    return max;
  }
  return x;
}

void escs_update(rotor_speeds_t *rotor_speeds) {
  const uint16_t level_a = ESCS_TICKS_PER_MS + (uint16_t) (((float) ESCS_TICKS_PER_MS) * clamp(rotor_speeds->a, MIN_ROTOR_SPEED, MAX_ROTOR_SPEED));
  const uint16_t level_b = ESCS_TICKS_PER_MS + (uint16_t) (((float) ESCS_TICKS_PER_MS) * clamp(rotor_speeds->b, MIN_ROTOR_SPEED, MAX_ROTOR_SPEED));
  const uint16_t level_c = ESCS_TICKS_PER_MS + (uint16_t) (((float) ESCS_TICKS_PER_MS) * clamp(rotor_speeds->c, MIN_ROTOR_SPEED, MAX_ROTOR_SPEED));
  const uint16_t level_d = ESCS_TICKS_PER_MS + (uint16_t) (((float) ESCS_TICKS_PER_MS) * clamp(rotor_speeds->d, MIN_ROTOR_SPEED, MAX_ROTOR_SPEED));
  escs_levels.a = level_a;
  escs_levels.b = level_b;
  escs_levels.c = level_c;
  escs_levels.d = level_d;
}
