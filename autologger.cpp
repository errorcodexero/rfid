#include "rfid.h"

//line_number is zero-indexed
int addLineToFile(std::string file_name, std::string str, int line_number) {
	std::vector<std::string> last_lines;
	int line_sizes = 0;
	
	std::ifstream file_in(file_name);
	int curr_line = 0;
	while (!file_in.eof()) {
		std::string line;
		getline(file_in, line);
		if (curr_line >= line_number) {
			last_lines.push_back(line);
		} else {
			line_sizes += line.size() + 1;
		}
		curr_line++;
	}
	file_in.close();
	
	if (last_lines.size() != 0) {
		str.append("\r\n");
	} else if (curr_line < line_number) {//Line greater than number of lines in file - add blank lines to this line instead? (do below)
		return 1;
	} else {//Line is after the last line
		str.insert(0, "\r\n");
		line_sizes--;
	}
	
	std::ofstream file_app(file_name, std::ios::app);
	for (unsigned int i = 0; i < str.size(); i++) {
		file_app<<" ";
	}
	file_app.close();
	
	std::fstream file_out(file_name, std::ios::out | std::ios::in);
	file_out.seekp(line_sizes, std::ios::beg);
    file_out.write(str.c_str(), str.size());
	file_out.seekp(line_sizes + str.size(), std::ios::beg);
	for (unsigned int i = 0; i < last_lines.size(); i++) {
		std::string line = last_lines[i];
		if (i != (last_lines.size() - 1)) line.append("\n");
		file_out.write(line.c_str(), line.size());
	}
	file_out.close();
	return 0;
}

int main() {
	std::ifstream names("ids_and_names.txt");
	std::ofstream log("log.txt", std::ios::app);
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
	log.close();
	return 0;
}
