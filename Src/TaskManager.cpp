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

TaskManager::TaskManager() : mInvokedEvents {EventType::NONE}
{
	memset(mTaskList, 0, sizeof(mTaskList));
	memset(mEventList, 0, sizeof(mEventList));
}

void TaskManager::invokeEvent(EventType::type event)
{
	if(event != EventType::NONE)
	{
		for(int i = 0; i < MAX_EVENTS; i++)
		{
			if(mInvokedEvents[i] == event) //don't insert same event again
			{
				break;
			}

			if(mInvokedEvents[i] == EventType::NONE)
			{
				mInvokedEvents[i] = event;
				break;
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
	EventList eventList = task->getEvents();
	while(!eventList.empty())
	{
		addEvent(eventList.front());
		eventList.pop_front();
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
	volatile TaskMode taskModeLevel = TaskMode::NONE;

	//execute events first
	for(int i = 0; i < MAX_EVENTS; i++)
	{
		if(mInvokedEvents[i] == EventType::NONE)
		{
			break; //last event reached
		}
		else  //invoked event list is not empty
		{
			EventType::type event = mInvokedEvents[i];
			for(int j = 0; j < MAX_EVENTS; j++)
			{
				if(mEventList[j] != nullptr)
				{
					if(mEventList[j]->getType() == event)
					{
						(*mEventList[j])();
					}

				}
				else
					break;
			}
			mInvokedEvents[i] = EventType::NONE;
		}
	}

	//than task normally
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
		this->sleep();
	else if (taskModeLevel == TaskMode::DEEPSLEEP)
		this->deepSleep();
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
	LL_PWR_ClearFlag_WU();
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
