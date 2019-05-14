#include "pch.h"
#include <iostream>

int lensDriver::CommunicationError() {
	std::cout << "Error ID" << std::endl;
	std::cout << "1 - CRC failed" << std::endl;
	std::cout << "2 - Command not available in firmware type" << std::endl;
	std::cout << "3 - Command not recognized" << std::endl;
	std::cout << "4 - Lens is not compatible with firmware"<< std::endl;
	return 1;
}
