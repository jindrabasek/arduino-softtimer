/**
 * File: SoftTimer.cpp
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

#include <Arduino.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <SoftTimer.h>
#include <Task.h>

#include <defines.h>

#ifdef USE_CONCURENCY
#include <SingleThreadPool.h>
#include <Thread.h>
#endif

// Jindra: do it manually
/*void loop() {
 SoftTimer.run();
 }*/

inline SoftTimerT::SoftTimerT() :
        tasks(NULL),
        lastTask(NULL) {
}

/**
 * Register a task in the timer manager.
 */
void SoftTimerT::add(Task* task) {

    // -- A task should be registered only once.
    task->remove();

    if (this->tasks == NULL) {

        // -- This is the first task being registered.
        this->tasks = task;
        this->lastTask = task;
        task->prevToThisTask = &(this->tasks);

    } else {
        // -- Last task found, let's add this task to the end of the chain.
        this->lastTask->nextTask = task;
        task->prevToThisTask = &(this->lastTask->nextTask);
        this->lastTask = task;
    }

    task->lastCallTimeMicros = micros();
    task->nextTask = NULL;
}

/**
 * Walk through the chain looking for task to call.
 */
void SoftTimerT::run(void (*loggingCallback)(Task * executedTask)) {
    Task* task = this->tasks;
    // -- (If this->_tasks is NULL, than nothing is registered.)
    while (task != NULL) {
        if (task->isEnabled() && !task->isRunning()) {
            if (task->test()) {
#ifdef USE_CONCURENCY
                if (task->threadPool != NULL) {
                    Thread * thread =
                            task->threadPool->aquireThreadNonBlocking();
                    if (thread == NULL) {
                        // cannot start, there is no thread available
                        // try again at earliest opportunity
                        task->startAtEarliestOportunity();
                    } else {
                        if (loggingCallback != NULL) {
                            loggingCallback(task);
                        }
                        task->setRunning(true);
                        thread->setRunnable(task);
                        thread->enable();
                    }
                } else {
#endif
                    if (loggingCallback != NULL) {
                        loggingCallback(task);
                    }
                    task->setRunning(true);
                    task->markJustCalled();
                    task->run();
                    task->setRunning(false);
#ifdef USE_CONCURENCY
                }
#endif
            }
        }

        task = task->nextTask;
        yield();
    }
}

SoftTimerT SoftTimer;

