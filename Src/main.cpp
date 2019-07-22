/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <main.h>
#include "dma.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LedClock.h"
#include "dcf77_decoder.h"

extern "C"
{
	void SystemClock_Config(void);
}

//const uint32_t buffer[] = {3,3,6,3,6,3,6,3,3,3,6,3,6,3,6,3,3,3,3,3,6,3,6,3,3,3,6,6,6,6,6,6,3,3,6,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,6,6,6,6,6,3,3,3,3,3,3,3,3,3,6,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
time_t current_time;
volatile uint32_t systickcounter = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t clock_state;

void delay_ms(int ms)
{
	int current = systickcounter;
	while(systickcounter - current < ms);
}

uint8_t sample_input_pin() {
	uint8_t state =  LL_GPIO_IsInputPinSet(DCF_IN_GPIO_Port, DCF_IN_Pin);
	if(state != 0)
	{
		LL_GPIO_SetOutputPin(TESTPOINT_GPIO_Port, TESTPOINT_Pin);
	}
	return state;
}

void (*systick_callback)();

void systick_attach_callback(void(*func)() )
{
	systick_callback = func;
}

void systick_init(uint32_t clocks_1ms)
{
	SysTick->LOAD  = (uint32_t)(clocks_1ms  - 1UL);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
//WS2812<60+12> ws2812;
Settings settings;

LedClock ledClock(&settings);
RtcClock _clock;
DCF77 dcf(&_clock, false);

void EnterStandbyMode(void)
{
  /* Turn-off LED */
  /* Note: LED state at this step depends on blinking state at the instant of user button is pressed. */
  LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);

  /* Disable all used wakeup sources */
  LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN2);
  LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN4);

  /* Clear all wake up Flag */
  LL_PWR_ClearFlag_WU();

  /* Set wakeup pin polarity */
  LL_PWR_SetWakeUpPinPolarityLow(LL_PWR_WAKEUP_PIN2);
  LL_PWR_SetWakeUpPinPolarityLow(LL_PWR_WAKEUP_PIN4);

  /* Enable wakeup pin */
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN4);

  /* As default User push-button state is high level, need to clear all wake up Flag again */
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

int second = 0xFF;

time_t dcf_time = 0;

int main(void)
{
  /* USER CODE BEGIN 1 */
  struct tm y2k = {0};

  //ws2812.initialize();

  y2k.tm_hour = 2;   y2k.tm_min = 0; y2k.tm_sec = 0;
  y2k.tm_year = 102; y2k.tm_mon = 3; y2k.tm_mday = 1;

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/

  /** Enable the PVD Output 
  */
  LL_PWR_EnablePVD();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  //MX_RTC_Init();
  _clock.initialize();
  MX_TIM1_Init();

  ledClock.enable(true);
  dcf.Start();
  /* USER CODE BEGIN 2 */
#ifdef DEBUG
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DBGMCU);
  LL_DBGMCU_EnableDBGStopMode();
  LL_DBGMCU_EnableDBGStandbyMode();
  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_DBGMCU);
