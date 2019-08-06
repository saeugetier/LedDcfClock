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

class Buttons : public InterruptPeripheral
{
public:
	enum ButtonSource : uint32_t
	{
		NONE = 0x00,
		PUSHBUTTON = 0x01,
		SETTINGSBUTTON1 = 0x02,
		SETTINGSBUTTON2 = 0x04
	};
	ButtonSource getButtonsPressed();

	void handleInterrupt();
protected:
	void initialize();
	void shutdown();

	void setPinsToWakeup();
	void setPinsToInterrupt();
	bool isWakeupFlagSet();
	void clearWakeupFlags();
};



#endif /* INC_BUTTONS_H_ */
