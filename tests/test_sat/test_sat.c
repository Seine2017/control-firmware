#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "sat.h"

typedef int64_t int64_t;

#define N 1e6

int64_t rand_int(int64_t min, int64_t max) {
  int64_t r1 = (int64_t) rand();
  int64_t r2 = (int64_t) rand();
  return (((r1 << 32) ^ r2) % (max - min + 1)) + min;
}

int64_t clamp(int64_t x, int64_t min, int64_t max) {
  if (x < min) {
    return min;
  }
  if (x > max) {
    return max;
  }
  return x;
}

static int test_s16_add() {
  printf("test_s16_add... ");
  int i;
  for (i = 0; i < N; i++) {
    const int64_t x = rand_int(-0x8000L, 0x7FFFL);
    const int64_t y = rand_int(-0x8000L, 0x7FFFL);
    const int64_t true_result = x + y;
    const int64_t expected_result = clamp(true_result, -0x8000L, 0x7FFFL);
    const int64_t actual_result = (int64_t) sat_s16_add((int16_t) x, (int16_t) y);
    if (expected_result != actual_result) {
      printf(" failed: %ld + %ld (expected %ld, got %ld)\n", x, y, expected_result, actual_result);
      return 1;
    }
  }
  printf("success\n");
  return 0;
}

static int test_s16_sub() {
  printf("test_s16_sub... ");
  int i;
  for (i = 0; i < N; i++) {
    const int64_t x = rand_int(-0x8000L, 0x7FFFL);
    const int64_t y = rand_int(-0x8000L, 0x7FFFL);
    const int64_t true_result = x - y;
    const int64_t expected_result = clamp(true_result, -0x8000L, 0x7FFFL);
    const int64_t actual_result = (int64_t) sat_s16_sub((int16_t) x, (int16_t) y);
    if (expected_result != actual_result) {
      printf("failed: %ld - %ld (expected %ld, got %ld)\n", x, y, expected_result, actual_result);
      return 1;
    }
  }
  printf("success\n");
  return 0;
}

static int test_s16_mul() {
  printf("test_s16_mul... ");
  int i;
  for (i = 0; i < N; i++) {
    const int64_t x = rand_int(-0x8000L, 0x7FFFL);
    const int64_t y = rand_int(-0x8000L, 0x7FFFL);
    const int64_t true_result = x * y;
    const int64_t expected_result = clamp(true_result, -0x8000L, 0x7FFFL);
    const int64_t actual_result = (int64_t) sat_s16_mul((int16_t) x, (int16_t) y);
    if (expected_result != actual_result) {
      printf("failed: %ld * %ld (expected %ld, got %ld)\n", x, y, expected_result, actual_result);
      return 1;
    }
  }
  printf("success\n");
  return 0;
}

static int test_s16_div() {
  printf("test_s16_div... ");
  int i;
  for (i = 0; i < N; i++) {
    const int64_t x = rand_int(-0x8000L, 0x7FFFL);
    const int64_t y = rand_int(-0x8000L, 0x7FFFL);
    const int64_t true_result = (y == 0) ? 0 : (x / y);
    const int64_t expected_result = clamp(true_result, -0x8000L, 0x7FFFL);
    const int64_t actual_result = (int64_t) sat_s16_div((int16_t) x, (int16_t) y);
    if (expected_result != actual_result) {
      printf("failed: %ld / %ld (expected %ld, got %ld)\n", x, y, expected_result, actual_result);
      return 1;
    }
  }
  printf("success\n");
  return 0;
}

static int test_s32_add() {
  printf("test_s32_add... ");
  int i;
  for (i = 0; i < N; i++) {
    const int64_t x = rand_int(-0x80000000L, 0x7FFFFFFFL);
    const int64_t y = rand_int(-0x80000000L, 0x7FFFFFFFL);
    const int64_t true_result = x + y;
    const int64_t expected_result = clamp(true_result, -0x80000000L, 0x7FFFFFFFL);
    const int64_t actual_result = (int64_t) sat_s32_add((int32_t) x, (int32_t) y);
    if (expected_result != actual_result) {
      printf(" failed: %ld + %ld (expected %ld, got %ld)\n", x, y, expected_result, actual_result);
      return 1;
    }
  }
  printf("success\n");
  return 0;
}

static int test_s32_sub() {
  printf("test_s32_sub... ");
  int i;
  for (i = 0; i < N; i++) {
    const int64_t x = rand_int(-0x80000000L, 0x7FFFFFFFL);
    const int64_t y = rand_int(-0x80000000L, 0x7FFFFFFFL);
    const int64_t true_result = x - y;
    const int64_t expected_result = clamp(true_result, -0x80000000L, 0x7FFFFFFFL);
    const int64_t actual_result = (int64_t) sat_s32_sub((int32_t) x, (int32_t) y);
    if (expected_result != actual_result) {
      printf("failed: %ld - %ld (expected %ld, got %ld)\n", x, y, expected_result, actual_result);
      return 1;
    }
  }
  printf("success\n");
  return 0;
}

static int test_s32_mul() {
  printf("test_s32_mul... ");
  int i;
  for (i = 0; i < N; i++) {
    const int64_t x = rand_int(-0x80000000L, 0x7FFFFFFFL);
    const int64_t y = rand_int(-0x80000000L, 0x7FFFFFFFL);
    const int64_t true_result = x * y;
    const int64_t expected_result = clamp(true_result, -0x80000000L, 0x7FFFFFFFL);
    const int64_t actual_result = (int64_t) sat_s32_mul((int32_t) x, (int32_t) y);
    if (expected_result != actual_result) {
      printf("failed: %ld * %ld (expected %ld, got %ld)\n", x, y, expected_result, actual_result);
      return 1;
    }
  }
  printf("success\n");
  return 0;
}

static int test_s32_div() {
  printf("test_s32_div... ");
  int i;
  for (i = 0; i < N; i++) {
    const int64_t x = rand_int(-0x80000000L, 0x7FFFFFFFL);
    const int64_t y = rand_int(-0x80000000L, 0x7FFFFFFFL);
    const int64_t true_result = (y == 0) ? 0 : (x / y);
    const int64_t expected_result = clamp(true_result, -0x80000000L, 0x7FFFFFFFL);
    const int64_t actual_result = (int64_t) sat_s32_div((int32_t) x, (int32_t) y);
    if (expected_result != actual_result) {
      printf("failed: %ld / %ld (expected %ld, got %ld)\n", x, y, expected_result, actual_result);
      return 1;
    }
  }
  printf("success\n");
  return 0;
}

int main() {
  int failed = 0;

  failed |= test_s16_add();
  failed |= test_s16_sub();
  failed |= test_s16_mul();
  failed |= test_s16_div();
  failed |= test_s32_add();
  failed |= test_s32_sub();
  failed |= test_s32_mul();
  failed |= test_s32_div();

  if (failed) {
    printf("FAIL");
    return 1;
  }
  else {
    printf("SUCCESS");
    return 0;
  }
}
