# Experimental DJITelloPy TelloSwarm and ESPTelloCLI

DJITelloPy TelloSwarm works with ESPTelloCLI. swarm.py is slightly modified to
work with ESPTelloCLI. I do not understand most of the thread code in this
class. Fortunately it seems to work at least for my test cases so far. tello.py
is mostly ESPTelloCLI.py with more changes to make look like DJITelloPy. I will
continue development on this version instead of the version in
../python/ESPTelloCLI.py.

Note LICENSE.txt from the DJITelloPy project is included since swarm.py is
nearly identical. tello.py was written from scratch to work with the Arduino
ESPTelloCLI program but has been modified to make it look like the DJITelloPy
Tello class so it can be used with TelloSwarm. The license for my code is also
MIT so there is no conflict.

```python
from swarm import TelloSwarm
import time

swarm = TelloSwarm.fromSerialSSID([
    ["/dev/cu.usbserial-0001", "TELLO-xxxxxx"],
    ["/dev/cu.usbserial-10", "TELLO-yyyyyy"]
])

swarm.connect()
swarm.get_battery()
swarm.takeoff()
swarm.rotate_clockwise(90)
time.sleep(1)
swarm.rotate_counter_clockwise(180)
time.sleep(1)
swarm.rotate_clockwise(90)
time.sleep(1)
swarm.land()
```

This was tested on a Macbook Air connected to two ESP32 boards running the
ESPTelloCLI arduino program and two regular Tellos.

```
             /--USB-- ESP32-1 --WiFi-- Tello-1
Macbook Air +
             \--USB-- ESP32-2 --WiFi-- Tello-2
```
