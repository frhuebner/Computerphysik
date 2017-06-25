#include<iostream>
#include<cstdlib>
#include<time.h>
#include<math.h>
#include<vector>
#include<limits.h>
#include<utility>
#include<fstream>
#include"abgabe4_runge_kutta.cpp"

using namespace std;

void check_butcher(){
	for(int i=0;i<RUNGE_KUTTA_SIZE;i++){
		cout << RUNGE_KUTTA_C[i] << "\t|\t";
		double sum = 0;
		for(int j=0;j<RUNGE_KUTTA_SIZE;j++){
			cout << RUNGE_KUTTA_A[i][j] << "\t";
			sum += RUNGE_KUTTA_A[i][j];
		}
		cout << "|\t" << sum << endl;
	}
	cout << "-------------------------------------------------------------------------------" << endl;
	for(int i=0;i<2;i++){
		cout  << "\t|\t";
		double sum = 0;
		for(int j=0;j<RUNGE_KUTTA_SIZE;j++){
			cout << RUNGE_KUTTA_B[i][j] << "\t";
			sum += RUNGE_KUTTA_B[i][j];
		}
		cout << "|\t" << sum << endl;
	}
}

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
	check_butcher();
	cout << endl << endl;
	std::cout.precision(10);
	ofstream fout("test.txt");
	runge_kutta_init(f,0,{1});
	for(int i=0;i<10000;i++){
		runge_kutta_iterate();
		cout << runge_kutta_t << "\t" << runge_kutta_y[0] << "\t" << (runge_kutta_y[0]/exp(runge_kutta_t*runge_kutta_t)-1) << "\t" << runge_kutta_h << endl;
		fout << runge_kutta_t << "\t" << runge_kutta_y[0] << "\t" << (runge_kutta_y[0]/exp(runge_kutta_t*runge_kutta_t)-1) << "\t" << runge_kutta_h << endl;  
	}
	fout.close();
	fout.open("test1.txt");
	runge_kutta_init(f1,0,{1,0});
	for(int i=0;i<10000;i++){
		runge_kutta_iterate();
		cout << runge_kutta_t << "\t" << runge_kutta_y << "\t" << (runge_kutta_y[0]/cos(runge_kutta_t)-1) << "\t" << runge_kutta_h << endl;
		fout << runge_kutta_t << "\t" << runge_kutta_y << "\t" << (runge_kutta_y[0]/cos(runge_kutta_t)-1) << "\t" << runge_kutta_h << endl;  
	}
	fout.close();
	
	
	return 0;
}
