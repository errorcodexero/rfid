#include "rfid.h"

void sortSignInsOuts(std::pair<std::vector<Time>, std::vector<std::string> > &rankings) {
	for (unsigned int i = 0; i < rankings.first.size(); i++) {
		
	}
}

int main() {
	std::cout<<"Time Rankings: "<<std::endl;
	std::ifstream names("ids_and_names.txt");
	std::pair<std::vector<Time>, std::vector<std::string> > rankings; //Total time, name
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
	sortSignInsOuts(rankings);
	for(unsigned int i = 0; i < rankings.first.size(); i++) {
		for (unsigned int i = rankings.second[i].size(); i < NAME_SPACE; i++) std::cout<<" ";
		//Config option/flag for hours/extended
		std::cout<<i<<". "<<rankings.second[i].size()<<"- "<<rankings.first[i].hour<<" hour"<<((rankings.first[i].hour != 1) ? "s" : "")<<std::endl;
	}
	names.close();
	return 0;
}