#endif
  //DCF77_Clock::setup();
  //DCF77_Clock::set_input_provider(sample_input_pin);

  current_time = mktime(&y2k);

  PWR->PDCRA = PWR_PDCRA_PD8;
  PWR->CR3 |= PWR_CR3_APC | PWR_CR3_ENB_ULP;
  LL_GPIO_SetOutputPin(POWER_MODE_GPIO_Port, POWER_MODE_Pin);

  //for(int i = 0; i < 32000; i++);

  LL_TIM_ClearFlag_UPDATE(TIM3);

  LL_TIM_ConfigDMABurst(TIM3, LL_TIM_DMABURST_BASEADDR_CCR3, LL_TIM_DMABURST_LENGTH_1TRANSFER);
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH3);
  LL_TIM_EnableAllOutputs(TIM3);
  LL_TIM_SetUpdateSource(TIM3, LL_TIM_UPDATESOURCE_COUNTER);
  LL_TIM_EnableCounter(TIM3);
  LL_TIM_EnableDMAReq_UPDATE(TIM3);

  LL_TIM_IC_SetActiveInput(TIM1, LL_TIM_CHANNEL_CH3, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetActiveInput(TIM1, LL_TIM_CHANNEL_CH4, LL_TIM_ACTIVEINPUT_INDIRECTTI);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH4);
  LL_TIM_EnableIT_CC3(TIM1);
  LL_TIM_EnableIT_CC4(TIM1);
  LL_TIM_EnableCounter(TIM1);

  //LL_GPIO_ResetOutputPin(POWER_MODE_GPIO_Port, POWER_MODE_Pin);

  //if(HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK)
  //  {
      /* Starting PWM generation Error */
   //   Error_Handler();
   // }

    /*## Start DMA Burst transfer ###########################################*/
   // HAL_TIM_DMABurst_WriteStart(&htim2, TIM_DMABASE_ARR, TIM_DMA_UPDATE,
    //                            (uint32_t*)aSRC_Buffer, TIM_DMABURSTLENGTH_3TRANSFERS);

  LL_SYSTICK_EnableIT();
  LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);
  LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);
  /* USER CODE END 2 */

  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_BKP_SetRegister(TAMP, 0, 0x66);
  LL_RTC_EnableWriteProtection(RTC);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	 /*int length = dcf77_encode(&current_time);
	 if(length == 0)
	 {
		 LL_GPIO_SetOutputPin(LED4_GPIO_Port, LED4_Pin);
		 delay_ms(100);
		 LL_GPIO_ResetOutputPin(LED4_GPIO_Port, LED4_Pin);
		 delay_ms(900);
	 }
	 else if(length == 1)
	 {
		 LL_GPIO_SetOutputPin(LED4_GPIO_Port, LED4_Pin);
		 delay_ms(200);
		 LL_GPIO_ResetOutputPin(LED4_GPIO_Port, LED4_Pin);
		 delay_ms(800);
	 }
	 else
	 {
		 delay_ms(1000);
	 }

	 current_time++;*/

	 //clock_state = DCF77_Clock::get_clock_state();
	 /*if(clock_state != 0)
	 {
		 LL_GPIO_SetOutputPin(LED4_GPIO_Port, LED4_Pin);
	 }
	 else
	 {
		 LL_GPIO_ResetOutputPin(LED4_GPIO_Port, LED4_Pin);
	 }*/

	  //__WFI();

	  //if(LL_RTC_TIME_GetSubSecond(RTC) < 20)
	  //{
		  //LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);

		  if(LL_RTC_IsActiveFlag_RS(RTC))
		  {

			  //LL_RTC_TIME_Get(RTC);
			  LL_RTC_DATE_Get(RTC);

			  y2k.tm_hour = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC));
			  y2k.tm_min = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC));
			  y2k.tm_sec =  __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC));

			  ledClock.displayTime(y2k, true, (LL_RTC_TIME_GetSubSecond(RTC) * 128) / 1025);

				  //ws2812.clearBuffer();

				  /*second = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC));
				  int subsecond = (LL_RTC_TIME_GetSubSecond(RTC) * 200) / 1025;
				  int minute = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC));
				  int hour = (__LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC)) % 12) * 5;

				  ws2812.setPixelColor(RGB(0,0,subsecond), second);
				  ws2812.setPixelColor(RGB(0,0,200-subsecond), (second + 1) % 60);
				  ws2812.setPixelColor(RGB(200,0,ws2812.getPixelColor(minute).b), minute);
				  ws2812.setPixelColor(RGB(ws2812.getPixelColor(hour).r,200,ws2812.getPixelColor(hour).b), hour);
				  ws2812.showLeds();*/

				  /*if(!(second % 40) && second != 0)
				  {
					  EnterStandbyMode();
				  }*/

			  LL_RTC_ClearFlag_RS(RTC);

		  }
		  while(!ledClock.isReady());

		  int temp_time = dcf.getTime();
		  if(temp_time != 0)
			  dcf_time = temp_time;

	 //}
	 // else
	  //{
		//  LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);
	  //}
  }
  /* USER CODE END 3 */
}


