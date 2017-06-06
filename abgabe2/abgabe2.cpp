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

//Precision for finding the roots 
#define PRECISION 1E-15

using namespace std;

typedef pair<double, bool> PDB;

static double xi;

/*
 * Function for the even case
 * z: variable
 * return: tan(z) - sqrt(xi*xi-z*z)/z
 */ 
double g(double z){
	return tan(z) - sqrt(xi*xi-z*z)/z;
}

/*
 * Function for the odd case
 * z: variable
 * return: -cot(z) - sqrt(xi*xi-z*z)/z
 */ 
double h(double z){
	return -1/tan(z) - sqrt(xi*xi-z*z)/z;
}

/*
 * Computes a root of f in the range (l,r) via bisection-method with the precision of PRECISION
 * Expects that f(l) and f(r) have different sign!
 * The precission is PRECISION or the machine precission
 * f: function which roots should be found
 * l: the left end of the search interval
 * r: the right range of the search interval
 * return: a root of f in (l,r) with the precision PRECISION
 */ 
double bisektion(double (*f)(double), double l, double r){
	/* n is the number of iterations
	 * n = log2((r-l)/PRECISION)
	 * +2 to avoid rounding errors
	 */ 
	int n = (int) (log2(r-l)-log2(PRECISION)) + 2;
	for(int i=0;i<n;i++){
		//take the middle of the interval
		double m = (l+r)/2;
		//foound zero of f?
		if(f(m) == 0) return m;
		//adjust l or r
		if(f(m)*f(l) > 0){
			l = m;
		}else{
			r = m;
		}
	}
	//return the middle of the leftover interval
	return (l+r)/2;
}

/*
 * Returns a list of all z that are either root of g or h
 * due to the algorithm the list will be sorted with increasing z
 * return: a list of a pair of double and bool.
 * 	the double is the z value of the zero
 *  the bool is true if its an solution for the even case, else it is false
 */
vector<PDB> getListofz(){
	//vector that will be returned
	vector<PDB> ret;
	//in every pi intervall will be a zero for each case
	//so iterate over all these intervalls
	for(int k = 0;k*PI<xi;k++){
		//even case
		double l = k*PI;
		double r = min(l+PI/2,xi)-PRECISION; //subtract PRECISION to make sure that r is in range
		//check if r > l, this is especially important for the last intervall
		if(r > l){
			//do the bisection-method
			double z = bisektion(g,l,r);
			//add the zero to the list
			PDB pdb(z,true);
			ret.push_back(pdb);
		}
		//odd case
		l = k*PI+PI/2;
		r = min(l+PI/2,xi)-PRECISION; //subtract PRECISION to make sure that r is in range
		//check if r > l, this is especially important for the last intervall
		if(r > l){
			//do the bisection-method
			double z = bisektion(h,l,r);
			//add the zero to the list
			PDB pdb(z,false);
			ret.push_back(pdb);
		}
	}
	return ret;
}

/*
 * Calculates all the the Energys for the potential well problem
 * the first argument is the half of the width of the potential well in pm
 * the second argument is the depth of the potential well in eV
 */
int main(int argc, char** argv){
	
	std::cout.precision(15);
	
	//expects 2 arguments, else stop programm
	if(argc != 3){
		cerr << "Wrong number of arguments!" << endl;
		return 1;
	}
	//a in pm
	double a;
	//V0 in eV 
	double v0;
	//read them from the arguments
	sscanf(argv[1], "%lf", &a);
	sscanf(argv[2], "%lf", &v0);
	
	//calulate xi
	xi = sqrt(2*MASS_ELECTRON*v0*CHARGE_ELECTRON)/H_BAR*a*1E-9;
	cout << "a: " << a << " pm" << endl;
	cout << "V0: " << v0 << " eV" << endl;
	cout << "xi: " << xi << endl;
	cout << endl;
	
	//print the number of solutions in both cases
	cout << "Number of solutions in the even case: " << floor(xi/PI)+1 << endl;
	cout << "Number of solutions in the odd case: " << floor(xi/PI-0.5)+1 << endl;
	
	//get list of all roots
	vector<PDB> res = getListofz();
	
	//print the number of actually found cases
	//should be the sum of the two numbers above,  could differ because of machine precision etc.
	cout << "Number of solutions actually found: " << res.size() << endl;
	cout << endl;
	
	cout << "Energy" << "\t" << "Case" << endl;
	//output the solutions
	
	for(auto v: res){
		//calculate the energy out of z, ie E = (z/a)²*H_BAR²/2/m-V0
		double energy = (v.first/a)*(v.first/a)*1e18*H_BAR*H_BAR/2/MASS_ELECTRON-v0*CHARGE_ELECTRON;
		//print the energy and even or odd
		cout << energy << "\t" << (v.second?"even":"odd") << endl;  
	}
	/*
	cout << endl;
	for(auto v: res){
		//calculate the energy out of z, ie E = (z/a)²*H_BAR²/2/m-V0
		double energy = (v.first/a)*(v.first/a)*1e18*H_BAR*H_BAR/2/MASS_ELECTRON-v0*CHARGE_ELECTRON;
		//
		cout << v.first << "\t" << v.second << "\t" << (v.second?g(v.first):h(v.first)) << "\t" << energy << endl;  
	}*/
	
	return 0;
}
