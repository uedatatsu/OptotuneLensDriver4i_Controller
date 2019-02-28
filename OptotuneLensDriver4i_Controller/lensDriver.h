#pragma once

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <string>
#include <windows.h>

typedef std::basic_string<TCHAR> tstring;

static constexpr int baudrate = 115200;	//ボーレート
static constexpr int waitTime = 100;
static constexpr int maxCurrent = 293;

class Serial;

class lensDriver {
private:
	HANDLE commHandle;


public:
	lensDriver(int comNum, int bitRate);

	virtual ~lensDriver();

	/** Writes a NULL terminated string.
	*
	* @param buffer the string to send
	*
	* @return int the number of characters written
	*/
	int write(const char buffer[]);

	/** Writes a string of bytes to the serial port.
	*
	* @param buffer pointer to the buffer containing the bytes
	* @param buffLen the number of bytes in the buffer
	*
	* @return int the number of bytes written
	*/
	int write(const char *buffer, int buffLen);

	/** Reads a string of bytes from the serial port.
	*
	* @param buffer pointer to the buffer to be written to
	* @param buffLen the size of the buffer
	* @param nullTerminate if set to true it will null terminate the string
	*
	* @return int the number of bytes read
	*/
	int read(char *buffer, int buffLen, bool nullTerminate = true);

	/** Flushes everything from the serial port's read buffer
	*/
	void flush();


	
	int Handshake();

	int SetCurrent(double inputCurrent);
	double GetCurrent();

	double GetMaxOutputCurrent();

};

#endif