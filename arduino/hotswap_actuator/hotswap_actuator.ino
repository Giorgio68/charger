/*
 * File for control of automated battery swap system
 * hotswap actuator unit
 * (c) Giorgio Salvemini, 2025
 */


/* header files */
#include <stdarg.h>
#include <stdint.h>
#include "Stepper.h"


/* defining consts */
#define STEPPER_MOTOR_PIN 10
//#define BATTERY_LIFTER 11
#define FORWARD_PWM_PIN 3
#define FORWARD_BRAKE_PIN 9
#define FORWARD_DIR 12
#define BACKWARDS_PWM_PIN 11
#define BACKWARDS_BRAKE_PIN 8
#define BACKWARDS_DIR 13

#define UP 5
#define DOWN 6
#define IS_DONE 7


/* defining globals */
const uint16_t STEPS_PER_REV = 200; // 2mm linear
const double LINEAR_PER_REV = 2.0;
Stepper base_motor = Stepper(STEPS_PER_REV, FORWARD_DIR, BACKWARDS_DIR);


void setup() {
  /* initialize all required pins */
  pinMode(STEPPER_MOTOR_PIN, OUTPUT);

  // Set the PWM and brake pins so that the direction pins can be used to control the motor:
  pinMode(FORWARD_PWM_PIN, OUTPUT);
  pinMode(FORWARD_BRAKE_PIN, OUTPUT);
  pinMode(BACKWARDS_PWM_PIN, OUTPUT);
  pinMode(BACKWARDS_BRAKE_PIN, OUTPUT);

  digitalWrite(FORWARD_PWM_PIN, HIGH);
  digitalWrite(FORWARD_BRAKE_PIN, LOW);
  digitalWrite(BACKWARDS_PWM_PIN, HIGH);
  digitalWrite(BACKWARDS_BRAKE_PIN, LOW);

  base_motor.setSpeed(60); /* unit is rpm */

  /* initialize comms */
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(IS_DONE, OUTPUT);
  digitalWrite(IS_DONE, LOW);

  /* initialize serial interface for debugging */
  Serial.begin(115200);
  (void)Serial.println("Setup completed, ready");
}


void loop() {
  double max_extension = 71.0; // mm
  int32_t rev_per_base = 45; // max_extension / LINEAR_PER_REV;
  debug_msg("Movement: %d revolutions per mm", rev_per_base);
  bool input_received = false, move_upwards, move_downwards;
  uint8_t dir = 0;

  while (!input_received) {
    Serial.println(digitalRead(UP));
    Serial.println(digitalRead(DOWN));
    move_upwards = (bool)digitalRead(UP);
    move_downwards = (bool)digitalRead(DOWN);

    if (move_upwards) dir |= 1;
    if (move_downwards) dir |= 2;
    input_received = move_upwards || move_downwards;

    debug_msg("Dir: %d", dir);
    delay(1000);
  }

  switch (dir) {
    case 1:
      debug_msg("Moving upwards...");
      move_forward(rev_per_base);
      break;
    case 2:
      debug_msg("Moving downwards...");
      move_backward(rev_per_base);
      break;
    case 3:
    default:
      debug_msg("Received invalid combination: %d", dir);
  }
  input_received = false;
  dir = 0;
}


void move_forward(double revs) {
  move_base(STEPS_PER_REV * revs);
  signal_done();
}


void move_backward(int32_t revs) {
  move_base(-1 * STEPS_PER_REV * revs);
  signal_done();
}


void signal_done() {
  digitalWrite(IS_DONE, HIGH);
  delay(3000);
  digitalWrite(IS_DONE, LOW);
}


void move_base(int32_t steps) {
  debug_msg("Moving %d steps", (int)steps);
  base_motor.step(steps);
}


void debug_msg(const char *fmt, ...) {
  va_list arg;
  char msg[256];
  va_start(arg, fmt);

  (void)vsnprintf(msg, sizeof(msg), fmt, arg);
  (void)Serial.println(msg);
}
