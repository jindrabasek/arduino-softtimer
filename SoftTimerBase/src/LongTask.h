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

    LongTask(unsigned int periodHours, unsigned int periodUs,
             bool enabled = true, uint8_t taskId = 255);

    virtual void markJustCalled();

    virtual void setPeriodUs(unsigned long periodUs);

    virtual void setPeriod(unsigned int periodHours, unsigned long periodUs);

protected:

    virtual bool test();

    static const uint8_t IN_LAST_PERIOD_FLAG_BIT = Task::next_free_flag_bit;
    static const uint8_t next_free_flag_bit = IN_LAST_PERIOD_FLAG_BIT + 1;

private:

    bool isInLastPeriod() const {
            return bitRead(flags, IN_LAST_PERIOD_FLAG_BIT);
    }

    void setInLastPeriod(bool inLastPeriod) {
        bitWrite(flags, IN_LAST_PERIOD_FLAG_BIT, inLastPeriod);
    }

    static unsigned long roundPeriodUs(unsigned long periodUs);

    static const unsigned long ONE_HOUR_US = 3600000000;

    unsigned int periodHours;
    unsigned long endPeriodUs;
    volatile unsigned int hoursPassed;

};

#endif /* LIBRARIES_SOFTTIMER_SRC_LONGTASK_H_ */
