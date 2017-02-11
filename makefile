CFLAGS = -Wall -std=c++11

ifeq ($(PLATFORM), linux)
	CFLAGS += -DLINUX
endif

ifeq ($(PLATFORM), windows)
	CFLAGS += -DWINDOWS
endif

all: logger log_reader autologger rankings check_order on_track reformat_log check_and_fix log_shop_times 

logger: rfid.cpp logger.cpp
	g++ rfid.cpp logger.cpp -static -static-libgcc -static-libstdc++ $(CFLAGS) -o "logger"
	
log_reader: rfid.cpp log_reader.cpp
	g++ rfid.cpp log_reader.cpp $(CFLAGS) -o "log_reader"
	
autologger: rfid.cpp autologger.cpp
	g++ rfid.cpp autologger.cpp $(CFLAGS) -o "autologger"
	
rankings: rfid.cpp rankings.cpp
	g++ rfid.cpp rankings.cpp $(CFLAGS) -o "rankings"

check_order: rfid.cpp check_order.cpp
	g++ rfid.cpp check_order.cpp $(CFLAGS) -o "check_order"
	
on_track: rfid.cpp on_track.cpp
	g++ rfid.cpp on_track.cpp $(CFLAGS) -o "on_track"

reformat_log: rfid.cpp reformat_log.cpp
	g++ rfid.cpp reformat_log.cpp $(CFLAGS) -o "reformat_log"
	
check_and_fix: rfid.cpp check_and_fix.cpp
	g++ rfid.cpp check_and_fix.cpp $(CFLAGS) -o "check_and_fix"

log_shop_times: rfid.cpp log_shop_times.cpp
	g++ rfid.cpp log_shop_times.cpp $(CFLAGS) -o "log_shop_times"
	
signed_in: rfid.cpp signed_in.cpp
	g++ rfid.cpp signed_in.cpp -Wall -std=c++11 -o "signed_in.exe"

.PHONY: clean
clean:
	rm logger.exe; rm log_reader.exe; rm autologger.exe; rm rankings.exe; rm check_order.exe; rm on_track.exe; rm reformat_log.exe; rm check_and_fix.exe; rm log_shop_times.exe; rm signed_in.exe;
