all:
	make logger; make log_reader;

logger_64: rfid.cpp logger.cpp
	x86_64-w64-mingw32-g++ rfid.cpp logger.cpp -Wall -static -static-libgcc -static-libstdc++ -std=c++11 -o "logger.exe"
	
logger: rfid.cpp logger.cpp
	g++ rfid.cpp logger.cpp -Wall -static -static-libgcc -static-libstdc++ -std=c++11 -o "logger.exe"
	
log_reader: rfid.cpp log_reader.cpp
	g++ rfid.cpp log_reader.cpp -Wall -o "log_reader.exe"
	
clean:
	rm logger.exe; rm log_reader.exe;