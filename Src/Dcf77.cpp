/*
  DCF77.c - DCF77 library
  Copyright (c) Thijs Elenbaas 2012
  Copyright (c) Timm Eversmeyer 2019

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  11 Apr 2012 - initial release
  23 Apr 2012 - added UTC support
  2  Jul 2012 - minor bugfix and additional noise rejection
  16 Jul 2019 - adaption to application on STM32G0
*/

#include "Dcf77.h"
#include <math.h>
#include <time.h>
#include <string.h>

#define _DCF77_VERSION 1_1_0 // software version of this library

#define HIGH 1
#define LOW 0

#define SECS_PER_MIN  60
#define SECS_PER_HOUR 3600

#define MIN_TIME 1563235200     // Date: 16-7-2019
#define MAX_TIME 4102444800     // Date:  1-1-2100

#define DCFRejectionTime 700    // Pulse-to-Pulse rejection time.
#define DCFRejectPulseWidth 50  // Minimal pulse width
#define DCFSplitTime 170        // Specifications distinguishes pulse width 100 ms and 200 ms. In practice we see 130 ms and 230
#define DCFSyncTime 1500        // Specifications defines 2000 ms pulse for end of sequence

/**
 * Constructor
 */
DCF77::DCF77(RtcClock* _clock, bool OnRisingFlank) : pulseDetector(OnRisingFlank)
{
	pulseDetector.registerPulseCallback(this);
	// Parameters shared between interupt loop and main loop
	filledBuffer = 0;
	filledTimestamp= 0;

	// DCF Buffers and indicators
	runningBuffer = 0;
	processingBuffer = 0;

	// DCF77 and internal timestamps
	latestupdatedTime= 0;
	previousUpdatedTime= 0;
	processingTimestamp= 0;
	previousProcessingTimestamp=0;

	pulseStart   = OnRisingFlank ? HIGH : LOW;

	initialize();
}

/**
 * Initialize parameters
 */
void DCF77::initialize(void)
{
	leadingEdge                         = 0;
	trailingEdge                        = 0;
	runningBuffer		                = 0;
	FilledBufferAvailable               = false;
	bufferPosition                      = 0;
	flags.parityDate                    = 0;
	flags.parityFlag                    = 0;
	flags.parityHour                    = 0;
	flags.parityMin                     = 0;
	CEST				                = 0;

	memset(subsecondsCircularBuffer,0,sizeof(subsecondsCircularBuffer));
}

/**
 * Start receiving DCF77 information
 */
void DCF77::Start(void)
{
	pulseDetector.initialize();
}

/**
 * Stop receiving DCF77 information
 */
void DCF77::Stop(void)
{
	pulseDetector.shutdown();
}

/**
 * Initialize buffer for next time update
 */
inline void DCF77::bufferinit(void)
{
	runningBuffer    = 0;
	bufferPosition   = 0;
}

/**
 * Callback handler that processes up-down flanks into pulses and stores these in the buffer
 */
void DCF77::notify()
{

	uint32_t risingEdgeTime = pulseDetector.getHighEdge();
	uint32_t fallingEdgeTime = pulseDetector.getLowEdge();

	if(pulseStart == HIGH)
	{
		trailingEdge = leadingEdge + fallingEdgeTime; //add last leading edge time
		leadingEdge += risingEdgeTime;
	}
	else
	{
		trailingEdge = leadingEdge + risingEdgeTime; //add last leading edge time
		leadingEdge += fallingEdgeTime;
	}

	if (leadingEdge<DCFRejectionTime) {
		return;
	}

	int32_t difference=trailingEdge - leadingEdge;

	if (difference<DCFRejectPulseWidth) {
		return;
	}

	subsecondsCircularBuffer[subsecondsCircularBufferPos] = clock->getSubsecond();
	subsecondsCircularBufferPos++;
	if(subsecondsCircularBufferPos >= (sizeof(subsecondsCircularBuffer)/sizeof(uint16_t)))
		subsecondsCircularBufferPos = 0;

	medianSubsecond = 0;
	for(unsigned int i = 0; i < (sizeof(subsecondsCircularBuffer)/sizeof(uint16_t)); i++)
		medianSubsecond += subsecondsCircularBuffer[i];
	medianSubsecond = medianSubsecond / 64;

	if (leadingEdge > DCFSyncTime) {
		finalizeBuffer();
	}

	if (difference < DCFSplitTime) { appendSignal(0); } else { appendSignal(1); }

	leadingEdge = 0;
	trailingEdge = 0;
}

/**
 * Add new bit to buffer
 */
inline void DCF77::appendSignal(unsigned char signal) {
	//Log(signal, DEC);
	runningBuffer = runningBuffer | ((uint64_t) signal << bufferPosition);
	bufferPosition++;
	if (bufferPosition > 59) {
		// Buffer is full before at end of time-sequence
		// this may be due to noise giving additional peaks
		//LogLn("EoB");
		finalizeBuffer();
	}
}

/**
 * Finalize filled buffer
 */
inline void DCF77::finalizeBuffer(void) {
  if (bufferPosition == 59) {
		// Buffer is full
		//LogLn("BF");
		// Prepare filled buffer and time stamp for main loop
		filledBuffer = runningBuffer;
		filledTimestamp = clock->now();
		// Reset running buffer
		bufferinit();
		FilledBufferAvailable = true;
    } else {
		// Buffer is not yet full at end of time-sequence
		//LogLn("EoM");
		// Reset running buffer
		bufferinit();
    }
}

