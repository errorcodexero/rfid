#ifndef RFID_H
#define RFID_H

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

#define NAME_SPACE 24
#define HOUR_SPACE 3
#define HOURS_TO_ADD 3

#define HOURS_NEEDED 90
#define MAX_OFFSEASON_HOURS 30

enum check {CHECK_MONTH_DAYS};
const int time_pos_values[5] = {60, 60, 24, CHECK_MONTH_DAYS, 12}; //Seconds in a minute, minutes in an hour, hours in a day, days in a month, months in a year
const int month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //Days in January, February, March, April, May, June, July, August, September, October, November, December

//Used for both logged time and time logged in, needs to be made into seperate structs or something
struct Time {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	Time();
	Time(int y, int mo, int d, int h, int m, int s);
};

Time operator-(Time t1, Time t2);
Time operator+(Time t1, Time t2);
bool operator<(Time t1, Time t2);
std::ostream& operator<<(std::ostream& os, Time t);
Time getTime();
void removeLineBreaks(std::string &str);
bool checkName(std::string &name);
std::string formatTime(Time time);
std::string formatTimeAlt(Time time);
Time parseFormattedTime(std::string time_string);
std::pair<std::vector<Time>, std::vector<Time> > getSignInsOuts(std::string name, std::string file="log.txt");
int addLineToFile(std::string file_name, std::string str, int line_number=-1);
void logAttendance(std::string name, int line_number=-1, Time time=getTime());
void logAttendance(std::string name, Time time, std::string file_name);
void sortSignInsOuts(std::pair<std::vector<Time>, std::vector<std::string> > &rankings);
Time getTotalTime(std::pair<std::vector<Time>, std::vector<Time> > &sign_ins_outs);
int getTimeInHours(Time t);
int getTimeInDays(Time t);

#endif	
