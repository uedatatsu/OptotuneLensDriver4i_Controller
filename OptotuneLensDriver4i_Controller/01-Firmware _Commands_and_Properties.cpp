#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <tuple>

int lensDriver::Handshake() {
	char ReplyCmd[100];

	write("Start");
	Sleep(waitTime);
	read(ReplyCmd, 100, true);
	std::cout <<  (char*)ReplyCmd << std::endl;
	return 0;
}

char lensDriver::FirmwareType() {
	auto crc16 = new crc16ibm();

	unsigned char SendCmd[3] = { 'H', NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, 1);	//右辺…dataの要素数

	SendCmd[1] = get_low8(cs);
	SendCmd[2] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));

	//disp(SendCmd, COUNTOF(SendCmd));

	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, COUNTOF(ReplyCmd));

	unsigned short value;

	value = ReplyCmd[1] & 0xff;
	std::cout << "Firmware type: " << (char)value << std::endl;

	return (char)value;
}

int lensDriver::FirmwareBranch() {
	auto crc16 = new crc16ibm();

	unsigned char SendCmd[3] = { 'F', NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, 1);	//右辺…dataの要素数

	SendCmd[1] = get_low8(cs);
	SendCmd[2] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));

	//disp(SendCmd, COUNTOF(SendCmd));

	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, COUNTOF(ReplyCmd));

	unsigned short value;

	value = ReplyCmd[1] & 0xff;

	switch (value)
	{
	case(0):
		std::cout << "Firmware branch : trunk" << std::endl;
		break;
	case(1):
		std::cout << "Firmware branch : branch1" << std::endl;
		break;
	case(2):
		std::cout << "Firmware branch : branch2" << std::endl;
		break;
	default:
		break;
	}

	return value;
}

int lensDriver::PartNumberCommand() {
	auto crc16 = new crc16ibm();

	unsigned char SendCmd[3] = { 'J', NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, 1);	//右辺…dataの要素数

	SendCmd[1] = get_low8(cs);
	SendCmd[2] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));

	//disp(SendCmd, COUNTOF(SendCmd));

	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	disp(ReplyCmd, COUNTOF(ReplyCmd));

	unsigned int value;

	value = ReplyCmd[1] & 0xff;
	value = (value << 8) | (ReplyCmd[2] & 0xff);
	value = (value << 8) | (ReplyCmd[3] & 0xff);

	std::cout << "Part number command: " << value << std::endl;

	return value;
}

int lensDriver::SetUpperSoftwareCurrentLimit(double upperSwingLimit) {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'C','w','U','A',NULL,NULL,NULL,NULL };
	double maxCurrent = GetMaxOutputCurrent();

	if (abs(upperSwingLimit) > maxCurrent) {
		std::cout << "This current value is out of range." << std::endl;
		std::cout << "Please set the absolute value less than " << maxCurrent << "[mA]." << std::endl;
		return -1;
	}

	int value = upperSwingLimit * ((double)4095 / (double)GetMaxOutputCurrent());

	SendCmd[4] = get_high8(value);
	SendCmd[5] = get_low8(value);
	auto cs = crc16->calc_checksum(SendCmd, 8);	//右辺…dataの要素数
	SendCmd[8] = get_low8(cs);
	SendCmd[9] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 4);

	return 0;
}

double lensDriver::GetUpperSoftwareCurrentLimit() {
	auto crc16 = new crc16ibm();

	unsigned char SendCmd[10] = { 'C','r','U','A',NULL,NULL,NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, 8);	//右辺…dataの要素数

	SendCmd[8] = get_low8(cs);
	SendCmd[9] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));

	//disp(SendCmd, 8);

	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	unsigned short value;

	value = ReplyCmd[2] & 0xff;
	value = (value << 8) | (ReplyCmd[3] & 0xff);

	return signed16to10(value) * (GetMaxOutputCurrent() / (double)4095);

}

int lensDriver::SetLowerSoftwareCurrentLimit(double lowerSwingLimit) {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'C','w','L','A',NULL,NULL,NULL,NULL };
	double maxCurrent = GetMaxOutputCurrent();

	if (abs(lowerSwingLimit) > maxCurrent) {
		std::cout << "This current value is out of range." << std::endl;
		std::cout << "Please set the absolute value less than " << maxCurrent << "[mA]." << std::endl;
		return -1;
	}

	int value = lowerSwingLimit * ((double)4095 / (double)GetMaxOutputCurrent());

	SendCmd[4] = get_high8(value);
	SendCmd[5] = get_low8(value);
	auto cs = crc16->calc_checksum(SendCmd, 8);	//右辺…dataの要素数
	SendCmd[8] = get_low8(cs);
	SendCmd[9] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 4);

	return 0;
}


