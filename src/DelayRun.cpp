/**
 * File: DelayRun.cpp
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

#include "Arduino.h"
#include "DelayRun.h"
#include <Task.h>

#define STATE_STARTING 0
#define STATE_ON_DELAY 1

DelayRun::DelayRun(unsigned long delayMs, Task* followedBy)
    : Task(delayMs), followedBy(followedBy) {
}

void DelayRun::run() {
    // -- Remove me from Timer Manager.
    remove();

    SoftTimer::instance().add(followedBy);
    followedBy->startAtEarliestOportunity();
    followedBy->setEnabled(true);
}
