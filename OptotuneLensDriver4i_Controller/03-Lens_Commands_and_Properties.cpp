#include "pch.h"
#include <iostream>
#include <tuple>

void lensDriver::SerialNumber(unsigned char * serialNumber) {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[3] = { 'X',NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[1] = get_low8(cs);
	SendCmd[2] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, COUNTOF(SendCmd));
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, COUNTOF(ReplyCmd));

	std::cout << "Serial number : ";
	for (int i = 0; i < 8; i++) {
		serialNumber[i] = ReplyCmd[i + 1];
		std::cout << serialNumber[i];
	}
	std::cout << std::endl;
}


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

	SendCmd[2] = get_high8(value);
	SendCmd[3] = get_low8(value);
	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[4] = get_low8(cs);
	SendCmd[5] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	std::cout << "Set current : " << inputCurrent << " [mA]" << std::endl;

	return 0;
}


double lensDriver::GetCurrent() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[6] = { 'A','r',NULL,NULL };	//
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[4] = get_low8(cs);
	SendCmd[5] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 6);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	unsigned short value;
	value = ReplyCmd[1] & 0xff;
	value = (value << 8) | (ReplyCmd[2] & 0xff);

	std::cout << "Get current : "<< signed16to10(value) * (GetMaxOutputCurrent() / (double)4095) << " [mA]" << std::endl;

	return signed16to10(value) * (GetMaxOutputCurrent() / (double)4095);
}


double lensDriver::GetSignalGeneratorUpperCurrentLimit() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'P','r','U','A',NULL,NULL,NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[8] = get_low8(cs);
	SendCmd[9] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 8);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	unsigned short value;
	value = ReplyCmd[3] & 0xff;
	value = (value << 8) | (ReplyCmd[4] & 0xff);

	return signed16to10(value) * (GetMaxOutputCurrent() / (double)4095);
}



int lensDriver::SetSignalGeneratorUpperCurrentLimit(double upperSwingLimit) {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'P','w','U','A',NULL,NULL,NULL,NULL };
	double maxCurrent = GetMaxOutputCurrent();

	if (abs(upperSwingLimit) > maxCurrent) {
		std::cout << "This current value is out of range." << std::endl;
		std::cout << "Please set the absolute value less than " << maxCurrent << "[mA]." << std::endl;
		return -1;
	}

	int value = upperSwingLimit * ((double)4095 / (double)GetMaxOutputCurrent());

	SendCmd[4] = get_high8(value);
	SendCmd[5] = get_low8(value);
	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[8] = get_low8(cs);
	SendCmd[9] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 4);

	return 0;
}

double lensDriver::GetSignalGeneratorLowerCurrentLimit() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'P','r','L','A',NULL,NULL,NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[8] = get_low8(cs);
	SendCmd[9] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 8);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	unsigned short value;
	value = ReplyCmd[3] & 0xff;
	value = (value << 8) | (ReplyCmd[4] & 0xff);

	return signed16to10(value) * (GetMaxOutputCurrent() / (double)4095);
}

int lensDriver::SetSignalGeneratorLowerCurrentLimit(double lowerSwingLimit) {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'P','w','L','A',NULL,NULL,NULL,NULL };
	double maxCurrent = GetMaxOutputCurrent();

	if (abs(lowerSwingLimit) > maxCurrent) {
		std::cout << "This current value is out of range." << std::endl;
		std::cout << "Please set the absolute value less than a" << maxCurrent << "[mA]." << std::endl;
		return -1;
	}

	int value = lowerSwingLimit * ((double)4095 / (double)GetMaxOutputCurrent());

	SendCmd[4] = get_high8(value);
	SendCmd[5] = get_low8(value);
	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[8] = get_low8(cs);
	SendCmd[9] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 4);

	return 0;
}
double lensDriver::GetSignalGeneratorFrequency() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'P','r','F','A',NULL,NULL,NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[8] = get_low8(cs);
	SendCmd[9] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 8);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

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
	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[8] = get_low8(cs);
	SendCmd[9] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 4);

	return 0;

}



int	lensDriver::SetTemperatureLimits(double maximumTemperature, double minimumTemperature) {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'P','w','T', 'A' };	//
	unsigned char ReplyCmd[100];

	if (maximumTemperature < minimumTemperature) {
		std::cout << "This maximum temperature value is less than minimum temperature value." << std::endl;
		std::cout << "Please set correct values." << std::endl;
		return -1;
	}
	else if (maximumTemperature>=120) {
		std::cout << "This maximum temperature value is out of range." << std::endl;
		std::cout << "Please set the absolute value less than 120 [℃]." << std::endl;
		return -1;
	}
	else if (minimumTemperature <= -120) {
		std::cout << "This minimum temperature value is out of range." << std::endl;
		std::cout << "Please set the absolute value more than -120 [℃]." << std::endl;
		return -1;
	}

	int value_maximumTemperature = maximumTemperature * 16;
	int value_minimumTemperature = minimumTemperature * 16;

	SendCmd[4] = get_high8(value_maximumTemperature);
	SendCmd[5] = get_low8(value_maximumTemperature);
	SendCmd[6] = get_high8(value_minimumTemperature);
	SendCmd[7] = get_low8(value_minimumTemperature);
	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[8] = get_low8(cs);
	SendCmd[9] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	Sleep(waitTime);
	disp(SendCmd, COUNTOF(SendCmd));


	read((char *)ReplyCmd, 100, true);
	disp(ReplyCmd, 8);

	unsigned short value;
	unsigned short value_;

	value = ReplyCmd[3] & 0xff;
	value = (value << 8) | (ReplyCmd[4] & 0xff);
	value_ = ReplyCmd[5] & 0xff;
	value_ = (value_ << 8) | (ReplyCmd[6] & 0xff);

	std::cout << "Max focal power limit : " << signed16to10(value) / 200.0 << std::endl;
	std::cout << "Min focal power limit : " << signed16to10(value_) / 200.0 << std::endl;
	std::cout << std::endl;

	return 0;

}