double lensDriver::GetLowerSoftwareCurrentLimit() {
	auto crc16 = new crc16ibm();

	unsigned char SendCmd[10] = { 'C','r','L','A',NULL,NULL,NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, 8);	//右辺…dataの要素数

	SendCmd[8] = get_low8(cs);
	SendCmd[9] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));

	//disp(SendCmd, 8);

	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	unsigned short value;

	value = ReplyCmd[2] & 0xff;
	value = (value << 8) | (ReplyCmd[3] & 0xff);

	return signed16to10(value) * (GetMaxOutputCurrent() / (double)4095);
}

int lensDriver::FirmwareVersionRead() {
	auto crc16 = new crc16ibm();

	unsigned char SendCmd[3] = { 'V', NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, 1);	//右辺…dataの要素数

	SendCmd[1] = get_low8(cs);
	SendCmd[2] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));

	//disp(SendCmd, COUNTOF(SendCmd));

	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	disp(ReplyCmd, COUNTOF(ReplyCmd));

	unsigned short value;

	value = ReplyCmd[1] & 0xff;
	std::cout << "Firmware Major: " << value << std::endl;
	value = ReplyCmd[2] & 0xff;
	std::cout << "Firmware Minor: " << value << std::endl;
	value = ReplyCmd[3] & 0xff;
	value = (value << 8) | (ReplyCmd[4] & 0xff);
	std::cout << "Build         : " << signed16to10(value) << std::endl;
	value = ReplyCmd[5] & 0xff;
	value = (value << 8) | (ReplyCmd[6] & 0xff);
	std::cout << "Revison       : " << signed16to10(value) << std::endl;


	return value;
}

void lensDriver::DeviceID_Read(unsigned char * deviceID) {
	auto crc16 = new crc16ibm();

	unsigned char SendCmd[12] = { 'I', 'R', NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, 10);	//右辺…dataの要素数

	SendCmd[10] = get_low8(cs);
	SendCmd[11] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));

	//disp(SendCmd, COUNTOF(SendCmd));

	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	disp(ReplyCmd, COUNTOF(ReplyCmd));

	std::cout << "Device ID : ";
	for (int i = 0; i < 8; i++) {
		deviceID[i] = ReplyCmd[i + 2];
		std::cout << deviceID[i];
	}
	std::cout << std::endl;
}

void lensDriver::DeviceID_Write(unsigned char * deviceID) {
	auto crc16 = new crc16ibm();

	unsigned char SendCmd[12] = { 'I', 'W', NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
	unsigned char ReplyCmd[100];

	for (int i = 0; i < 8; i++) {
		SendCmd[i + 2] = deviceID[i];
	}

	auto cs = crc16->calc_checksum(SendCmd, 10);	//右辺…dataの要素数

	SendCmd[10] = get_low8(cs);
	SendCmd[11] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));

	//disp(SendCmd, COUNTOF(SendCmd));

	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, COUNTOF(ReplyCmd));

	std::cout << "Device ID : ";
	for (int i = 0; i < 8; i++) {
		deviceID[i] = ReplyCmd[i + 2];
		std::cout << deviceID[i];
	}
	std::cout << std::endl;
}

