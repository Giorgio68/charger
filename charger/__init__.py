from sys import exit

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
    print("Autonomous Battery Replacement 1.0 started")
    request = setup_gpio()

    with request:
        while True:
            print("""
############################################
# Menu:                                    #
# 1. Select battery position 1             #
# 2. Select battery position 2             #
# 3. Raise battery actuator                #
# 4. Lower battery actuator                #
# 5. Raise hotswap actuator                #
# 6. Lower hotswap actuator                #
# 7. Automated battery replacement process #
# 8. Quit                                  #
############################################
""")
            try:
                user_input = int(input("Choice: "))

            except ValueError:
                print("Invalid choice selected")
                continue

            if user_input == 1:
                select_battery_one(request)
                continue

            elif user_input == 2:
                select_battery_two(request)
                continue

            elif user_input == 3:
                raise_battery_actuator(request)
                continue

            elif user_input == 4:
                lower_battery_actuator(request)
                continue

            elif user_input == 5:
                raise_hotswap_actuator(request)
                continue

            elif user_input == 6:
                lower_hotswap_actuator(request)
                continue

            elif user_input == 7:
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
                continue

            elif user_input == 8:
                print("Goodbye")
                exit(0)
