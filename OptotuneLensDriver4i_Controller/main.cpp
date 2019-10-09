// OptotuneLensDriver4i_Controller.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>

using namespace std;

int main()
{
	int comNum = 9;

	lensDriver optlens(comNum,baudrate);

	optlens.Handshake();

	/*----------- EEPROMが使用可能か -------------*/
	std::vector<std::vector<double>>mat(60, std::vector<double>(600));		//FTLデータシート読み込み用
	int status = optlens.GetStatus();			//FTLの状態確認（EEPROMが使用可能か）
	if (status == 2) {			//EEPROMが使用可能でない場合、レンズドライバの電流制御モードを使用
		diopterTemperatureToCurrent_init(mat);			//FTLデータシート読み込み
		optlens.ChangeToCurrentMode();
	}
	if (status == 0) {								//EEPROMが使用可能である場合、レンズドライバのジオプトリ制御モードを使用
		optlens.ChangeToFocalPowerControlledMode();
	}

	// ジオプトリ設定はこのように行う
	//{
	//	current = diopterTemperatureToCurrent(mat, diopter, optlens.TemperatureReading());
	//	optlens.SetCurrent(current); 
	//}
	/*--------------------------------------------*/

	//unsigned char deviceID[]="A";
	//optlens.SerialNumber(deviceID);
	//std::cout<< deviceID <<std::endl;


	std::cout << std:: endl;

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
	std::cout << optlens.GetStatus() << std::endl;

	optlens.SetFocalPower(2.6);
	std::cout << optlens.GetFocalPower() << std::endl;

	FILE *fp1;
	char fname[50];
	sprintf_s(fname, "currentTemperatureToDioper_LUT_.csv");
	fopen_s(&fp1,fname, "w");
	int c = 3;
	//for (int c = -300; c < 300; c++) {
		for (int t = 0; t < 60; t++) {
			fprintf(fp1, "%d,%d,%f \n", c,t, optlens.InterpoaltionCommand(c, t));
		}
	//}
	fclose(fp1);
	//optlens.InterpoaltionCommand(100, 30);


}
