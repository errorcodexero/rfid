#include <stdio.h>
#include <tchar.h>
#include "SerialClass.h"
#include <string>
#include <fstream>

// application reads from the specified serial port and reports the collected data
int _tmain(int argc, _TCHAR* argv[])
{
	printf("Welcome to the serial test app!\n");

	/*
	Serial* SP = new Serial("COM3");    // adjust as needed - "\\\\.\\" is necessary as a prefix for COM ports 10 and above

	if (SP->IsConnected())
		printf("We're connected\n");
	*/
	
	/*
	char incomingData[256] = "";			// don't forget to pre-allocate memory
	int dataLength = 8;
	int readResult = 0;
	*/
	
	/*
	OVERLAPPED o = {0};
	o.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	*/
	
	HANDLE hCom;
	hCom = CreateFile( TEXT("\\\\.\\COM3"),
        GENERIC_READ | GENERIC_WRITE,
        0,    // exclusive access 
        NULL, // default security attributes 
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL 
        );
	
	SetCommMask(hCom, EV_RXCHAR);
	
	DWORD dwEvtMask;
	char chBuffer[1024] = "";
	DWORD dwBytesRead = 0;
    WaitCommEvent(hCom, &dwEvtMask, NULL);
	ReadFile(hCom, &chBuffer, 8, &dwBytesRead, NULL);
	std::string uid(chBuffer);
	uid = uid.substr(0, 8);
	printf("%s\n", uid.c_str());
	
	DWORD dwBytesWritten = 0;
	//char toWrite[1024] = "";
	WriteFile(hCom, chBuffer, dwBytesRead, &dwBytesWritten, NULL);
	
	//printf("Written\n");
	
	/*while (!WaitCommEvent(hCom, &dwEvtMask, NULL)) {}
	ReadFile(hCom, &chBuffer, 8, &dwBytesRead, NULL);
	printf("%s\n", chBuffer);*/
	
	/*while(SP->IsConnected())
	{
		readResult = SP->ReadData(incomingData,dataLength);
		//printf("Bytes read: (-1 means no data available) %i\n",readResult);
		
		std::string test(incomingData);
		
		//if (readResult != -1) {
		//	printf("%s\n",incomingData);
		//	Sleep(1000);
		//}
		
		//std::ofstream print_to;
		//print_to.open("print_to.txt", std::ofstream::app);
		//print_to<<incomingData<<std::endl;
		//print_to.close();
		
		test = "";
	}*/
	return 0;
}