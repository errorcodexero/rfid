#include "rfid.h"

/*
Possible printouts:
mm/dd/yyyy hh:mm:ss

mm/dd/yyyy hh:mm:ss - hh:mm:ss
OR
mm/dd/yyyy hh:mm:ss - mm/dd/yyyy hh:mm:ss
if it goes to the next day

NAME has a total time of HOURS hours.
*/

//Prints out a person's sign-ins and sign-outs
void printSignInsOuts(std::pair<std::vector<Time>, std::vector<Time> > &sign_ins_outs) {
	for(unsigned int i = 0; i < sign_ins_outs.first.size(); i++) {
		std::cout<<std::endl<<"Signed in:  "<<formatTimeAlt(sign_ins_outs.first[i])<<std::endl;
		if (i < sign_ins_outs.second.size()) std::cout<<"Signed out: "<<formatTimeAlt(sign_ins_outs.second[i])<<std::endl;
	}
}

int main() {
	bool quitting = false;
	
	while(!quitting) {
		std::cout<<std::endl<<"Please enter a first and last name for the matching log records, or \"quit\" to quit: ";
		std::string name;
		getline(std::cin, name);
		
		if (name != "quit") {
			bool valid_name = checkName(name);
			if (valid_name) {
				std::pair<std::vector<Time>, std::vector<Time> > sign_ins_outs = getSignInsOuts(name);
				printSignInsOuts(sign_ins_outs);
				Time total_time = getTotalTime(sign_ins_outs);
				//Config option for hours vs extended time
				//Make this a flag
				//std::cout<<std::endl<<name<<" has a total time of "<<total_time<<"."<<std::endl;
				std::cout<<std::endl<<name<<" has a total time of "<<getTimeInHours(total_time)<<" hour"<<((getTimeInHours(total_time) != 1) ? "s" : "")<<"."<<std::endl;
			} else {
				std::cout<<std::endl<<"Invalid entry.  Either the name was entered incorrectly or no such name is stored in the system."<<std::endl;
			}
			std::cout<<"---------------------------------------------------------------";
		} else {
			quitting = true;
		}
	}
	
	return 0;
}
