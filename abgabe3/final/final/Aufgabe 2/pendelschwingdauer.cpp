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

#define _USE_MATH_DEFINES

using namespace std;

int main()
{
	vector<double> w {0.5/3, 1/1.5, 0.5/3};													// simpson-coefficients
	int n = 2;
	double a = 0;
	double b = M_PI/2;
	double h = (b-a)/n;
	double sum = 0;
	double l;
	cout << "length of pendulum:" << endl << "l = " << endl;
	cin >> l;																				//taking in the length of the pendulum
	
	ofstream myfile;
	myfile.open("data2.txt");
	
	/*integrating the function with simpson-method for all deflections phi_0 between 0 and 180 degrees*/
	for (int k = 0; k < 181; k++)
	{
		sum = 0;
		//calculating the sum for newton-cotes
		for (int i = 0; i < (n+1); i++) 
		{
			double x = a + i*h;																//calculation of x_i
			double f = w[i] * 1/sqrt(1-pow(sin((2*M_PI)*(k/2.0)/360.0),2)*pow(sin(x),2));	//calculation of f_i(x_i)
			sum = sum + f;																	//adding f_i to sum
		}
		double T = 4 * sqrt(l/9.81)* (b-a)* sum;											//calculation of Time T with Newton-Cotes-fomula
		double T0 = 2*M_PI*sqrt(l/9.81);													//calculation of harmonic value								
		double Tges = T - T0;																//calculation of difference
		
		cout << k << "\t" << Tges << endl;
		myfile << k << "\t" << Tges << endl;
	}
	myfile.close();
	return 0;
}	
