#ifndef RFID_H
#define RFID_H

#include <string>
#include <fstream>
#include <iostream>

struct Time {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
};

bool checkName(std::string *name);
std::string formatTime(Time time);

#endif	