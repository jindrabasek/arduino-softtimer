/**
 * File: Rotary.h
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

#ifndef ROTARY_H
#define ROTARY_H

#include <SoftTimer.h>
#include <PciListener.h>
#include <Arduino.h>
#include <PciListenerImp2.h>
#include <IPciChangeHandler.h>

#define DIRECTION_CW 1
#define DIRECTION_CCW -1

class Rotary : public Task, public IPciChangeHandler {
public:
    Rotary(byte pinA, byte pinB,
           void (*onRotation)(short direction, Rotary* rotary),
           bool pullUp = false);

    /**
     * Please call this function on interrupt.
     */
    virtual void pciHandleChange(byte changedTo, PciListenerImp2* listener);

protected:
    virtual void run();

private:
    PciListenerImp2 listenerA;
    PciListenerImp2 listenerB;
    volatile int stateCW;
    volatile int stateCCW;
    void (*onRotation)(short direction, Rotary* rotary);
};

#endif

