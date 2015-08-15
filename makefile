main:
	x86_64-w64-mingw32-g++ Serial.cpp main.cpp -Wall -static -static-libgcc -static-libstdc++
	
test:
	x86_64-w64-mingw32-g++ test.cpp -Wall -static -static-libgcc -static-libstdc++
	
clean:
	rm a.exe