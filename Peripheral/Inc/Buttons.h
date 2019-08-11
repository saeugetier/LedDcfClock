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

class Button : public InterruptPeripheral
{
public:
	Button(uint32_t pin);

	bool getButtonPressed();

	void handleInterrupt();

	void setPinsToWakeup();
	void setPinsToInterrupt();
	bool isWakeupFlagSet();
	void clearWakeupFlags();
protected:
	uint32_t mPin;
	void initialize();
	void shutdown();
};

class PushButton : public Button
{
public:
	PushButton() : Button(0) {}

};

class Settings1Button : public Button
{
public:
	Settings1Button() : Button(2) {	}
};

class Settings2Button : public Button
{
public:
	Settings2Button() : Button(4) {}
};

#endif /* INC_BUTTONS_H_ */
