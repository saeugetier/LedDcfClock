/*
 * Peripheral.h
 *
 *  Created on: May 30, 2019
 *      Author: timm
 */

#ifndef PERIPHERAL_INC_PERIPHERAL_H_
#define PERIPHERAL_INC_PERIPHERAL_H_


class Peripheral
{
public:
	virtual void initialize() = 0;
	virtual void shutdown() = 0;
};






#endif /* PERIPHERAL_INC_PERIPHERAL_H_ */
