#include "rfid.h"

void backLine(std::ifstream& i, int line_length) {
	i.seekg(-line_length, std::ios_base::cur);
	std::cout<<i.peek()<<std::endl;
}
//move to last line (name)
//seek back line length
//unget
//unget until peek is \n
//seek forward 2

template<typename T>
bool vectorIncludes(std::vector<T> v, T i) {
	//Iterate over members of v and check if i is on of them
	return false;
}

int getLineCount() {
	return 4;
}

int main() {
	std::ifstream log("log.txt");
	Time last_difference, current_time = getTime();
	bool first = true, moving_back = false;
	int line_number = 0;
	int line_length;
	std::vector<Time> broken_times;
	std::vector<int> lines;
	//while (line_number != getLineCount) {
	while (!log.eof()) {
		//if (!vectorIncludes(lines, line_number)) {
		std::string line;
		getline(log, line);
		Time line_time = parseFormattedTime(line.substr(line.find("=") + 2));
		Time difference = current_time - line_time;
		if (!first && (last_difference < difference)) {
			if (!moving_back) {
				broken_times.push_back(line_time);
				lines.push_back(line_number);
				//std::cout<<line_number<<std::endl;
				backLine(log, line_length);//maybe just line.length
				break;
				//std::cout<<log.tellg()<<std::endl;
				//difference to lastdifference
				//if moving back, don't change lastdifference
				//bool for moving back
				//function
			} else {
				
			}
		} else {
			last_difference = difference;
			line_number++;
		}
		line_length = line.size();
		first = false;
		//}
	}
	for (int i : lines) {
		std::cout<<i<<std::endl;
	}
	//std::cout<<broken_times.size()<<"     "<<lines.size()<<std::endl;
	//}
	//removeLines(lines);
	/*for (Time t : broken_times) {
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
	}*/
	
	return 1;
}
