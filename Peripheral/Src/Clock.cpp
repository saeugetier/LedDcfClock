#include "Clock.h"
#include "main.h"

#define REASONABLE_SECONDS_CALIBRATION_LIMIT 10

template<>
RtcClock* InterruptPeripheral<RtcClock>::mPeripheralInstance = nullptr;

RtcClock::RtcClock() : mCurrentAlarm(Alarm::NONE)
{
	mPeripheralInstance = this;
}

void RtcClock::initialize()
{
	LL_RTC_InitTypeDef RTC_InitStruct =	{ 0 };
	LL_RTC_TimeTypeDef RTC_TimeStruct =	{ 0 };
	LL_RTC_DateTypeDef RTC_DateStruct =	{ 0 };
	LL_RTC_AlarmTypeDef RTC_AlarmStruct = { 0 };

	/* Peripheral clock enable */
	LL_RCC_EnableRTC();
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_RTC);

	if (!LL_RTC_IsActiveFlag_INITS(RTC) || LL_RTC_IsActiveFlag_INIT(RTC))
	{   //RTC is not initialized or RTC is in initialization mode
		RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
		RTC_InitStruct.AsynchPrescaler = 127;
		RTC_InitStruct.SynchPrescaler = 255;
		LL_RTC_Init(RTC, &RTC_InitStruct);
		RTC_TimeStruct.Hours = 0x0;
		RTC_TimeStruct.Minutes = 0x0;
		RTC_TimeStruct.Seconds = 0x0;

		LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BIN, &RTC_TimeStruct);
		RTC_DateStruct.WeekDay = LL_RTC_WEEKDAY_FRIDAY;
		RTC_DateStruct.Month = LL_RTC_MONTH_DECEMBER;
		RTC_DateStruct.Day = 0x29;
		RTC_DateStruct.Year = 0x16;

		LL_RTC_DATE_Init(RTC, LL_RTC_FORMAT_BIN, &RTC_DateStruct);
		/** Enable the Alarm A
		 */
		RTC_AlarmStruct.AlarmTime.Hours = 0x0;
		RTC_AlarmStruct.AlarmTime.Minutes = 0x0;
		RTC_AlarmStruct.AlarmTime.Seconds = 0x0;
		RTC_AlarmStruct.AlarmMask = LL_RTC_ALMA_MASK_NONE;
		RTC_AlarmStruct.AlarmDateWeekDaySel = LL_RTC_ALMA_DATEWEEKDAYSEL_DATE;
		RTC_AlarmStruct.AlarmDateWeekDay = 0x1;

		LL_RTC_ALMA_Init(RTC, LL_RTC_FORMAT_BIN, &RTC_AlarmStruct);
		LL_RTC_SetOutputPolarity(RTC, LL_RTC_OUTPUTPOLARITY_PIN_HIGH);
		LL_RTC_SetAlarmOutputType(RTC, LL_RTC_ALARM_OUTPUTTYPE_OPENDRAIN);
		LL_RTC_DisableAlarmPullUp(RTC);
	}
}

void RtcClock::shutdown()
{
	LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_RTC); //shut down register access to RTC. RTC is running further....
}

time_t RtcClock::now()
{
	tm currentTime;

	uint32_t time = LL_RTC_TIME_Get(RTC);
	uint32_t date = LL_RTC_DATE_Get(RTC);

	currentTime.tm_hour = __LL_RTC_GET_HOUR(time);
	currentTime.tm_min = __LL_RTC_GET_MINUTE(time);
	currentTime.tm_sec = __LL_RTC_GET_SECOND(time);

	currentTime.tm_mday = __LL_RTC_GET_DAY(date);
	currentTime.tm_mon = __LL_RTC_GET_MONTH(date);
	currentTime.tm_year = __LL_RTC_GET_YEAR(date) + 2000 - 1900;

	return mktime(&currentTime);
}

uint16_t RtcClock::getSubsecond()
{
	return LL_RTC_TIME_GetSubSecond(RTC);
}

void RtcClock::calibrate(time_t time, uint16_t subseconds)
{
	time_t currentTime = now();
	//time_t lastCalibrationTime = mSettings->getLastCalibrationTimestamp();
	uint16_t currentSubseconds = LL_RTC_TIME_GetSubSecond(RTC);
	mSettings->setLastCalibrationTimestamp(time);

	int16_t subsecondDifference = currentSubseconds - subseconds;
	int32_t secondDifference = currentTime - time;

	if (secondDifference > REASONABLE_SECONDS_CALIBRATION_LIMIT)
	{
		setTime(time, subseconds);
	}
	else
	{
		//calculate calibration value... for testing, a simple method is used. decrement/increment by 1
		int32_t correction_value = 0;
		if (secondDifference < 0)
			correction_value = -1;
		else if (secondDifference > 0)
			correction_value = 1;
		else if (subsecondDifference < 0)
			correction_value = -1;
		else if (subsecondDifference > 0)
			correction_value = 1;

		applyCorrectionValue(correction_value);
	}
}

