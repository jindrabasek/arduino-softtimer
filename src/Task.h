/**
 * File: Task.h
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

#ifndef TASK_H
#define TASK_H

#include <Runnable.h>
#include <stdbool.h>
#include <stddef.h>
#include <Scheduler.h>
#include <SingleThreadPool.h>

/**
 * Task is a job that should be called repeatedly,
 */
class Task : public Runnable {
public:
    virtual ~Task();

    /**
     * Construct a task with defining a period and a callback handler function.
     *  periodMs - Call the task in every X milliseconds. Do not add values greater then 4,294,967,000 which is about 71 minutes!
     *  callback - Is a static function reference, the function will be called each time. The callback function need to
     * have one argument, which is the currently running task.
     */
    Task(unsigned long periodUs, bool enabled = true);

    /**
     * Remove registration of a task in the timer manager.
     */
    void remove();

    virtual void setPeriodUs(unsigned long periodUs);

    void startAtEarliestOportunity();

    virtual void markJustCalled();

    bool isEnabled() const {
        return enabled;
    }

    void setEnabled(bool enabled) {
        this->enabled = enabled;
    }

    unsigned long getPeriodUs() const {
        return periodUs;
    }

    void setThreadPool(SingleThreadPool* threadPool) {
        this->threadPool = threadPool;
    }

    friend class SoftTimer;

protected:

    virtual void loop(){
        setStartAtEarliest(false);
        markJustCalled();
        run();
        running = false;
        Scheduler.disable();
        threadPool->releaseThread();
    }

    virtual bool test();

    /**
     * The function that will be called when the period time was passed since the lastCallTime.
     */
    virtual void run()=0;

    volatile unsigned long getLastCallTimeMicros() const {
        return lastCallTimeMicros;
    }

    void setLastCallTimeMicros(volatile unsigned long lastCallTimeMicros) {
        this->lastCallTimeMicros = lastCallTimeMicros;
    }

    volatile bool isStartAtEarliest() const {
        return startAtEarliest;
    }

    void setStartAtEarliest(volatile bool startAtEarliest) {
        this->startAtEarliest = startAtEarliest;
    }

private:

    /**
     * The timeslot in milliseconds the handler should be called. If the value is near 1 the handler will be called in every loop.
     */
    volatile unsigned long periodUs;

    /**
     * The last call (start) time of the task. You can reset the task by setting this value to micros().
     */
    volatile unsigned long lastCallTimeMicros;

    /**
     * This member is for internal use only. Do not change!
     */
    Task * volatile nextTask;

    /**
     * This member is for internal use only. Do not change!
     */
    Task* volatile * volatile prevToThisTask;

    SingleThreadPool * volatile threadPool;

    volatile bool enabled;

    volatile bool startAtEarliest;

    volatile bool running;

};

#endif
