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
 * finds all the local Maxima of v, i.e. all the points that are greater than their neightbors 
 * v: the array where to find the maxima
 * return: an array with the indicies of all the maxima
 */ 
vector<int> findMax(vector<double>& v){
	//there's always a maximum at 0
	vector<int> ret = {0};
	//iterate over the whole vector
	for(int i=1;i<v.size()-1;i++){
		//if it's a local maximum -> add to result
		if(v[i-1] < v[i] && v[i] > v[i+1]){
			ret.push_back(i);
		}
	}
	return ret;
}

/*
 * finds all the local Minima of v, i.e. all the points that are lower than their neightbors 
 * v: the array where to find the minima
 * return: an array with the indicies of all the minima
 */ 
vector<int> findMin(vector<double>& v){
	//init the vector for returning
	vector<int> ret;
	//iterate over the whole vector
	for(int i=1;i<v.size()-1;i++){
		//if it's a local minimum -> add to result
		if(v[i-1] > v[i] && v[i] < v[i+1]){
			ret.push_back(i);
		}
	}
	return ret;
}

int main(int argc, char** argv){
	
	//plot will happen in range [0:theta_max]
	double theta_max = 1e-2;
	
	//calculate the distance between 2 points in the output
	double dtheta = theta_max/(SAMPLES-1);
	
	//stores the whole plot
	vector<double> intensity(SAMPLES);
	
	//at 0 the intensity is 1
	intensity[0] = 1;
	
	for(int i=1;i<SAMPLES;i++){
		//calculate the intensity here with the formulas from the sheet
		double theta = i*dtheta;
		double q = PI/632.8e-6*sin(theta);
		intensity[i] = pow(2*bessel(1,q)/q,2);
	}
	
	//find all the maxima
	cout << "Maxima:" << endl;
	
	vector<int> maxima = findMax(intensity);
	//... and output them
	for(int i = 0; i<maxima.size();i++){
		cout << i << ". maximum at " << maxima[i]*dtheta << ", I/I0 = " << intensity[maxima[i]] << endl;
	}
	cout << endl;
	
	//find all the minima
	cout << "Minima:" << endl;
	vector<int> minima = findMin(intensity);
	//... and output them
	for(int i = 0; i<minima.size();i++){
		cout << (i+1) << ". minimum at " << minima[i]*dtheta << ", I/I0 = " << intensity[minima[i]] << endl;
	}
	
	//output intensity function to a file
	ofstream fout;
	fout.open("slit.txt");
	for(int i=0;i<SAMPLES;i++){
		fout << i*dtheta << "\t" << intensity[i] << endl;
	}
	fout.close();
		
	return 0;
}
