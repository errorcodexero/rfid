// • Project: A draft for an RFID input based attendance program. 
// • Author(s): Adrian Hardt, Matthew Macovsky, and Logan Traffas. FIRST Team #1425 "ERROR CODE XERO".
// • Description: A program that will take an input identification from an RFID tag and input it into a log.

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

using namespace std;

bool checked = false;

//error codes w/ light

vector<vector<string> > checkSigns(string RFID_code) {
	ifstream log_of_times("logOfTimes.txt");
	vector<vector<string> > out_vector;
	vector<string> sign_ins;
	vector<string> sign_outs;
	string line;
	while(!(log_of_times.eof())) {
		getline(log_of_times, line);
		std::size_t found = line.find(RFID_code);
		if ((found != std::string::npos) && (line.substr((found + RFID_code.size()), 1) == "=")) {
			int time_place = found + (RFID_code.size() + 1);
			string time = line.substr(time_place, 24);
			if (sign_ins.size() == sign_outs.size()) {
				sign_ins.push_back(time);
			} else {
				sign_outs.push_back(time);
			}
		}
	}
	out_vector.push_back(sign_ins);
	out_vector.push_back(sign_outs);
	return out_vector;
}

string readUID() {//reads UID from read result file
	string line;
	string UID;
	ifstream result("result.txt");
	getline(result, line);
	getline(result, line);
	if (line != "") {
		size_t found = line.find("\"", 8);
		if (found != std::string::npos) {
			UID = line.substr(8, (found - 8));
		} else {
			UID = "null";
		}
	}
	return UID;
}

string inTime(){//Sets the time the people signed in.
	string sign_in_time;
	time_t beginning_time;
	struct tm* start_time;
	time(&beginning_time);
	start_time=localtime(&beginning_time);
	sign_in_time=asctime(start_time);
	sign_in_time.erase(24,2);
	string move_year=sign_in_time.substr(19,5);
	sign_in_time.erase(19,5);
	sign_in_time.insert(10,move_year);
	return sign_in_time;
}

string getName(string RFID_in, bool use_leds = true){//Sets the name of the person signing in.
	string name;
	string temp_id;
	ifstream input_name;
	input_name.open("IdAndNames.txt");
	while(!(input_name.eof())){
		string line;
		getline(input_name,line);
		temp_id = line.substr(0, line.find("="));
		if(RFID_in==temp_id){
			if (!input_name.eof()) line.erase((line.size() - 1), 1);
			name = line.substr((temp_id.size() + 1), (line.size() - (temp_id.size() + 1)));
			break;
		} //Meow. Logan was here.
	}
	input_name.close();
	if (RFID_in == "null") {
		cout<<"There was an error reading the RFID tag."<<endl;
		if (use_leds) system("echo 2 >/dev/ttyS2");
		ofstream result;
		result.open("result.txt");
		result<<"";
		result.close();
	} else if(RFID_in!=temp_id) {
		cout<<endl<<"Error. There is no name associated with the id \""<<RFID_in<<"\". Please try again."<<endl;
		if (use_leds) system("echo 2 >/dev/ttyS2");
		ofstream result;
		result.open("result.txt");
		result<<"";
		result.close();		
	} else {
		vector<vector<string> > signs = checkSigns(RFID_in);
		if (signs[0].size() != signs[1].size()) {
			//signing in
			ofstream arduino(">/dev/ttyS2");
			arduino<<'1';
			//if (use_leds) system("echo 0 >/dev/ttyS2");
			if (use_leds) cout<<"Signed out "<<name<<" at "<<inTime().substr(16, 5)<<endl;
		} else {
			//signing out
			//if (use_leds) system("echo 1 >/dev/ttyS2");
			if (use_leds) cout<<"Signed in "<<name<<" at "<<inTime().substr(16, 5)<<endl;
		}
	}
	return name;
}

void outputAttendance(string RFID_in, string name, string sign_in_time){//Outputs the attendance to the corresponding file.
	if(name!=""){
		ofstream log_of_times;
		ifstream result2("result.txt");
		log_of_times.open("logOfTimes.txt",ios::app);
		log_of_times<<name<<"="<<RFID_in<<"="<<sign_in_time<<endl; 
		log_of_times.close();
		log_of_times.open("result.txt");
		log_of_times<<"";
		log_of_times.close();
	}
}

void checkTime() {
	string time_check = inTime();
	if ((time_check.substr(16, 2) == "03") && (checked == false)) {
		ifstream list_of_names("IdAndNames.txt");
		while(!list_of_names.eof()) {
			string name_line;
			getline(list_of_names, name_line);
			if (name_line != "-") {
				size_t name_point = name_line.find("=");
				name_line = name_line.substr(0, name_point);
				vector<vector<string> > name_signs = checkSigns(name_line);
				if (name_signs[0].size() != name_signs[1].size()) {
					string current_time = inTime();
					string last_time = name_signs[0][(name_signs[0].size() - 1)];
					if (atoi((last_time.substr(16, 2)).c_str()) > atoi((current_time.substr(16, 2)).c_str())) {
						int month_number;
						if (last_time.substr(4, 3) != current_time.substr(4, 3)) {
							vector<string> months {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
							for (int k; k<12; k++) {
								if (last_time.substr(4, 3) == months[k]) {
									k = (k + 1);
									if (k == 2) {
										month_number = 28;
									} else if (k < 8){
										month_number = ((k % 2) ? 31 : 30);
									} else {
										month_number = ((k % 2) ? 30 : 31);
									}
									break;
								}
							}
						}
						if (atoi((last_time.substr(8, 2)).c_str()) == month_number) {
							last_time.replace(8, 2, " 1");
						} else {
							stringstream ss;
							ss<<(atoi((last_time.substr(8, 2)).c_str()) + 1);
							last_time.replace(8, 2, ss.str());
						}
					}
					current_time.replace(0, 15, last_time.substr(0, 15));
					outputAttendance(name_line, getName(name_line, false), current_time);
				}
			}
		}
		checked = true;
	} else if ((!(time_check.substr(16, 2) == "03") || (checked == false))) {
		checked = false;
	}
}

int main(){//Input the RFID.
	ofstream result;
	result.open("result.txt");
	result<<"";
	result.close();	
	while(1){
		string name;
		string sign_in_time;
		string RFID_in = readUID();
		if (RFID_in != "") outputAttendance(RFID_in, getName(RFID_in), inTime());
		checkTime();
	}
	return 0;
}