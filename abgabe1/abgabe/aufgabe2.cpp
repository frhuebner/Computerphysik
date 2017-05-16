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

/*
 * Returns a random double in [min,max)
 * min: lower limit
 * max: upper limit
 * return: random double in [min,max)
 */ 
double getRand(double min, double max){
	return rand()/(RAND_MAX+1.0)*(max-min)+min;
}

/*
 * Returns a vector with length 1 in a random direction
 * return: vector with length 1 in a random direction
 */ 
vector<double> getRandomVector(){
	double theta = acos(getRand(-1,1));
	double phi = getRand(0, 2*PI);
	return {cos(phi)*sin(theta), sin(phi)*sin(theta),cos(theta)};
}

/*
 * Returns the euclidian length of a vector, i.e. sqrt(sum x[i]^2)
 * v: the vector
 * return: |v|
 */ 
double getAbs(vector<double>& v){
	double sum = 0;
	for(double a: v){
		sum+=a*a;
	}
	return sqrt(sum);
}

/*
 * Returns the Mean free path at distance r from the center of the sun
 * r: distance in m of the photon
 * r_sun: radius in mof the radiation zone
 * return: Mean free path in m at distance r
 */  
double getLambda(double r, double r_sun){
	double rho = -150000*r/r_sun + 150000;
	return MASS_H/(SIGMA_T*rho);
}

/*
 * Simulates a path of a photon starting in the center of the sun till it leaves the radiation zone
 * r_sun: radius in m of the radiation zone
 * return: the amount time in s the photon needed 
 */ 
double simulate(double r_sun){
	//current position in m of the particle, starting at 0
	vector<double> pos = {0,0,0};
	//distance in m from the center of the sun, i.e. |pos|
	double r = 0;
	//the total distance the photon traveled
	double total_dist = 0;
	
	//Simulation the photon till it leaves the sun
	for(unsigned int step = 0;r < r_sun;step++){
		//get an random direction vector
		vector<double> u = getRandomVector();
		
		//get the mean free path in m
		double lambda = getLambda(r, r_sun);
		
		//calculate the new position of the photon, i.e. pos = pos + lambda*u
		for(int i=0;i<3;i++){
			pos[i]+=lambda * u[i];
		}
		//update r and total_dist
		r = getAbs(pos);
		total_dist += lambda;
	}
	
	//return the needed time in s (Light travels with the speed of light)
	return total_dist/LIGHT_SPEED;
}

/*
 * Simulates 1000 photons at a given sun radius and returns the average time needed
 * r_sun: radius of the radiation zone in m
 * return: the average time in s that the photons needed
 */ 
double simulate1000(double r_sun){
	//add all travel times in s of all photons together
	double t_ges = 0;
	
	for(int i=0;i<1000;i++){
		//simulate one photon
		double t = simulate(r_sun);
		//update t_ges
		t_ges += t;
	}
	//return the average time
	return t_ges/1000;
}

/*
 * Iterates over the radius of the radiation zone and returns the average times needed for photons to escape this radius
 * The radius is increased by a constant factor (so that there are equal distances in a logarithmic diagram)
 * start: the radius in m with which to start
 * end: the radius in m with which to end
 * n: the number of iterations
 * return: array of pairs of double (radius in m, average time in s)
 */ 
vector<PDD> iterateSunRadius(double start, double end, int n){
	//calculate the factor between two iterations
	double factor = pow(end/start,1.0/(n-1));
	cout << "factor: " << factor << endl;
	
	//set the sun radius in m to start
	double r_sun = start;
	
	//init the vector of pairs that will be returned in the end
	vector<PDD> ret;
	
	for(int i=0;i<n;i++){
		//get the average time
		double t = simulate1000(r_sun);
		cout << "Radius: " << r_sun << "\tTime: " << t << endl;
		
		//create a pair and add it to the vector
		PDD p(r_sun, t);
		ret.push_back(p);
		
		//update the factor
		r_sun*=factor;
	}
	return ret;
}

int main(){
	//set the random seed to the current time
	srand(time(NULL));
	
	//get the array of pairs of radii and times
	vector<PDD> result = iterateSunRadius(0.01,0.1,10);
	
	//output in file "output.txt"
	ofstream fout;
	fout.open("output.txt");
	
	//print the array in the file
	for(PDD p: result){
		fout << p.first << "\t" << p.second << endl;
	}
	
	//close file
	fout.close();
	
	return 0;
}
