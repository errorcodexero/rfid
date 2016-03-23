#include "rfid.h"

bool vectorIncludes(std::vector v, int i) {
	//Iterate over members of v and check if i is on of them
	return false;
}

int getLineCount() {
	return 4;
}

int main() {
	std::ifstream log("log.txt");
	Time last_difference;
	bool first = true;
	std::vector<Time> broken_times;
	std::vector<int> lines;
	int line_number = 0;
	while (line_number != getLineCount) {
		while (!log.eof()) {
			if (!vectorIncludes(lines, line_number)) {
				std::string line;
				getline(log, line);
				Time line_time = parseFormattedTime(line.substr(line.find("=") + 2));
				Time difference = getTime() - line_time;
				if (!first && (last_difference < difference)) {
					broken_times.push_back(line_time);
					lines.push_back(line_number);
				}
				last_difference = difference;
				line_number++;
				first = false;
			}
		}
	}
	//removeLines(lines);
	for (Time t : broken_times) {
		std::ifstream log("log.txt");
		int line_number = 0;
		Time last_difference;
		bool first = true;
		while (!log.eof()) {
			std::string log_line;
			getline(log, log_line);
			Time difference = t - parseFormattedTime(log_line.substr(log_line.find("=") + 2));
			if (!first && (last_difference < difference)) break;
			last_difference = difference;
			first = false;
			line_number++;
		}
		std::cout<<line_number<<"       "<<t<<std::endl;
		//logAttendance(name, line_number, t);
		log.close();
	}
	return 1;
}