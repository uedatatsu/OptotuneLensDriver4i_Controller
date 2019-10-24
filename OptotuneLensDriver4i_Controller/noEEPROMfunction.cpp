#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//FTLデータシートの読み込み（初回のみ）
void diopterTemperatureToCurrent_init(std::vector<std::vector<double>> &matrix) {
	char fname[50];
	sprintf_s(fname, "./data/currentTemperatureToDioper_matrix.csv");
	std::ifstream file(fname);
	std::vector<std::vector<std::string>> values;

	std::string str;
	int p;

	if (file.fail()) {
		std::cerr << "failed." << std::endl;
		exit(0);
	}

	while (getline(file, str)) {
		//コメント箇所は除く
		if ((p = str.find("//")) != str.npos) continue;
		std::vector<std::string> inner;

		//コンマがあるかを探し、そこまでをvaluesに格納
		while ((p = str.find(",")) != str.npos) {
			inner.push_back(str.substr(0, p));

			//strの中身は","の1文字を飛ばす
			str = str.substr(p + 1);
		}
		inner.push_back(str);
		values.push_back(inner);
	}

	for (int i = 0; i < values.size(); i++) {
		for (int j = 0; j < values[0].size(); j++) {
			matrix[i][j] = std::stod(values[i][j]);
			//std::cout << matrix[i][j] <<" ";
		}
		//std::cout << std::endl;

	}

}


//ジオプトリと温度からデータシートより適切な電流値へと変換
double diopterTemperatureToCurrent(std::vector<std::vector<double>> matrix, double diopter, double temperature) {

	return getNearestValue(matrix[(int)temperature], diopter) - 300;
}

// 最も近い値を格納した配列の添え字を求める
int getNearestValue(std::vector<double> list, double value) {
	int i;			// ループ用
	int num;		// 配列の添え字
	double minv;	// 配列値-指定値vの絶対値

	// 配列の個数が1未満の処理
	if (1 > list.size())return -1;

	// 指定値と全ての配列値の差を比較
	num = 0;
	minv = abs(list[0] - value);


	for (i = 1; i < list.size(); ++i) {
		if (abs(list[i] - value) < minv) {
			num = i;
			minv = abs(list[i] - value);
			//std::cout << " " << minv << std::endl;
		}
		if (minv < 0.01)break;
	}
	return num;
}