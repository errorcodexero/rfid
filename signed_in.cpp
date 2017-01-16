#include "rfid.h"

int main() {
	std::ifstream names("ids_and_names.txt");
	while (!names.eof()) {
		std::string line, name;
		getline(names, line);
		removeLineBreaks(line);
		name = line.substr(line.find("=") + 2);
		assert(checkName(name));
		std::pair<std::vector<Time>, std::vector<Time> > sign_ins_outs = getSignInsOuts(name);
		sign_ins_outs = getSignInsOuts(name, "log.txt");
		if (sign_ins_outs.first.size() > sign_ins_outs.second.size()) std::cout<<name<<std::endl;
	}
	return 0;
}