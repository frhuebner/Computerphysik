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

vector<int> findMax(vector<double>& v){
	vector<int> ret = {0};
	for(int i=1;i<v.size()-1;i++){
		if(v[i-1] < v[i] && v[i] > v[i+1]){
			ret.push_back(i);
		}
	}
	return ret;
}

vector<int> findMin(vector<double>& v){
	vector<int> ret;
	for(int i=1;i<v.size()-1;i++){
		if(v[i-1] > v[i] && v[i] < v[i+1]){
			ret.push_back(i);
		}
	}
	return ret;
}

int main(int argc, char** argv){
	
	double theta_max = 1e-2;
	
	double dtheta = theta_max/(SAMPLES-1);
	
	vector<double> intensity(SAMPLES);
	
	intensity[0] = 1;
	
	for(int i=1;i<SAMPLES;i++){
		double theta = i*dtheta;
		double q = PI/632.8e-6*sin(theta);
		intensity[i] = pow(2*bessel(1,q)/q,2);
	}
	
	cout << "Maxima:" << endl;
	
	vector<int> maxima = findMax(intensity);
	for(int i = 0; i<maxima.size();i++){
		cout << i << ". maximum at " << maxima[i]*dtheta << ", I/I0 = " << intensity[maxima[i]] << endl;
	}
	cout << endl;
	
	cout << "Minima:" << endl;
	vector<int> minima = findMin(intensity);
	for(int i = 0; i<minima.size();i++){
		cout << (i+1) << ". minimum at " << minima[i]*dtheta << ", I/I0 = " << intensity[minima[i]] << endl;
	}
	
	//output intensity function in file
	ofstream fout;
	fout.open("slit.txt");
	for(int i=0;i<SAMPLES;i++){
		fout << i*dtheta << "\t" << intensity[i] << endl;
	}
	fout.close();
		
	return 0;
}
