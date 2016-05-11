/**
 * File: Dimmer.cpp
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
#include <Dimmer.h>
#include <stdbool.h>
#include <SoftPwmTask.h>
#include <SoftTimer.h>

Dimmer::Dimmer(SoftPwmTask* pwm, int frequencyMs) :
        Task(10) {
    this->pwm = pwm;
    this->direction = DIMMER_DIRECTION_HIGH;
    this->value = 0;
    this->pwm->analogWrite((byte) this->value);
    this->stepCount = DEFAULT_STEP_COUNT;

    this->setFrequency(frequencyMs);
}

void Dimmer::startPulsate() {
    this->stopOnLimit = false;
    SoftTimer.add(this->pwm);
    SoftTimer.add(this);
}

void Dimmer::hold() {
    remove();
}

void Dimmer::off() {
    this->hold();
    this->pwm->off();
    remove();
    this->pwm->remove();
}

inline void Dimmer::revertDirection() {
    this->direction *= -1;
}

void Dimmer::setFrequency(int frequencyMs) {
    this->stepLevel = (float) this->pwm->getUpperLimit()
            / (float) this->stepCount;
    setPeriodUs((float) frequencyMs * 500.0 / (float) this->stepCount);

}

inline byte Dimmer::getUpperLimit() {
    return this->pwm->getUpperLimit();
}

void Dimmer::run() {

    boolean isOnLimit = false;

    this->value += this->direction * this->stepLevel;
    if ((this->direction < 0) && (this->value < 0)) {
        this->value = 0;
        this->direction *= -1; // -- next time go in the other direction
        isOnLimit = true;
    } else if ((this->direction > 0)
            && (this->value > this->pwm->getUpperLimit())) {
        this->value = this->pwm->getUpperLimit();
        this->direction *= -1; // -- next time go in the other direction
        isOnLimit = true;
    }

    this->pwm->analogWrite((byte) this->value);

    if (isOnLimit && this->stopOnLimit) {
        remove();
    }

}

