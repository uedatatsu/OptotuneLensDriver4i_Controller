#pragma once
#include<vector>

void diopterTemperatureToCurrent_init(std::vector<std::vector<double>> &matrix);
double diopterTemperatureToCurrent(std::vector<std::vector<double>> matrix, double diopter, double temperature);
int getNearestValue(std::vector<double> list, double value);