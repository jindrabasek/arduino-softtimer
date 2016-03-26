/**
 * File: SoftPwmTask.cpp
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

#include "SoftTimer.h"
#include "SoftPwmTask.h"

SoftPwmTask::SoftPwmTask(int pin) :
		Task(30), outPin(pin), value(0), counter(0), upperLimit(255),
		bitMask(digitalPinToBitMask(pin)), portRegister(
				portOutputRegister(digitalPinToPort(pin))) {
	pinMode(outPin, OUTPUT);
}

void SoftPwmTask::analogWrite(byte value) {
	this->value = value;
}

void SoftPwmTask::off() {
	*this->portRegister &= ~this->bitMask;
}

void SoftPwmTask::run() {
	if (this->counter == this->upperLimit) {
		// -- Reached the upper limit.
		if (this->value != 0) {
			// -- Set to HIGH.
			*this->portRegister |= this->bitMask;
		}
		this->counter = 0;
	} else {
		if (this->counter >= this->value) {
			// -- Reached the value level.
			// -- Set to LOW.
			*this->portRegister &= ~this->bitMask;
		}
		this->counter++;
	}
}
