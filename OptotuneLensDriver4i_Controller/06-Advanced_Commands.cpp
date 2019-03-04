#include "pch.h"
#include <iostream>

double lensDriver::InterpoaltionCommand(double current, double temperature) {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[7] = { 'Y' };
	unsigned char ReplyCmd[100];

	double maxCurrent = GetMaxOutputCurrent();

	if (abs(current) > maxCurrent) {
		std::cout << "This current value is out of range." << std::endl;
		std::cout << "Please set the absolute value less than " << maxCurrent << "[mA]." << std::endl;
		return -1;
	}
	int value_current = current * ((double)4095 / (double)maxCurrent);

	int value_temperature = temperature * 256;

	SendCmd[1] = get_high8(value_current);
	SendCmd[2] = get_low8(value_current);
	SendCmd[3] = get_high8(value_temperature);
	SendCmd[4] = get_low8(value_temperature);
	auto cs = crc16->calc_checksum(SendCmd, 5);	//âEï”ÅcdataÇÃóvëfêî
	SendCmd[5] = get_low8(cs);
	SendCmd[6] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	disp(ReplyCmd, 7);

	unsigned short value_focalPower;

	value_focalPower = ReplyCmd[1] & 0xff;
	value_focalPower = (value_focalPower << 8) | (ReplyCmd[2] & 0xff);

	std::cout <<" aguj;wo" <<signed16to10(value_focalPower) / 200.0 << std::endl;
	
	return signed16to10(value_focalPower) / 200.0;


}