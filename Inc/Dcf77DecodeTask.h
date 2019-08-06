/*
 * Dcf77DecodeTask.h
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#ifndef INC_DCF77DECODETASK_H_
#define INC_DCF77DECODETASK_H_

#include "Task.h"

class Dcf77DecodeTask : public Task
{
public:
	virtual void run();
	virtual void handleEvent(EventType::type event);
protected:
	virtual void taskModeChanged(TaskMode mode);
};

#endif /* INC_DCF77DECODETASK_H_ */
