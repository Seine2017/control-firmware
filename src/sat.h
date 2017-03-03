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
static int16_t sat_s16_add(int16_t x, int16_t y) {
  int16_t res = x + y;
  if (~x & ~y & res & 0x8000) {
    return 0x7FFF;
  }
  if (x & y & ~res & 0x8000) {
    return 0x8000;
  }
  return res;
}

// Saturating subtraction on 16-bit signed numbers.
static int16_t sat_s16_sub(int16_t x, int16_t y) {
  int16_t res = x - y;
  if (~x & y & res & 0x8000) {
    return 0x7FFF;
  }
  if (x & ~y & ~res & 0x8000) {
    return 0x8000;
  }
  return res;
}

// Saturating multiplication on 16-bit signed numbers.
static int16_t sat_s16_mul(int16_t x, int16_t y) {
  int32_t res_wide = ((int32_t) x) * ((int32_t) y);
  if (res_wide > 0x7FFF) {
    return 0x7FFF;
  }
  if (res_wide < -0x8000) {
    return -0x8000;
  }
  return res_wide & 0xFFFF;
}

// Saturating division on 16-bit signed numbers.
static int16_t sat_s16_div(int16_t x, int16_t y) {
  // Check for div/0.
  if (y == 0) {
    return 0;
  }
  // The only situation in which overflow can occur (the result would be 0x8000,
  // which is unrepresentable).
  if (x == -0x8000 && y == -0x0001) {
    return 0x7FFF;
  }
  return x / y;
}

// Saturating addition on 32-bit signed numbers.
static int32_t sat_s32_add(int32_t x, int32_t y) {
  int32_t res = x + y;
  if (~x & ~y & res & 0x80000000L) {
    return 0x7FFFFFFFL;
  }
  if (x & y & ~res & 0x80000000L) {
    return 0x80000000L;
  }
  return res;
}

// Saturating subtraction on 32-bit signed numbers.
static int32_t sat_s32_sub(int32_t x, int32_t y) {
  int32_t res = x - y;
  if (~x & y & res & 0x80000000L) {
    return 0x7FFFFFFFL;
  }
  if (x & ~y & ~res & 0x80000000L) {
    return 0x80000000L;
  }
  return res;
}

// Saturating multiplication on 32-bit signed numbers.
static int32_t sat_s32_mul(int32_t x, int32_t y) {
  int64_t res_wide = ((int64_t) x) * ((int64_t) y);
  if (res_wide > 0x7FFFFFFFL) {
    return 0x7FFFFFFFL;
  }
  if (res_wide < -0x80000000L) {
    return -0x80000000L;
  }
  return res_wide & 0xFFFFFFFFL;
}

// Saturating division on 32-bit signed numbers.
static int32_t sat_s32_div(int32_t x, int32_t y) {
  // Check for div/0.
  if (y == 0) {
    return 0;
  }
  // The only situation in which overflow can occur (the result would be
  // 0x80000000, which is unrepresentable).
  if (x == -0x80000000L && y == -0x00000001) {
    return 0x7FFFFFFFL;
  }
  return x / y;
}
