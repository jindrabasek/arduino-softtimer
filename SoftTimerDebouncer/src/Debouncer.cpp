/**
 * File: Debouncer.cpp
 * Description:
 * SoftTimer library is a lightweight but effective event based timeshare solution for Arduino.
 *
 * Author: Balazs Kelemen
 * Contact: prampec+arduino@gmail.com
 * Copyright: 2012 Balazs Kelemen
 * Copying permission statement:
 This file is part of SoftTimer.

 SoftTimer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#include <Arduino.h>
#include <ButtonHandler.h>
#include <Debouncer.h>
#include <stdbool.h>
#include <stdint.h>
#include <SoftTimer.h>
#include <Task.h>

Debouncer::Debouncer(uint8_t pin, uint8_t pushMode, ButtonHandler * handler,
bool pullUp) :
        PciListener(pin),
        Task(NULL, DEFAULT_DEBOUNCE_DELAY_MILIS, false),
        onLevel(pushMode),
        pressStart(0),
        handler(handler) {
    if (pullUp) {
        pinMode(pin, INPUT_PULLUP);
    } else {
        pinMode(pin, INPUT);
    }
    this->state =
            digitalRead(this->pciPin) == this->onLevel ? STATE_ON : STATE_OFF;

    SoftTimer.add(this);
}

void Debouncer::pciHandleInterrupt(uint8_t vect) {
    if ((this->state == STATE_OFF) || (this->state == STATE_ON)) {
        uint8_t oppositeLevel =
                this->state == STATE_OFF ? this->onLevel : !this->onLevel;
        // -- Test pin level, probably more pins are used by this interrupt.
        volatile uint8_t val = digitalRead(this->pciPin);
        if (val == oppositeLevel) {
            if (this->state == STATE_OFF) {
                this->pressStart = millis(); // -- Save the first time to the start of this task.
            }
            // -- After pin change we have the opposite level, lets start the bouncing timespan.
            this->state += 1;
            markJustCalled();
            setEnabled(true);
        }
    }
}

void Debouncer::run() {
    if ((this->state == STATE_OFF) || (this->state == STATE_ON)) {
        setEnabled(false);
        return;
    }
    uint8_t val = digitalRead(this->pciPin);
    this->setEnabled(false);
    ;
    if (this->state == STATE_OFFON_BOUNCING) {
        if (val == this->onLevel) {

            // -- Still pressing.
            this->state = STATE_ON;

            // -- Call the callback.
            this->handler->onPressed();

        } else {

            // -- Released while bouncing.
            this->state = STATE_OFF;
        }
    } else if (this->state == STATE_ONOFF_BOUNCING) {
        if (val == !this->onLevel) {

            // -- Really released.
            this->state = STATE_OFF;

            volatile unsigned long pressTimespan = millis() - this->pressStart; // -- If millis() overflows this calculation will be still good.
            this->handler->onReleased(pressTimespan);

        } else {

            // -- Repressed while bouncing.
            this->state = STATE_ON;
        }
    }
    return;
}
