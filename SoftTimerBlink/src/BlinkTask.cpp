/**
 * File: BlinkTask.cpp
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
#include <BlinkTask.h>
#include <SoftTimer.h>

BlinkTask::BlinkTask(byte outPin, unsigned long onOffUs) :
        BlinkTask(outPin, onOffUs, onOffUs, 0, 0) {
}

BlinkTask::BlinkTask(byte outPin, unsigned long onUs, unsigned long offUs) :
        BlinkTask(outPin, onUs, offUs, 0, 0) {
}

BlinkTask::BlinkTask(byte outPin, unsigned long onUs, unsigned long offUs,
                     byte count) :
        BlinkTask(outPin, onUs, offUs, count, 0) {
}

BlinkTask::BlinkTask(byte outPin, unsigned long onUs, unsigned long offUs,
                     byte count, unsigned long delayUs) :
        Task(onUs),
        _counter(0),
        _state(STATE_OFF),
        onUs(onUs),
        offUs(offUs),
        count(count),
        delayUs(delayUs),
        _bitMask(digitalPinToBitMask(outPin)),
        _portRegister(portOutputRegister(digitalPinToPort(outPin))),
        onLevel(HIGH) {
    pinMode(outPin, OUTPUT);
}

void BlinkTask::start() {
    this->_state = STATE_OFF;
    this->_counter = 0;
    this->setPeriodUs(0);
    SoftTimer.add(this);
}

void BlinkTask::start(byte count) {
    this->count = count;
    this->start();
    this->delayUs = 0;
}

void BlinkTask::stop() {
    remove();
}

void BlinkTask::run() {
    if (this->_state == STATE_ON) {
        // -- Turn off.
        if (this->onLevel == HIGH) {
            *this->_portRegister &= ~this->_bitMask;
        } else {
            *this->_portRegister |= this->_bitMask;
        }
        this->_counter += 1;
        this->_state = STATE_OFF;
        this->setPeriodUs(this->offUs);
    } else {
        // -- state == OFF or WAIT
        // -- Turn on.
        if (this->onLevel == HIGH) {
            *this->_portRegister |= this->_bitMask;
        } else {
            *this->_portRegister &= ~this->_bitMask;
        }
        this->_state = STATE_ON;
        this->setPeriodUs(this->onUs);
    }
    if ((this->count > 0) && (this->_counter >= this->count)) {
        // -- Count was defined, and we reached it.
        this->_counter = 0;

        if (this->delayUs > 0) {
            // -- delay was defined.
            this->_state = STATE_WAIT;
            this->setPeriodUs(this->delayUs);
        } else {
            this->stop();
        }
    }
}
