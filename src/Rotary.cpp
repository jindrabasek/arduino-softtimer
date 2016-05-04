/**
 * File: Rotary.cpp
 * Description:
 * SoftTimer library is a lightweight but effective event based timeshare solution for Arduino.
 *
 * Author: Balazs Kelemen
 * Contact: prampec+arduino@gmail.com
 * Copyright: 2015 Balazs Kelemen
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
#include <PciManager.h>
#include <Rotary.h>
#include <stdbool.h>
#include <SoftTimer.h>
#include <Task.h>

#define EVENT_NOTIFIED 0
#define EVENT_OCCURRED 1
#define EVENT_CLEARED  2

Rotary::Rotary(byte pinA, byte pinB,
               void (*onRotation)(short direction, Rotary* rotary), bool pullUp) :
        Task(0, false),
        listenerA(pinA, this, pullUp),
        listenerB(pinB, this, pullUp),
        stateCW(EVENT_NOTIFIED),
        stateCCW(EVENT_NOTIFIED),
        onRotation(onRotation) {

    PciManager.registerListener(&this->listenerA);
    PciManager.registerListener(&this->listenerB);

    SoftTimer.add(this);
}

void Rotary::pciHandleChange(byte changedTo, PciListenerImp2* listener) {
    if (listener == &this->listenerB) {
        // -- pinB changes
        if ((changedTo == HIGH) && (this->listenerA.lastVal == LOW)) {
            if (this->stateCW == EVENT_CLEARED) {
                this->stateCW = EVENT_OCCURRED;
                this->setEnabled(true);
            }
        } else if ((changedTo == LOW) && (this->listenerA.lastVal == HIGH)) {
            this->stateCW = EVENT_CLEARED;
        }
    }
    if (listener == &this->listenerA) {
        // -- pinA changes
        if ((changedTo == HIGH) && (this->listenerB.lastVal == LOW)) {
            if (this->stateCCW == EVENT_CLEARED) {
                this->stateCCW = EVENT_OCCURRED;
                this->setEnabled(true);
            }
        } else if ((changedTo == LOW) && (this->listenerB.lastVal == HIGH)) {
            this->stateCCW = EVENT_CLEARED;
        }
    }
}

void Rotary::run() {

    if (this->stateCW == EVENT_OCCURRED) {
        this->onRotation(DIRECTION_CW, this);
        this->stateCW = EVENT_NOTIFIED;
    }
    if (this->stateCCW == EVENT_OCCURRED) {
        this->onRotation(DIRECTION_CCW, this);
        this->stateCCW = EVENT_NOTIFIED;
    }
    this->setEnabled(false);
}

