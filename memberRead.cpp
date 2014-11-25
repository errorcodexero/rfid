// • Project: A draft for an RFID input based attendance program. 
// • Author(s): Matthew Macovsky. FIRST Team #1425 "ERROR CODE XERO".
// • Description: A program that will take an input identification from an RFID tag and input it into a log.
// • WARNING: Is inaccurate after a continuous signed-in period of more than 1 month.

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

//full month/leapyear functionality (low priority)
//use typedefs for vector<int> s
//check time_string is in correct format

//Returns amount of unit given in next unit up
int getUnitSize(int j) {
	switch (j) {
		case 1: //months
			return 12;
		case 2: //days
			if (j == 2) { //months are numbered from 1
				return 28;
			} else if (j < 8){
				return ((j % 2) ? 31 : 30);
			} else {
				return ((j % 2) ? 30 : 31);
			}
			break;
		case 3: //hours
			return 24;
			break;
		default: //minutes/seconds
			return 60;
			break;
	}
}

vector<string> spaceParse(string parsing_string) {
	vector<string> sections;
	unsigned int string_place = 0;
	stringstream section;
	string substring;
	while (string_place < parsing_string.size()) {
		substring = parsing_string.substr(string_place, 1);
		if ((substring != " ") && (substring != ":")) {
			section<<substring;
		} else {
			if ((section.str()).size() > 0) sections.push_back(section.str());
			section.str("");
		}
		string_place += 1;
	}
	sections.push_back(section.str());
	section.str("");
	return sections;
}

vector<int> parseTime(string time_string) {//time_string should be in "DAY MON dd yyyy hh:mm:ss" format
	vector<int> numbers;
	vector<string> time_sections = spaceParse(time_string);
	for(unsigned int i = 1; i<time_sections.size(); i++) {
		if (i == 1) {
			vector<string> months {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
			for (int k = 0; k<12; k++) {
				if (time_sections[1] == months[k]) numbers.push_back(k + 1);
			}
		} else {
			numbers.push_back(atoi(time_sections[i].c_str()));
		}
	}
	swap(numbers[0], numbers[2]);
	swap(numbers[1], numbers[2]);
	return numbers;
}

vector<string> memberRead(string person_name){//Reads out (from the log) the times the person was present.
	ifstream logOfTimes("logOfTimes.txt");
	vector<string> sign_ins;
	vector<string> sign_outs;
	vector<vector<int> > times;
	vector<int> final_times {0, 0, 0, 0, 0, 0};
	vector<int> times_in;
	vector<int> times_out;
	vector<int> place;
	string line;
	string item_name;
	//Get total amount of time
	while(!(logOfTimes.eof())) {
		getline(logOfTimes, line);
		std::size_t found = line.find(person_name);
		if ((found != std::string::npos) && (line.substr(person_name.size(), 1) == "=")) {
			int time_place = (line.find("=", (person_name.size() + 1)) + 1);
			int real_length = distance(line.begin(), line.end());
			string time = line.substr(time_place, (real_length - time_place));
			if (sign_ins.size() == sign_outs.size()) {
				sign_ins.push_back(time);
				times_in = parseTime(time);
			} else {
				sign_outs.push_back(time);
				times_out = parseTime(time);
				times.push_back(place);
				for (int i = 0; i < 6; i++) {
					times[times.size() - 1].push_back(times_out[i] - times_in[i]);
				}
			}
		}
	}
	vector<string> output_array;
	//Prepare output
	if (sign_ins.size() != 0) {
		for (unsigned int i = 0; i < times.size(); i++) {
			for (int j = 5; j > -1; j--) {
				if (times[i][j] < 0) {
					times[i][j - 1] -= 1;
					times[i][j] = (getUnitSize(j) - (times[i][j] * -1));
				}
				final_times[j] += times[i][j];
				while (final_times[j] > getUnitSize(j)) {
					final_times[j - 1] += 1;
					final_times[j] -= getUnitSize(j);
				}
			}
		}
		stringstream new_output;
		unsigned int sign_ins_place = 0;
		unsigned int sign_outs_place = 0;
		new_output<<"Sign-ins and sign-outs for "<<person_name<<":"<<endl;
		output_array.push_back(new_output.str());
		new_output.str("");		
		while (((sign_ins_place + 1) <= sign_ins.size()) || ((sign_ins_place + 1)) <= sign_outs.size()) {
			new_output<<"Signed in: "<<sign_ins[sign_ins_place]<<endl;
			sign_ins_place++;
			if ((sign_outs_place + 1) <= sign_outs.size()) {
				new_output<<"Signed out: "<<sign_outs[sign_outs_place]<<endl;
				sign_outs_place++;
			}
			output_array.push_back(new_output.str());
			new_output.str("");
		}
		if (sign_outs.size() > 0) {
			new_output<<"This person has a total attendance time of ";
			for (int i = 0; i < 6; i++) {
				if (final_times[i] != 0) {
					if (new_output.str() != "This person has a total attendance time of ") new_output<<", ";
					new_output<<final_times[i];				
					switch (i) {
						case 0:
							new_output<<" year";
							break;
						case 1:
							new_output<<" month";
							break;
						case 2:
							new_output<<" day";
							break;
						case 3:
							new_output<<" hour";
							break;
						case 4:
							new_output<<" minute";
							break;
						case 5:
							new_output<<" second";
							break;
					}
					if (final_times[i] > 1) new_output<<"s";
				}
			}
			new_output<<".";
			output_array.push_back(new_output.str());
		}
	} else {
		output_array.push_back("This person has no attendance times on record.");
	}
	return output_array;
}

int main(){//Input the RFID.
	while(1){
		cout<<"Please input a name to view their times logged or \"exit\" to exit: ";
		string name;
		getline(cin,name);
		vector<string> to_print = memberRead(name);
		if(name=="exit")break;
		while(to_print.size() != 0) {
			cout<<to_print[0]<<endl;
			to_print.erase(to_print.begin());
		}
		cout<<endl;
	}
	return 0;
}