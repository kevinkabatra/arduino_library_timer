#include "Arduino.h"
#include "Timer.h"

Timer::Timer(unsigned long startMillisecond, TimeUnit unitOfTime, short quantityOfTimeUnit, bool shouldTimerStart) {
	_quantityOfTimeUnit = quantityOfTimeUnit;
	_startMillisecond = startMillisecond;
	_unitOfTime = unitOfTime;
	init(shouldTimerStart);
}

void Timer::init(bool& shouldTimerStart) {
	if(shouldTimerStart) {
		StartTimer();
		_isTimerRunning = true;
	}

	_lastMillisecondReading = _startMillisecond;
	_remainingTime = ConvertToMilliseconds(_unitOfTime, _quantityOfTimeUnit);
}

/*
	milliseconds conversion
		Second = 1000
		Minute = 60000
		Hour   = 3,600,000
		Day    = 86,400,000
*/
unsigned long Timer::ConvertToMilliseconds(TimeUnit& unitOfTime, short& quantityOfTimeUnit) {
	switch(_unitOfTime) {
		case Timer::TimeUnit::Second:
			return (1000UL * quantityOfTimeUnit); 
		case Timer::TimeUnit::Minute:
			return (60000L * quantityOfTimeUnit);
		case Timer::TimeUnit::Hour:
			return (3600000UL * quantityOfTimeUnit);
		case Timer::TimeUnit::Day:
			return (86400000UL * quantityOfTimeUnit);
		default:
			return 0;
	}
}

bool Timer::GetIsTimerRunning() {
	return _isTimerRunning;
}

unsigned long Timer::GetRemainingTime() {
	return _remainingTime;
}

void Timer::ResetTimer() {
	_startMillisecond = millis();
	init(_isTimerRunning);
}

void Timer::ResetTimer(unsigned long startMillisecond, TimeUnit unitOfTime, bool shouldTimerStart) {
	_startMillisecond = startMillisecond;
	_unitOfTime = unitOfTime;

	init(shouldTimerStart);
	UpdateTimer();
}

bool Timer::StartTimer() {
	_isTimerRunning = true;
	return _isTimerRunning;
}

bool Timer::StopTimer() {
	_isTimerRunning = false;
	return _isTimerRunning;
}

unsigned long Timer::UpdateTimer() {
	unsigned long maxMillisecond = 4294967295UL;
	unsigned long currentMillisecond = millis();
	unsigned long subtrahend = 0;

	Serial.print("Current reading: ");
	Serial.println(currentMillisecond);
	Serial.print("Last reading: ");
	Serial.println(_lastMillisecondReading);

	//Overflow check, millis() will overflow after running for 50-ish days
	if(currentMillisecond < _lastMillisecondReading) {
		//Get the milliseconds that were remaining til overflow from the last reading
		unsigned long millisecondTilOverflow = maxMillisecond - _lastMillisecondReading;
		subtrahend = currentMillisecond + millisecondTilOverflow;
	}
	else {
		subtrahend = currentMillisecond - _lastMillisecondReading;
	}

	if(_remainingTime < subtrahend) {
		_remainingTime = ConvertToMilliseconds(_unitOfTime, _quantityOfTimeUnit);	
	}
	else {
		_remainingTime = _remainingTime - subtrahend;
	}
	
	_lastMillisecondReading = currentMillisecond;

	return _remainingTime;
}