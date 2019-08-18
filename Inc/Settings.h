/*
 * Settings.h
 *
 *  Created on: Jul 2, 2019
 *      Author: timm
 */

#ifndef INC_SETTINGS_H_
#define INC_SETTINGS_H_

#include "main.h"
#include "PeripheralReference.h"
#include "SettingsStorage.h"

class Settings
{
public:
	Settings(PeripheralReference<SettingsStorage> storage);
	uint8_t getBrightness();
	uint8_t getMode();
	uint32_t getLastCalibrationTimestamp();
	void setBrightness(uint8_t brightness);
	void setMode(uint8_t mode);
	void setLastCalibrationTimestamp(uint32_t timeStamp);
protected:
	PeripheralReference<SettingsStorage> mStorage;
};


#endif /* INC_SETTINGS_H_ */
