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

//definiton of constants
double m = 0.2;
double L = 0.2;
double g = 9.81;

//vector of differential equations
vector<double> f(double t, vector<double> y){
	
	vector<double> v(4);
	
	double a = 1+ sin(y[0]-y[1])*sin(y[0]-y[1]);
	double b = cos(y[0]-y[1]);
	double c = 1/(m*L*L);
	double d = (y[2]*y[3]*sin(y[0]-y[1]))/a;
	double e = ((y[2]*y[2]+2*y[3]*y[3]-2*y[2]*y[3]*b)*sin(y[0]-y[1])*b)/(a*a);
	
	v[0] = c*(y[2]-y[3]*b)/a;				//d phi_1/dt	
	v[1] = c*(2*y[3]-y[2]*b)/a;				//d phi_2/dt
	v[2] = -c*d+c*e-2*m*g*L*sin(y[0]);		//d p_1/dt
	v[3] = c*d-c*e-m*g*L*sin(y[2]);			//d p_2/dt
	
	return v;
}

//calculation of hamilton
double energy(double phi_1,double phi_2,double p_1,double p_2){
	
	double a = 1/(m*L*L);
	double b = cos(phi_1-phi_2);
	double c = sin(phi_1-phi_2);
	double d = p_1*p_1+p_2*p_2-2*p_1*p_2*b;
	double e = 1+c*c;
	double f = m*g*L*(4-2*cos(phi_1)-cos(phi_2));
	
	return a*d/e+f;							//hamilton-function
}


int main(int argc, char** argv){
	std::cout.precision(10);
		
	ofstream fout("1b.txt");
	
	double phi_1;
	double phi_2;
	double p_1;
	double p_2;
	
	//getting initial conditions
	cout << "Anfangswerte der Winkel in Grad:" << endl;
	cout << "Phi_1: ";
	cin >> phi_1;
	cout << "Phi_2: ";
	cin >> phi_2;
	cout << endl << "Anfangswerte der Impulse in kg*m*s^(-1):" << endl;
	cout << "p_1: ";
	cin >> p_1;
	cout << "p_2: ";
	cin >> p_2;
	
	phi_1 = M_PI*phi_1/180;								//conversion from degrees to radian
	phi_2 = M_PI*phi_2/180;								//conversion from degrees to radian
	
	double E_tot = energy(phi_1,phi_2,p_1,p_2);			//hamilton-function with inital conditions
	double E;
	cout << "E_tot: " << E_tot << endl;
	
	runge_kutta_init(f,0,{phi_1,phi_2,p_1,p_2});
	for(int i=0;runge_kutta_t<30;i++){
		//do one step
		runge_kutta_iterate();
		//get current time by runge_kutta_t
		//get current y vector as runge_kutta_y
		
		E=E_tot-energy(runge_kutta_y[0],runge_kutta_y[1],runge_kutta_y[2],runge_kutta_y[3]); //calculting difference beteen hamilton-function with runge-kutta and inital conditions
		
		cout << runge_kutta_t << "\t" << runge_kutta_y << "\t" << E << endl;
		fout << runge_kutta_t << "\t" << runge_kutta_y << "\t" << E << endl;  
	}
	fout.close();
	
	
	return 0;
}
