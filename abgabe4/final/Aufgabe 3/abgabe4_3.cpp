#include<iostream>
#include<cstdlib>
#include<time.h>
#include<math.h>
#include<vector>
#include<limits.h>
#include<utility>
#include<fstream>
#include<stdlib.h>
#include"../abgabe4_runge_kutta.cpp"

//constants in SI units
#define PI 3.14159265358979323846
#define MU0 4*PI*1e-7
#define CHARGE_PROTON 1.6021766208e-19
#define MASS_PROTON 1.672621898e-27
#define RADIUS_EARTH 6370000
using namespace std;

//the magentic moment of the earth in SI
static double moment = 7.75*1e22;

/*
 * the function for the differential equation
 * t: current time (not needed for the ODE, but requestet by the solver) 
 * y: current y = {x,v}
 * return: {v,a(x)} with a(x) the accleration due to the magnetic force
 */ 
vector<double> f(double t, vector<double> y){
	vector<double> x = {y[0],y[1],y[2]};
	vector<double> v = {y[3],y[4],y[5]};
	double r = abs(x);
	vector<double> m = {0,0,moment};
	vector<double> B = MU0/4/PI/pow(r,5)*(((3*x[2]*moment)*x)-((r*r)*m)); 
	vector<double> a = CHARGE_PROTON/MASS_PROTON*(crossproduct(v,B));
	return {v[0],v[1],v[2],a[0],a[1],a[2]};
}


int main(int argc, char** argv){
	std::cout.precision(10);
	
	//the time when the last output happened
	double t = 0;
	
	//init the runge kutta with different initial values
	runge_kutta_init(f,t,{6000000+RADIUS_EARTH,0,0,-1000000,0,100000});
	//runge_kutta_init(f,t,{6000000+RADIUS_EARTH,0,0,-1000000,0,1000000});
	//runge_kutta_init(f,t,{6000000+RADIUS_EARTH,0,0,-1000000,0,3000000});

	runge_kutta_precission = 1e-8;
	
	//output the initial values
	cout << t << "\t" << runge_kutta_y << endl;
	while(runge_kutta_t <= 100){
		//do one step
		runge_kutta_iterate();
		
		//if we've done 0.1s then output
		if(t + 0.1 <= runge_kutta_t){
			cout << runge_kutta_t << "\t" << runge_kutta_y << endl;
			//set t to our current time
			t = runge_kutta_t;
		}
	}
	
	return 0;
}
