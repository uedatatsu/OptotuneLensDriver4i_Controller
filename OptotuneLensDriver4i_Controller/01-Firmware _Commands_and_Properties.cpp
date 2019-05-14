#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
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


