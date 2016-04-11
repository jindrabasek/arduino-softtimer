/**
 * File: TonePlayer.cpp
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

#include "TonePlayer.h"
#include "SoftTimer.h"

const float TonePlayer::trot = 1.05946309435929; // -- The twelfth root of two
const float TonePlayer::A440 = 440.0; // -- A440 (pitch standard)

TonePlayer::TonePlayer(int pin, unsigned long baseLength) :
        Task(0),
        outPin(pin),
        baseLength(baseLength),
        playing(false),
        pos(0) {
}

void TonePlayer::play(String tones) {
    this->tones = tones;

    remove();
    this->setPeriodUs(0);
    this->playing = true;
    this->pos = 0;
    SoftTimer::instance().add(this);
}

void TonePlayer::run() {
    // -- play a small silence after each tone
    if (this->playing) {
        this->playing = false;
        noTone(this->outPin);
        this->setPeriodUs(this->baseLength / 20);
        return;
    }

    // -- finished
    if (this->pos >= this->tones.length()) {
        remove();
        noTone(this->outPin);
        return;
    }

    // -- calculate length
    char cLength = this->tones[this->pos + 1];
    this->setPeriodUs((unsigned long) (cLength - '0') * this->baseLength);

    // -- calculate tone
    char cPitch = this->tones[this->pos];
    if (cPitch == '_') {
        // -- add silence
        noTone(this->outPin);
    } else {
        float val = A440;
        int tune = (int) (cPitch - 'j'); // -- 'j' character means A note
        if (tune < -9) {
            tune += 6; // -- Skip characters between character capital 'Z' and 'a'.
        }
        if (tune > 0) {
            while (tune != 0) {
                val *= trot;
                --tune;
            }
        } else {
            while (tune != 0) {
                val /= trot;
                ++tune;
            }
        }

        // -- play tone
        tone(this->outPin, static_cast<int>(val));
    }

    this->playing = true;
    this->pos += 2;
}

