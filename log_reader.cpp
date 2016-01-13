#include "rfid.h"

enum check {CHECK_MONTH_DAYS};
int time_pos_values[5] = {60, 60, 24, CHECK_MONTH_DAYS, 12}; //Seconds in a minute, minutes in an hour, hours in a day, days in a month, months in a year
int month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//Returns the amount of whatever unit of time are in the next unit of time
int getTimePosValue(int pos, int month = 0) {
	int pos_value = time_pos_values[pos];
	if (pos_value == CHECK_MONTH_DAYS) {
		int array_month = month - 1;
		return month_days[array_month];
	}
	return pos_value;
}

//Corrects for going over into the next of a larger unit of time by making adjustments to other members of the Time structure
void correctTime(int &val1, int &val2, int pos, int month = 0) {
	if (val1 < 0) {
		val1 = ((pos == 3) ? getTimePosValue(3, month) : getTimePosValue(pos)) + val1;
		val2 -= 1;
	} else if (val1 >= ((pos == 3) ? getTimePosValue(3, 4) : getTimePosValue(pos))) { //Always uses a value of 30 for month length
		val1 = val1 - ((pos == 3) ? getTimePosValue(3, 4) : getTimePosValue(pos));   //Here too
		val2 += 1;
	}
}

Time operator-(Time t1, Time t2) {
	Time result;
	result.second = t1.second - t2.second;
	result.minute = t1.minute - t2.minute;
	result.hour = t1.hour - t2.hour;
	result.day = t1.day - t2.day;
	result.month = t1.month - t2.month;
	result.year = t1.year - t2.year;
	correctTime(result.second, result.minute, 0);
	correctTime(result.minute, result.hour, 1);
	correctTime(result.hour, result.day, 2);
	correctTime(result.day, result.month, 3, t2.month);
	correctTime(result.month, result.year, 4);
	return result;
}

Time operator+(Time t1, Time t2) {
	Time result;
	result.second = t1.second + t2.second;
	result.minute = t1.minute + t2.minute;
	result.hour = t1.hour + t2.hour;
	result.day = t1.day + t2.day;
	result.month = t1.month + t2.month;
	result.year = t1.year + t2.year;
	correctTime(result.second, result.minute, 0);
	correctTime(result.minute, result.hour, 1);
	correctTime(result.hour, result.day, 2);
	correctTime(result.day, result.month, 3);
	correctTime(result.month, result.year, 4);
	return result;
}

//Determines whether a member of a Time structure is the last to have a value greater than zero
bool isLast(Time t, int pos) {
	int values[4] = {t.second, t.minute, t.hour, t.day};
	for (int i = 0; i < pos; i++) {
		if (values[i] > 0) return false;
	}
	return true;
}

std::ostream& operator<<(std::ostream& os, Time t) {
	bool use_comma = false;
	if (t.year > 0) {
		os<<t.year<<" year"<<(t.year > 1 ? "s" : "");
		use_comma = true;
	}
	if (t.month > 0) {
		os<<(use_comma ? (isLast(t, 4) ? ", and " : ", ") : "")<<t.month<<" month"<<(t.month > 1 ? "s" : "");
		use_comma = true;
	}
	if (t.day > 0) {
		os<<(use_comma ? (isLast(t, 3) ? ", and " : ", ") : "")<<t.day<<" day"<<(t.day > 1 ? "s" : "");
		use_comma = true;
	}
	if (t.hour > 0) {
		os<<(use_comma ? (isLast(t, 2) ? ", and " : ", ") : "")<<t.hour<<" hour"<<(t.hour > 1 ? "s" : "");
		use_comma = true;
	}
	if (t.minute > 0) {
		os<<(use_comma ? (isLast(t, 1) ? ", and " : ", ") : "")<<t.minute<<" minute"<<(t.minute > 1 ? "s" : "");
		use_comma = true;
	}
	if (t.second > 0) os<<(use_comma ? ", and " : "")<<t.second<<" second"<<(t.second > 1 ? "s" : "");
	else if (!use_comma) os<<"0 hours";
	return os;
}

/*
Possible printouts:
mm/dd/yyyy hh:mm:ss

mm/dd/yyyy hh:mm:ss - hh:mm:ss
OR
mm/dd/yyyy hh:mm:ss - mm/dd/yyyy hh:mm:ss
if it goes to the next day

NAME has a total time of HOURS hours.
*/

//Prints out a person's sign-ins and sign-outs
void printSignInsOuts(std::pair<std::vector<Time>, std::vector<Time> > &sign_ins_outs) {
	for(unsigned int i = 0; i < sign_ins_outs.first.size(); i++) {
		std::cout<<std::endl<<"Signed in:  "<<formatTimeAlt(sign_ins_outs.first[i])<<std::endl;
		if (i < sign_ins_outs.second.size()) std::cout<<"Signed out: "<<formatTimeAlt(sign_ins_outs.second[i])<<std::endl;
	}
}

//Computes the total amount of time a person has been signed in
Time getTotalTime(std::pair<std::vector<Time>, std::vector<Time> > &sign_ins_outs) {
	Time total;
	for (unsigned int i = 0; i < sign_ins_outs.second.size(); i++) {
		total = total + (sign_ins_outs.second[i] - sign_ins_outs.first[i]);
	}
	return total;
}

int main() {
	bool quitting = false;
	
	while(!quitting) {
		std::cout<<std::endl<<"Please enter a first and last name for the matching log records, or \"quit\" to quit: ";
		std::string name;
		getline(std::cin, name);
		
		if (name != "quit") {
			bool valid_name = checkName(name);
			if (valid_name) {
				std::pair<std::vector<Time>, std::vector<Time> > sign_ins_outs = getSignInsOuts(name);
				printSignInsOuts(sign_ins_outs);
				//Config option for hours vs extended time
				Time total_time = getTotalTime(sign_ins_outs);
				//Make this a flag
				//std::cout<<std::endl<<name<<" has a total time of "<<total_time<<"."<<std::endl;
				std::cout<<std::endl<<name<<" has a total time of "<<total_time.hour<<" hour"<<((total_time.hour != 1) ? "s" : "")<<"."<<std::endl;
			} else {
				std::cout<<std::endl<<"Invalid entry.  Either the name was entered incorrectly or no such name is stored in the system."<<std::endl;
			}
			std::cout<<"---------------------------------------------------------------";
		} else {
			quitting = true;
		}
	}
	
	return 0;
}