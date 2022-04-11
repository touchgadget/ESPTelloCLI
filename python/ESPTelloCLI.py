""" Simple ESPTelloCLI test """
import time
import serial

# Substitute the name of your Serial port here
drone1 = serial.Serial('/dev/ttyUSB0', 115200, timeout=1,
                       parity=serial.PARITY_NONE)
# If 1 drone works, try 2 at the same time.
#drone2 = serial.Serial('/dev/ttyUSB1', 115200, timeout=1,
#                       parity=serial.PARITY_NONE)

def command_response(drone, command, expected, secs):
    """ Write Tello command then return Tello response """
    drone.write(command)
    print(command)
    for i in range(0, secs):
        aLine = drone.read_until('\n')
        print(i)
        if aLine:
            print(aLine)
            if not expected or expected in aLine:
                return aLine
    return False

# Fill in the SSID of your Tello
if command_response(drone1, b'connect TELLO-xxxxxx\n', b'connected', 20):
    if command_response(drone1, b'command\n', b'ok', 3):
        command_response(drone1, b'battery?\n', b'', 3)
        if command_response(drone1, b'takeoff\n', b'ok', 20):
            command_response(drone1, b'height?\n', b'', 3)
            # Hover for 10 seconds
            time.sleep(10)
            # Put that thing down and don't make me tell you twice!
            if not command_response(drone1, b'land\n', b'ok', 10):
                command_response(drone1, b'land\n', b'ok', 10)
        else:
            print('takeoff failed')
    else:
        print('SDK enable failed')
else:
    print('Connect failed')
