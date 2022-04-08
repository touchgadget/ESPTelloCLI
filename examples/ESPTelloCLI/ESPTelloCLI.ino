/*
 * MIT License
 *
 * Copyright (c) 2022 touchgadgetdev@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <ESPTelloCLI.h>

ESPTelloCLI TelloCLI;

bool Connected;

void WiFiEvent(WiFiEvent_t event){
    switch(event) {
#ifdef ESP8266
      case WIFI_EVENT_STAMODE_GOT_IP:
#else
      case ARDUINO_EVENT_WIFI_STA_GOT_IP:
#endif
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());
          TelloCLI.begin();
          // Turn off telemetry if not needed.
          TelloCLI.setTelemetry(false);
          Connected = true;
          break;
#ifdef ESP8266
      case WIFI_EVENT_STAMODE_DISCONNECTED:
#else
      case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
#endif
          Serial.println("WiFi lost connection");
          TelloCLI.end();
          Connected = false;
          break;
      default: break;
    }
}

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(0);
  Connected = false;
  WiFi.mode(WIFI_STA);
  WiFi.onEvent(WiFiEvent);
}

void loop()
{
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    char SSID[65];

    if (command.length() == 0) command = Serial.readStringUntil('\r');
    if (command.length() > 0) {
      command.trim();
      if (command.equalsIgnoreCase("connect?")) {
        if (Connected)
          Serial.print("ok\r\n");
        else
          Serial.print("error\r\n");
      }
      else if (command.startsWith("connect")) {
        command.replace("connect", "");
        command.trim();
        int blank = command.indexOf(' ');
        if (blank == -1) {
          strcpy(SSID, command.c_str());
          WiFi.begin(SSID);
        }
        else {
          strcpy(SSID, command.substring(0, blank).c_str());
          command.replace(SSID, "");
          command.trim();
          WiFi.begin(SSID, command.c_str());
        }
        Serial.print("ok\r\n");
      }
      else if (command.equalsIgnoreCase("telemetryon")) {
        TelloCLI.setTelemetry(true);
        Serial.print("ok\r\n");
      }
      else if (command.equalsIgnoreCase("telemetryoff")) {
        TelloCLI.setTelemetry(false);
        Serial.print("ok\r\n");
      }
      else {
        if (Connected) TelloCLI.write(command.c_str(), command.length());
      }
    }
  }
}