std::tuple<double, double> lensDriver::GetTemperatureLimits() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'P','r', 'T', 'A',NULL,NULL,NULL,NULL };	//
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[8] = get_low8(cs);
	SendCmd[9] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	Sleep(waitTime);
	disp(SendCmd, 10);

	read((char *)ReplyCmd, 100, true);
	disp(ReplyCmd, 8);

	unsigned short value;
	unsigned short value_;

	value = ReplyCmd[2] & 0xff;
	value = (value << 8) | (ReplyCmd[3] & 0xff);
	value_ = ReplyCmd[4] & 0xff;
	value_ = (value_ << 8) | (ReplyCmd[5] & 0xff);
	
	std::cout << "Max temperature limit : " << signed16to10(value) / 16.0 << std::endl;
	std::cout << "Min temperature limit : " << signed16to10(value_) / 16.0 << std::endl;
	std::cout << std::endl;

	return std::forward_as_tuple(signed16to10(value) / 16.0, signed16to10(value_) / 16.0);
	//返答コマンドが不整合の恐れあり、要検証

}

std::tuple<double, double>lensDriver::GetEEPROMTemperatureValues() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'P', 'r', 'O', 'A',NULL,NULL,NULL,NULL };	//
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[8] = get_low8(cs);
	SendCmd[9] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	Sleep(waitTime);
	disp(SendCmd, 10);

	read((char *)ReplyCmd, 100, true);
	disp(ReplyCmd, 8);

	unsigned short value;
	unsigned short value_;

	value = ReplyCmd[2] & 0xff;
	value = (value << 8) | (ReplyCmd[3] & 0xff);
	value_ = ReplyCmd[4] & 0xff;
	value_ = (value_ << 8) | (ReplyCmd[5] & 0xff);

	std::cout << "First EEPROM temperature  : " << signed16to10(value) / 16.0 << std::endl;
	std::cout << "Second EEPROM temperature : " << signed16to10(value_) / 16.0 << std::endl;
	std::cout << std::endl;

	return std::forward_as_tuple(signed16to10(value) / 16.0, signed16to10(value_) / 16.0);

	//返答コマンドが不整合の恐れあり、要検証
}

int	lensDriver::SetEEPROMTemperatureValues(double firstTemperature, double secondTemperature) {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'P', 'r', 'T', 'A',NULL,NULL,NULL,NULL };	//
	unsigned char ReplyCmd[100];

	int value_firstTemperature = firstTemperature * 16;
	int value_secondTemperature = secondTemperature * 16;

	SendCmd[4] = get_high8(value_firstTemperature);
	SendCmd[5] = get_low8(value_firstTemperature);
	SendCmd[6] = get_high8(value_secondTemperature);
	SendCmd[7] = get_low8(value_secondTemperature);
	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[8] = get_low8(cs);
	SendCmd[9] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	Sleep(waitTime);
	disp(SendCmd, COUNTOF(SendCmd));
	return 1;
}


double	lensDriver::GetFocalPower() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[10] = { 'P','r','D','A',NULL,NULL,NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);

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

	int value = focalPower * 200;

	SendCmd[4] = get_high8(value);
	SendCmd[5] = get_low8(value);
	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[8] = get_low8(cs);
	SendCmd[9] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 4);
	std::cout << "Set focal power : " << focalPower << " [D]" << std::endl;

	return 0;

}

int	lensDriver::SetMaxOutputCurrent(double maxCurrent) {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[8] = { 'C','w', 'M', 'A' };
	unsigned char ReplyCmd[100];

	int value = maxCurrent*100;

	SendCmd[4] = get_high8(value);
	SendCmd[5] = get_low8(value);
	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[6] = get_low8(cs);
	SendCmd[7] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	unsigned short value_;

	value_ = ReplyCmd[3] & 0xff;
	value_ = (value_ << 8) | (ReplyCmd[4] & 0xff);

	return signed16to10(value_) / 100.0;
}

double lensDriver::GetMaxOutputCurrent() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[8] = { 'C','r','M','A',NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
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

	return value / 100.0;
}

