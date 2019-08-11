/*
 * DcfWakeup.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: timm
 */

#include "DcfWakeup.h"
#include "stm32g0xx_ll_pwr.h"

DcfWakeup::DcfWakeup()
{
	LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN6);
}

void DcfWakeup::initialize()
{

}

void DcfWakeup::shutdown()
{

}

void DcfWakeup::handleInterrupt()
{

}
