#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <utility>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <fstream>

//constants
#define _USE_MATH_DEFINES
#define PLANCK 6.626176e-34
#define c_0 299792458
#define k_B 1.38066e-23

using namespace std;

int main()
{
	vector<double> w {3.5/45, 8/22.5, 3/22.5, 8/22.5, 3.5/45};											//Milne-coefficients
	int n = 4;
	double a = 380e-9;
	double b = 780e-9;
	double h = (b-a)/n;
	double sum = 0;

	ofstream myfile;
	myfile.open("data3.txt");
	
	//integrating the function with newton-cotes-method for Temperatures between 100K and 50000K
	for (int T = 100; T < 50001; T+=200)
	{
		sum = 0;
		//calculating the sum fpr newton-cotes
		for (int i = 0; i < (n+1); i++) 
		{
			double x = a + i*h;																			//calculation of x_i
			double f = w[i] * ((2*M_PI*PLANCK*pow(c_0,2))/pow(x,5) * 1/(exp(PLANCK*c_0/(x*k_B*T))-1));	//calculating f_i
			sum = sum + f;																				//adding f_i to sum
		}
		double E = ((b-a)*sum)/(pow(T,4)*2*pow(M_PI,5)*pow(k_B,4)/(15*pow(PLANCK,3)*pow(c_0,2)));		//calculating Energy E
	
		cout << T << "\t" << E << endl;
		myfile << T << "\t" << E << endl;
	}
	myfile.close();
	return 0;
}	
