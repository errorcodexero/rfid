#include "rfid.h"

int main() {
	std::ifstream names("ids_and_names.txt");
	std::ofstream log("log.txt", std::ios::app);
	std::string line;
	std::string name;
	while (!names.eof()) {
		getline(names, line);
		removeLineBreaks(line);
		name = line.substr(line.find("=") + 2);
		std::pair<std::vector<Time>, std::vector<Time> > sign_ins_outs = getSignInsOuts(name);
		if (sign_ins_outs.first.size() != sign_ins_outs.second.size()) {
			logAttendance(name);
		}
	}
	return 1;
}