#include "rfid.h"

int main(int arc, char** argv) {
	std::string filename = "log.txt";
	if (arc > 1) filename = argv[1];
	std::ifstream log(filename);
	if (!log.good()) {
		std::cout<<"Error opening log file."<<std::endl;
		return 1;
	}
	int line_number = 0;
	Time last_difference;
	Time difference;
	bool first = true;
	while (!log.eof()) {
		std::string line;
		getline(log, line);
		difference = getTime() - parseFormattedTime(line.substr(line.find("=") + 2));
		if (!first && (last_difference < difference)) break;
		last_difference = difference;
		first = false;
		line_number++;
	}
	log.close();
	std::cout<<"At line number "<<line_number<<std::endl;
	return 0;
}