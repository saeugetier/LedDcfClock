/*
 * SystemManager.h
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#ifndef INC_SYSTEMMANAGER_H_
#define INC_SYSTEMMANAGER_H_

#include "TaskManager.h"
#include "Peripheral.h"
//Peripheral
#include "SystemTick.h"
#include "LedPowerEnable.h"

template<class periph>
class PeripheralReference: protected PeripheralInitializer
{
public:
	PeripheralReference(periph& peripheral);
	PeripheralReference(PeripheralReference& copy);
	periph& getInstance() const;
	void init();
	void shutdown();
	virtual ~PeripheralReference();
protected:
	bool mInitializationRequested = false;
	static uint8_t mInitializationCounter;
	periph& mReference;
};

class SystemManager
{
public:
	SystemManager();
	void runTasks();
	PeripheralReference<SystemTick> getSystemTickReference();
protected:
	TaskManager mTaskManager;
	SystemTick mSystemTick;
};

#endif /* INC_SYSTEMMANAGER_H_ */
