#include "rfid.h"

int main() {
	std::ifstream log("log.txt");
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
	std::cout<<"Difference: "<<difference<<std::endl;
	return 0;
}