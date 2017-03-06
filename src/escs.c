#include <avr/io.h>
#include <stdint.h>

#include "escs_interface.h"

// Clamp the duty cycle to the range 0..1, then scale it to the range 0..max and
// round it to the nearest integer
static uint16_t scale_duty_cycle(float duty_cycle, float max) {
  if (duty_cycle < 0.0) {
    duty_cycle = 0.0;
  }
  if (duty_cycle > 1.0) {
    duty_cycle = 1.0;
  }
  return (uint16_t) (duty_cycle * max);
}

#if __AVR_ATmega32U4__
  // We have two 16-bit timers available (timers 1 and 3).

  void escs_init() {
    // WGM1 = 0011 (phase-correct PWM mode, TOP = 0x03FF)
    // CS1 = 011 (prescaler = 64; gives about 122 Hz pulse rep rate)
    // COM1A = 00 (OC1A disconnected)
    // COM1B = 10 (OC1B cleared when up-counting, set when down-counting)
    // COM1C = 10 (OC1C cleared when up-counting, set when down-counting)
    TCCR1A = _BV(COM1B1) | _BV(COM1C1) | _BV(WGM11) | _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(CS10);
    OCR1BH = 0;
    OCR1BL = 0;
    OCR1CH = 0;
    OCR1CL = 0;
    TIMSK1 = 0; // all interrupts disabled

    // WGM3 = 0011 (phase-correct PWM mode, TOP = 0x03FF)
    // CS3 = 011 (prescaler = 64; gives about 122 Hz pulse rep rate)
    // COM3A = 00 (OC1A disconnected)
    // COM3B = 10 (OC1B cleared when up-counting, set when down-counting)
    // COM3C = 10 (OC1C cleared when up-counting, set when down-counting)
    TCCR3A = _BV(COM3B1) | _BV(COM3C1) | _BV(WGM31) | _BV(WGM30);
    TCCR3B = _BV(CS31) | _BV(CS30);
    OCR3BH = 0;
    OCR3BL = 0;
    OCR3CH = 0;
    OCR3CL = 0;
    TIMSK3 = 0; // all interrupts disabled
  }

  void escs_update(duty_cycles_t *duty_cycles) {
    const uint16_t a = scale_duty_cycle(duty_cycles->a, 0x03FF);
    const uint16_t b = scale_duty_cycle(duty_cycles->b, 0x03FF);
    const uint16_t c = scale_duty_cycle(duty_cycles->c, 0x03FF);
    const uint16_t d = scale_duty_cycle(duty_cycles->d, 0x03FF);
    OCR1BH = (uint8_t) (a >> 8);
    OCR1CH = (uint8_t) (b >> 8);
    OCR3BH = (uint8_t) (c >> 8);
    OCR3CH = (uint8_t) (d >> 8);
    OCR1BL = (uint8_t) a;
    OCR1CL = (uint8_t) b;
    OCR3BL = (uint8_t) c;
    OCR3CL = (uint8_t) d;
  }

#elif defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
  // We have a 16-bit timer (timer 1) and an 8-bit timer (timer 2).
  // Print a warning about this non-ideal situation.
  #warning "The platform you are compiling for does not have enough 16-bit timers; falling back to using an 8-bit timer for PWM outputs C and D."

  void escs_init() {
    // WGM1 = 0011 (phase-correct PWM mode, TOP = 0x03FF)
    // CS1 = 011 (prescaler = 64; gives about 122 Hz pulse rep rate)
    // COM1A = 10 (OC1A cleared when up-counting, set when down-counting)
    // COM1B = 10 (OC1B cleared when up-counting, set when down-counting)
    TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(CS10);
    OCR1AH = 0;
    OCR1AL = 0;
    OCR1BH = 0;
    OCR1BL = 0;
    TIMSK1 = 0; // all interrupts disabled

    // WGM2 = 001 (phase-correct PWM mode, TOP = 0xFF)
    // CS2 = 110 (prescaler = 256; gives about 123 Hz pulse rep rate)
    // COM2A = 10 (OC1A cleared when up-counting, set when down-counting)
    // COM2B = 10 (OC1B cleared when up-counting, set when down-counting)
    TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
    TCCR2B = _BV(CS22) | _BV(CS21);
    OCR2A = 0;
    OCR2B = 0;
    TIMSK2 = 0; // all interrupts disabled
  }

  void escs_update(duty_cycles_t *duty_cycles) {
    const uint16_t a = scale_duty_cycle(duty_cycles->a, 0x03FF);
    const uint16_t b = scale_duty_cycle(duty_cycles->b, 0x03FF);
    const uint16_t c = scale_duty_cycle(duty_cycles->c, 0x00FF);
    const uint16_t d = scale_duty_cycle(duty_cycles->d, 0x00FF);
    OCR1AH = (uint8_t) (a >> 8);
    OCR1BH = (uint8_t) (b >> 8);
    OCR1AL = (uint8_t) a;
    OCR1BL = (uint8_t) b;
    OCR2A = (uint8_t) c;
    OCR2B = (uint8_t) d;
  }

#else
  // Unknown timer layout.
  #error "Unknown hardware timer layout - please consult the datasheet and update escs.c."

#endif
