#include "pch.h"
#include <iostream>

int lensDriver::ChangeToSinusoidalSignal() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[6] = { 'M','w','S','A' };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[4] = get_low8(cs);
	SendCmd[5] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 8);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	return 0;
}

int lensDriver::ChangeToRectangularSignal() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[6] = { 'M','w','Q','A' };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[4] = get_low8(cs);
	SendCmd[5] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 8);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	return 0;
}

int lensDriver::ChangeToCurrentMode() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[6] = { 'M','w','D','A' };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[4] = get_low8(cs);
	SendCmd[5] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 8);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	return 0;
}

int lensDriver::ChangeToFocalPowerControlledMode() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[6] = { 'M','w','C','A' };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[4] = get_low8(cs);
	SendCmd[5] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 8);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	disp(ReplyCmd, 8);

	unsigned short value;
	unsigned short value_;

	value = ReplyCmd[4] & 0xff;
	value = (value << 8) | (ReplyCmd[5] & 0xff);
	value_ = ReplyCmd[6] & 0xff;
	value_ = (value_ << 8) | (ReplyCmd[7] & 0xff);	

	std::cout << "Max focal power limit : " << signed16to10(value) / 200.0 << std::endl;
	std::cout << "Min focal power limit : " << signed16to10(value_) / 200.0 << std::endl;
	std::cout << std::endl;

	return 0;
}

int lensDriver::ReadCurrentlyActiveMode() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[5] = { 'M','M','A' };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[3] = get_low8(cs);
	SendCmd[4] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 8);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	disp(ReplyCmd, 8);

	/*Current Mode = 1
	- Sinusoidal Signal Mode = 2
	- Triangular Mode = 3
	- Retangular Mode = 4
	- Focal Power Mode = 5
	- Analog Mode = 6
	- Position Controlled Mode = 7*/
	std::cout << "Currently Active Mode is ";
	switch (ReplyCmd[3]) {
	case(1):
		std::cout << "Current Mode" << std::endl;
		break;
	case(2):
		std::cout << "Sinusoidal Signal Mode" << std::endl;
		break;
	case(3):
		std::cout << "Triangular Mode" << std::endl;
		break;
	case(4):
		std::cout << "Retangular Mode" << std::endl;
		break;
	case(5):
		std::cout << "Focal Power Mode" << std::endl;
		break;
	case(6):
		std::cout << "Analog Mode" << std::endl;
		break;
	case(7):
		std::cout << "Position Controlled Mode" << std::endl;
		break;
	}
	return ReplyCmd[3];
}

int lensDriver::ChangeToAnalogMode() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[6] = { 'M','w','A','A' };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[4] = get_low8(cs);
	SendCmd[5] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 8);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	return 0;
}