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
#ifndef __ESPTELLO_H__
#define __ESPTELLO_H__
#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESPAsyncUDP.h>
#else
#include <WiFi.h>
#include <AsyncUDP.h>
#endif

class ESPTelloCLI : public Stream
{
    public:
        ESPTelloCLI();
        ~ESPTelloCLI();

        void begin();
        void end();

        virtual int available(void) { return 0; };
        virtual int peek(void) { return -1; };
        virtual int read(void) { return -1; };
        virtual int availableForWrite(void) { return 1024; };
        virtual void flush (void) {};
	    virtual size_t write(uint8_t byt) {
            if (Connected) {
                return Command.write(&byt, 1);
            }
            else {
                return 0;
            }
        };
	    virtual size_t write(const uint8_t* buf, size_t len) {
            if (Connected) {
                return Command.write(buf, len);
            }
            else {
                return 0;
            }
        };
	    using Print::write; // pull in write(str) and write(buf, size) from Print
        void setTelemetry(bool telemetry_on) {
            Telemetry = telemetry_on;
        };
	    operator bool() {
            return Connected;
        };

    private:
        bool Telemetry;
        const uint16_t COMMAND_PORT = 8889;
        const uint16_t STATE_PORT = 8890;   // Also known as telemetry port

        AsyncUDP Command;
        AsyncUDP State;

        volatile bool Connected = false;
};

#endif  /* __ESPTELLO_H__ */