/**
 * Returns whether there is a new time update available
 * This functions should be called prior to getTime() function.
 */
bool DCF77::receivedTimeUpdate(void) {
	// If buffer is not filled, there is no new time
	if(!FilledBufferAvailable) {
		return false;
	}
	// if buffer is filled, we will process it and see if this results in valid parity
	if (!processBuffer()) {
		//LogLn("Invalid parity");
		return false;
	}

	// Since the received signal is error-prone, and the parity check is not very strong,
	// we will do some sanity checks on the time
	time_t processedTime = latestupdatedTime + (clock->now() - processingTimestamp);
	if (processedTime<MIN_TIME || processedTime>MAX_TIME) {
		//LogLn("Time outside of bounds");
		return false;
	}

	// If received time is close to internal clock (2 min) we are satisfied
	time_t difference = abs(processedTime - clock->now());
	if(difference < 2*SECS_PER_MIN) {
		//LogLn("close to internal clock");
		storePreviousTime();
		return true;
	}

	// Time can be further from internal clock for several reasons
	// We will check if lag from internal clock is consistent
	time_t shiftPrevious = (previousUpdatedTime - previousProcessingTimestamp);
	time_t shiftCurrent = (latestupdatedTime - processingTimestamp);
	time_t shiftDifference = abs(shiftCurrent-shiftPrevious);
	storePreviousTime();
	if(shiftDifference < 2*SECS_PER_MIN) {
		//LogLn("time lag consistent");
		return true;
	} else {
		//LogLn("time lag inconsistent");
	}

	// If lag is inconsistent, this may be because of no previous stored date
	// This would be resolved in a second run.
	return false;
}

/**
 * Store previous time. Needed for consistency
 */
void DCF77::storePreviousTime(void) {
	previousUpdatedTime = latestupdatedTime;
	previousProcessingTimestamp = processingTimestamp;
}

/**
 * Calculate the parity of the time and date.
 */
void DCF77::calculateBufferParities(void) {
	// Calculate Parity
	flags.parityFlag = 0;
	for(int32_t pos=0;pos<59;pos++) {
		bool s = (processingBuffer >> pos) & 1;

		// Update the parity bits. First: Reset when minute, hour or date starts.
		if (pos ==  21 || pos ==  29 || pos ==  36) {
			flags.parityFlag = 0;
		}
		// save the parity when the corresponding segment ends
		if (pos ==  28) {flags.parityMin = flags.parityFlag;};
		if (pos ==  35) {flags.parityHour = flags.parityFlag;};
		if (pos ==  58) {flags.parityDate = flags.parityFlag;};
		// When we received a 1, toggle the parity flag
		if (s == 1) {
			flags.parityFlag = flags.parityFlag ^ 1;
		}
	}
}

/**
 * Evaluates the information stored in the buffer. This is where the DCF77
 * signal is decoded
 */
bool DCF77::processBuffer(void) {

	/////  Start interaction with interrupt driven loop  /////

	// Copy filled buffer and timestamp from interrupt driven loop
	processingBuffer = filledBuffer;
	processingTimestamp = filledTimestamp;
	// Indicate that there is no filled, unprocessed buffer anymore
	FilledBufferAvailable = false;


	/////  End interaction with interrupt driven loop   /////

	//  Calculate parities for checking buffer
	calculateBufferParities();
	tm time;

	struct DCF77Buffer *rx_buffer;
	rx_buffer = (struct DCF77Buffer *)(uint64_t)&processingBuffer;


	// Check parities
    if (flags.parityMin == rx_buffer->P1  &&
        flags.parityHour == rx_buffer->P2 &&
        flags.parityDate == rx_buffer->P3 &&
		rx_buffer->CEST != rx_buffer->CET)
    {
      //convert the received buffer into time
      time.tm_sec   = 0;
	  time.tm_min   = rx_buffer->Min-((rx_buffer->Min/16)*6);
      time.tm_hour  = rx_buffer->Hour-((rx_buffer->Hour/16)*6);
      time.tm_mday  = rx_buffer->Day-((rx_buffer->Day/16)*6);
      time.tm_mon   = rx_buffer->Month-((rx_buffer->Month/16)*6) - 1;
      time.tm_year  = 2000 + rx_buffer->Year-((rx_buffer->Year/16)*6) - 1900;
	  latestupdatedTime = mktime(&time);
	  CEST = rx_buffer->CEST;
	  //Parity correct
	  return true;
	} else {
	  //Parity incorrect
	  return false;
	}
}

/**
 * Get most recently received time
 * Note, this only returns an time once, until the next update
 */
time_t DCF77::getTime(void)
{
	if (!receivedTimeUpdate()) {
		return(0);
	} else {
		// Send out time, taking into account the difference between when the DCF time was received and the current time
		time_t currentTime =latestupdatedTime + (clock->now() - processingTimestamp);
		return(currentTime);
	}
}

/**
 * Get most recently received time in UTC
 * Note, this only returns an time once, until the next update
 */
time_t DCF77::getUTCTime(void)
{
	if (!receivedTimeUpdate()) {
		return(0);
	} else {
		// Send out time UTC time
		int32_t UTCTimeDifference = (CEST ? 2 : 1)*SECS_PER_HOUR;
		time_t currentTime =latestupdatedTime - UTCTimeDifference + (clock->now() - processingTimestamp);
		return(currentTime);
	}
}

int32_t DCF77::getSummerTime(void)
{
  return (CEST)?1:0;
}
