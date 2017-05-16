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
#define MASS_H 1.6738232e-27
#define SIGMA_T 6.65e-29
#define SUN_RADIUS 500000000
#define LIGHT_SPEED 300000000

using namespace std;

typedef pair<double, double> PDD;

double getRand(double min, double max){
	return rand()/(RAND_MAX+1.0)*(max-min)+min;
}

vector<double> getRandomVector(){
	double theta = acos(getRand(-1,1));
	double phi = getRand(0, 2*PI);
	return {cos(phi)*sin(theta), sin(phi)*sin(theta),cos(theta)};
}

double getAbs(vector<double>& v){
	double sum = 0;
	for(double a: v){
		sum+=a*a;
	}
	return sqrt(sum);
}

double getLambda(double r, double r_sun){
	double rho = -150000*r/r_sun + 150000;
	//cout << "rho " << rho << endl;
	return MASS_H/(SIGMA_T*rho);
}

double simulate(double r_sun){
	vector<double> pos = {0,0,0};
	double r = 0;
	double total_dist = 0;
	for(unsigned int step = 0;r < r_sun;step++){
		vector<double> u = getRandomVector();
		double lambda = getLambda(r, r_sun);
		//cout << "l " << lambda << endl;
		for(int i=0;i<3;i++){
			pos[i]+=lambda * u[i];
		}
		r = getAbs(pos);
		total_dist += lambda;
		if(r > 1){
				break;
		}
		if(step%10000000==0){
			cout << "Distance: " << r << ", Lambda: " << lambda << endl;
		}
	}
	
	return total_dist/LIGHT_SPEED;
}

double simulate1000(double r_sun){
	double t_ges = 0;
	for(int i=0;i<1000;i++){
		double t = simulate(r_sun);
		t_ges += t;
	}
	return t_ges/1000;
}

vector<PDD> iterateSunRadius(double start, double end, int n){
	double factor = pow(end/start,1.0/(n-1));
	cout << "factor " << factor << endl;
	double r_sun = start;
	
	vector<PDD> ret;
	
	for(int i=0;i<n;i++){
		double t = simulate1000(r_sun);
		cout << r_sun << " " << t << endl;
		PDD p(r_sun, t);
		ret.push_back(p);
		r_sun*=factor;
	}
	return ret;
}

int main(){
	srand(time(NULL));
	
	long long timestamp = time(NULL);
	
	double res = simulate(SUN_RADIUS);
	cout << res << endl;
	cout << time(NULL)-timestamp << endl;
	
	return 0;
}
