/*
	Arduino library that will support Timer operations.
*/

#ifndef Timer_h
#define Timer_h

#include "Arduino.h"

class Timer {
	public:
		/*
			millis() on the Arduino is an unsigned long and will 
			overflow once it after approx 50 days of running.
			Due to this the largest unit of time that this library
			will support.

			See: https://www.arduino.cc/reference/en/language/functions/time/millis/
		*/
		enum TimeUnit { Second, Minute, Hour, Day };

		/* Constructor */
		Timer(unsigned long startMillisecond, TimeUnit unitOfTime, short _quantityOfTimeUnit, bool shouldTimerStart);

		bool GetIsTimerRunning();
		unsigned long GetRemainingTime();
		void init(bool& shouldTimerStart);
		void ResetTimer();
		void ResetTimer(unsigned long startMillisecond, TimeUnit unitOfTime, bool shouldTimerStart);
		bool StartTimer();
		bool StopTimer();
		unsigned long UpdateTimer();

		bool isTimerExpired;
	private:
		bool _isTimerRunning;
		unsigned long _lastMillisecondReading;
		short _quantityOfTimeUnit;
		unsigned long _remainingTime;
		unsigned long _startMillisecond;
		TimeUnit _unitOfTime;

		/*
			milliseconds conversion
				Second = 1000
				Minute = 60000
				Hour   = 3,600,000
				Day    = 86,400,000
		*/
		unsigned long ConvertToMilliseconds(TimeUnit& unitOfTime, short& quantityOfTimeUnit);
};

#endif