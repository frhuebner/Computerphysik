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
#define PARSEC 3.08567758149136E16

//Number of samples for integrating, i.e. h=1/(SAMPLES-1) 
#define SAMPLES 1E8
//Number of samples for output
#define SAMPLES_PLOT 1E3

using namespace std;

//H0 in SI and density-parameters
static double h0, omega_l, omega_m, omega_r, omega_k;

/*
 * the function E(a) as defined on the sheet
 * return: E(a)
 */ 
double E(double a){
	return omega_l + omega_m/a/a/a + omega_r/a/a/a/a + omega_k/a/a; 
}

/*
 * the function to integrate, see pdf
 * return: f(a)
 */ 
double f(double a){
	//handle a=0 because function undefined there, see pdf
	if(a==0){
		if(omega_m != 0 || omega_r != 0) return 0;
		return 1/sqrt(omega_k);
	}
	//else return the normal value there
	return 1/a/sqrt(E(a));
}

/*
 * Integrates f(a) from 0 to 1 in steps of 1/(SAMPLES-1)
 * return: a vector with size SAMPLES containing the antiderivative F at i'th sample.
 * 	i -> F(i*1/(SAMPLES-1))
 */ 
vector<double> integrate(){
	//calculate the step da
	double da = 1/(SAMPLES-1);
	//vector to return later
	vector<double> antiderivative(SAMPLES);
	//init antiderivative at 0 to 0
	antiderivative[0] = 0;
	//integrate from 0 to 1
	for(int i=1;i<SAMPLES;i++){
		//calculate current a
		double a = i*da;
		//calculate the integral at a by adding the approximate integral
		antiderivative[i] = antiderivative[i-1] + da*(f(a-da)+f(a))/2;
	}
	return antiderivative;
}

/*
 * Calculates the time in y of the universe in terms of z
 * the first argument is H0 in km/s/Mpc 
 * the second ist Omega_Lambda
 * the third is Omega_m
 * the fourth is Omega_r
 */ 
int main(int argc, char** argv){
	//expecting 4 arguments
	if(argc != 5){
		cerr << "Wrong number of arguments!" << endl;
		return 1;
	}
	//read all parameters
	sscanf(argv[1], "%lf", &h0);
	sscanf(argv[2], "%lf", &omega_l);
	sscanf(argv[3], "%lf", &omega_m);
	sscanf(argv[4], "%lf", &omega_r);
	//calculate omega_k
	omega_k = 1-omega_l-omega_m-omega_r;
	
	//calculate 1/H0 in years
	double time_unit = 1/h0*PARSEC*1E3/3600/24/365.25;
	
	//get the antiderivative
	vector<double> integral = integrate();
	
	//output the plot as a list of points, iterate over all samples, skip 0 because it belongs to oo  
	for(int i = 1;i<SAMPLES_PLOT;i++){
		//calculate a 
		double a = i/(SAMPLES_PLOT-1);
		//calculate z
		double z = 1/a-1;
		//calculate the index at which one can find the value in the array
		double a_i = a*(SAMPLES-1);
		//if a_i is a floating point number -> do a linear interpolation between the next two integers
		//calculate the next integers
		double a_i_c = ceil(a_i), a_i_f = floor(a_i);
		double value;
		//if a_i is an integer -> take the value at this index
		if(a_i_c == a_i_f) value = integral[a_i];
		//else do a linear interpolation between these 2 points
		else value = integral[a_i_f] + (integral[a_i_c]-integral[a_i_f])/(a_i_c-a_i_f)*(a_i-a_i_f);
		
		//before output cast time to years
		cout << z << "\t" << value*time_unit << endl;
	}
	return 0;
}