#define DCF_TIMEBASE    100             //Zeitauflösung für PWM-Input in µsek.

#define DCF_SYNC        0x00000001      //Kennung für synchr. DCF77-Signal
#define DCF_NOSYNC      0xFFFFFFFE      //Kennung für nicht synchr. DCF77-Signal
#define DCF_OK          0x00000002      //Kennung für DCF77-Signal innerhalb der Toleranz
#define DCF_NOOK        0xFFFFFFFD      //Kennung für DCF77-Signal ausserhalb der Toleranz
#define DCF_RDY         0x00000004      //Kennung für DCF77 fertig
#define DCF_NORDY       0xFFFFFFFB       //Kennung für nicht DCF77 nicht fertig
#define ZEITBEGINN      21
#define DATUMBEGINN     36

#define PULS_MIN_L      65              //min. Pulslänge für Lowsignal
#define PULS_MAX_L      135             //max. Pulslänge für Lowsignal
#define PULS_MIN_H      165             //min. Pulslänge für Highsignal
#define PULS_MAX_H      235             //max. Pulslänge für Highsignal
#define SYNC_MIN        1700            //min. Periode für Synchronisierung
#define SYNC_MAX        2300            //max. Periode für Synchronisierung
#define PERIOD_MIN      700             //min. Periode für DCF-Empfang
#define PERIOD_MAX      1300            //max. Periode für DCF-Empfang

void DCF77_init(void);
unsigned int dcf_testperiode(void);
unsigned int dcf_testpuls(void);
unsigned int DCF_Auswertung_Zeit(unsigned int wert, tm *ergebnis);
unsigned int DCF_Auswertung_Datum(unsigned int wert, tm *ergebnis);
void EXTIT2_Handler(void);
void START_DCF(void);
void STOPP_DCF(void);
void dcf_fehler(void);

unsigned int puls, period, stat_dcf77 = 0;


/*************************************************************************
 * Function Name: DCF_Auswertung_Zeit
 * Parameters:  struct RTC_TIME - Pointer auf Ergebnisspeicher
 *              unsigned int wert - umzuwandelnder Wert
 * Return: 0 - Paritätsfehler
 *         1 - alles ok
 * Description: Auswertung der DCF-Zeitinformation
 *
 *************************************************************************/

/*
unsigned int DCF_Auswertung_Zeit(unsigned int wert, tm *ergebnis)
{
  unsigned int i, j, lauf;

#ifdef DEBUG
    debug();
#endif

  //Parität Minuten testen
  for(i = 0, lauf = wert, j = 0;i < 7;i++)
  {
    j += (lauf & 0x01);
    lauf = lauf >> 1;
  }
  j = j % 2;
  i = lauf & 0x01;
  lauf = lauf >> 1;
  if(i != j)            //Parität ok?
    return(0x00);       //nein: raus mit Fehlermeldung
  //Parität Stunden testen
  for(i = 0, j = 0;i < 6;i++)
  {
    j += (lauf & 0x01);
    lauf = lauf >> 1;
  }
  j = j % 2;
  i = lauf & 0x01;
  lauf = lauf >> 1;
  if(i != j)            //Parität ok?
    return(0x00);       //nein: raus mit Fehlermeldung
  j = ((wert & 0x70) >> 4) * 10;        //Minuten berechnen
  j = j + (wert & 0x0F);                //
  ergebnis->minutes = (unsigned char)j; //abspeichern
  j = ((wert & 0x3000) >> 12) * 10;     //Stunden berechnen
  j = j + ((wert & 0xF00) >> 8);        //
  ergebnis->hours = (unsigned char)j;   //abspeichern
  ergebnis->seconds = 0;                //Sekunden 0
  ergebnis->milliseconds = 0;
  return(0x01);

}*/

