#include <sstream>

#include "rfid.h"

Time::Time() {
	year = 0;
	month = 0;
	day = 0;
	hour = 0;
	minute = 0;
	second = 0;
}

Time::Time(int y, int mo, int d, int h, int m, int s) {
	year = y;
	month = mo;
	day = d;
	hour = h;
	minute = m;
	second = s;
}

//Returns the amount of whatever unit of time are in the next unit of time
int getTimePosValue(int pos, int month = 1, int year = 0) {
	int pos_value = time_pos_values[pos];
	if (pos_value == CHECK_MONTH_DAYS) {
		int array_month = month - 1;
		int days = month_days[array_month];
		return ((month==2) && (year%4==0)) ? (days + 1) : days;
	}
	return pos_value;
}

//Corrects for going over into the next of a larger unit of time by making adjustments to other members of the Time structure
void correctTime(int &val1, int &val2, int pos, int month = 1, int year = 0) {
	int pos_value = (pos == 3) ? getTimePosValue(3, month, year) : getTimePosValue(pos); 
	if (val1 < 0) {
		val1 = pos_value + val1;
		val2 -= 1;
	} else if (val1 >= pos_value) {
		val1 = val1 - pos_value;
		val2 += 1;
	}
}

void correctYear(int &val) {
	if (val < 0) {
		val = 0;
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
	correctTime(result.day, result.month, 3, t2.month, t2.year);
	correctTime(result.month, result.year, 4);
	correctYear(result.year);
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

bool operator<(Time t1, Time t2) {
	if (t1.year < t2.year) {
		return true;
	} else if (t1.month < t2.month && t1.year == t2.year) {
		return true;
	} else if (t1.day < t2.day && t1.month == t2.month && t1.year == t2.year) {
		return true;
	} else if (t1.hour < t2.hour && t1.day == t2.day && t1.month == t2.month && t1.year == t2.year) {
		return true;
	} else if (t1.minute < t2.minute && t1.hour == t2.hour && t1.day == t2.day && t1.month == t2.month && t1.year == t2.year) {
		return true;
	} else if (t1.second < t2.second && t1.minute == t2.minute && t1.hour == t2.hour && t1.day == t2.day && t1.month == t2.month && t1.year == t2.year) {
		return true;
	}
	return false;
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

//Gets the current time and returns a Time structure filled in with the correct values
Time getTime() {
	time_t rawtime;
	struct tm * ptm;
	time (&rawtime);
	ptm = localtime(&rawtime);
	Time t;
	t.year = (ptm->tm_year) + 1900;
	t.month = (ptm->tm_mon) + 1;
	t.day = ptm->tm_mday;
	t.hour = (ptm->tm_hour);
	t.minute = ptm->tm_min;
	t.second = ptm->tm_sec;
	return t;
}

//Removes line break characters from a string
void removeLineBreaks(std::string &str) {
	for (unsigned int i=0; i < str.size(); i++){
		if (str[i] == '\n' || str[i] == '\r') str.erase(str.begin() + i);
	}
}

//Checks to see if an entered name is valid
bool checkName(std::string &name) {
	std::string name_use = name;
	std::transform(name_use.begin(), name_use.end(), name_use.begin(), ::tolower);
	std::string line;
	std::string line_normal;
	std::ifstream ids_and_names("ids_and_names.txt");
	while (!ids_and_names.eof()) {
		getline(ids_and_names, line);
		removeLineBreaks(line);
		line_normal = line;
		std::transform(line.begin(), line.end(), line.begin(), ::tolower);
		if (line.find(name_use) != std::string::npos) {
			if ((line.substr(line.find(name_use)) == name_use) && (line.find(name_use) == (line.find("=") + 2))) {
				//std::cout<<"BEEP BEEP BOOP"<<std::endl;
				ids_and_names.close();
				name = line_normal.substr(line.find(name_use));
				return true;
			}
		}
	}
	ids_and_names.close();
	return false;
}

//Formats the time from a Time structure to a string to be used in the log - hh:mm:ss mm/dd/yyyy
std::string formatTime(Time time) {
	std::stringstream formatted_time;
	if (time.hour < 10) formatted_time<<0;
	formatted_time<<time.hour<<":";
	if (time.minute < 10) formatted_time<<0;
	formatted_time<<time.minute<<":";
	if (time.second < 10) formatted_time<<0;
	formatted_time<<time.second<<" ";
	if (time.month < 10) formatted_time<<0;
	formatted_time<<time.month<<"/";
	if (time.day < 10) formatted_time<<0;
	formatted_time<<time.day<<"/";
	formatted_time<<time.year;
	return formatted_time.str();
}

//Formats the time from a Time structure to a string to be used when printing to the console - mm/dd/yyyy hh:mm:ss
std::string formatTimeAlt(Time time) {
	std::stringstream formatted_time;
	if (time.month < 10) formatted_time<<0;
	formatted_time<<time.month<<"/";
	if (time.day < 10) formatted_time<<0;
	formatted_time<<time.day<<"/";
	formatted_time<<time.year<<" ";
	if (time.hour < 10) formatted_time<<0;
	formatted_time<<time.hour<<":";
	if (time.minute < 10) formatted_time<<0;
	formatted_time<<time.minute<<":";
	if (time.second < 10) formatted_time<<0;
	formatted_time<<time.second;
	return formatted_time.str();
}

//Breaks up a formatted time and creates a Time out of it
Time parseFormattedTime(std::string time_string) {
	Time t;
	t.hour = (int) strtol(time_string.substr(0, 2).c_str(), nullptr, 10);
	t.minute = (int) strtol(time_string.substr(3, 2).c_str(), nullptr, 10);
	t.second = (int) strtol(time_string.substr(6, 2).c_str(), nullptr, 10);
	t.month = (int) strtol(time_string.substr(9, 2).c_str(), nullptr, 10);
	t.day = (int) strtol(time_string.substr(12, 2).c_str(), nullptr, 10);
	t.year = (int) strtol(time_string.substr(15, 4).c_str(), nullptr, 10);
	return t;
}

//Gets the sign-ins and sign-outs of the given person
std::pair<std::vector<Time>, std::vector<Time> > getSignInsOuts(std::string name, std::string file) {
	std::ifstream log(file);
	std::vector<Time> sign_ins, sign_outs;
	bool sign_in = true;
	std::string line;
	while (!log.eof()) {
		getline(log, line);
		removeLineBreaks(line);
		if (line.find(name) != std::string::npos) {
			std::string time_string = line.substr(line.find("=") + 2);
			(sign_in ? sign_ins : sign_outs).push_back(parseFormattedTime(time_string));
			sign_in = !sign_in;
		}
	}
	return std::make_pair(sign_ins, sign_outs);
}

//Adds a line to a file at the given line
//line_number is zero-indexed, a value of -1 adds to the end of the file
int addLineToFile(std::string file_name, std::string str, int line_number) {
	std::vector<std::string> last_lines;
	int line_sizes = 0;
	
	std::ifstream file_in(file_name);
	int curr_line = 0;
	while (!file_in.eof()) {
		std::string line;
		getline(file_in, line);
		if ((curr_line >= line_number) && (line_number != -1)) {
			last_lines.push_back(line);
		} else {
			line_sizes += line.size() + 1;
		}
		curr_line++;
	}
	file_in.close();
	
	if (last_lines.size() != 0) {
		str.append("\r\n");
	} else if (curr_line < line_number) {//Line greater than number of lines in file - add blank lines to this line instead? (do below)
		return 1;
	} else {//Line is after the last line
		str.insert(0, "\r\n");
		line_sizes--;
	}
	
	std::ofstream file_app(file_name, std::ios::app);
	for (unsigned int i = 0; i < str.size(); i++) {
		file_app<<" ";
	}
	file_app.close();
	
	std::fstream file_out(file_name, std::ios::out | std::ios::in);
	file_out.seekp(line_sizes, std::ios::beg);
	file_out.write(str.c_str(), str.size());
	file_out.seekp(line_sizes + str.size(), std::ios::beg);
	for (unsigned int i = 0; i < last_lines.size(); i++) {
		std::string line = last_lines[i];
		if (i != (last_lines.size() - 1)) line.append("\n");
		file_out.write(line.c_str(), line.size());
	}
	file_out.close();
	return 0;
}

//Prints a line in the log file with the person's name and the current time and tells the user
void logAttendance(std::string name, int line_number, Time time) {
	std::string log_line = name;
	for (int i = name.size(); i < NAME_SPACE; i++) log_line.append(" ");
	log_line.append("= ");
	log_line.append(formatTime(time));
	addLineToFile("log.txt", log_line, line_number);
	std::pair<std::vector<Time>, std::vector<Time> > sign_ins_outs = getSignInsOuts(name);
	std::string sign_in_or_out = (sign_ins_outs.first.size() == sign_ins_outs.second.size()) ? "out" : "in";
	std::cout<<"Signed "<<sign_in_or_out<<" "<<name<<" at "<<formatTimeAlt(time)<<"."<<std::endl;
}

void logAttendance(std::string name, Time time, std::string file_name) {
	std::string log_line = name;
	for (int i = name.size(); i < NAME_SPACE; i++) log_line.append(" ");
	log_line.append("= ");
	log_line.append(formatTime(time));
	addLineToFile(file_name, log_line);
	std::pair<std::vector<Time>, std::vector<Time> > sign_ins_outs = getSignInsOuts(name, file_name);
	std::string sign_in_or_out = (sign_ins_outs.first.size() == sign_ins_outs.second.size()) ? "out" : "in";
	std::cout<<"Signed "<<sign_in_or_out<<" "<<name<<" at "<<formatTimeAlt(time)<<"."<<std::endl;
}

//Sort the times and names by the time
void sortSignInsOuts(std::pair<std::vector<Time>, std::vector<std::string> > &rankings) {
	for (unsigned int i = 0; i < (rankings.first.size() - 1); i++) {
		for (unsigned int j = (i + 1); j < rankings.first.size(); j++) {
			if (rankings.first[i] < rankings.first[j]) {
				std::swap(rankings.first[i], rankings.first[j]);
				std::swap(rankings.second[i], rankings.second[j]);
			}
		}
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

//Converts from a Time to hours
int getTimeInHours(Time t) {
	return (int) ((t.year * 8760) + (t.month * 720) + (t.day * 24) + t.hour + (t.minute / 60) + (t.second / 3600));
}

//Converts from a Time to days
int getTimeInDays(Time t) {
	return (int) ((t.year * 365) + (t.month * 30) + t.day + (t.hour / 24) + (t.minute / 1440) + (t.second / 86400));
}