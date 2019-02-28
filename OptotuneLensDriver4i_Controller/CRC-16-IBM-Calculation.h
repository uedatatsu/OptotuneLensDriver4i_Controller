#pragma once


class crc16ibm {
	const unsigned short polynomial = 0xA001;
	unsigned short table[256];

public:

	crc16ibm();

	unsigned short calc_checksum(unsigned char* data, int len);		



};

unsigned char get_low8(unsigned short crc16);
unsigned char get_high8(unsigned short crc16);

void disp(unsigned char *data, int len);
int signed16to10(unsigned short value16);