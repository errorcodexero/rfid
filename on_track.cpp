#include "rfid.h"

int main() {
	Time current_time = getTime();
	Time start(2016, 1, 9, 0, 0, 0);
	Time end(2016, 3, 9, 0, 0, 0);
	int days_since_start = getTimeInDays(current_time - start);
	int days_to_end = getTimeInDays(end - current_time);
	std::ifstream names("ids_and_names.txt");
	while (!names.eof()) {
		std::string line, name;
		getline(names, line);
		removeLineBreaks(line);
		name = line.substr(line.find("=") + 2);
		assert(checkName(name));
		std::pair<std::vector<Time>, std::vector<Time> > sign_ins_outs = getSignInsOuts(name);
		int season_hours = getTimeInHours(getTotalTime(sign_ins_outs));
		sign_ins_outs = getSignInsOuts(name, "logs/2015_offseason.txt");
		int offseason_hours = std::min(getTimeInHours(getTotalTime(sign_ins_outs)), MAX_OFFSEASON_HOURS);
		double avg_hours = (double) season_hours / days_since_start;
		double projected_hours = (avg_hours * days_to_end) + offseason_hours + season_hours;
		std::string on_track = (projected_hours >= HOURS_NEEDED) ? "On track    " : "Not on track";
		int hours_needed = std::max((HOURS_NEEDED - (offseason_hours + season_hours)), 0);
		std::cout<<name;
		for (unsigned int i = name.size(); i < NAME_SPACE; i++) std::cout<<" ";
		std::cout<<" - "<<on_track<<" - "<<(int) projected_hours;
		for (int i = (int) log10((double) (projected_hours==0 ? 1 : projected_hours)); i < HOUR_SPACE; i++) std::cout<<" ";
		std::cout<<"projected hour"<<((projected_hours != 1) ? "s" : "");
		std::cout<<" - "<<hours_needed;
		for (int i = (int) log10((double) (hours_needed==0 ? 1 : hours_needed)); i < HOUR_SPACE; i++) std::cout<<" ";
		std::cout<<"hours needed"<<std::endl;
	}
	names.close();
	return 1;
}