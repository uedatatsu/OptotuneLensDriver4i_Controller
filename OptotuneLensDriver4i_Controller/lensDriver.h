#pragma once

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <string>
#include <windows.h>

typedef std::basic_string<TCHAR> tstring;

static constexpr int baudrate = 115200;	//ボーレート
static constexpr int waitTime = 100;
static constexpr int maxFrequency = 500;

#define COUNTOF(array) (sizeof(array) / sizeof(array[0]))

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


	//01-Firmware _Commands_and_Properties.cpp
	int		Handshake();
	char	FirmwareType();
	int		FirmwareBranch();
	int		PartNumberCommand();
	int		SetUpperSoftwareCurrentLimit(double upperCurrentLimit);
	double	GetUpperSoftwareCurrentLimit();
	int		SetLowerSoftwareCurrentLimit(double lowerCurrentLimit);
	double	GetLowerSoftwareCurrentLimit();
	int		FirmwareVersionRead();
	void	DeviceID_Read(unsigned char * deviceID);
	void	DeviceID_Write(unsigned char * deviceID);
	std::tuple<double, double, double, double, double, double, double, double, double> SetDriftVariables(double G1, double G2, double G3, double G4, double T1up, double T1down, double T2, double T3, double T4 );
	std::tuple<double, double, double, double, double, double, double, double, double> GetDriftVariables();

	//02-Error_Commands.cpp
	int		CommunicationError();

	//03-Lens_Commands_and_Properties.cpp
	void	SerialNumber(unsigned char * serialNumber);
	int		SetCurrent(double inputCurrent);
	double	GetCurrent();
	double	GetSignalGeneratorUpperCurrentLimit();
	int		SetSignalGeneratorUpperCurrentLimit(double upperSwingLimit);
	double	GetSignalGeneratorLowerCurrentLimit();
	int		SetSignalGeneratorLowerCurrentLimit(double lowerSwingLimit);
	double	GetSignalGeneratorFrequency();
	int		SetSignalGeneratorFrequency(double frequency);
	int		SetTemperatureLimits(double maximumTemperature, double minimumTemperature);
	std::tuple<double, double>GetTemperatureLimits();
	std::tuple<double, double>GetEEPROMTemperatureValues();
	int		SetEEPROMTemperatureValues(double firstTemperature, double secondTemperature);
	double	GetFocalPower();
	int		SetFocalPower(double focalPower);
	int		SetMaxOutputCurrent(double maxCurrent);
	double	GetMaxOutputCurrent();
	int		SetCalibrationFactor(double currentFactor);
	double	GetCalibrationFactor();
	double	TemperatureReading();
	double	GetStatus();
	double	ReadAddressFromTheEEPROM();
	int		AnalogInputReading();
	int		WriteByteToEEPROM(int registerID, int byteToWrite);
	int		ReadEEPROM();

	//04-Operation_Mode.cpp
	int		ChangeToSinusoidalSignal();
	int		ChangeToRectangularSignal();
	int		ChangeToCurrentMode();
	int		ChangeToFocalPowerControlledMode();
	int		ReadCurrentlyActiveMode();
	int		ChangeToAnalogMode();

	//06-Advanced_Commands.cpp
	int		GPIOToogle(int toogleCommand);
	int		SetLensDriverToDFUMode();
	int		Nodemonstration();
	double InterpoaltionCommand(double current, double temperature);

};

#endif