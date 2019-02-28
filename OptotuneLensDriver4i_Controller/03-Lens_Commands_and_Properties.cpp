#include "pch.h"
#include <iostream>


int lensDriver::SetCurrent(double inputCurrent) {

	auto crc16 = new crc16ibm();
	unsigned char SendCmd[6] = { 'A','w' };
	double maxCurrent = GetMaxOutputCurrent();
	char buf[200];
	char *endptr;

	if (abs(inputCurrent) > maxCurrent) {
		std::cout << "This current value is out of range." << std::endl;
		std::cout << "Please set the absolute value less than " << maxCurrent << "[mA]." << std::endl;
		return -1;
	}

	int value = inputCurrent * ((double)4095 / (double)GetMaxOutputCurrent());

	//if (inputCurrent < 0) {
	//	value = -value;
	//	snprintf(buf, 200, "%#x", value);
	//	value = -strtol(buf, &endptr, 16);
	//	printf("%s--------------\n", buf);

	//}
	//else {
	//	snprintf(buf, 200, "%#x", value);
	//	//printf("%s--------------\n", buf);
	//	value = strtol(buf, NULL, 16);

	//}

	SendCmd[2] = get_high8(value);
	SendCmd[3] = get_low8(value);
	auto cs = crc16->calc_checksum(SendCmd, 4);	//右辺…dataの要素数
	SendCmd[4] = get_low8(cs);
	SendCmd[5] = get_high8(cs);

	write((char *)SendCmd, 6);
	disp(SendCmd, 4);
	//std::cout << "Set Current" << std::endl;

	return 0;
}


double lensDriver::GetCurrent() {

	auto crc16 = new crc16ibm();

	unsigned char SendCmd[6] = { 'A','r',NULL,NULL };	//
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, 4);	//右辺…dataの要素数

	SendCmd[4] = get_low8(cs);
	SendCmd[5] = (cs >> 8);

	write((char *)SendCmd, 6);

	//disp(SendCmd, 6);

	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	unsigned short value;

	printf("%x   %x\n", ReplyCmd[1], ReplyCmd[2]);
	value = ReplyCmd[1] & 0xff;
	value = (value << 8) | (ReplyCmd[2] & 0xff);
	printf("%x\n", value);

	return signed16to10(value) * (GetMaxOutputCurrent() / (double)4095);
}