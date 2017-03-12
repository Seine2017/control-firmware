#ifndef __PID_H
#define __PID_H

typedef struct {
  float integral;
  float previous;
} pid_state_t;

// Initial value for variables of type pid_state_t.
#define INITIAL_PID_STATE {0.0, 0.0}

static float clamp(float x, float min, float max) {
  if (x < min) {
    return min;
  }
  if (x > max) {
    return max;
  }
  return x;
}

// This is an inline function so that accesses to 'state', which will usually be
// a static (i.e. global) variable, can be changed from indirect to direct
// addressing, which is much faster.
static inline float pid_cycle(
  const float desired,
  const float measured,
  pid_state_t *state,
  const float dt,
  const float p_gain,
  const float i_gain,
  const float d_gain,
  const float min_integral,
  const float max_integral) {

  // Calculate error.
  const float error = desired - measured;

  // Calculate integral of error.
  const float integral = clamp(state->integral + error * dt, min_integral, max_integral);
  state->integral = integral;

  // Calculate derivative of error.
  const float derivative = (error - state->previous) / dt;
  state->previous = error;

  // Calculated weighted sum of error, error integral and error derivative.
  return p_gain * error + i_gain * integral + d_gain * derivative;
}

#endif
