#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

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

pair<string, int> makeRank(string person_name){//Reads out (from the log) the times the person was present.
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
		if (line.size()>0) {
			size_t found = line.find(person_name);
			if ((found != string::npos) && (line.substr(person_name.size(), 1) == "=")) {
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
	}
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
	}
	float hours = ((final_times[0]*8544)+(final_times[1]*720)+(final_times[2]*24)+final_times[3]+(final_times[4]*(1/60))+(final_times[5]*(1/360)));
	int hours_final = round(hours);
	pair<string, int> rank;
	stringstream rank_out;
	rank_out<<person_name<<" has a total signed-in time of "<<hours_final<<" hours."<<endl;
	rank.first = rank_out.str();
	rank.second = hours_final;
	return rank;
}

vector<string> makeFinalRankings(vector<pair<string, int>> ranking) {
	vector<string> final_ranking;
	int top = 0;
	while (ranking.size()>1) {
		for(unsigned int i = 0; i<ranking.size(); i++) {
			if (max(ranking[i].second, ranking[top].second)!=ranking[top].second) top = i;
		}
		final_ranking.push_back(ranking[top].first);
		ranking.erase(ranking.begin()+top);
		top = 0;
	}
	return final_ranking;
}

int main() {
	ifstream names;
	names.open("IdAndNames.txt");
	string line;
	vector<pair<string, int>> ranks;
	while (!names.eof()) {
		getline(names, line);
		if (line!="") {
			line = line.substr(9, (line.size()-10));
			ranks.push_back(makeRank(line));
		}	
	}
	names.close();
	vector<string> outputs = makeFinalRankings(ranks);
	for(unsigned int i = 0; i<outputs.size(); i++) {
		cout<<i+1<<". "<<outputs[i];
	}
	return 1;
}