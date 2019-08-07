/*
 * Interrupts.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: timm
 */

#include "PulseDetector.h"
#include "SystemTick.h"


//using extern "C" to get the right function reference -- ABI changed for C++ function calls
extern "C"
{
void TIM1_CC_IRQHandler(void)
{
	PulseDetector::getInstance()->handleInterrupt();
}
}

extern "C"
{
void SysTick_Handler()
{
	SystemTick::getInstance()->handleInterrupt();
}
}

extern "C"
{
void EXTI0_1_IRQHandler()
{

}
}

extern "C"
{
void EXTI2_3_IRQHandler()
{

}
}

extern "C"
{
void EXTI4_15_IRQHandler()
{

}
}

