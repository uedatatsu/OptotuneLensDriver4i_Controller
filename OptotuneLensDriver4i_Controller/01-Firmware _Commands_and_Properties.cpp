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




