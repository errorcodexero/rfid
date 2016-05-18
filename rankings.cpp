#include "rfid.h"

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
		std::pair<std::vector<Time>, std::vector<Time> > shop_sign_ins_outs  = getSignInsOuts(name, "shop_log.txt");
		rankings.first.push_back(getTotalTime(sign_ins_outs) + getTotalTime(shop_sign_ins_outs));
		rankings.second.push_back(name);
	}
	sortSignInsOuts(rankings);
	for(unsigned int i = 0; i < rankings.first.size(); i++) {
		std::cout<<i + 1<<". "<<rankings.second[i];
		for (unsigned int j = (rankings.second[i].size() + (int) log10((double) (i + 1))); j < NAME_SPACE; j++) std::cout<<" ";
		//Config option/flag for hours/extended
		std::cout<<"- "<<getTimeInHours(rankings.first[i])<<" hour"<<((getTimeInHours(rankings.first[i]) != 1) ? "s" : "")<<std::endl;
	}
	names.close();
	return 0;
}
