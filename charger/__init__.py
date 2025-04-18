from .control import setup_gpio, GPIO, enable_pin, disable_pin, wait_stepper_done

def main():
    request = setup_gpio()

    with request:
        while True:
            # lower battery one from drone
            # start by raising actuator
            enable_pin(request, GPIO.STEPPER_BATTERY_UP)
            wait_stepper_done(request, GPIO.STEPPER_BATTERY_DONE)
            disable_pin(request, GPIO.STEPPER_BATTERY_UP)

            # now we can lower the actuator
            enable_pin(request, GPIO.STEPPER_BATTERY_DOWN)
            wait_stepper_done(request, GPIO.STEPPER_BATTERY_DONE)
            disable_pin(request, GPIO.STEPPER_BATTERY_DOWN)
