all: logger log_reader autologger

logger_64: rfid.cpp logger.cpp
	x86_64-w64-mingw32-g++ rfid.cpp logger.cpp -Wall -static -static-libgcc -static-libstdc++ -std=c++11 -o "logger.exe"
	
logger: rfid.cpp logger.cpp
	g++ rfid.cpp logger.cpp -Wall -static -static-libgcc -static-libstdc++ -std=c++11 -o "logger.exe"
	
log_reader: rfid.cpp log_reader.cpp
	g++ rfid.cpp log_reader.cpp -Wall -std=c++11 -o "log_reader.exe"
	
autologger: rfid.cpp autologger.cpp
	g++ rfid.cpp autologger.cpp -Wall -std=c++11 -o "autologger.exe"

.PHONY: clean
clean:
	rm logger.exe; rm log_reader.exe; rm autologger.exe;