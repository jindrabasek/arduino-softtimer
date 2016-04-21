/**
 * File: Task.cpp
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
#include <stdbool.h>
#include <stddef.h>
#include <Task.h>

Task::Task(unsigned long periodUs, bool enabled) :
        periodUs(periodUs),
        lastCallTimeMicros(0),
        nextTask(NULL),
        prevToThisTask(
        NULL),
        threadPool(NULL),
        enabled(enabled),
        startAtEarliest(false),
        running(false){
}

void Task::startAtEarliestOportunity() {
    this->startAtEarliest = true;
}

void Task::markJustCalled() {
    this->lastCallTimeMicros = micros();
}

void Task::setPeriodUs(unsigned long periodUs) {
    this->periodUs = periodUs;
}

void Task::remove() {
    if (prevToThisTask != NULL) {
        *prevToThisTask = nextTask;
        prevToThisTask = NULL;
        nextTask = NULL;
    }
}

/**
 * Test a task and call the callback if its period was passed since last call.
 */
bool Task::test() {
    unsigned long now = micros();
    unsigned long calc = lastCallTimeMicros + periodUs;
    if (startAtEarliest || ((now >= calc) && (calc >= lastCallTimeMicros // -- Nothing was overflown.
            || lastCallTimeMicros > now // -- Both timer and interval-end overflows
            )) || (now < lastCallTimeMicros && lastCallTimeMicros <= calc)) // -- timer overflows, but interval-end does not
            {
        startAtEarliest = false;
        return true;
    }
    return false;
}

Task::~Task() {
    remove();
}
