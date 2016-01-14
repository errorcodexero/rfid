#include "rfid.h"

std::pair<std::vector<Time>, std::string> sortSignInsOuts(std::pair<std::vector<Time>, std::string> rankings) {
	for (int i = 0; i < rankings.first.size(); i++) {
		
	}
}

int main() {
	std::cout<<"Time Rankings: "<<std::endl;
	std::ifstream names("ids_and_names.txt");
	std::pair<std::vector<Time>, std::string> rankings; //Total time, name
	std::string line, name;
	while (!names.eof()) {
		getline(names, line);
		removeLineBreaks(line);
		name = line.substr(line.find("=") + 2);
		assert(checkName(name));
		std::pair<std::vector<Time>, std::vector<Time> > sign_ins_outs = getSignInsOuts(name);
		rankings.first.push_back(getTotalTime(sign_ins_outs));
		rankings.second.push_back(name);
	}
	rankings = sortSignInsOuts(rankings);
	for(int i = 0; i < rankings.first.size(); i++) {
		for (int i = rankings.second[i].size(); i < NAME_SPACE; i++) log<<" ";
		//Config option/flag for hours/extended
		std::cout<<rankings.second[i].size()<<"- "<<rankings.first[i].hour<<" hour"<<((rankings.first[i].hour != 1) ? "s" : "")<<std::endl;
	}
	names.close();
	return 0;
}
