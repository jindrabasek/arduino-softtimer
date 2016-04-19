/**
 * File: BlinkTask.h
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

#ifndef BLINKTASK_H
#define BLINKTASK_H

#include <Arduino.h>
#include <Task.h>

#define STATE_OFF  0
#define STATE_ON   1
#define STATE_WAIT 2

class BlinkTask : public Task {
public:
    /**
     * By using this constructor you will be create a perpetual blinker with even on and off time-spans.
     *  outPin - The pin to blink.
     *  onOffMs - On and off time-span of the blinking in milliseconds.
     */
    BlinkTask(byte outPin, unsigned long onOffMs);

    /**
     * Using this constructor you can create a perpetual blinker with different on and off time-spans.
     *  outPin - The pin to blink.
     *  onMs - The pin will be on for this amount of milliseconds.
     *  offMs - The pin will be off for this amount of milliseconds.
     */
    BlinkTask(byte outPin, unsigned long onUs, unsigned long offUs);

    /**
     * With this constructor you can create a blinker that stops after an amount of on states.
     *  outPin - The pin to blink.
     *  onUs - The pin will be on for this amount of milliseconds.
     *  offUs - The pin will be off for this amount of milliseconds.
     *  count - After this amount of ON state the blinker will remove itself from the Timer Manager.
     */
    BlinkTask(byte outPin, unsigned long onUs, unsigned long offUs, byte count);

    /**
     * Using this constructor you can create a perpetual blinker that has a sleeping phase after an amount of blinking.
     *  outPin - The pin to blink.
     *  onUs - The pin will be on for this amount of milliseconds.
     *  offUs - The pin will be off for this amount of milliseconds.
     *  count - The blinker will sleep some time after this amount of ON states.
     *  delayUs - After a "count" of ON states the blinker will sleep for this amount of milliseconds. When the time is passed
     *   the blinking phase will continue.
     */
    BlinkTask(byte outPin, unsigned long onUs, unsigned long offUs, byte count,
              unsigned long delayUs);

    /**
     * Register the blink Task to the Timer Manager: start blinking.
     */
    void start();

    /**
     * Register the blink Task to the Timer Manager, and start blinking of the amount of time.
     * After blinking for this amount of time a blinker will be de-registered from the Timer Manager.
     */
    void start(byte count);

    virtual void run();

    /**
     * Remove the blink Task from the Timer Manager.
     */
    void stop();

private:
    byte _counter;
    /** Can be STATE_OFF, STATE_ON, STATE_WAIT */
    byte _state;

    /** The pin will be on for this amount of microseconds. */
    unsigned long onUs;
    /** The pin will be off for this amount of microseconds. */
    unsigned long offUs;
    /** The count of ON states before the task will sleep or be stopped. See delayUs for details. */
    byte count;
    /**
     * After a "count" of ON states the blinker will sleep for this amount of microseconds.
     * Value zero (0) has a special meaning: delay is disabled.
     * If delay is disabled AND count is specified, the task will be automatically removed after the count of ON states.
     */
    unsigned long delayUs;

    uint8_t _bitMask;
    volatile uint8_t *_portRegister;

    /** Pin level for the ON state. By default the ON means HIGH, you can change this to be LOW. */
    byte onLevel;
};

#endif
