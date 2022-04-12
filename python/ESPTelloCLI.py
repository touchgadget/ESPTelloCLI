""" ESPTelloCLI class based on DJITelloPy API Reference """
import time
import serial

class ESPTelloCLI:
    """ ESPTelloCLI class """
    def __init__(self, serial_name, tello_SSID):
        self.serial_name = serial_name
        self.tello_SSID = tello_SSID
        self.serial_port = None

    @staticmethod
    def constrain(x, lower_limit, upper_limit):
        """ Constrain x to be lower_limit <= x <= upper_limit """
        if x < lower_limit:
            return lower_limit
        else:
            if x > upper_limit:
                return upper_limit
        return x

    @staticmethod
    def command_response(serial_port, command, expected, secs):
        """ Write Tello command then return Tello response """
        serial_port.write(command.encode('utf-8'))
        print(command)
        if secs == 0:
            return True
        expected = expected.encode('utf-8')
        for i in range(0, secs):
            a_line = serial_port.read_until('\n')
            print(i)
            if a_line:
                print(a_line)
                if not expected or expected in a_line:
                    return a_line
        return False

    def connect(self):
        """ Connect ESPTelloCLI adapter on serial port to Tello drone SSID """
        self.serial_port = serial.Serial(self.serial_name, 115200, timeout=1,
                                         parity=serial.PARITY_NONE)
        print(self.serial_port)
        return self.command_response(self.serial_port,
            'connect ' + self.tello_SSID + '\n', 'connected', 20)

    def sdkmode(self):
        """ Put Tello drone in SDK mode """
        return self.command_response(self.serial_port, 'command\n', 'ok', 3)

    def set_speed(self, speed):
        """ Set speed """
        speed = str(self.constrain(speed, 10, 100))
        return self.command_response(self.serial_port, 'speed' + speed + '\n', 'ok', 3)

    def set_rc(self, left_right, forward_back, up_down, rotate):
        """ Radio/Control (rc) mode. Note: no response """
        left_right = str(self.constrain(left_right, -100, 100))
        forward_back = str(self.constrain(forward_back, -100, 100))
        up_down = str(self.constrain(up_down, -100, 100))
        rotate = str(self.constrain(rotate, -100, 100))
        command = 'rc ' + left_right + ' ' + forward_back + ' ' + up_down + ' ' + rotate + '\n'
        return self.command_response(self.serial_port, command, '', 0)

    def move_up(self, cm):
        """ Go up centimeters """
        cm = str(self.constrain(cm, 20, 500))
        return self.command_response(self.serial_port, 'up ' + cm + '\n', 'ok', 5)

    def move_down(self, cm):
        """ Go down centimeters """
        cm = str(self.constrain(cm, 20, 500))
        return self.command_response(self.serial_port, 'down ' + cm + '\n', 'ok', 5)

    def move_forward(self, cm):
        """ Go left centimeters """
        cm = str(self.constrain(cm, 20, 500))
        return self.command_response(self.serial_port, 'forward ' + cm + '\n', 'ok', 5)

    def move_back(self, cm):
        """ Go left centimeters """
        cm = str(self.constrain(cm, 20, 500))
        return self.command_response(self.serial_port, 'back ' + cm + '\n', 'ok', 5)

    def move_left(self, cm):
        """ Go left centimeters """
        cm = str(self.constrain(cm, 20, 500))
        return self.command_response(self.serial_port, 'left ' + cm + '\n', 'ok', 5)

    def move_right(self, cm):
        """ Go right centimeters """
        cm = str(self.constrain(cm, 20, 500))
        return self.command_response(self.serial_port, 'right ' + cm + '\n', 'ok', 5)

    def rotate_clockwise(self, degrees):
        """ Rotate clock wise degrees """
        degrees = str(self.constrain(degrees, 1, 3600))
        return self.command_response(self.serial_port, 'cw ' + degrees + '\n', 'ok', 3)

    def rotate_counter_clockwise(self, degrees):
        """ rotate counter clock wise degrees """
        degrees = str(self.constrain(degrees, 1, 3600))
        return self.command_response(self.serial_port, 'ccw ' + degrees + '\n', 'ok', 3)

    def flip_forward(self):
        """ Flip forward """
        return self.command_response(self.serial_port, 'flip f\n', 'ok', 5)

    def flip_back(self):
        """ Flip backward """
        return self.command_response(self.serial_port, 'flip b\n', 'ok', 5)

    def flip_left(self):
        """ Flip left """
        return self.command_response(self.serial_port, 'flip l\n', 'ok', 5)

    def flip_right(self):
        """ Flip right """
        return self.command_response(self.serial_port, 'flip r\n', 'ok', 5)

    def go_xyz_speed(self, x, y, z, speed):
        """ Fly to x y z at speed (cm/s) """
        x = str(self.constrain(x, -500, 500))
        y = str(self.constrain(y, -500, 500))
        z = str(self.constrain(z, -500, 500))
        speed = str(self.constrain(speed, 10, 100))
        command = 'go ' + x + ' ' + y + ' ' + z + ' ' + speed + '\n'
        return self.command_response(self.serial_port, command, 'ok', 3)

    def curve_xyz_speed(self, x1, y1, z1, x2, y2, z2, speed):
        """ Fly a curve defined by the current and two given coords with speed cm/s """
        x1 = str(self.constrain(x1, -500, 500))
        y1 = str(self.constrain(y1, -500, 500))
        z1 = str(self.constrain(z1, -500, 500))
        x2 = str(self.constrain(x2, -500, 500))
        y2 = str(self.constrain(y2, -500, 500))
        z2 = str(self.constrain(z2, -500, 500))
        speed = str(self.constrain(speed, 10, 60))
        command = 'curve ' + x1 + ' ' + y1 + ' ' + z1 + ' ' + \
            x2 + ' ' + y2 + ' ' + z2 + ' ' + speed + '\n'
        return self.command_response(self.serial_port, command, 'ok', 3)

    def get_battery(self):
        """ Return battery percentage """
        return self.command_response(self.serial_port, 'battery?\n', '', 3)

    def get_height(self):
        """ Return height """
        return self.command_response(self.serial_port, 'height?\n', '', 3)

    def get_speed(self):
        """ Return speed """
        return self.command_response(self.serial_port, 'speed?\n', '', 3)

    def get_flight_time(self):
        """ Return time motors have been active """
        return self.command_response(self.serial_port, 'time?\n', '', 3)

    def get_temp(self):
        """ Return temp """
        return self.command_response(self.serial_port, 'temp?\n', '', 3)

    def get_attitude(self):
        """ Return attitude """
        return self.command_response(self.serial_port, 'attitude?\n', '', 3)

    def get_barometer(self):
        """ Return barometric pressure """
        return self.command_response(self.serial_port, 'baro?\n', '', 3)

    def get_acceleration(self):
        """ Return acceleration """
        return self.command_response(self.serial_port, 'acceleration?\n', '', 3)

    def get_distance_tof(self):
        """ Return current distance value from TOF in cm """
        return self.command_response(self.serial_port, 'tof?\n', '', 3)

    def get_wifi(self):
        """ Return WiFi signal to noise ratio """
        return self.command_response(self.serial_port, 'wifi?\n', '', 3)

    def takeoff(self):
        """ Takeoff """
        return self.command_response(self.serial_port, 'takeoff\n', 'ok', 20)

    def land(self):
        """ Land """
        return self.command_response(self.serial_port, 'land\n', 'ok', 10)

    def emergency(self):
        """ Emergency -- all motors off NOW """
        return self.command_response(self.serial_port, 'emergency\n', 'ok', 10)

