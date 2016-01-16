#include "rfid.h"

void sortSignInsOuts(std::pair<std::vector<Time>, std::vector<std::string> > &rankings) {
	for (unsigned int i = 0; i < (rankings.first.size() - 1); i++) {
		for (unsigned int j = (i + 1); j < rankings.first.size(); j++) {
			if (rankings.first[i] < rankings.first[j]) {
				Time temp = rankings.first[i];
				rankings.first[i] = rankings.first[j];
				rankings.first[j] = temp;
				
				//std::swap(rankings.first[j], rankings.first[j + 1]);
				//std::swap(rankings.second[j], rankings.second[j + 1]);
			}
		}
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
		std::cout<<rankings.first.back()<<std::endl;
	}
	sortSignInsOuts(rankings);
	for(unsigned int i = 0; i < rankings.first.size(); i++) {
		std::cout<<i + 1<<". "<<rankings.second[i];
		for (unsigned int j = (rankings.second[i].size() + (int) log10((double) (i + 1))); j < NAME_SPACE; j++) std::cout<<" ";
		//Config option/flag for hours/extended
		std::cout<<"- "<<rankings.first[i].hour<<" hour"<<((rankings.first[i].hour != 1) ? "s" : "")<<std::endl;
	}
	names.close();
	return 0;
}
