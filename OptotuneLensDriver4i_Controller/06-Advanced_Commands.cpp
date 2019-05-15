#include "pch.h"
#include <iostream>

int	lensDriver::GPIOToogle(int toogleCommand){
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[7] = { 'Q', 'w' ,'T', 'A' };	//
	unsigned char ReplyCmd[100] = { 0 };
	SendCmd[4] = get_low8(toogleCommand);

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[5] = get_low8(cs);
	SendCmd[6] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 6);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 100);
	return 0;
}

int	lensDriver::SetLensDriverToDFUMode() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[7] = { 'R', 'E' ,'S', 'E', 'T' };	//
	unsigned char ReplyCmd[100] = { 0 };

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[5] = get_low8(cs);
	SendCmd[6] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 6);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 100);
	return 0;
}

int	lensDriver::Nodemonstration() {
	char ReplyCmd[100];

	write("NODEMO");
	Sleep(waitTime);
	read(ReplyCmd, 100, true);
	std::cout << (char*)ReplyCmd << std::endl;
	return 0;
}



double lensDriver::InterpoaltionCommand(double current, double temperature) {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[7] = { 'Y' };
	unsigned char ReplyCmd[100];

	double maxCurrent = GetMaxOutputCurrent();
	int value_current = current * ((double)4095 / (double)maxCurrent);
	int value_temperature = temperature * 256;

	SendCmd[1] = get_high8(value_current);
	SendCmd[2] = get_low8(value_current);
	SendCmd[3] = get_high8(value_temperature);
	SendCmd[4] = get_low8(value_temperature);
	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[5] = get_low8(cs);
	SendCmd[6] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	unsigned short value_focalPower;

	value_focalPower = ReplyCmd[1] & 0xff;
	value_focalPower = (value_focalPower << 8) | (ReplyCmd[2] & 0xff);

	std::cout <<" Focal power : " <<signed16to10(value_focalPower) / 200.0 << std::endl;
	
	return signed16to10(value_focalPower) / 200.0;


}