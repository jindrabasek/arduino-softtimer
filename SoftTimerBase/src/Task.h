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

#include <Arduino.h>
#include <Runnable.h>
#include <stdbool.h>
#include <stdint.h>
#include <Scheduler.h>
#include <SingleThreadPool.h>

// Do not define virtual destructor on purpose - class
// and its children is not expected to need destructors,
// it saves a lot of SRAM otherwise occupied by VTABLE
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"

/**
 * Task is a job that should be called repeatedly,
 */
class Task : public Runnable {
public:
    ~Task();

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
        return bitRead(flags, ENABLED_FLAG_BIT);
    }

    void setEnabled(bool enabled) {
        bitWrite(flags, ENABLED_FLAG_BIT, enabled);
    }

    unsigned long getPeriodUs() const {
        return periodUs;
    }

    void setThreadPool(SingleThreadPool* threadPool) {
        this->threadPool = threadPool;
    }

    friend class SoftTimerT;

protected:

    virtual void loop() {
        setStartAtEarliest(false);
        markJustCalled();
        run();
        setRunning(false);
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

    bool isStartAtEarliest() const {
        return bitRead(flags, START_AT_EARLIEST_FLAG_BIT);
    }

    void setStartAtEarliest(bool startAtEarliest) {
        bitWrite(flags, START_AT_EARLIEST_FLAG_BIT, startAtEarliest);
    }

    bool isRunning() const {
        return bitRead(flags, RUNNING_FLAG_BIT);
    }

    void setRunning(bool running) {
        bitWrite(flags, RUNNING_FLAG_BIT, running);
    }

    volatile uint8_t flags;

    static const uint8_t ENABLED_FLAG_BIT = 0;
    static const uint8_t START_AT_EARLIEST_FLAG_BIT = ENABLED_FLAG_BIT + 1;
    static const uint8_t RUNNING_FLAG_BIT = START_AT_EARLIEST_FLAG_BIT + 1;
    static const uint8_t next_free_flag_bit = RUNNING_FLAG_BIT + 1;

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
};

#pragma GCC diagnostic pop

#endif
