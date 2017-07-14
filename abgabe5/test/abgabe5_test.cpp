#include<iostream>
#include<cstdlib>
#include<time.h>
#include<math.h>
#include<vector>
#include<limits.h>
#include<utility>
#include<fstream>
//@fiona: this is important
#include"../abgabe5_runge_kutta.cpp"

using namespace std;

void check_butcher(RungeKuttaAlgorithm algorithm){
	for(int i=0;i<algorithm.size;i++){
		cout << algorithm.c[i] << "\t|\t";
		double sum = 0;
		for(int j=0;j<algorithm.size;j++){
			cout << algorithm.a[i][j] << "\t";
			sum += algorithm.a[i][j];
		}
		cout << "|\t" << sum << endl;
	}
	cout << "-------------------------------------------------------------------------------" << endl;
	for(int i=0;i<2;i++){
		cout  << "\t|\t";
		double sum = 0;
		for(int j=0;j<algorithm.size;j++){
			cout << algorithm.b[i][j] << "\t";
			sum += algorithm.b[i][j];
		}
		cout << "|\t" << sum << endl;
	}
}
// function for y' = f(t,y) = 2*t*y -> solution is y(t) = exp(t²)
vector<double> f(double t, vector<double> y){
	return (2*t)*y;
}

vector<double> real(double t){
	return {exp(t*t)};
}

vector<double> f1(double t, vector<double> y){
	return {y[1],-y[0]};
}

vector<double> real1(double t){
	return {cos(t),sin(t)};
}

int main(int argc, char** argv){
	std::cout.precision(4);
	check_butcher(RungeKutta::classic);
	cout << endl << endl;
	std::cout.precision(10);
	
		
	ofstream fout("ctest.txt");
	//@fiona: init with function f (see above) and t0 = 0 and y_0 = {0}
	RungeKutta expot(f,0,{1}, RungeKutta::fehlberg);
	for(int i=0;i<10000;i++){
		//do one step
		expot.iterate();
		//get current time by runge_kutta_t
		//get current y vector as runge_kutta_y
		cout << expot.t << "\t" << expot.y[0] << "\t" << (expot.y[0]/exp(expot.t*expot.t)-1) << "\t" << expot.getStepSize() << endl;
		fout << expot.t << "\t" << expot.y[0] << "\t" << (expot.y[0]/exp(expot.t*expot.t)-1) << "\t" << expot.getStepSize() << endl;  
	}
	fout.close();
	
	
	
	
	fout.open("ctest1.txt");
	RungeKutta trig(f1,0,{1,0}, RungeKutta::classic);
	for(int i=0;i<10000;i++){
		trig.iterate();
		cout << trig.t << "\t" << trig.y << "\t" << (trig.y[0]/cos(trig.t)-1) << "\t" << trig.getStepSize() << endl;
		fout << trig.t << "\t" << trig.y << "\t" << (trig.y[0]/cos(trig.t)-1) << "\t" << trig.getStepSize() << endl;  
	}
	fout.close();
	
	
	return 0;
}
