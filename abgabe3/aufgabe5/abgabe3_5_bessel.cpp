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

//Precision for finding the roots 
#define PRECISION 1E-15
#define INTEGRATE_DX 1E-5
#define SAMPLES 1000

using namespace std;


double bessel(int n, double x){
	int m = (int)(PI/INTEGRATE_DX) + 1;
	double res = ((n%2==1)?1:0);
	for(int i=1;i<m;i++){
		double t = i*INTEGRATE_DX;
		res += cos(n*t - x*sin(t));
	}
	return res*INTEGRATE_DX/PI;
}


int main(int argc, char** argv){
	
	if(argc != 4){
		cerr << "Wrong number of arguments!" << endl;
		return 1;
	}
	
	int n;
	double l;
	double r;
	sscanf(argv[1], "%d", &n);
	sscanf(argv[2], "%lf", &l);
	sscanf(argv[3], "%lf", &r);
	double dx = (r-l)/SAMPLES;
	
	for(int i=0;i<=SAMPLES;i++){
		double x = i*dx; 
		cout << x << "\t" << bessel(n,x) << endl;
	}
		
	return 0;
}
