/*
 * SettingsStorage.h
 *
 *  Created on: Aug 18, 2019
 *      Author: timm
 */

#ifndef INC_SETTINGSSTORAGE_H_
#define INC_SETTINGSSTORAGE_H_

#include "Peripheral.h"

class SettingsStorage : public Peripheral
{
public:
	uint32_t readAddress(uint8_t address);
	void writeAddress(uint8_t address, uint32_t data);
protected:
	void initialize();
	void shutdown();
};


#endif /* INC_SETTINGSSTORAGE_H_ */
