// OptotuneLensDriver4i_Controller.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>


int main()
{
	int comNum = 4;

	lensDriver optlens(comNum,baudrate);

	optlens.Handshake();
	optlens.ChangeToCurrentMode();
	optlens.SetCurrent(100);
	optlens.ReadCurrentlyActiveMode();
	std::cout<<"Current: "<<optlens.GetCurrent()<<std::endl;
	//optlens.getCurrent();
	std::cout << optlens.GetMaxOutputCurrent() << std::endl;
	optlens.ChangeToSinusoidalSignal();
	optlens.SetSignalGeneratorFrequency(17);
	optlens.GetSignalGeneratorFrequency();

	optlens.ChangeToFocalPowerControlledMode();
	optlens.SetFocalPower(2.6);
	std::cout << optlens.GetFocalPower() << std::endl;

	//FILE *fp1;
	//char fname[50];
	//sprintf_s(fname, "currentTemperatureToDioper_LUT.csv");
	//fopen_s(&fp1,fname, "w");
	//for (int c = -300; c < 300; c++) {
	//	for (int t = 0; t < 60; t++) {
	//		fprintf(fp1, "%d,%d,%f \n", c,t, optlens.InterpoaltionCommand(c, t));
	//	}
	//}
	//fclose(fp1);
	//optlens.InterpoaltionCommand(100, 30);


}
