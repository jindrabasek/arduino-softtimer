/*
 * ButtonHandler.h
 *
 *  Created on: 14. 1. 2016
 *      Author: jindra
 */

#ifndef LIBRARIES_SOFTTIMER_SRC_BUTTONHANDLER_H_
#define LIBRARIES_SOFTTIMER_SRC_BUTTONHANDLER_H_

class ButtonHandler {
public:
    ButtonHandler();
    virtual ~ButtonHandler();

    virtual void onPressed();
    virtual void onReleased(unsigned long pressTimespan);

    static ButtonHandler & voidButtonHandler();
};

#endif /* LIBRARIES_SOFTTIMER_SRC_BUTTONHANDLER_H_ */
