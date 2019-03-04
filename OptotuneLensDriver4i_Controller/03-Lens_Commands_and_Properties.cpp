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

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 4);
	std::cout << "Set Current : " << inputCurrent << " [mA]" << std::endl;

	return 0;
}


double lensDriver::GetCurrent() {

	auto crc16 = new crc16ibm();

	unsigned char SendCmd[6] = { 'A','r',NULL,NULL };	//
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, 4);	//右辺…dataの要素数

	SendCmd[4] = get_low8(cs);
	SendCmd[5] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));

	//disp(SendCmd, 6);

	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	disp(ReplyCmd, 7);

	unsigned short value;

	value = ReplyCmd[1] & 0xff;
	std::cout << "adfa" << value << std::endl;

	value = (value << 8) | (ReplyCmd[2] & 0xff);

	std::cout << "adfa"<<value << std::endl;

	return signed16to10(value) * (GetMaxOutputCurrent() / (double)4095);
}


double lensDriver::GetSignalGeneratorUpperCurrentLimit() {
	auto crc16 = new crc16ibm();

	unsigned char SendCmd[10] = { 'P','r','U','A',NULL,NULL,NULL,NULL };
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



int lensDriver::SetSignalGeneratorUpperCurrentLimit(double upperSwingLimit) {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'P','r','U','A',NULL,NULL,NULL,NULL };
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
	//std::cout << "Set Current" << std::endl;

	return 0;
}

double lensDriver::GetSignalGeneratorLowerCurrentLimit() {
	auto crc16 = new crc16ibm();

	unsigned char SendCmd[10] = { 'P','r','L','A',NULL,NULL,NULL,NULL };
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
int lensDriver::SetSignalGeneratorLowerCurrentLimit(double lowerSwingLimit) {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'P','r','L','A',NULL,NULL,NULL,NULL };
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
	//std::cout << "Set Current" << std::endl;

	return 0;
}
double lensDriver::GetSignalGeneratorFrequency() {
	auto crc16 = new crc16ibm();

	unsigned char SendCmd[10] = { 'P','r','F','A',NULL,NULL,NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, 8);

	SendCmd[8] = get_low8(cs);
	SendCmd[9] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));

	//disp(SendCmd, 8);

	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	unsigned char org_str[] = { ReplyCmd[3] , ReplyCmd[4], ReplyCmd[5], ReplyCmd[6] };
	std::string str(org_str,std::end(org_str));

	unsigned int value;

	value = ReplyCmd[3] & 0xff;
	value = (value << 8) | (ReplyCmd[4] & 0xff);
	value = (value << 8) | (ReplyCmd[5] & 0xff);		
	value = (value << 8) | (ReplyCmd[6] );

	std::cout << "Get Frequency : " << value / 1000.0 <<" [Hz]"<< std::endl;

	return value/1000.0;

}
int lensDriver::SetSignalGeneratorFrequency(double frequency) {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'P','w','F','A' };

	if (abs(frequency) > maxFrequency) {
		std::cout << "This frequency value is out of range." << std::endl;
		std::cout << "Please set the absolute value less than " << maxFrequency << "[Hz]." << std::endl;
		return -1;
	}

	int value = frequency * 1000;

	SendCmd[4] = get_high8(value);
	SendCmd[5] = get_low8(value);

	SendCmd[7] = value & 0xff;
	SendCmd[6] = (value >> 8) & 0xff;
	SendCmd[5] = (value >> 16) & 0xff;
	SendCmd[4] = (value >> 24);
	auto cs = crc16->calc_checksum(SendCmd, 8);	//右辺…dataの要素数
	SendCmd[8] = get_low8(cs);
	SendCmd[9] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 4);

	return 0;

}

double lensDriver::GetMaxOutputCurrent() {

	auto crc16 = new crc16ibm();

	unsigned char SendCmd[8] = { 'C','r','M','A',NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, 6);	//右辺…dataの要素数

	SendCmd[4] = get_low8(cs);
	SendCmd[5] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));

	//disp(SendCmd, 8);

	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	unsigned short value;

	value = ReplyCmd[3] & 0xff;
	value = (value << 8) | (ReplyCmd[4] & 0xff);

	return value / 100;

}

double	lensDriver::GetFocalPower() {
	auto crc16 = new crc16ibm();

	unsigned char SendCmd[10] = { 'P','r','D','A',NULL,NULL,NULL,NULL };
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

	return signed16to10(value) / 200.0;

}


int	lensDriver::SetFocalPower(double focalPower) {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'P','w', 'D', 'A',NULL,NULL,NULL,NULL };
	//double maxCurrent = GetMaxOutputCurrent();
	char buf[200];
	char *endptr;

	//if (abs(inputCurrent) > maxCurrent) {
	//	std::cout << "This current value is out of range." << std::endl;
	//	std::cout << "Please set the absolute value less than " << maxCurrent << "[mA]." << std::endl;
	//	return -1;
	//}

	int value = focalPower * 200;

	SendCmd[4] = get_high8(value);
	SendCmd[5] = get_low8(value);
	auto cs = crc16->calc_checksum(SendCmd, 8);	//右辺…dataの要素数
	SendCmd[8] = get_low8(cs);
	SendCmd[9] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 4);
	std::cout << "Set focal power : " << focalPower << " [D]" << std::endl;

	return 0;

}
