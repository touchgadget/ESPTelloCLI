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

#define TELLO_DEBUG 0
#if TELLO_DEBUG
#define Dbg_begin(...) Serial.begin(__VA_ARGS__)
#define Dbg_print(...) Serial.print(__VA_ARGS__)
#define Dbg_println(...) Serial.println(__VA_ARGS__)
#define Dbg_printf(...) Serial.printf(__VA_ARGS__); Serial.println()
#else
#define Dbg_begin(...)
#define Dbg_print(...)
#define Dbg_println(...)
#define Dbg_printf(...)
#endif

#include "ESPTelloCLI.h"

ESPTelloCLI::ESPTelloCLI()
{
    Connected = false;
}

ESPTelloCLI::~ESPTelloCLI()
{
}

void ESPTelloCLI::begin()
{
  this->Telemetry = false;
  if (this->Command.connect(IPAddress(192, 168, 10, 1), COMMAND_PORT)) {
    this->Connected = true;
    Dbg_println(F("UDP Command connected"));
    this->Command.onPacket([](void *thisobj, AsyncUDPPacket packet) {
        ESPTelloCLI *instance = (ESPTelloCLI *)thisobj;
        Dbg_print(F("Command UDP Packet Type: "));
        Dbg_print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
        Dbg_print(F(", From: "));
        Dbg_print(packet.remoteIP());
        Dbg_print(':');
        Dbg_print(packet.remotePort());
        Dbg_print(F(", To: "));
        Dbg_print(packet.localIP());
        Dbg_print(':');
        Dbg_print(packet.localPort());
        Dbg_print(F(", Length: "));
        Dbg_print(packet.length());
        Serial.write(packet.data(), packet.length());
        }, this);
  }
  else {
    Dbg_println(F("Command UDP socket connect failed!"));
  }

  if (this->State.listen(STATE_PORT)) {
    Dbg_println(F("UDP State listening"));
    this->State.onPacket([](void *thisobj, AsyncUDPPacket packet) {
        ESPTelloCLI *instance = (ESPTelloCLI *)thisobj;
        Dbg_print(F("State UDP Packet Type: "));
        Dbg_print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
        Dbg_print(F(", From: "));
        Dbg_print(packet.remoteIP());
        Dbg_print(':');
        Dbg_print(packet.remotePort());
        Dbg_print(F(", To: "));
        Dbg_print(packet.localIP());
        Dbg_print(':');
        Dbg_print(packet.localPort());
        Dbg_print(F(", Length: "));
        Dbg_print(packet.length());
        // The following is how the SDK doc describes the Tello state data.
        // “pitch:%d;roll:%d;yaw:%d;vgx:%d;vgy%d;vgz:%d;templ:%d;temph:%d;tof:%d;h:%d;bat:%d;baro:%.2f; time:%d;agx:%.2f;agy:%.2f;agz:%.2f;\r\n”
        // The following is data from a Tello.
        // pitch:1;roll:-13;yaw:-27;vgx:0;vgy:0;vgz:0;templ:54;temph:56;tof:10;h:0;bat:100;baro:185.77;time:0;agx:60.00;agy:119.00;agz:-936.00;
        if (instance->Telemetry) Serial.write(packet.data(), packet.length());
        }, this);
  }
  else {
    Dbg_println(F("State UDP socket listen failed!"));
  }
}

void ESPTelloCLI::end()
{
  this->Command.close();
  this->State.close();
  this->Connected = false;
}
