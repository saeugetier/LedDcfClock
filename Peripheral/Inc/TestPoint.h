/*
 * TestPoint.h
 *
 *  Created on: Aug 18, 2019
 *      Author: timm
 */

#ifndef INC_TESTPOINT_H_
#define INC_TESTPOINT_H_

#include "Peripheral.h"

class TestPoint : public Peripheral
{
public:
	TestPoint();
	void setOutput(bool high);
protected:
	virtual void initialize();
	virtual void shutdown();
};



#endif /* INC_TESTPOINT_H_ */
