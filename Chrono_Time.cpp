#include "Chrono_Time.h"
#include <stdio.h>

Chrono_Time::Chrono_Time()
{
	second = 0;
	minute = 0;
	hour   = 0;
}

void Chrono_Time::ResetTime()
{
	second = 0;
	minute = 0;
	hour = 0;
}

void Chrono_Time::addSecond()
{
	second++;
	if (second >= 60) {
		second = 0;
		minute++;
		if (minute >= 60) {
			minute = 0;
			hour++;
		}
	}
}

char * Chrono_Time::getTime()
{
	char * theTime = new char[200];
	sprintf_s(theTime, 200, "%02i:%02i:%02i", hour, minute, second);
	return theTime;
}
