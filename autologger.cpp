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
			Time to_add(0, 0, 0, HOURS_TO_ADD, 0, 0);
			Time out_time = sign_ins_outs.first.back() + to_add;
			std::ifstream log("log.txt");
			int line_number = 0;
			Time last_difference;
			bool first = true;
			while (!log.eof()) {
				std::string log_line;
				getline(log, log_line);
				Time difference = out_time - parseFormattedTime(log_line.substr(log_line.find("=") + 2));
				std::cout<<last_difference<<"    "<<difference<<"    "<<last_difference.year<<"    "<<difference.year<<std::endl;
				if (!first && (last_difference < difference)) break;
				last_difference = difference;
				first = false;
				line_number++;
			}
			log.close();
			std::cout<<"Line number: "<<line_number<<std::endl;
			logAttendance(name, line_number, out_time);
		}
	}
	names.close();
	return 0;
}
