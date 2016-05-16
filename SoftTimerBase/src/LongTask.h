/*
 * LongTask.h
 *
 *  Created on: 17. 3. 2016
 *      Author: jindra
 */

#ifndef LIBRARIES_SOFTTIMER_SRC_LONGTASK_H_
#define LIBRARIES_SOFTTIMER_SRC_LONGTASK_H_

#include <Task.h>

class LongTask : public Task {
public:

    LongTask(Schedulable * toRun, unsigned long periodHours,
             unsigned long periodUs,
             bool enabled = true);

    virtual void markJustCalled();

    virtual void setPeriodUs(unsigned long periodUs);

    void setPeriod(unsigned long periodHours, unsigned long periodUs);

protected:

    virtual bool test();

private:
    static unsigned long roundPeriodUs(unsigned long periodUs);

    static const unsigned long ONE_HOUR_US = 3600000000;

    bool inLastPeriod;

    unsigned long periodHours;
    unsigned long endPeriodUs;
    volatile unsigned long hoursPassed;

};

#endif /* LIBRARIES_SOFTTIMER_SRC_LONGTASK_H_ */
