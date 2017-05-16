#include<iostream>
#include<cstdlib>
#include<time.h>
#include<math.h>
#include<vector>
#include<limits.h>
#include<utility>
#include<fstream>

//Number of points in the input 
#define NUMBER 10

using namespace std;

typedef pair<double, double> PDD;

/*
 * Returns a pair (a,b) where a and b are the fittet parameters for a linear fit a*x+b
 * x and y must have the same size
 * x: x values of datapoints
 * y: y values of datapoints
 * return: pair (a,b)
 */ 
PDD linearFit(vector<double>& x, vector<double>& y){
	//number of points
	int n = x.size();
	
	//m? is the mean value of ?
	double mx = 0;
	double my = 0;
	double mxy = 0;
	double mxx = 0;
	
	//calculate mean values: add all data
	for(int i=0;i<n;i++){
		mx+=x[i];
		my+=y[i];
		mxy+=x[i]*y[i];
		mxx+=x[i]*x[i];
	}
	//calculate mean values: divide through the number of points
	mx/=n;
	my/=n;
	mxy/=n;
	mxx/=n;
	
	//calculate a and b
	double a = (mxy-mx*my)/(mxx-mx*mx);
	double b = my-a*mx;
	
	//create a pair for returning
	PDD p(a,b);
	return p;
}

/*
 * Returns the vector with all entries logarithm to base e 
 * a: initial vector, a[i] > 0 or all i
 * return vector {ln(a[0]), ln(a[1]), ..., ln(a[n-1])}
 */ 
vector<double> logVector(vector<double>& a){
	//get the size of a
	int n = a.size();
	
	//vector for returning
	vector<double> loga(n);
	
	//logarithm all entries
	for(int i=0;i<n;i++){
		loga[i] = log(a[i]); 
	}
	
	return loga;
}

/*
 * reads in points from the standart input and do a power function fit c*x^d and outputs the fittet function
 * power function fit c*x^d: logarithm all data and do an linear fit ln(y) = d*ln(x) + ln(c)
 * input has to be in order: x1 y1 x2 y2 x3 y3 x4 y4 ...
 */ 
int main(){
	
	//vectors of x and y components
	vector<double> x(NUMBER), y(NUMBER);
	
	//read them from the standart input
	for(int i=0;i<NUMBER;i++){
		cin >> x[i] >> y[i];
	}
	
	//logarithm bpth vectors
	vector<double> logx = logVector(x);
	vector<double> logy = logVector(y);
	
	//do an linear fit
	PDD p = linearFit(logx,logy);
	
	//calculate the parameters of an power function fit: c = exp(b) and d = a
	cout << "Function: f(x) = " << exp(p.second) << "*" << "x^" << p.first << endl; 
	
	return 0;
}
