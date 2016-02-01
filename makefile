all: logger log_reader autologger rankings check_order

logger_64: rfid.cpp logger.cpp
	x86_64-w64-mingw32-g++ rfid.cpp logger.cpp -Wall -static -static-libgcc -static-libstdc++ -std=c++11 -o "logger.exe"
	
logger: rfid.cpp logger.cpp
	g++ rfid.cpp logger.cpp -Wall -static -static-libgcc -static-libstdc++ -std=c++11 -o "logger.exe"
	
log_reader: rfid.cpp log_reader.cpp
	g++ rfid.cpp log_reader.cpp -Wall -std=c++11 -o "log_reader.exe"
	
autologger: rfid.cpp autologger.cpp
	g++ rfid.cpp autologger.cpp -Wall -std=c++11 -o "autologger.exe"
	
rankings: rfid.cpp rankings.cpp
	g++ rfid.cpp rankings.cpp -Wall -std=c++11 -o "rankings.exe"

check_order: rfid.cpp check_order.cpp
	g++ rfid.cpp check_order.cpp -Wall -std=c++11 -o "check_order.exe"

.PHONY: clean
clean:
	rm logger.exe; rm log_reader.exe; rm autologger.exe; rm rankings.exe; rm check_order.exe;
