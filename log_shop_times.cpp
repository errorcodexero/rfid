#include "rfid.h"

int main() {
	Time t;
	std::string input;
	std::pair<std::vector<Time>, std::vector<std::string> > times;
	#define GET_NUM(name, num) std::cout<<#name": "; \
	getline(std::cin, input); \
	t.num = stoi(input);
	GET_NUM(Month, month);
	GET_NUM(Day, day);
	GET_NUM(Year, year);
	while(true) {
		std::string name;
		std::cout<<"Name: ";
		getline(std::cin, name);
		if (!checkName(name)) std::cout<<"Invalid name."<<std::endl;
		else {
			std::cout<<"In:"<<std::endl;
			GET_NUM(Hour, hour);
			GET_NUM(Minute, minute);
			times.first.push_back(t);
			times.second.push_back(name);
			std::cout<<"Out:"<<std::endl;
			GET_NUM(Hour, hour);
			GET_NUM(Minute, minute);
			times.first.push_back(t);
			times.second.push_back(name);
		}
		std::cout<<"Continue (y/n)? ";
		getline(std::cin, input);
		if (input == "n") break;
	}
	if (times.first.size() % 2 != 0) {
		std::cout<<"Something went wrong when entering times! Sorry :("<<std::endl;
		return 1;
	}
	sortSignInsOuts(times);
	for(unsigned i = times.first.size() - 1; i > 0; i--) {
		logAttendance(times.second[i], times.first[i], "shop_log.txt");
	}
	return 0;
}