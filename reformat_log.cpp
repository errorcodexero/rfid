//Reformats the old log version to work with the new stuff
#include "rfid.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout<<"Requires the file to update as an argument"<<std::endl;
		return 1;
	}
	std::ifstream log(argv[1]);
	if (!log.is_open()) {
		std::cout<<"Not a valid file"<<std::endl;
		return 1;
	}
	std::string new_name = argv[1];
	int extension = new_name.find(".");
	new_name.insert(((extension != -1) ? extension : new_name.size()), "_reformatted");
	std::ofstream reformatted(new_name);
		
	reformatted.close();
	log.close();
	std::cout<<"Reformatted log: "<<new_name<<std::endl;
	return 0;
}
