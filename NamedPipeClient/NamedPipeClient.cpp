// NamedPipeClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hPipe;
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\examplePipe");
	LPTSTR lpvMessage = TEXT("Hello from C++");
	BOOL fSuccess = FALSE;
	DWORD dwMode, cbToWrite, cbWritten;

	hPipe = CreateFile(
		lpszPipename,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		NULL
		);

	if (hPipe == INVALID_HANDLE_VALUE)
	{
		_tprintf( TEXT("Could not open pipe. GLE=%d\n"), GetLastError() );
		return -1;
	}

	// Pipe connected. Set mode to message.
	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		hPipe,
		&dwMode,
		NULL,
		NULL
		);
	if (!fSuccess)
	{
		_tprintf( TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError() );
		return -1;
	}


	cbToWrite = WideCharToMultiByte(CP_UTF8, 0, lpvMessage, -1, NULL, 0, NULL, NULL); 
	LPSTR lpvMBMessage = new CHAR[cbToWrite];
	WideCharToMultiByte(CP_UTF8, 0, lpvMessage, -1, lpvMBMessage, cbToWrite, NULL, NULL);
	_tprintf( TEXT("Sending %d byte message: \"%s\"\n"), cbToWrite, lpvMessage); 

	fSuccess = WriteFile(
		hPipe,
		lpvMBMessage,
		cbToWrite,
		&cbWritten,
		NULL
		);

	delete [] lpvMBMessage;

	if (!fSuccess)
	{
		_tprintf( TEXT("Write to pipe failed. GLE=%d\n"), GetLastError() );
		return -1;
	}

	_tprintf( TEXT("Closing pipe.") );
	CloseHandle(hPipe);

	_tprintf( TEXT("Done.") );
	return 0;


	return 0;
}

