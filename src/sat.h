// This file implements saturating arithmetic operations. Saturating arithmetic
// means that in the event of overflow/underflow, the closest number to the true
// result is returned (generally the largest or smallest possible representable
// value). In contrast, computers usually use modular arithmetic instead, in
// which an overflow causes the value to "wrap around" to the other end of the
// range of representable values.
// Examples (signed 8 bit values):
//   Operation    True result    Result in saturating    Result in modular
//                                    arithmetic            arithmetic
//   100 + 20         120                120                    120
//   100 + 30         130                127                   -126

#include <stdint.h>

// Saturating addition on 16-bit signed numbers.
static  sat_s16_add(int16_t x, int16_t y) {
  int16_t res = x + y;
  if (x & y & ~res & 0x8000) {
    return 0x7FFF;
  }
  else if (~x & ~y & res & 0x8000) {
    return 0x8000;
  }
  return res;
}

// Saturating subtraction on 16-bit signed numbers.
static int16_t sat_s16_sub(int16_t x, int16_t y) {
  int16_t res = x - y;
  if (x & ~y & ~res & 0x8000) {
    return 0x7FFF;
  }
  else if (~x & y & res & 0x8000) {
    return 0x8000;
  }
  return res;
}
