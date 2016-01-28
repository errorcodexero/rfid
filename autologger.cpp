#include "rfid.h"

int main() {
	std::ifstream names("ids_and_names.txt");
	std::string line, name;
	while (!names.eof()) {
		getline(names, line);
		removeLineBreaks(line);
		name = line.substr(line.find("=") + 2);
		assert(checkName(name));
		std::pair<std::vector<Time>, std::vector<Time> > sign_ins_outs = getSignInsOuts(name);
		if (sign_ins_outs.first.size() != sign_ins_outs.second.size()) {
			logAttendance(name);
		}
	}
	names.close();
	return 0;
}
