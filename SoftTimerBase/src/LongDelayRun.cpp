/*
 * LongDelayRun.cpp
 *
 *  Created on: 28. 3. 2016
 *      Author: jindra
 */

#include <LongDelayRun.h>
#include <SoftTimer.h>
#include <Task.h>

LongDelayRun::LongDelayRun(unsigned long delayHours, unsigned long delayMs,
                           Task* followedBy) :
        LongTask(delayHours, delayMs, true, 150 + followedBy->getTaskId()),
        followedBy(followedBy) {
}

void LongDelayRun::run() {
    // -- Remove me from Timer Manager.
    remove();

    SoftTimer.add(followedBy);
    followedBy->startAtEarliestOportunity();
    followedBy->setEnabled(true);
}
