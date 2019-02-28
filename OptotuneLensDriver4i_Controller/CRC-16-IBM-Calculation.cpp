#include "pch.h"

crc16ibm::crc16ibm() {
	for (int i = 0; i < 256; i++) {
		unsigned short s = i;
		for (int dig = 0; dig < 8; dig++)
			s = (s & 1) ? (s >> 1) ^ polynomial : s >> 1;
		table[i] = s;
	}
}

unsigned short crc16ibm::calc_checksum(unsigned char* data, int len) {
	unsigned short div = 0;  //上位バイトの割り算の剰余
	for (int i = 0; i < len; i++)
		div = (div >> 8) ^ table[(div ^ data[i]) & 0xff];
	return div;
}

unsigned char get_low8(unsigned short crc16) {
	return crc16 & 0xFF;
}

unsigned char get_high8(unsigned short crc16) {
	return crc16 >> 8;
}

void disp(unsigned char *data, int len) {
	for (int i = 0; i < len; i++) {
		printf("%02x ", data[i]);
	}
	printf("\n");
}

int signed16to10(unsigned short value16) {
	int value10;
	if ((value16 >> 12) == 0xf) {
		
		value16 = (~value16 + 1) & 0xffff;
		
		value10 = -value16;
	}
	else value10 = value16;

	return value10;
}