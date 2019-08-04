/*
 * Buttons.h
 *
 *  Created on: Aug 2, 2019
 *      Author: timm
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include "Peripheral.h"

class Buttons : public Peripheral
{
protected:
	void initialize();
	void shutdown();
};



#endif /* INC_BUTTONS_H_ */
