#include<iostream>
#include<cstdlib>
#include<time.h>
#include<math.h>
#include<vector>
#include<limits.h>
#include<utility>
#include<fstream>

#include"../abgabe5_runge_kutta.cpp"

using namespace std;

//vector of differential equations
vector<double> f(double t, vector<double> y){
	
	vector<double> v(4);
	
	v[0] = y[1];				//y'(x)
	v[1] = 2*y[0]/pow(1+t,2);	//y''(x)
	v[2] = y[3];				//w'(x)
	v[3] = 2*y[2]/pow(1+t,2);	//w''(x), with w(x)=F'(alpha)
	
	return v;
}

int main(int argc, char** argv){
	std::cout.precision(10);
		
	ofstream fout("1.txt");	
	double da=1;	
	double a=2.0;
	//calculating alpha with accuracy 1e-5
	for(int n=0; da > 1e-5 || da < -1e-5;n++){									
		RungeKutta solver(f,0,{1,a,0,1}, RungeKutta::fehlberg);
		//iterating solving	to y(1,alpha)=y(1)
		for(int i=0;solver.t<=1;i++){
			solver.iterate();
		}
		//calculating new alpha
		da = (solver.y[0]-0.5)/(solver.y[2]);
		a=a-da;
		//calculate F(alpha)
		double F_a = solver.y[0]-0.5;
		cout << n << "\t" << a+da << "\t" << solver.y << "\t" << F_a << endl;
		fout << n << "\t" << a+da << "\t" << solver.y << "\t" << F_a << endl;
	}
	fout.close();	
	return 0;
}
