/*
 * Buttons.h
 *
 *  Created on: Aug 2, 2019
 *      Author: timm
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include "stdint.h"
#include "Peripheral.h"
#include "Callback.h"

class Button
{
public:
	virtual bool getButtonPressed() = 0;
	virtual bool isWakeupFlagSet() = 0;
	virtual void clearWakeupFlag() = 0;
};

class PushButton : public Button, public InterruptPeripheral<PushButton>
{
public:
	PushButton();
	bool getButtonPressed();
	bool isWakeupFlagSet();
	void clearWakeupFlag();
	void handleInterrupt();
protected:
	void initialize();
	void shutdown();
};

class Settings1Button : public Button, public InterruptPeripheral<Settings1Button>
{
public:
	Settings1Button();
	bool getButtonPressed();
	bool isWakeupFlagSet();
	void clearWakeupFlag();
	void handleInterrupt();
protected:
	void initialize();
	void shutdown();
};

class Settings2Button : public Button, public InterruptPeripheral<Settings2Button>
{
public:
	Settings2Button();

	bool getButtonPressed();
	bool isWakeupFlagSet();
	void clearWakeupFlag();
	void handleInterrupt();
protected:
	void initialize();
	void shutdown();
};

#endif /* INC_BUTTONS_H_ */
