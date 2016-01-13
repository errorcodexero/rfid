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
std::pair<std::vector<Time>, std::vector<Time> > getSignInsOuts(std::string name) {
	std::ifstream log("log.txt");
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

//Prints a line in the log file with the person's name and the current time and tells the user
void logAttendance(std::string name) {
	Time time = getTime();
	std::string formatted_time = formatTime(time);
	std::ofstream log("log.txt", std::ofstream::app);
	log<<"\r\n"<<name;
	for (int i = name.size(); i < NAME_SPACE; i++) log<<" ";
	log<<"="<<" "<<formatted_time;
	log.close();
	std::pair<std::vector<Time>, std::vector<Time> > sign_ins_outs = getSignInsOuts(name);
	std::string sign_in_or_out = (sign_ins_outs.first.size() == sign_ins_outs.second.size()) ? "out" : "in";
	std::cout<<"Signed "<<sign_in_or_out<<" "<<name<<" at "<<formatTimeAlt(time)<<"."<<std::endl;
}