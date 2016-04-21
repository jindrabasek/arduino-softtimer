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
#include <stddef.h>
#include <SoftTimer.h>
#include <Task.h>

// Jindra: do it manually
/*void loop() {
 SoftTimer.run();
 }*/

inline SoftTimer::SoftTimer() :
        tasks(NULL),
        lastTask(NULL) {
}

SoftTimer& SoftTimer::instance() {
    static SoftTimer instance;
    return instance;
}

/**
 * Register a task in the timer manager.
 */
void SoftTimer::add(Task* task) {

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
void SoftTimer::run() {
    Task* task = this->tasks;
    // -- (If this->_tasks is NULL, than nothing is registered.)
    while (task != NULL) {
        if (task->isEnabled() && !task->running) {
            if (task->test()){
                task->running = true;
                if (task->threadPool != NULL) {
                    Thread * thread = task->threadPool->aquireThread();
                    thread->setRunnable(task);
                    thread->enable();
                } else {
                    task->markJustCalled();
                    task->run();
                    task->running = false;
                }
            }
        }
        task = task->nextTask;
        yield();
    }
}

