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