/*************************************************************************
 * Function Name: DCF_Auswertung_Datum
 * Parameters:  struct RTC_DATE - Pointer auf Ergebnisspeicher
 *              unsigned int wert - umzuwandelnder Wert
 * Return: 0 - Paritätsfehler
 *         1 - alles ok
 * Description: Auswertung der DCF-Zeitinformation
 *
 *************************************************************************/
/*
unsigned int DCF_Auswertung_Datum(unsigned int wert, tm *ergebnis)
{
  unsigned int i, j, lauf;

#ifdef DEBUG
    debug();
#endif

  //Parität testen Anzahl der '1' ermitteln
  for(i = 0, lauf = wert, j = 0;i < 22;i++)
  {
    j += (lauf & 0x01);
    lauf = lauf >> 1;
  }
  j = j % 2;
  i = lauf & 0x01;
  lauf = lauf >> 1;
  if(i != j)            //Parität ok?
    return(0x00);       //nein:raus mit Fehlermeldung
  j = ((wert & 0x30) >> 4) * 10;        //Tag berechnen
  j = j + (wert & 0x0F);                //
  ergebnis->tm_yday = j;                    //und abspeichern
  j = ((wert & 0x1C0) >> 6);            //Wochentag berechnen
  ergebnis->tm_wday= (unsigned char)j; //und abspeichern
  j = ((wert & 0x200) >> 13) * 10;      //Monat berechnen
  j = j + ((wert & 0x1E00) >> 9);       //
  ergebnis->tm_mon = (unsigned char)j;   //und abspeichern
  j = ((wert & 0x3C0000) >> 18) * 10;   //Jahr berechnen
  j = j + ((wert & 0x3C000) >> 14);     //
  ergebnis->tm_year = (unsigned char)j + 2000;    //und abspeichern
  return(0x01);
}
*/

/*************************************************************************
 * Function Name: EXTIT2_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Auswertung des DCF77- Signals
 *
 *************************************************************************/

unsigned int dcf_zeit, dcf_datum;
unsigned char count_dcf;
/*
void EXTIT2_Handler(void)
{

  unsigned int bit, diff;

#ifdef DEBUG
  debug();
#endif

  WIU_ClearITPendingBit(WIU_Line18);    //Interrupt-Flag(EXTIT2) rücksetzen
  stat_dcf77 = stat_dcf77 & DCF_NORDY;  //DCF-Status auf nicht fertig setzen
  bit = dcf_testperiode();              //Signale testen(Periode)
  if(bit == 3)                         //fehlerhafte Periodendauer?
  {                                     //ja:
    dcf_fehler();                       //Fehler signalisieren
    return;                             //und raus
  }
  diff = dcf_testpuls();                //Signale testen(Periode)
  if(diff == 2)                         //fehlerhafter Puls?
  {                                     //ja:
    dcf_fehler();                       //Fehler signalisieren
    return;                             //und raus
  }
  if((count_dcf > (ZEITBEGINN - 1)) && (count_dcf < DATUMBEGINN)) //Zeitinformation?
    dcf_zeit = dcf_zeit | (diff << (count_dcf - ZEITBEGINN));           //ja:also speichern
  if((count_dcf > (DATUMBEGINN - 1)) && (count_dcf < 59))         //Datumsinformation?
    dcf_datum = dcf_datum | (diff << (count_dcf - DATUMBEGINN));        //ja:also speichern
  if(bit == 2)        //Minutenimpuls ?
  {                                                     //ja:
    if((count_dcf == 58) && (stat_dcf77 & DCF_SYNC))    //alle Impulse glatt durchgegangen?
    {
      if(DCF_Auswertung_Zeit(dcf_zeit, &time))  //Zeitauswertung ok?
      {
        RTC_SetTime(time);       //RTC Register setzen
      }
      else
      {
        dcf_fehler();
        return;
      }
      if(DCF_Auswertung_Datum(dcf_datum, &date))        //Datumsauswertung ok?
      {
        RTC_SetDate(date);       //RTC Register setzen
      }
      else
      {
        dcf_fehler();
        return;
      }
      STOPP_DCF();
      alarm.day = date.day;
 //     alarm.day = 0;
      alarm.hours = time.hours;
      diff = 5;
      bit = time.minutes;
      if((bit + diff) > 59)     //Stundensprung?
      {                         //ja
        alarm.hours += 1;
        alarm.minutes = (60 - bit) + diff;
      }
      else
        alarm.minutes = time.minutes + diff;

 //     alarm.minutes = 2;
 //     alarm.hours = 0;
      alarm.seconds = 0;
      RTC_ClearFlag(RTC_FLAG_Alarm);
      RTC_SetAlarm(alarm);     //RTC-Alarm setzen
      RTC_ITConfig(RTC_IT_Alarm,ENABLE);      //Enable RTC-Alarm Interrupt
      RTC_AlarmCmd(ENABLE);                   //Enable alarm
      VIC_Config(RTC_ITLine, VIC_IRQ, RTC_INT_PR);   //Configure and enable RTC interrupt in VIC
      VIC_ITCmd(RTC_ITLine, ENABLE);
    }
    count_dcf = 0;            //Impulszähler rücksetzen
    dcf_zeit = 0;
    dcf_datum = 0;
    stat_dcf77 |= DCF_SYNC;   //Status synchronisiert setzen hat Minutenimpuls bekommen
    return;
  }
  */
