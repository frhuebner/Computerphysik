#include<iostream>
#include<cstdlib>
#include<time.h>
#include<math.h>
#include<vector>
#include<limits.h>
#include<utility>
#include<fstream>

#include"../abgabe5_runge_kutta.cpp"

#define TIME 1000

using namespace std;

//definiton of constants
double m1 = 1;
double m2 = 1;
double G = 1;
double mu = 0.5;

/*function to calculate acceleration of r with 
 * differential equation*/
vector<double> acceleration(vector<double> y){
	vector<double> a(3);
	//calculating value of r
	double r = sqrt(y[0]*y[0]+y[1]*y[1]+y[2]*y[2]);
	a[0] = -G*(m1*m2)/(r*r*mu)*(y[0])/r; 	//a(x)=d²r(x)/dt²
	a[1] = -G*(m1*m2)/(r*r*mu)*(y[1])/r;	//a(y)=d²r(y)/dt²
	a[2] = -G*(m1*m2)/(r*r*mu)*(y[2])/r;	//a(z)=d²r(z)/dt²
	return a;
}

//vector of differential equations
vector<double> f(double t, vector<double> y){
	
	vector<double> v(6);
	
	v[0] = y[3];						//v(x)=dr(x)/dt							
	v[1] = y[4];						//v(y)=dr(y)/dt
	v[2] = y[5];						//v(z)=dr(z)/dt
	//calculating acceleration
	vector<double> a = acceleration(v);
	v[3] = a[1];						//a(x)
	v[4] = a[2];						//a(y)
	v[5] = a[3];						//a(z)

	return v;
}

/*function to make a step for leapfrog-method*/
vector<double> leapfrog(double t, vector<double> y){
	
	vector<double> v(6);
	
	//x_(i+1) = x_i+v_(i+1/2)*h
	v[0] = y[0]+y[3]*0.01;	
	v[1] = y[1]+y[4]*0.01;
	v[2] = y[2]+y[5]*0.01;
	vector<double> a = acceleration(v);	
	//v_(i+3/2)=v_(i+1/2)+a(x_i)*h
	v[3] = y[3]+a[0]*0.01;
	v[4] = y[4]+a[1]*0.01;
	v[5] = y[5]+a[2]*0.01;
	return v;
}

/*function to calulate laplace_runge_lenz_vector*/
vector<double> laplace_runge_lenz(vector<double> y){
	//calculating value of r
	double r = sqrt(y[0]*y[0]+y[1]*y[1]+y[2]*y[2]);
	//calculating momentum p=m*v with m being reduced mass mu
	vector<double> p(3);
	p[0] = mu*y[3];
	p[1] = mu*y[4];
	p[2] = mu*y[5];
	//calucating anular momentum L=r x p
	vector<double> l(3);
	l[0] = y[1]*p[2]-y[2]*p[1];
	l[1] = y[2]*p[0]-y[0]*p[2];
	l[2] = y[0]*p[1]-y[1]*p[0];
	//calulating laplace-runge-lenz-vector A=p x L-m*a*er
	//a = m1m2G, m =mu
	vector<double> v(3);
	v[0] = p[1]*l[2]-p[2]*l[1]-mu*m1*m2*G*y[0]/r;
	v[1] = p[2]*l[0]-p[0]*l[2]-mu*m1*m2*G*y[1]/r;
	v[2] = p[0]*l[1]-p[1]*l[0]-mu*m1*m2*G*y[2]/r;
	
	return v;
}

/*function calculating energy of kepler orbit at time t*/
double energy(vector<double> y){
	
	//calculating value of r
	double r = sqrt(y[0]*y[0]+y[1]*y[1]+y[2]*y[2]);
	//calculating value of v
	double v = sqrt(y[3]*y[3]+y[4]*y[4]+y[5]*y[5]);
	//calculating energy e=ekin-epot
	double e = 0.5*mu*v*-G*m1*m2/r;
	
	return e;
}
	

int main(int argc, char** argv){
	std::cout.precision(10);
		
	ofstream fout("2-leapfrog.txt");	
	
	cout << "LEAPFROG" << endl;
	//define vector of initial conditions for leapfrog
	vector<double> y = {1,0,0,-0.01,1,0};
	
	double t = 0;
	//solving equations with leapfrog
	for(int j=0;j<TIME;j++){
		y = leapfrog(t,y);
		t = t+0.01;
		//define y_sync to calculate v_i=v_(i+1/2)-a(x_i)*h/2
		vector<double> y_sync = y;
		vector<double> a = acceleration(y_sync);
		y_sync[3] -= a[0]*0.005;
		y_sync[4] -= a[1]*0.005;
		y_sync[5] -= a[2]*0.005;
		//calculating laplace_runge_lenz and energy
		vector<double> lrl = laplace_runge_lenz(y_sync);
		double e = energy(y_sync);
		cout << t << "\t" << y_sync << "\t" << lrl << "\t" << e << endl;
		fout << t << "\t" << y_sync << "\t" << lrl << "\t" << e << endl;
	}
	
	fout.close();
	ofstream pout("2-runge-kutta.txt");
	cout << "RUNGE-KUTTA" << endl;
	//solving equations with runge-kutta
	RungeKutta solver(f,0,{1,0,0,0,1,0}, RungeKutta::classic);
	solver.setStepSize(0.01);
	for(int i=0;i<TIME;i++){
		solver.iterate();
		//calculating laplace_runge_lenz and energy
		vector<double> lrl = laplace_runge_lenz(solver.y);
		double e = energy(solver.y);
		cout << solver.t << "\t" << solver.y << "\t" << lrl << "\t" << e << endl;
		pout << solver.t << "\t" << solver.y << "\t" << lrl << "\t" << e << endl;  
	}
	pout.close();
	
	return 0;
}
