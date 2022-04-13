#ifndef LIBRERIA_H
#define LIBRERIA_H

#include <iostream>
#include <cmath>
#include <vector>
#include <TCanvas.h>
#include <TRandom3.h>
#include <TApplication.h>
#include <TGraphErrors.h>
#include <fstream>


using namespace std;


void readfile(string nomefile, int &n, vector<string>& at, string& a1, string& a2, double& b1, double& b2,vector<double>& X, vector<double>& Y, vector<double>& Z);

void writefile (ofstream& file, int& n, vector<string>& at, string& a1, string& a2, double& b1, double& b2, vector<double>& X, vector<double>& Y, vector<double>& Z);

double energia_pot(int& n, double& epsilon, double& sigma,double& kb, vector<double>& X, vector<double>& Y, vector<double>& Z);

double energia_cin(int& n, double& m, vector<double>& VX, vector<double>& VY, vector<double>& VZ);

void forza(int& n, double& epsilon, double& sigma, vector<double>& X, vector<double>& Y, vector<double>& Z, vector<double> &fx, vector<double> &fy, vector<double> &fz);



#endif