def mymain():
    """ Test this class WARNING: *** Drone will FLY *** """
    # Fill in serial port name and the Tello SSID
    drone1 = ESPTelloCLI('/dev/ttyUSB0', 'TELLO-xxxxxx')
    if drone1.connect():
        if drone1.sdkmode():
            print(drone1.get_battery())
            if drone1.takeoff():
                drone1.move_down(20)
                drone1.move_up(20)
                drone1.move_left(20)
                drone1.move_right(20)
                drone1.move_forward(20)
                drone1.move_back(20)
                drone1.flip_left()
                drone1.flip_right()
                drone1.flip_forward()
                drone1.flip_back()
                drone1.rotate_clockwise(360)
                drone1.rotate_counter_clockwise(360)
                print(drone1.get_height())
                print(drone1.get_speed())
                print(drone1.get_flight_time())
                print(drone1.get_temp())
                print(drone1.get_attitude())
                print(drone1.get_barometer())
                print(drone1.get_acceleration())
                print(drone1.get_distance_tof())
                print(drone1.get_wifi())
                # Put that thing down and don't make me tell you twice!
                if not drone1.land():
                    drone1.land()
            else:
                print('takeoff failed')
        else:
            print('SDK enable failed')
    else:
        print('Connect failed')

if __name__ == '__main__':
    mymain()
