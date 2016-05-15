/*
 * ButtonHandler.h
 *
 *  Created on: 14. 1. 2016
 *      Author: jindra
 */

#ifndef LIBRARIES_SOFTTIMER_SRC_BUTTONHANDLER_H_
#define LIBRARIES_SOFTTIMER_SRC_BUTTONHANDLER_H_

// Do not define virtual destructor on purpose - class
// and its children is not expected to need destructors,
// it saves a lot of SRAM otherwise occupied by VTABLE
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"

class ButtonHandler {
public:
    ButtonHandler() {
    }

    virtual void onPressed() {
    }

    virtual void onReleased(unsigned long pressTimespan){
    }

    static ButtonHandler voidButtonHandler;
};

#pragma GCC diagnostic pop

#endif /* LIBRARIES_SOFTTIMER_SRC_BUTTONHANDLER_H_ */
