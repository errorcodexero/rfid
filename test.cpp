#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include <stdio.h>
#include <string>

int _tmain(
            int argc, 
            TCHAR *argv[]
           )
{
    HANDLE hCom;
    OVERLAPPED o;
    BOOL fSuccess;
    DWORD dwEvtMask;

    hCom = CreateFile( TEXT("\\\\.\\COM12"),
        GENERIC_READ | GENERIC_WRITE,
        0,    // exclusive access 
        NULL, // default security attributes 
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        NULL 
        );

    if (hCom == INVALID_HANDLE_VALUE) 
    {
        // Handle the error. 
        printf("CreateFile failed with error %d.\n", GetLastError());
        return 0;
    }

    // Set the event mask. 

    fSuccess = SetCommMask(hCom, EV_RXCHAR);

    if (!fSuccess) 
    {
        // Handle the error. 
        printf("SetCommMask failed with error %d.\n", GetLastError());
        return 0;
    }

    // Create an event object for use by WaitCommEvent. 

    o.hEvent = CreateEvent(
        NULL,   // default security attributes 
        TRUE,   // manual-reset event 
        FALSE,  // not signaled 
        NULL    // no name
         );

    // Initialize the rest of the OVERLAPPED structure to zero.
    o.Internal = 0;
    o.InternalHigh = 0;
    o.Offset = 0;
    o.OffsetHigh = 0;

    assert(o.hEvent);

	char chBuffer[1024] = "";
	DWORD dwBytesRead = 0;
    while (!WaitCommEvent(hCom, &dwEvtMask, &o)) {}
	ReadFile(hCom, &chBuffer, 1024, &dwBytesRead, &o);
	std::string stringy(chBuffer);
	printf("%s\n", chBuffer);
    /*else
    {
        DWORD dwRet = GetLastError();
        if( ERROR_IO_PENDING == dwRet)
        {
            printf("I/O is pending...\n");

            // To do.
        }
        else 
            printf("Wait failed with error %d.\n", GetLastError());
    }*/
	return 0;
}