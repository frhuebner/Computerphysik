#include<iostream>
#include<cstdlib>
#include<time.h>
#include<math.h>
#include<vector>
#include<limits.h>
#include<utility>
#include<fstream>

//constants in SI units
#define PI 3.14159265358979323846

//Precision for integrating, = h
#define INTEGRATE_DX 1E-5
//number of samples for the plot
#define SAMPLES 1000

using namespace std;

/*
 * calculates the besselfunction at x
 * n: the parameter of the besselfunction, n is a natural number
 * x: the coordinate
 * return: the besselfunction J_n(x)
 */ 
double bessel(int n, double x){
	//get the number of steps needed
	int m = (int)(PI/INTEGRATE_DX) + 1;
	//init the result with the border terms 1/2*(f(a)+f(b))
	//in this case there are either 0 or 1
	double res = ((n%2==1)?1:0);
	//do a trapezoidal integration
	for(int i=1;i<m;i++){
		double t = i*INTEGRATE_DX;
		res += cos(n*t - x*sin(t));
	}
	//return the result divided by Pi
	return res*INTEGRATE_DX/PI;
}

/*
 * Calculates the besselfunction J_n from l to r
 * the first argument is a natural number n
 * the second is the left interval limit
 * the third is the right interval limit
 */ 
int main(int argc, char** argv){
	//expect 3 parameters 
	if(argc != 4){
		cerr << "Wrong number of arguments!" << endl;
		return 1;
	}
	
	int n;
	double l;
	double r;
	//read parameters from the input
	sscanf(argv[1], "%d", &n);
	sscanf(argv[2], "%lf", &l);
	sscanf(argv[3], "%lf", &r);
	//calculate the distance between 2 samples
	double dx = (r-l)/SAMPLES;
	
	//generate the list of points
	for(int i=0;i<=SAMPLES;i++){
		//get the current x
		double x = i*dx; 
		//output x and the value of the bessel function
		cout << x << "\t" << bessel(n,x) << endl;
	}
		
	return 0;
}
