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
			Time last_time = sign_ins_outs.first.back();
			std::ifstream log("log.txt");
			int line_number = 0;
			while (!log.eof()) {
				std::string log_line;
				getline(log, log_line);
				if (log_line.find(formatTime(last_time)) != std::string::npos) break;
				line_number++;
			}
			log.close();
			Time to_add(0, 0, 0, HOURS_TO_ADD, 0, 0);
			Time out_time = last_time + to_add;
			logAttendance(name, line_number + 1, out_time);
		}
	}
	names.close();
	return 0;
}
