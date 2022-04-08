# ESPTelloCLI -- Experiments with Tello drone and ESP32/ESP8266

The Tello SDK is based on UDP packets so an ESP32/ESP8266 board with WiFi can
easily communicate with the Tello. The Tello only works as an Access Point (AP)
so one ESP board is required for each Tello. This project is designed to work
with the original Tello so it does not depend on the newer features of the
Tello EDU or the RMTT. No testing has been done with the Tello EDU and RMTT due
to lack of access to the newer drones. This program may work with newer drones
and/or newer versions of the SDK because it acts as a bi-directional pass
through.

```
PC (Win,Mac,Linux) <-USB Serial-> ESP board <-WiFi-> Tello
```

The ESP board is a bridge between the PC and the Tello drone. It communicates
with the Tello using Tello SDK UDP packets. It communicates with the PC over
USB serial. For example, if the PC sends "battery?" out the USB serial port,
the ESP board forwards "battery?" using a UDP packet to the Tello. When the
Tello responds, the ESP board extracts the response then sends it to the PC
over USB serial. The program running on the ESP board is named ESPTelloCLI
because it presents a bi-directional Command Line Interface (CLI) via USB
serial port. For the PC programmer, juggling multiple serial ports is easier
than juggling multiple USB WiFi interfaces. ESP boards are cheaper and widely
available compared to other boards with WiFi and USB serial.

One ESP board running ESPTelloCLI is required for each Tello. Each ESPTelloCLI board
appears as a USB serial COM port.

From a terminal program or from the Arduino Serial monitor send commands
to connect to Tello like this. SSID and PSK are case-sensitive so be careful.
```
 connect TELLO-xxxxxx
```
 or
```
connect TELLO-xxxxxx yyyyyyyy
```

Check for the Tello connection status by sending `connect?`
```
 connect?
```

`connect?` returns ok if connected and error if not.

At this point, commands sent to ESPTelloCLI are passed on to the Tello via
the Tello command UDP port. The SDK response is returned without modification.

After connecting to Tello enable SDK mode send `command`

```
 command
```

At this point any SDK command may be sent such as `battery?`,
`takeoff`, `land`, etc.

These are the additional commands implemented by ESPTelloCLI.
```
  connect SSID PASSWORD
  connect?
  telemetryon
  telemetryoff
```

telemetryon turns on output from the Tello SDK State UDP port. The data is
formatted in exactly the same way as specified in the SDK document.
telemetryoff turns this feature off.

If Tello Telemetry is on, set the Arduino Serial port to the highest possible
baud rate. The maximum baud depends on the USB serial chip and the OS driver
for the chip.

If using ESP32-S2 or ESP32-S3, use the hardware USB port because it much faster
than any UART port. The baud rate is ignored when using the USB hardware port.
Regular ESP32 and ESP8266 do not have hardware USB.

There is no support for forwarding video data from the Tello. This might be too
much work for an ESP32. The new ESP32-S3 may work but it has not yet been
tried.

## Installation

### Arduino IDE

* [Arduino IDE](https://www.arduino.cc/en/software)
* [Arduino for ESP32](https://github.com/espressif/arduino-esp32)
* [Arduino for ESP8266](https://github.com/esp8266/Arduino)

### Arduino Libraries

* [ESPAsyncUDP](https://github.com/me-no-dev/ESPAsyncUDP)
* [ESPTelloCLI](https://github.com/touchgadget/ESPtelloCLI)

## References

* [Tello SDK](https://www.ryzerobotics.com/tello/downloads)
