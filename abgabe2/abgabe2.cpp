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
#define MASS_ELECTRON 9.10938356e-31
#define H_BAR 1.054571800e-34
#define CHARGE_ELECTRON 1.6021766208e-19

#define PRECISSION 1E-15

using namespace std;

typedef pair<double, bool> PDB;

static double xi;


double g(double z){
	return tan(z) - sqrt(xi*xi-z*z)/z;
}

double h(double z){
	return -1/tan(z) - sqrt(xi*xi-z*z)/z;
}

double bisektion(double (*f)(double), double l, double r){
	for(int i=0;r-l > PRECISSION && i<100;i++){
		double m = (l+r)/2;
		if(f(m) == 0) return m;
		if(f(m)*f(l) > 0){
			l = m;
		}else{
			r = m;
		}
	}
	return (l+r)/2;
}

vector<PDB> getListofz(){
	vector<PDB> ret;
	for(int k = 0;k*PI<xi;k++){
		//cout << "k " << k << endl;
		//even case
		double l = k*PI;
		double r = min(l+PI/2,xi)-PRECISSION; //subtract PRECISSION to make sure that r is in range
		if(r > l){
			double z = bisektion(g,l,r);
			PDB pdb(z,true);
			ret.push_back(pdb);
		}
		//odd case
		l = k*PI+PI/2;
		r = min(l+PI/2,xi)-PRECISSION; //subtract PRECISSION to make sure that r is in range
		if(r > l){
			double z = bisektion(h,l,r);
			PDB pdb(z,false);
			ret.push_back(pdb);
		}
	}
	return ret;
}

int main(int argc, char** argv){
	
	std::cout.precision(15);
	
	if(argc != 3){
		cerr << "Wrong number of arguments!" << endl;
		return 1;
	}
	//a in pm
	double a;
	//V0 in eV 
	double v0;
	sscanf(argv[1], "%lf", &a);
	sscanf(argv[2], "%lf", &v0);
	
	xi = sqrt(2*MASS_ELECTRON*v0*CHARGE_ELECTRON)/H_BAR*a*1E-9;
	cout << "a: " << a << " pm" << endl;
	cout << "V0: " << v0 << " eV" << endl;
	cout << "xi: " << xi << endl;
	
	vector<PDB> res = getListofz();
	for(auto v: res){
		double energy = (v.first/a)*(v.first/a)*1e18*H_BAR*H_BAR/2/MASS_ELECTRON-v0*CHARGE_ELECTRON;
		cout << v.first << "\t" << v.second << "\t" << (v.second?g(v.first):h(v.first)) << "\t" << energy << endl;  
	}
	
	return 0;
}
