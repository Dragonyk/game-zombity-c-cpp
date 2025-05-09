#pragma once
class Chrono_Time
{
public:
	Chrono_Time();

	void ResetTime();
	void addSecond();
	char * getTime();

private:
	int second;
	int minute;
	int hour;
};

