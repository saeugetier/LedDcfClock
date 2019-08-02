/*
 * TaskManager.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#include "TaskManager.h"
#include "stm32g0xx.h"
#include <string.h>
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_cortex.h"

TaskManager::TaskManager()
{
	memset(mTaskList, 0, sizeof(mTaskList));
	memset(mEventList, 0, sizeof(mEventList));
}

void TaskManager::invokeEvent(EventType event)
{
	for(int i = 0; i < MAX_EVENTS; i++)
	{
		if(mEventList[i] != nullptr)
		{
			if(mEventList[i]->getType() == event)
			{
				(*mEventList[i])();
			}

		}
	}
}

void TaskManager::addTask(Task* task)
{
	for (int i = 0; i < MAX_TASKS; i++)
	{
		if (mTaskList[i] == nullptr)
		{
			mTaskList[i] = task;
			break;
		}
	}
}

void TaskManager::addEvent(Event* event)
{
	for (int i = 0; i < MAX_EVENTS; i++)
	{
		if (mEventList[i] == nullptr)
		{
			mEventList[i] = event;
			break;
		}
	}
}

void TaskManager::runTasks()
{
	TaskMode taskModeLevel = TaskMode::NONE;
	for (int i = 0; i < MAX_TASKS; i++)
	{
		if (mTaskList[i] != nullptr)
		{
			TaskMode itemTaskModeLevel = mTaskList[i]->getTaskMode();
			taskModeLevel =
					itemTaskModeLevel < taskModeLevel ?
							itemTaskModeLevel : taskModeLevel;
			if (itemTaskModeLevel == TaskMode::WAKE)
				mTaskList[i]->run();
		}
	}

	if (taskModeLevel == TaskMode::SLEEP)
		sleep();
	else if (taskModeLevel == TaskMode::DEEPSLEEP)
		deepSleep();
}

void TaskManager::sleep()
{
	/* Disable Deep Sleep flag */
	LL_LPM_EnableSleep();

	/* Request Wait For Interrupt */
	__WFI();
}

void TaskManager::deepSleep()
{
	/** Request to enter Standby mode
	 * Following procedure describe in STM32G0xx Reference Manual
	 * See PWR part, section Low-power modes, Standby mode
	 */
	/* Set Standby mode when CPU enters deepsleep */
	LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);

	/* Set SLEEPDEEP bit of Cortex System Control Register */
	LL_LPM_EnableDeepSleep();

	/* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM)
  __force_stores();
#endif
	/* Request Wait For Interrupt */
	__WFI();
}
