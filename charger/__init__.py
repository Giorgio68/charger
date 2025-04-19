from .control import (
    setup_gpio,
    raise_battery_actuator,
    lower_battery_actuator,
    raise_hotswap_actuator,
    lower_hotswap_actuator,
    select_battery_one,
    select_battery_two,
)


def main():
    request = setup_gpio()

    with request:
        # lower battery one from drone
        # start by adding hotswap
        raise_hotswap_actuator(request)

        # then get ready to remove battery
        raise_battery_actuator(request)

        # now we can lower the actuator
        lower_battery_actuator(request)

        # change battery base
        select_battery_two(request)

        # now we can lift the new battery
        raise_battery_actuator(request)

        # now we can lower the actuator
        lower_battery_actuator(request)

        # remove hotswap power
        lower_hotswap_actuator(request)