/*
  if((count_dcf > (ZEITBEGINN - 1)) && (count_dcf < DATUMBEGINN))      //Zeitinformation?
    dcf_zeit = dcf_zeit | (bit << (count_dcf - ZEITBEGINN));
  if((count_dcf > (DATUMBEGINN - 1)) && (count_dcf < 59))       //Datumsinformation
    dcf_datum = dcf_datum | (bit << (count_dcf - DATUMBEGINN));
*/
  //GPIO_WriteBit(GPIO3, GPIO_Pin_2, (BitAction)((1-GPIO_ReadBit(GPIO3, GPIO_Pin_2)))); //Toggle LED P3.2
 // count_dcf++;

//}

void dcf_fehler(void)
{
  //GPIO_WriteBit(GPIO3, GPIO_Pin_1, (BitAction)((1-GPIO_ReadBit(GPIO3, GPIO_Pin_1)))); //Toggle LED P3.1
  stat_dcf77 &= DCF_NOOK;       //Status DCF-Signal ok rücksetzen
  stat_dcf77 &= DCF_NOSYNC;     //Status synchronisiert rücksetzen
  dcf_zeit = 0;
  dcf_datum = 0;
  count_dcf = 0;
  return;
}

/*************************************************************************
 * Function Name: dcf_testperiode
 * Parameters: none
 *
 * Description: Auswertung des DCF77- Signals (Periodendauer)
 *
 * Return: 1 - alles ok (normaler Puls)
 *         2 - Synchronisierung (Minutenimpuls)
 *         3 - Fehler
 *************************************************************************/

unsigned int dcf_testperiode(void)
{
  if((period > SYNC_MIN) && (period < SYNC_MAX))        //Minutenimpuls ?
  {                                                     //ja:
    return(0x02);
  }
  else if((period < PERIOD_MIN) || (period > PERIOD_MAX))  //Periodendauer innerhalb der Toleranzen?
  {                                                   //nein
    return(0x03);                                     //und raus
  }
  else
    return(0x01);
}

/*************************************************************************
 * Function Name: dcf_testpuls
 * Parameters: none
 *
 * Description: Auswertung des DCF77- Signals (Pulsdauer)
 *
 * Return: 0 - Low-Signal
 *         1 - High- Signal
 *         2 - Fehler
 *************************************************************************/

unsigned int dcf_testpuls(void)
{

  if((puls > PULS_MIN_L) && (puls < PULS_MAX_L))        //Information innerhalb der Low-Toleranzen?
  {
    return(0x00);
  }
  else if((puls > PULS_MIN_H) && (puls < PULS_MAX_H))   //Information innerhalb der High-Toleranzen?
  {
    return(0x01);
  }
  else                                         //Fehler
    return(0x02);                              //und raus
}


