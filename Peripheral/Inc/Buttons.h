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
	enum ButtonPin : uint32_t
	{
		PUSH_BUTTON_PIN,
		SETTINGS1_BUTTON_PIN,
		SETTINGS2_BUTTON_PIN
	};

	Button(ButtonPin pin);

	bool getButtonPressed();

	void handleInterrupt();

	void disableInterrupt();
	void setPinToInterrupt();
	bool isWakeupFlagSet();
	void clearWakeupFlag();
protected:
	ButtonPin mPin;
	void initialize();
	void shutdown();
};

class PushButton : public Button
{
public:
	PushButton() : Button(PUSH_BUTTON_PIN) {}

};

class Settings1Button : public Button
{
public:
	Settings1Button() : Button(SETTINGS1_BUTTON_PIN) {	}
};

class Settings2Button : public Button
{
public:
	Settings2Button() : Button(SETTINGS2_BUTTON_PIN) {}
};

#endif /* INC_BUTTONS_H_ */
