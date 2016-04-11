/*
 * LongDelayRun.h
 *
 *  Created on: 28. 3. 2016
 *      Author: jindra
 */

#ifndef LIBRARIES_SOFTTIMER_SRC_LONGDELAYRUN_H_
#define LIBRARIES_SOFTTIMER_SRC_LONGDELAYRUN_H_

#include <LongTask.h>

class LongDelayRun : public LongTask {
public:
    LongDelayRun(unsigned long delayHours, unsigned long delayMs,
                 Task* followedBy);

protected:
    virtual void run();

private:
    /** The task should be started after this one was finished. */
    Task* followedBy;
};

#endif /* LIBRARIES_SOFTTIMER_SRC_LONGDELAYRUN_H_ */