void RtcClock::applyCorrectionValue(int32_t correction_value)
{
	while (correction_value)
	{
		if (correction_value > 0)
		{
			if (LL_RTC_CAL_IsPulseInserted(
					RTC) && LL_RTC_CAL_GetMinus(RTC) == RTC_CALR_CALM_Msk) //is 0.9ppm
			{
				LL_RTC_CAL_SetPulse(RTC, LL_RTC_CALIB_INSERTPULSE_NONE); // handle overflow
				LL_RTC_CAL_SetMinus(RTC, 0);
			}
			else if (LL_RTC_CAL_GetMinus(RTC) != RTC_CALR_CALM_Msk) //while not fullscale
			{
				LL_RTC_CAL_SetMinus(RTC, 1 + LL_RTC_CAL_GetMinus(RTC));
			}

			correction_value--;
		}
		else if (correction_value < 0)
		{
			if (!LL_RTC_CAL_IsPulseInserted(RTC)
					&& LL_RTC_CAL_GetMinus(RTC) == 0) //is 0ppm
			{
				LL_RTC_CAL_SetPulse(RTC, LL_RTC_CALIB_INSERTPULSE_SET); // handle overflow
				LL_RTC_CAL_SetMinus(RTC, RTC_CALR_CALM_Msk);
			}
			else if (!LL_RTC_CAL_IsPulseInserted(RTC)
					&& LL_RTC_CAL_GetMinus(RTC) != 0) //while not fullscale
			{
				LL_RTC_CAL_SetMinus(RTC, LL_RTC_CAL_GetMinus(RTC) - 1);
			}
			correction_value++;
		}
	}
}

bool RtcClock::setTime(time_t time, uint16_t subseconds)
{
	tm currentTime;
	currentTime = *(localtime(&time));

	int32_t temp_subsecond = LL_RTC_TIME_GetSubSecond(RTC);
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_EnterInitMode(RTC);
	// set clock
	LL_RTC_TIME_Config(RTC, LL_RTC_HOURFORMAT_24HOUR, currentTime.tm_hour,
			currentTime.tm_min, currentTime.tm_sec);
	// transform tm to rtc time
	if (currentTime.tm_wday == 0)
		currentTime.tm_wday = 7;
	currentTime.tm_mon = currentTime.tm_mon + 1;
	currentTime.tm_year = currentTime.tm_year + 1900 - 2000;
	// set calendar
	LL_RTC_DATE_Config(RTC, currentTime.tm_wday, currentTime.tm_mday,
			currentTime.tm_mon, currentTime.tm_year);
	LL_RTC_DisableInitMode(RTC);
	// synchronize subseconds
	int32_t difference = temp_subsecond - subseconds;
	if (difference < 0)
	{
		LL_RTC_TIME_Synchronize(RTC, LL_RTC_SHIFT_SECOND_ADVANCE,
				LL_RTC_GetSynchPrescaler(RTC) + difference);
	}
	else if (difference > 0)
	{
		LL_RTC_TIME_Synchronize(RTC, LL_RTC_SHIFT_SECOND_DELAY,
				LL_RTC_GetSynchPrescaler(RTC) + difference);
	}
	while (LL_RTC_IsActiveFlag_SHP(RTC))
		;
	LL_RTC_EnableWriteProtection(RTC);

	return true;
}

bool RtcClock::isClockSet()
{
	return LL_RTC_IsActiveFlag_INITS(RTC) && !LL_RTC_IsActiveFlag_INIT(RTC);
}

void RtcClock::handleInterrupt()
{
	if (mCallback != nullptr)
		mCallback->notify();

	setAlarmMode(mCurrentAlarm); //update alarm trigger
}

void RtcClock::setAlarmMode(RtcClock::Alarm alarm)
{
	LL_RTC_AlarmTypeDef alarm_init = { 0 };
	uint32_t currentMinute = LL_RTC_ALMA_GetMinute(RTC);

	mCurrentAlarm = alarm;

	alarm_init.AlarmMask = LL_RTC_ALMA_MASK_DATEWEEKDAY | LL_RTC_ALMA_MASK_HOURS
			| LL_RTC_ALMA_MASK_SECONDS;

	LL_RTC_DisableWriteProtection(RTC);

	switch (alarm)
	{
	case Alarm::NONE:
		LL_RTC_ALMA_Disable(RTC);
		break;
	case Alarm::HOURLY:
		alarm_init.AlarmTime.Minutes = 0;
		LL_RTC_ALMA_Init(RTC, LL_RTC_FORMAT_BIN, &alarm_init);
		LL_RTC_ALMA_Enable(RTC);
		break;
	case Alarm::EVERY_15_MINUTES:
		currentMinute -= currentMinute % 15;
		currentMinute += 15;
		if (currentMinute > 59)
			currentMinute = 0;
		alarm_init.AlarmTime.Minutes = currentMinute;
		LL_RTC_ALMA_Init(RTC, LL_RTC_FORMAT_BIN, &alarm_init);
		LL_RTC_ALMA_Enable(RTC);
		break;
	case Alarm::EVERY_5_MINUTES:
		currentMinute -= currentMinute % 5;
		currentMinute += 5;
		if (currentMinute > 59)
			currentMinute = 0;
		alarm_init.AlarmTime.Minutes = currentMinute;
		LL_RTC_ALMA_Init(RTC, LL_RTC_FORMAT_BIN, &alarm_init);
		LL_RTC_ALMA_Enable(RTC);
		break;
	}

	LL_RTC_EnableWriteProtection(RTC);
}