int	lensDriver::SetCalibrationFactor(double currentFactor) {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[8] = { 'C','w', 'C', 'A' };
	unsigned char ReplyCmd[100];

	int value = currentFactor * 100;

	SendCmd[4] = get_high8(value);
	SendCmd[5] = get_low8(value);
	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[6] = get_low8(cs);
	SendCmd[7] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	unsigned short value_;
	value_ = ReplyCmd[3] & 0xff;
	value_ = (value_ << 8) | (ReplyCmd[4] & 0xff);

	return signed16to10(value_) / 100.0;
}

double lensDriver::GetCalibrationFactor() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[8] = { 'C','r','C','A',NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
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

	return value / 100.0;
}


double	lensDriver::TemperatureReading() {
	auto crc16 = new crc16ibm();

	unsigned char SendCmd[5] = { 'T','C','A' };	//
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);

	SendCmd[3] = get_low8(cs);
	SendCmd[4] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 6);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	unsigned short value;
	value = ReplyCmd[3] & 0xff;
	value = (value << 8) | (ReplyCmd[4] & 0xff);
	
	return signed16to10(value) * 0.0625;
}

double	lensDriver::GetStatus() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[4] = { 'S','r' };	//
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[2] = get_low8(cs);
	SendCmd[3] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, COUNTOF(SendCmd));

	Sleep(waitTime);
	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 10);

	std::cout << "Status : "<< std::endl;

	//Status Byte #1
	if (signed16to10(ReplyCmd[4] & 0x01) == 1)std::cout << "Not all hardware available" << std::endl;
	if (signed16to10(ReplyCmd[4] & 0x02) == 2)std::cout << "No or faulty EEPROM" << std::endl;
	if (signed16to10(ReplyCmd[4] & 0x04) == 4)std::cout << "No temperature limits received (for controlled mode)" << std::endl;
	if (signed16to10(ReplyCmd[4] & 0x08) == 8)std::cout << "Cannot reach lens focal power (Focal Power Controlled)/position (Position Controlled)" << std::endl;
	if (signed16to10(ReplyCmd[4] & 0x10) == 16)std::cout << "Focal power inversion (defined by user set temperature range)" << std::endl;
	if (signed16to10(ReplyCmd[4] & 0x20) == 32)std::cout << "Temperature is outside product specifications" << std::endl;
	if (signed16to10(ReplyCmd[4] & 0x20) == 32)std::cout << "Focal power out of guaranteed range (defined by user set temperature range) " << std::endl;
	if (signed16to10(ReplyCmd[4] & 0x40) == 64)std::cout << "Temp out of range specified by user" << std::endl;

	//Status Byte #2
	if (signed16to10(ReplyCmd[3] & 0x01) == 1)std::cout << "The connected lens is not compatible with the firmware on the lensdriver." << std::endl;

	//Status Byte #3
	if (signed16to10(ReplyCmd[2] & 0x01) == 1)std::cout;

	//Status Byte #4
	if (signed16to10(ReplyCmd[1] & 0x01) == 1)std::cout;

	//Other
	if (signed16to10(ReplyCmd[4]) == 0 && signed16to10(ReplyCmd[3]) == 0 && signed16to10(ReplyCmd[2]) == 0 && signed16to10(ReplyCmd[1]) == 0)std::cout << "No problem." << std::endl;

	return 0;
}

double	lensDriver::ReadAddressFromTheEEPROM() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[5] = { 'Z','r','\n' };	//
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[3] = get_low8(cs);
	SendCmd[4] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 6);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	disp(ReplyCmd, 7);

	unsigned short value;
	value = ReplyCmd[1] & 0xff;
	
	return signed16to10(value) ;
}

int	lensDriver::AnalogInputReading() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[5] = { 'G','A','A' };	//
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[3] = get_low8(cs);
	SendCmd[4] = (cs >> 8);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 6);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	unsigned short value;
	value = ReplyCmd[3] & 0xff;
	value = (value << 8) | (ReplyCmd[4] & 0xff);

	return signed16to10(value);
}

int	lensDriver::WriteByteToEEPROM(int registerID, int byteToWrite){
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[6] = { 'Z', 'w'};	//
	unsigned char ReplyCmd[100];
	
	SendCmd[2] = get_low8(registerID);
	SendCmd[3] = get_low8(byteToWrite);

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[4] = get_low8(cs);
	SendCmd[5] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 6);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	unsigned short value;
	value = ReplyCmd[1] & 0xff;

	return signed16to10(value);
}

int	lensDriver::ReadEEPROM() {
	auto crc16 = new crc16ibm();
	unsigned char SendCmd[5] = { 'D', NULL,NULL };	//
	unsigned char ReplyCmd[100] = { 0 };

	auto cs = crc16->calc_checksum(SendCmd, COUNTOF(SendCmd) - 2);
	SendCmd[3] = get_low8(cs);
	SendCmd[4] = get_high8(cs);

	write((char *)SendCmd, COUNTOF(SendCmd));
	//disp(SendCmd, 6);
	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 100);

	unsigned short value;
	value = ReplyCmd[1] & 0xff;

	return signed16to10(value);
	//返答コマンドが不整合の恐れあり、要検証
}



