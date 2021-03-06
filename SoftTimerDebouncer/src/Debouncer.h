/**
 * File: Debouncer.h
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

#ifndef DEBOUNDER_H
#define DEBOUNDER_H

#include <ButtonHandler.h>
#include <PciListener.h>
#include <stdbool.h>
#include <stdint.h>
#include <Task.h>

#define MODE_OPEN_ON_PUSH HIGH
#define MODE_CLOSE_ON_PUSH LOW

#define STATE_OFF 0
#define STATE_OFFON_BOUNCING 1
#define STATE_ON 2
#define STATE_ONOFF_BOUNCING 3

/** We say that 50 milliseconds are enough for the signal to stabilize. */
#define DEFAULT_DEBOUNCE_DELAY_MILIS 50

// Do not define virtual destructor on purpose - class
// and its children is not expected to need destructors,
// it saves a lot of SRAM otherwise occupied by VTABLE
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"

class Debouncer : public PciListener, public Task {
public:
    /**
     * Create a debouncing task with the following parameters.
     *  pin - Checking pin for input.
     *  pushMode - CLOSE_ON_PUSH / OPEN_ON_PUSH  - Your button are normally wired to be NO (Normally Openned), so USE CLOSE_ON_PUSH.
     *   But sometimes it is NC (Normally Closed), in this case use OPEN_ON_PUSH.
     *  onPressed() - A callback function pointer. This function is called when the bouncing button is really pushed. (Optional,
     *   pass NULL, if you do not want to use this feature.)
     *  onReleased(pressTimespan) - A callback function pointer. This function is called when the bouncing button is really
     *   released. (Optional, pass NULL, if you do not want to use this feature.)
     *   The callback receives the pressTimespan parameter that is the time in milliseconds the button was hold down before
     *   it was released.
     */
    Debouncer(uint8_t pin, uint8_t pushMode, ButtonHandler * handler,
              bool pullUp = false);

    /**
     * Please call this function on interrupt.
     */
    virtual void pciHandleInterrupt(uint8_t vect);

    void setHandler(ButtonHandler * handler) {
        this->handler = handler;
    }

    ButtonHandler * getHandler() {
        return handler;
    }

protected:

    virtual void run();

private:
    uint8_t onLevel;
    volatile uint8_t state; // 0=off, 1=bouncing, 2=pressing
    unsigned long pressStart;
    ButtonHandler * volatile handler;
};

#pragma GCC diagnostic pop

#endif

