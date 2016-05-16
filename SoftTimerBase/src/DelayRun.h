/**
 * File: DelayRun.h
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

#ifndef DELAYRUN_H
#define DELAYRUN_H

#include <Schedulable.h>
#include <Task.h>

/**
 * Run a callback after a specified delay. The task will stop when finished. Also chains tasks.
 */
class DelayRun : public Schedulable {
public:
    /**
     * Setup a delayed task.
     *  followedBy - If the followedBy was specified, than it will be started when this was finished.
     *   Starting the followedBy can be denied by returning FALSE in the callback.
     */
    DelayRun(Task* followedBy);
    virtual void run(Task* task);

private:
    /** The task should be started after this one was finished. */
    Task* followedBy;
};

#endif

