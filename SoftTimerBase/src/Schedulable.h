/*
 * Schedulable.h
 *
 *  Created on: 16. 5. 2016
 *      Author: jindra
 */

#ifndef LIBRARIES_SOFTTIMERBASE_SRC_SCHEDULABLE_H_
#define LIBRARIES_SOFTTIMERBASE_SRC_SCHEDULABLE_H_

#include <Task.h>

class Schedulable {
public:
    virtual void run(Task * task) =0;
};

#endif /* LIBRARIES_SOFTTIMERBASE_SRC_SCHEDULABLE_H_ */
