import time
from enum import IntEnum
import gpiod
from gpiod.line import Direction, Value


class GPIO(IntEnum):
    STEPPER_BATTERY_UP = 5 
    STEPPER_BATTERY_DOWN = 6
    STEPPER_BATTERY_DONE = 26

    STEPPER_BASE_1 = 14
    STEPPER_BASE_2 = 15
    STEPPER_BASE_DONE = 18

    STEPPER_HOTSWAP_UP = 13
    STEPPER_HOTSWAP_DOWN = 19
    STEPPER_HOTSWAP_DONE = 20


def setup_gpio():
    request = gpiod.request_lines(
        "/dev/gpiochip4",
        consumer="blink-example",
        config={
            GPIO.STEPPER_BATTERY_UP: gpiod.LineSettings(
                direction=Direction.OUTPUT, output_value=Value.ACTIVE
            ),
            GPIO.STEPPER_BATTERY_DOWN: gpiod.LineSettings(
                direction=Direction.OUTPUT, output_value=Value.ACTIVE
            ),
            GPIO.STEPPER_BATTERY_DONE: gpiod.LineSettings(
                direction=Direction.INPUT
            ),
            GPIO.STEPPER_BASE_1: gpiod.LineSettings(
                direction=Direction.OUTPUT, output_value=Value.ACTIVE
            ),
            GPIO.STEPPER_BASE_2: gpiod.LineSettings(
                direction=Direction.OUTPUT, output_value=Value.ACTIVE
            ),
            GPIO.STEPPER_BASE_DONE: gpiod.LineSettings(
                direction=Direction.INPUT
            ),
            GPIO.STEPPER_HOTSWAP_UP: gpiod.LineSettings(
                direction=Direction.OUTPUT, output_value=Value.ACTIVE
            ),
            GPIO.STEPPER_HOTSWAP_DOWN: gpiod.LineSettings(
                direction=Direction.OUTPUT, output_value=Value.ACTIVE
            ),
            GPIO.STEPPER_HOTSWAP_DONE: gpiod.LineSettings(
                direction=Direction.INPUT
            )
        },
    )

    return request


def enable_pin(req, line):
    req.set_value(line, Value.ACTIVE)
    print(f"Turning pin {line} on")


def disable_pin(req, line):
    req.set_value(line, Value.INACTIVE)
    print(f"Turning pin {line} off")


def wait_stepper_done(req, line):
    while not req.get_value(line):
        time.sleep(1)
