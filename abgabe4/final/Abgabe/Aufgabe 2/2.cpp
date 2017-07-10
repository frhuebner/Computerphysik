#include<iostream>
#include<cstdlib>
#include<time.h>
#include<math.h>
#include<vector>
#include<limits.h>
#include<utility>
#include<fstream>

#include"../abgabe4_runge_kutta.cpp"

using namespace std;

//definition of constants
#define GRAV 6.67408e-11
#define M_EARTH 5.9722e24
#define M_KOMET 4.084e-6
#define C_W 0.45
#define RHO_0 1.29
#define M_MOL 0.02896
#define G_0 9.81
#define R_GAS 8.314
#define R_EARTH 6370e3
#define R_KOMET 1e-3
#define A_KOMET 1.57e-6
#define TEMP 273

//vector of differential equations
vector<double> f(double t, vector<double> h){

	vector<double> v(2);
	
	double g = GRAV*M_EARTH/((h[0]+R_EARTH)*(h[0]+R_EARTH));	//gravitational force divided by M_KOMET
	double r = RHO_0*exp(-M_MOL*G_0*h[0]/(R_GAS*TEMP));		 	//pressure(h) 
	double fr = 1/(2*M_KOMET)*C_W*r*A_KOMET*h[1]*h[1];			//frictional force divided by M_KOMET

	v[0] = h[1];												
	v[1] = fr-g;
		
	return v;
}

int main(int argc, char** argv){
	std::cout.precision(10);
		
	ofstream fout("2d.txt");
	
	double h_0;
	double v_0;
	
	//getting initial conditions
	cout << "Anfangshöhe h_0 in km:";
	cin >> h_0;
	cout << endl << "Anfangsgeschwindigkeit Betrag von v_0 in km/s:";
	cin >> v_0;
	
	h_0 = h_0*1000;						//conversion from km to m
	v_0 = -v_0*1000;					//conversion from km/s to m/s
		
	runge_kutta_init(f,0,{h_0,v_0});	
	for(int i=0;runge_kutta_y[0] > 0;i++){
		//do one step
		runge_kutta_iterate();
		//get current time by runge_kutta_t
		//get current y vector as runge_kutta_y
		cout << runge_kutta_t << "\t" << runge_kutta_y[0]/1000 << "\t" << runge_kutta_y[1]/1000 << endl;
		fout << runge_kutta_t << "\t" << runge_kutta_y[0]/1000 << "\t" << runge_kutta_y[1]/1000 << endl;  
	}
	fout.close();
	
	
	return 0;
}
