/**
 * File: FrequencyTask.cpp
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
#include "FrequencyTask.h"

FrequencyTask::FrequencyTask(int outPin, float freq) : Task(0),
    stateOn(false),
    bitMask(digitalPinToBitMask(outPin)),
    portRegister(portOutputRegister(digitalPinToPort(outPin)))
{
  pinMode(outPin, OUTPUT);
  this->setFrequency(freq);
}


void FrequencyTask::setFrequency(float freq) {
  setPeriodUs(500000.0 / freq);
}

void FrequencyTask::run()
{
  // -- Invert state.
  if(this->stateOn) {
      *this->portRegister &= ~this->bitMask;
  } else {
      *this->portRegister |= this->bitMask;
  }
  
  this->stateOn = !this->stateOn;
}

