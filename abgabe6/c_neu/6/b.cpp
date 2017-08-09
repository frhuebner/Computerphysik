#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <limits.h>
#include <utility>
#include <fstream>

using namespace std;

vector<double> h(6),A(6),B(6),C(6),D(6),B_(6),D_(6),X(6),a(6),b(6),c(6),d(6);
vector<double> g;
double s;

/*
 * the function spline calculates x for coefficients a,b,c,d and knots
 * takes in coefficients of fitted polynomial for intervall [x_i,x_i+1]
 * returs s, solution for polynomial at x
 */
double spline(double a,double b,double c,double d,double x,double x_i){													
	double s=a*pow((x-x_i),3)+b*pow((x-x_i),2)+c*(x-x_i)+d;																
	return s;																	
}
/*
 * function to calculate coefficients for polynomials for intervall [x_i,x_i+1]
 */
void calculation(vector<double> p_x,vector<double> p_y){
	/*differece between knots*/
	for(int i=0;i<5;i++){																	
		h[i]=p_x[i+1]-p_x[i];
	}
	/*coefficients A,B,C,D*/
	for(int i=2;i<5;i++){																	
		A[i]=h[i-1];
	}
	for(int i=1;i<5;i++){																	
		B[i]=2*(h[i-1]+h[i]);
	}
	for(int i=1;i<4;i++){																	
		C[i]=h[i];
	}
	for(int i=1;i<5;i++){																	
		D[i]=6*((p_y[i+1]-p_y[i])/h[i]-(p_y[i]-p_y[i-1])/h[i-1]);
	}
	/*coefficients B',D'*/
	B_[1]=B[1];																	
	D_[1]=D[1];
	for(int i=2;i<5;i++){																	
		B_[i]=B[i]-C[i-1]*A[i]/B[i-1];													
		D_[i]=D[i]-D[i-1]*A[i]/B[i-1];
	}
	/*X as differential y''*/
	X[4]=D_[4]/B_[4];																
	for(int i=3;i>0;i--){																	
		X[i]=(D_[i]-C[i]*X[i+1])/B_[i];
	}
	/*setting x[0]=X[5]=0, curvature at the edge disappear*/
	X[0]=0;																				
	X[5]=0;
	/*calculation of coefficients a,b,c,d*/
	for(int i=0;i<5;i++){																	
		b[i]=X[i]/2;
		a[i]=(X[i+1]-X[i])/(6*h[i]);
		d[i]=p_y[i];
		c[i]=(p_y[i+1]-p_y[i])/h[i]-h[i]/6*(X[i+1]+2*X[i]);
		cout << i << "\t" << a[i] << "\t" << b[i] << "\t" << c[i] << "\t" << d[i] << endl;											
	}
}

/*
 * calculating cosmic background with spline interpolation in intervalls [x_i,x_i+1]
 */
void background(vector<double> a, vector<double> b, vector<double> c, vector<double> d, vector<double> x, vector<double> y, vector<double> p_x, vector<double> p_y){
	ofstream fout("cmb.dat");
	for(int j=0;j<1800;j++){																
		/*calculating x in intervall [0,x_1[ with spline function*/
		if(j<415){																		
			s=spline(a[0],b[0],c[0],d[0],x[j],p_x[0]);										
			fout << x[j] << "\t" << s << endl;				
			cout << x[j] << "\t" << s << endl;
		}	
		/*calculating x in intervall [x_1,x_2[ with spline function*/																			
		else if(j<715){
			s=spline(a[1],b[1],c[1],d[1],x[j],p_x[1]);
			fout << x[j] << "\t" << s << endl;
			cout << x[j] << "\t" << s << endl;				
		}
		/*calculating x in intervall [x_2,x_3[ with spline function*/
		else if(j<1015){
			s=spline(a[2],b[2],c[2],d[2],x[j],p_x[2]);
			fout << x[j] << "\t" << s << endl;
			cout << x[j] << "\t" << s << endl; 			
		}
		/*calculating x in intervall [x_3,x_4[ with spline function*/
		else if(j<1415){
			s=spline(a[3],b[3],c[3],d[3],x[j],p_x[3]);
			fout << x[j] << "\t" << s << endl;
			cout << x[j] << "\t" << s << endl;				
		}
		/*calculating x in intervall [x_4,oo] with spline function*/
		else{
			s=spline(a[4],b[4],c[4],d[4],x[j],p_x[4]);
			fout << x[j] << "\t" << s << endl;
			cout << x[j] << "\t" << s << endl;				
		}
	}
	fout.close();
}

/*
 * calculating gammaline without cmb
 */
void gamma(vector<double> x, vector<double> y){
	ifstream fin("cmb.dat");
	vector <double> c;
	for(int i=0;!fin.eof();i++){
		double m,c_;
		fin >> m >> c_;
		c.push_back(c_);
	}
	c.pop_back();
	fin.close();
	
	vector<double> g;
	ofstream fout("profile_cmb.dat");
	for(int i=0; i<1800; i++){
		double n=y[i]-c[i];
		cout << x[i] << "\t" << n << endl;
		fout << x[i] << "\t" << n << endl;
	}
}
	

int main(){
	ifstream fin("profile.dat");
	vector<double> x,y;
	double m;
	fin >> m;
	for(int i=0;!fin.eof();i++){
		double m,x_,y_;
		fin >> m >> x_ >> y_;
		x.push_back(x_);
		y.push_back(y_);
		cout << i << "\t" << m << "\t" << x_ << "\t" << x[i] << "\t" << y[i] << "\t" << y_ << endl;
	}
	x.pop_back();
	y.pop_back();
	fin.close();
	
	
	/*creating vectors of knots*/
	vector<double> p_x = {x[15],x[415],x[715],x[1015],x[1415],x[1715]};
	vector<double> p_y = {y[15],y[415],y[715],y[1015],y[1415],y[1715]};
	/*calculation of a,b,c,d*/
	calculation(p_x,p_y);
	/*calculation of background*/
	background(a,b,c,d,x,y,p_x,p_y);
	/*calculation of gammaline without background*/
	gamma(x,y);
	
	
	return 0;
}

