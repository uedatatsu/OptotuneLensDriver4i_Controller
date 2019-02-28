#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
int lensDriver::Handshake() {
	//Serial.obj(com_no, baudrate);
	
	char ReplyCmd[100];

	write("Start");
	
	Sleep(waitTime);
	read(ReplyCmd, 100, true);
	//std::cout << obj.read(ReplyCmd, 100, true) << std::endl;
	std::cout <<  (char*)ReplyCmd << std::endl;
	return 0;
}



double lensDriver::GetMaxOutputCurrent() {

	auto crc16 = new crc16ibm();

	unsigned char SendCmd[8] = { 'C','r','M','A',NULL,NULL };
	unsigned char ReplyCmd[100];

	auto cs = crc16->calc_checksum(SendCmd, 6);	//‰E•Ócdata‚Ì—v‘f”

	SendCmd[4] = get_low8(cs);
	SendCmd[5] = (cs >> 8);

	write((char *)SendCmd, 8);

	//disp(SendCmd, 8);

	Sleep(waitTime);

	read((char *)ReplyCmd, 100, true);
	//disp(ReplyCmd, 7);

	unsigned short value;

	value = ReplyCmd[3] & 0xff;
	value = (value << 8) | (ReplyCmd[4] & 0xff);

	return value / 100;

}

