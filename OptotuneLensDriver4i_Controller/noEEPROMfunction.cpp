#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//FTL�f�[�^�V�[�g�̓ǂݍ��݁i����̂݁j
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
		//�R�����g�ӏ��͏���
		if ((p = str.find("//")) != str.npos) continue;
		std::vector<std::string> inner;

		//�R���}�����邩��T���A�����܂ł�values�Ɋi�[
		while ((p = str.find(",")) != str.npos) {
			inner.push_back(str.substr(0, p));

			//str�̒��g��","��1�������΂�
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


//�W�I�v�g���Ɖ��x����f�[�^�V�[�g���K�؂ȓd���l�ւƕϊ�
double diopterTemperatureToCurrent(std::vector<std::vector<double>> matrix, double diopter, double temperature) {

	return getNearestValue(matrix[(int)temperature], diopter) - 300;
}

// �ł��߂��l���i�[�����z��̓Y���������߂�
int getNearestValue(std::vector<double> list, double value) {
	int i;			// ���[�v�p
	int num;		// �z��̓Y����
	double minv;	// �z��l-�w��lv�̐�Βl

	// �z��̌���1�����̏���
	if (1 > list.size())return -1;

	// �w��l�ƑS�Ă̔z��l�̍����r
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