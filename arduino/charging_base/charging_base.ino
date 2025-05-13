/*
 * File for control of automated battery swap system
 * Base unit
 * (c) Giorgio Salvemini, 2025
 */


/* header files */
#include "Stepper.h"


/* defining consts */
#define STEPPER_MOTOR_PIN 10
#define BATTERY_LIFTER 11
#define FORWARD_PWM_PIN 3
#define FORWARD_BRAKE_PIN 9
#define FORWARD_DIR 12
#define BACKWARDS_PWM_PIN 11
#define BACKWARDS_BRAKE_PIN 8
#define BACKWARDS_DIR 13

#define BASE_1 5
#define BASE_2 6
#define IS_DONE 7


/* defining globals */
const byte STEPS_PER_REV = 200; // 2mm linear
const double LINEAR_PER_REV = 2.0;
Stepper base_motor = Stepper(STEPS_PER_REV, FORWARD_DIR, BACKWARDS_DIR);


void setup() {
  /* initialize all required pins */
  pinMode(STEPPER_MOTOR_PIN, OUTPUT);

  /* Set the PWM and brake pins so that the direction pins can be used to control the motor */
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
  pinMode(BASE_1, INPUT);
  pinMode(BASE_2, INPUT);
  pinMode(IS_DONE, OUTPUT);
  digitalWrite(IS_DONE, LOW);

  /* initialize serial interface for debugging */
  Serial.begin(115200);
  (void)Serial.println("Setup completed, ready");
}


void loop() {
  //(void)read_serial();

  double distance_per_base = 85.0; // mm
  double rev_per_base = distance_per_base / LINEAR_PER_REV;
  bool input_received = false, base_1_high, base_2_high;
  uint8_t dir = 0;

  //(void)Serial.println("Initialized everything");

  while (!input_received) {
    base_1_high = (bool)digitalRead(BASE_1);
    base_2_high = (bool)digitalRead(BASE_2);

    //(void)Serial.println("Read inputs");

    if (base_1_high)
      dir |= 1;
    if (base_2_high)
      dir |= 2;
    input_received = base_1_high || base_2_high;
    delay(1000);
    //(void)Serial.println("Checked input value");
  }

  switch (dir) {
    case 1:
      debug_msg("Selecting battery 1...");
      move_forward(rev_per_base);
      break;
    case 2:
      debug_msg("Selecting battery 2...");
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


void move_backward(double revs) {
  move_base(-1 * STEPS_PER_REV * revs);
  signal_done();
}


void signal_done() {
  digitalWrite(IS_DONE, HIGH);
  delay(3000);
  digitalWrite(IS_DONE, LOW);
}


void move_base(int steps) {
  debug_msg("Moving %d steps", steps);
  base_motor.step(steps);
}


void debug_msg(char *fmt, ...) {
  va_list arg;
  char *msg;
  va_start(arg, fmt);

  (void)vsprintf(msg, fmt, arg);
  (void)Serial.println(msg);
}
