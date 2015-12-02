#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <cstddef>
#include <ctime>
#include <cassert>
#include <sstream>

#include "rfid.h"

#define NAME_SPACE 24

#define MANUAL_ENTRY_TIME 30000

enum Led_mode {LOGGED, LOGGING_ERROR, QUITTING};

struct data {
	char *buffer;
	std::size_t size;
};

DWORD WINAPI getInput(LPVOID arg) {
	data *buf = (data*)arg;
	return !std::cin.getline(buf->buffer, buf->size);
}

//Gets the current time and returns a Time structure filled in with the correct values
Time getTime() {
	time_t rawtime;
	struct tm * ptm;
	time (&rawtime);
	ptm = gmtime(&rawtime);
	Time t;
	t.year = (ptm->tm_year) + 1900;
	t.month = (ptm->tm_mon) + 1;
	t.day = ptm->tm_mday;
	t.hour = (ptm->tm_hour) + 16;
	t.minute = ptm->tm_min;
	t.second = ptm->tm_sec;
	return t;
}

//Prints a line in the log file with the person's name and the current time and tells the user
void logAttendance(std::string name) {
	Time time = getTime();
	std::string formatted_time = formatTime(time);
	std::ofstream log("log.txt", std::ofstream::app);
	log<<"\r\n"<<name;
	for (int i = name.size(); i < NAME_SPACE; i++) log<<" ";
	log<<"="<<" "<<formatted_time;
	log.close();
	std::pair<std::vector<Time>, std::vector<Time> > sign_ins_outs = getSignInsOuts(name);
	std::string sign_in_or_out = (sign_ins_outs.first.size() == sign_ins_outs.second.size()) ? "out" : "in";
	std::cout<<"Signed "<<sign_in_or_out<<" "<<name<<" at "<<formatTimeAlt(time)<<"."<<std::endl;
}

//Gets the name that goes with a uid
std::string getName(std::string uid) {
	std::ifstream ids_and_names("ids_and_names.txt");
	std::string line;
	while (!ids_and_names.eof()) {
		getline(ids_and_names, line);
		if (line.find(uid) != std::string::npos) break;
	}
	ids_and_names.close();
	std::string name = line.substr(line.find("=") + 2);
	name.erase(std::remove(name.begin(), name.end(), '\r'), name.end());
	return name;
}

//Checks to see if a received UID is valid
bool checkUID(std::string uid) {
	if (uid.size() == 8) {
		std::string line;
		std::ifstream ids_and_names("ids_and_names.txt");
		while (!ids_and_names.eof()) {
			getline(ids_and_names, line);
			if (line.find(uid) != std::string::npos) {
				ids_and_names.close();
				return true;
			}
		}
		ids_and_names.close();
	}
	return false;
}

int main(int argc, char* argv[]) {
	bool logging = false; //In config file eventually
	
	bool quitting = false;
	
	std::string no_arduino_str = "-noarduino";
	bool use_arduino = [&]() {
		for (int i = 0; i < argc; i++) {
			if (argv[i] == no_arduino_str) return false;
		}
		return true;
	}();
	
	if (use_arduino) {
		Led_mode led_mode;

		HANDLE hCom;
		hCom = CreateFile(
			TEXT("\\\\.\\COM4"),          //Change to whatever COM port the Arduino is on
			GENERIC_READ | GENERIC_WRITE,
			0,                            // exclusive access 
			NULL,                         // default security attributes 
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,        //FILE_FLAG_OVERLAPPED
			NULL
		);

		SetCommMask(hCom, EV_RXCHAR);

		DWORD dw_bytes_written_clear = 0;
		char to_write_clear[2] = {'9'};
		
		WriteFile(hCom, to_write_clear, 1, &dw_bytes_written_clear, NULL);
		
		while (!quitting) {
			DWORD dw_evt_mask;
			char ch_buffer[1024] = "";
			DWORD dw_bytes_read = 0;
			
			bool comm_event_sucess = WaitCommEvent(hCom, &dw_evt_mask, NULL);
			Sleep(250);
			
			ReadFile(hCom, &ch_buffer, 8, &dw_bytes_read, NULL);
			
			std::string uid(ch_buffer);
			uid = uid.substr(0, 8);
			
			if (logging) {
				std::ofstream serial_logging_file;
				serial_logging_file.open("serial_log.txt", std::fstream::app);
				serial_logging_file<<ch_buffer<<std::endl;
				serial_logging_file.close();
			}
			
			if (uid.size() > 0) {
				if (uid == "FFFFFFFF") {
					std::cout<<"Please enter either your first and last name or \"quit\" to quit."<<std::endl;
					
					bool timed_out = false;
					char buffer[256] = {0};
					data arg = {buffer, 256};
					HANDLE hThread; 
					DWORD id;
					std::string name;

					hThread = CreateThread(NULL, 0, getInput, &arg, 0, &id);

					if (hThread != NULL) {
						if (WaitForSingleObjectEx(hThread, MANUAL_ENTRY_TIME, FALSE) == WAIT_TIMEOUT) {
							std::cout<<"User timed out."<<std::endl;
							timed_out = true;
							/*std::streambuf* orig = std::cin.rdbuf();
							std::istringstream input("\n");
							std::cin.rdbuf(input.rdbuf());
							std::cin.rdbuf(orig);*/
						} else {
							name = buffer;
						}
					}
					
					CloseHandle(hThread);
					
					if (!timed_out) {
						if (name != "quit") {
							//std::cout<<std::endl<<"NAME: "<<name<<std::endl;
							/*for (unsigned int i = 0; i < name.size(); i++) {
								if (name[i] == '\n') name.erase(i, 1);
							}*/
							bool valid_name = checkName(name);
							if (valid_name) {
								logAttendance(name);
								led_mode = Led_mode::LOGGED;
							} else {
								std::cout<<"Invalid entry.  Either the name was entered incorrectly or no such name is stored in the system."<<std::endl;
								led_mode = Led_mode::LOGGING_ERROR;
							}
						} else {
							quitting = true;
							led_mode = Led_mode::QUITTING;
						}
					} else {
						led_mode = Led_mode::LOGGING_ERROR;
					}
				} else {
					bool valid_uid = checkUID(uid);
					if (valid_uid) {
						std::string name = getName(uid);
						logAttendance(name);
						led_mode = Led_mode::LOGGED;
					} else {
						std::cout<<"Invalid tag.  Either your tag is damaged or no such tag is stored in the system."<<std::endl;
						led_mode = Led_mode::LOGGING_ERROR;
					}
				}
				
				DWORD dw_bytes_written = 0;
				char to_write[2] = {[&]() {
					switch (led_mode) {
						case Led_mode::LOGGED:
							return '0';
						case Led_mode::LOGGING_ERROR:
							return '1';
						case Led_mode::QUITTING:
							return '2';
						default:
							assert(0);
					}
					return '3';
				}()};
				
				WriteFile(hCom, to_write, 1, &dw_bytes_written, NULL);
			}
		}
		CloseHandle(hCom);
	} else {
		while(!quitting) {
			std::cout<<"Please enter either your first and last name or \"quit\" to quit."<<std::endl;
			std::string name;
			getline(std::cin, name);
			if (name != "quit") {
				bool valid_name = checkName(name);
				if (valid_name) {
					logAttendance(name);
				} else {
					std::cout<<"Invalid entry.  Either the name was entered incorrectly or no such name is stored in the system."<<std::endl;
				}
			} else {
				quitting = true;
			}
		}
	}

	return 0;
}