std::tuple<double, double, double, double, double, double, double, double, double> lensDriver::SetDriftVariables(double G1, double G2, double G3, double G4, double T1up, double T1down, double T2, double T3, double T4) {
	auto crc16 = new crc16ibm();

	unsigned char SendCmd[23] = { 'O', 'w', 'D' };
	unsigned char ReplyCmd[100];

	int EncodedAmplitude_G1 = G1 * 256;
	int EncodedAmplitude_G2 = G2 * 256;
	int EncodedAmplitude_G3 = G3 * 256;
	int EncodedAmplitude_G4 = G4 * 256;
	int EncodedAmplitude_T1up = T1up * 256;
	int EncodedAmplitude_T1down = T1down * 256;
	int EncodedAmplitude_T2 = T2 * 256;
	int EncodedAmplitude_T3 = T3 * 256;
	int EncodedAmplitude_T4 = T4 * 256;

	SendCmd[3] = get_high8(EncodedAmplitude_G1);
	SendCmd[4] = get_low8( EncodedAmplitude_G1);
	SendCmd[5] = get_high8(EncodedAmplitude_G2);
	SendCmd[6] = get_low8( EncodedAmplitude_G2);
	SendCmd[7] = get_high8(EncodedAmplitude_G3);
	SendCmd[8] = get_low8( EncodedAmplitude_G3);
	SendCmd[9] = get_high8(EncodedAmplitude_G4);
	SendCmd[10] = get_low8( EncodedAmplitude_G4);
	SendCmd[11] = get_high8(EncodedAmplitude_T1up);
	SendCmd[12] = get_low8(EncodedAmplitude_T1up);
	SendCmd[13] = get_high8(EncodedAmplitude_T1down);
	SendCmd[14] = get_low8(EncodedAmplitude_T1down);
	SendCmd[15] = get_high8(EncodedAmplitude_T2);
	SendCmd[16] = get_low8( EncodedAmplitude_T2);
	SendCmd[17] = get_high8(EncodedAmplitude_T3);
	SendCmd[18] = get_low8( EncodedAmplitude_T3);
	SendCmd[19] = get_high8(EncodedAmplitude_T4);
	SendCmd[20] = get_low8( EncodedAmplitude_T4);

	auto cs = crc16->calc_checksum(SendCmd, 21);	//右辺…dataの要素数

	SendCmd[21] = get_low8(cs);
	SendCmd[22] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));

	//disp(SendCmd, COUNTOF(SendCmd));

	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, COUNTOF(ReplyCmd));

	unsigned short value;

	value = ReplyCmd[2] & 0xff;
	value = (value << 8) | (ReplyCmd[3] & 0xff);
	G1 = signed16to10(value) / 256.0;
	value = ReplyCmd[4] & 0xff;
	value = (value << 8) | (ReplyCmd[5] & 0xff);
	G2 = signed16to10(value) / 256.0;
	value = ReplyCmd[6] & 0xff;
	value = (value << 8) | (ReplyCmd[7] & 0xff);
	G3 = signed16to10(value) / 256.0;
	value = ReplyCmd[8] & 0xff;
	value = (value << 8) | (ReplyCmd[9] & 0xff);
	G4 = signed16to10(value) / 256.0;
	value = ReplyCmd[10] & 0xff;
	value = (value << 8) | (ReplyCmd[11] & 0xff);
	T1up = signed16to10(value) / 256.0;
	value = ReplyCmd[12] & 0xff;
	value = (value << 8) | (ReplyCmd[13] & 0xff);
	T1down = signed16to10(value) / 256.0;
	value = ReplyCmd[14] & 0xff;
	value = (value << 8) | (ReplyCmd[15] & 0xff);
	T2 = signed16to10(value) / 256.0;
	value = ReplyCmd[16] & 0xff;
	value = (value << 8) | (ReplyCmd[17] & 0xff);
	T3 = signed16to10(value) / 256.0;
	value = ReplyCmd[18] & 0xff;
	value = (value << 8) | (ReplyCmd[19] & 0xff);
	T4 = signed16to10(value) / 256.0;
	return std::forward_as_tuple(G1,  G2,  G3,  G4,  T1up,  T1down,  T2,  T3,  T4);
}

std::tuple<double, double, double, double, double, double, double, double, double> lensDriver::GetDriftVariables() {
	auto crc16 = new crc16ibm();

	unsigned char SendCmd[23] = { 'O', 'r', 'D' ,'X','X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X' };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, 21);	//右辺…dataの要素数

	SendCmd[21] = get_low8(cs);
	SendCmd[22] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));

	disp(SendCmd, COUNTOF(SendCmd));

	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, COUNTOF(ReplyCmd));

	unsigned short value;
	double G1=0,  G2 = 0,  G3 = 0,  G4 = 0,  T1up = 0,  T1down = 0,  T2 = 0,  T3 = 0,  T4 = 0;

	value = ReplyCmd[2] & 0xff;
	value = (value << 8) | (ReplyCmd[3] & 0xff);
	G1 = signed16to10(value) / 256.0;
	value = ReplyCmd[4] & 0xff;
	value = (value << 8) | (ReplyCmd[5] & 0xff);
	G2 = signed16to10(value) / 256.0;
	value = ReplyCmd[6] & 0xff;
	value = (value << 8) | (ReplyCmd[7] & 0xff);
	G3 = signed16to10(value) / 256.0;
	value = ReplyCmd[8] & 0xff;
	value = (value << 8) | (ReplyCmd[9] & 0xff);
	G4 = signed16to10(value) / 256.0;
	value = ReplyCmd[10] & 0xff;
	value = (value << 8) | (ReplyCmd[11] & 0xff);
	T1up = signed16to10(value) / 256.0;
	value = ReplyCmd[12] & 0xff;
	value = (value << 8) | (ReplyCmd[13] & 0xff);
	T1down = signed16to10(value) / 256.0;
	value = ReplyCmd[14] & 0xff;
	value = (value << 8) | (ReplyCmd[15] & 0xff);
	T2 = signed16to10(value) / 256.0;
	value = ReplyCmd[16] & 0xff;
	value = (value << 8) | (ReplyCmd[17] & 0xff);
	T3 = signed16to10(value) / 256.0;
	value = ReplyCmd[18] & 0xff;
	value = (value << 8) | (ReplyCmd[19] & 0xff);
	T4 = signed16to10(value) / 256.0;
	return std::forward_as_tuple(G1, G2, G3, G4, T1up, T1down, T2, T3, T4);

}

