/** Serial.cpp
*
* A very simple serial port control class that does NOT require MFC/AFX.
*
* @author Hans de Ruiter
*
* @version 0.1 -- 28 October 2008

************************************************
レンズドライバ　シリアル通信用C++ファイル
更新日：2018.9.13　上田龍幸
************************************************



*/
#include "pch.h"
#include <iostream>
#include <tchar.h>
using namespace std;



lensDriver::lensDriver(int comNum, int bitRate)
{
	TCHAR com[10];
	_stprintf_s(com, 10, _T("COM%d"), comNum);
	commHandle = CreateFile(com, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
		0, NULL);

	if (commHandle == INVALID_HANDLE_VALUE)
	{
		throw("ERROR: Could not open com port");
	}
	else
	{
		// set timeouts
		COMMTIMEOUTS cto = { MAXDWORD, 0, 0, 0, 0 };
		DCB dcb;
		if (!SetCommTimeouts(commHandle, &cto))
		{
			lensDriver::~lensDriver();
			throw("ERROR: Could not set com port time-outs");
		}

		// set DCB
		memset(&dcb, 0, sizeof(dcb));
		dcb.DCBlength = sizeof(dcb);
		dcb.BaudRate = bitRate;
		dcb.fBinary = 1;
		dcb.fDtrControl = DTR_CONTROL_ENABLE;
		dcb.fRtsControl = RTS_CONTROL_ENABLE;

		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 8;

		if (!SetCommState(commHandle, &dcb))
		{
			lensDriver::~lensDriver();
			throw("ERROR: Could not set com port parameters");
		}
	}
}

lensDriver::~lensDriver()
{
	ChangeToCurrentMode();
	SetCurrent(0);
	Sleep(waitTime);
	CloseHandle(commHandle);
}

int lensDriver::write(const char *buffer)
{
	DWORD numWritten;
	WriteFile(commHandle, buffer, strlen(buffer), &numWritten, NULL);

	return numWritten;
}

int lensDriver::write(const char *buffer, int buffLen)
{
	DWORD numWritten;
	WriteFile(commHandle, buffer, buffLen, &numWritten, NULL);

	return numWritten;
}

int lensDriver::read(char *buffer, int buffLen, bool nullTerminate)
{
	DWORD numRead;
	if (nullTerminate)
	{
		--buffLen;
	}

	BOOL ret = ReadFile(commHandle, buffer, buffLen, &numRead, NULL);

	if (!ret)
	{
		return 0;
	}

	if (nullTerminate)
	{
		buffer[numRead] = '\0';
	}

	return numRead;
}

#define FLUSH_BUFFSIZE 10

void lensDriver::flush()
{
	char buffer[FLUSH_BUFFSIZE];
	int numBytes = read(buffer, FLUSH_BUFFSIZE, false);
	while (numBytes != 0)
	{
		numBytes = read(buffer, FLUSH_BUFFSIZE, false);
	}
}

