/*
 * dcf77_decoder.h
 *
 *  Created on: 12.07.2019
 *      Author: timm
 */

#ifndef INC_DCF77_DECODER_H_
#define INC_DCF77_DECODER_H_

#include "time.h"
#include "stdint.h"
#include "PulseDetector.h"
#include "Clock.h"
#include "Callback.h"

class DCF77 {
private:
	//Private variables
	RtcClock* clock;
    uint8_t pulseStart;

    // DCF77 and internal timestamps
    time_t previousUpdatedTime;
    time_t latestupdatedTime;
    time_t processingTimestamp;
    time_t previousProcessingTimestamp;
    uint8_t CEST;
    // DCF time format structure
    struct DCF77Buffer {
      //uint64_t prefix       :21;
      uint64_t prefix     :17;
      uint64_t CEST       :1; // CEST
      uint64_t CET        :1; // CET
      uint64_t unused     :2; // unused bits
      uint64_t Min        :7; // minutes
      uint64_t P1         :1; // parity minutes
      uint64_t Hour       :6; // hours
      uint64_t P2         :1; // parity hours
      uint64_t Day        :6; // day
      uint64_t Weekday    :3; // day of week
      uint64_t Month      :5; // month
      uint64_t Year       :8; // year (5 -> 2005)
      uint64_t P3         :1; // parity
    };


    // DCF Parity format structure
    struct ParityFlags{
        uint8_t parityFlag    :1;
        uint8_t parityMin     :1;
        uint8_t parityHour    :1;
        uint8_t parityDate    :1;
    } flags;

    // Parameters shared between interupt loop and main loop
    volatile bool FilledBufferAvailable;
    volatile uint64_t filledBuffer;
    volatile time_t filledTimestamp;

    // DCF Buffers and indicators
    int32_t  bufferPosition;
    uint64_t runningBuffer;
    uint64_t processingBuffer;

    // Pulse flanks
    int32_t  leadingEdge;
    int32_t  trailingEdge;

    //Private functions
    void initialize(void);
    void bufferinit(void);
    void finalizeBuffer(void);
    bool receivedTimeUpdate(void);
    void storePreviousTime(void);
    void calculateBufferParities(void);
    bool processBuffer(void);
    void appendSignal(uint8_t signal);

    //subsecond synchronization buffer
    uint16_t subsecondsCircularBuffer[64];
    uint8_t subsecondsCircularBufferPos;
    uint16_t medianSubsecond;
public:
    // Public Functions
    DCF77(RtcClock* _clock, bool OnRisingFlank=true);

    time_t getTime(void);
    uint16_t getMedianSubsecond();
    time_t getUTCTime(void);

    int32_t getSummerTime();

    void decode(uint32_t risingEdge, uint32_t fallingEdge);
 };



#endif /* INC_DCF77_DECODER_H_ */
