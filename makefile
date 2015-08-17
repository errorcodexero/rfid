all:
	x86_64-w64-mingw32-g++ main.cpp -Wall -static -static-libgcc -static-libstdc++ -std=c++11
	
clean:
	rm a.exe