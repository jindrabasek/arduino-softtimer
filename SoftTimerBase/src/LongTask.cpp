/*
 * LongTask.cpp
 *
 *  Created on: 17. 3. 2016
 *      Author: jindra
 */

#include <Arduino.h>
#include <LongTask.h>
#include <stdbool.h>
#include <Task.h>

LongTask::LongTask(unsigned long periodHours, unsigned long periodUs,
                   bool enabled) :
        Task(periodHours == 0 ? roundPeriodUs(periodUs) : ONE_HOUR_US, enabled),
        inLastPeriod(periodHours == 0),
        periodHours(periodHours),
        endPeriodUs(roundPeriodUs(periodUs)),
        hoursPassed(0) {
}

void LongTask::markJustCalled() {
    Task::markJustCalled();
    hoursPassed = 0;
    Task::setPeriodUs(periodHours == 0 ? endPeriodUs : ONE_HOUR_US);
    inLastPeriod = periodHours == 0;
}

void LongTask::setPeriodUs(unsigned long periodUs) {
    periodHours = 0;
    unsigned long period = roundPeriodUs(periodUs);
    Task::setPeriodUs(period);
    endPeriodUs = period;
    inLastPeriod = true;
}

void LongTask::setPeriod(unsigned long periodHours, unsigned long periodUs) {
    this->periodHours = periodHours;
    unsigned long period = roundPeriodUs(periodUs);
    Task::setPeriodUs(periodHours == 0 ? period : ONE_HOUR_US);
    endPeriodUs = period;
    inLastPeriod = periodHours == 0;
}

bool LongTask::test() {
    if (isStartAtEarliest()) {
        setStartAtEarliest(false);
        return true;
    } else {
        unsigned long now = micros();
        unsigned long calc = getLastCallTimeMicros() + getPeriodUs();
        if (((now >= calc) && (calc >= getLastCallTimeMicros() // -- Nothing was overflown.
        || getLastCallTimeMicros() > now // -- Both timer and interval-end overflows
        ))
                || (now < getLastCallTimeMicros()
                        && getLastCallTimeMicros() <= calc)) // -- timer overflows, but interval-end does not
                {
            setLastCallTimeMicros(now);
            if (periodHours == 0 || hoursPassed >= periodHours) {
                if (inLastPeriod) {
                    return true;
                } else {
                    inLastPeriod = true;
                    if (endPeriodUs > 0) {
                        Task::setPeriodUs(
                                periodHours == 0 ? endPeriodUs : ONE_HOUR_US);
                    }
                }
            } else {
                hoursPassed++;
            }
        }
    }
    return false;
}

inline unsigned long LongTask::roundPeriodUs(unsigned long periodUs) {
    return periodUs >= ONE_HOUR_US ? ONE_HOUR_US - 1 : periodUs;
}
