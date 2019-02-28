// OptotuneLensDriver4i_Controller.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"
#include <iostream>


int main()
{
	int comNum = 4;

	lensDriver optlens(comNum,baudrate);

	optlens.Handshake();

	optlens.SetCurrent(250);

	std::cout<<"Current: "<<optlens.GetCurrent()<<std::endl;
	//optlens.getCurrent();
	std::cout << optlens.GetMaxOutputCurrent() << std::endl;

}
