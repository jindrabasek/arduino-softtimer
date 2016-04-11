/*
 * LongDelayRun.cpp
 *
 *  Created on: 28. 3. 2016
 *      Author: jindra
 */

#include "LongDelayRun.h"
#include "SoftTimer.h"

LongDelayRun::LongDelayRun(unsigned long delayHours, unsigned long delayMs,
                           Task* followedBy) :
        LongTask(delayHours, delayMs),
        followedBy(followedBy) {
}

void LongDelayRun::run() {
    // -- Remove me from Timer Manager.
    remove();

    SoftTimer::instance().add(followedBy);
    followedBy->startAtEarliestOportunity();
    followedBy->setEnabled(true);
}
