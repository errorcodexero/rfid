#ifndef RFID_H
#define RFID_H

#define NAME_SPACE 24

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>

//Used for both logged time and time logged in, needs to be made into seperate structs or something
struct Time {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	Time();
};

void removeLineBreaks(std::string &str);
bool checkName(std::string &name);
std::string formatTime(Time time);
std::string formatTimeAlt(Time time);
Time parseFormattedTime(std::string time_string);
std::pair<std::vector<Time>, std::vector<Time> > getSignInsOuts(std::string name);
Time getTime();
void logAttendance(std::string name);

#endif	