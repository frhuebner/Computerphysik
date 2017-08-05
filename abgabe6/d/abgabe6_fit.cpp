#include<iostream>
#include<cstdlib>
#include<time.h>
#include<math.h>
#include<vector>
#include<limits.h>
#include<utility>
#include<fstream>

//precision for gauss newton
#define PRECISION 1e-5

#define GAUSSNEWTON_LAMBDA_0 0.0003
#define GAUSSNEWTON_GAMMA 0.01
#define GAUSSNEWTON_DELTA 20
#define GAUSSNEWTON_LAMBDA_MAX 1e200

using namespace std;

typedef vector<vector<double> > Matrix;

/*
 * overloads the outstream operator << for vectors
 * o: the outstream
 * a: the vector to output
 * return: the outstream with the output vector 
 */ 
ostream& operator<<(ostream& o,const vector<double>& a){
	for(int i=0;i<a.size();i++){
		o << a[i] << (i==a.size()-1?"":"\t");
	}
	return o;
}

/*
 * overloads the outstream operator << for Matricies
 * o: the outstream
 * a: the Matrix to output
 * return: the outstream with the output matrix 
 */ 
ostream& operator<<(ostream& o,const Matrix& a){
	for(int i=0;i<a.size();i++){
		o << a[i];
		if(i<a.size()-1){
			o << endl;
		}
	}
	return o;
}

/*
 * returns the lenght of the vector a
 * return: sqrt(a*a)
 */ 
double abs(vector<double>& a){
	double ret = 0;
	for(int i=0;i<a.size();i++){
		ret += a[i]*a[i];
	}
	return sqrt(ret);
}

/*
 * overloads the operator + for vectors
 * return: a+b componentwise 
 */ 
const vector<double> operator+(const vector<double>& a,const vector<double>& b){
	vector<double> ret(a.size());
	for(int i=0;i<a.size();i++){
		ret[i] = a[i] + b[i];
	}
	return ret;
}

/*
 * param[0] = mu
 * param[1] = sigma
 * param[2] = n
 */ 
double gauss(double x, vector<double>& param){
	return param[2]*exp(-(x-param[0])*(x-param[0])/2/param[1]/param[1]);
}

/*
 * param[0] = mu
 * param[1] = sigma
 * param[2] = n
 */ 
vector<double> gaussGradient(double x, vector<double>& param){
	vector<double> grad(3);
	grad[0] = param[2]*(x-param[0])/2/param[1]/param[1]*exp(-(x-param[0])*(x-param[0])/2/param[1]/param[1]);
	grad[1] = param[2]*exp(-(x-param[0])*(x-param[0])/2/param[1]/param[1])*(x-param[0])*(x-param[0])/param[1]/param[1]/param[1];
	grad[2] = exp(-(x-param[0])*(x-param[0])/2/param[1]/param[1]);
	return grad;
}

void doLUPartition(Matrix& A){
	for(int d=0;d<A.size();d++){
		for(int j=d;j<A.size();j++){
			double sum = 0;
			for(int k=0;k<d;k++){
				sum += A[d][k]*A[k][j];
			}
			A[d][j] = A[d][j] - sum;
		}
		if(A[d][d] == 0){
			cerr << "Matrix can't be inverted because of a 0 in LU-partition" << endl;
			cout << A << endl;
			cin.get();
			A[0][0] = NAN;
			return;
		}
		for(int i=d+1;i<A.size();i++){
			double sum = 0;
			for(int k=0;k<d;k++){
				sum += A[i][k]*A[k][d];
			}
			A[i][d] = (A[i][d] - sum)/A[d][d];
		}
	}
}

vector<double> getInverseVectorU(Matrix& A, vector<double>& b){
	vector<double> res(b.size());
	for(int i=b.size()-1;i>=0;i--){
		double sum = 0;
		for(int j=i+1;j<b.size();j++){
			sum += A[i][j]*res[j];
		}
		res[i] = (b[i] - sum)/A[i][i]; 
	}
	return res;
}

vector<double> getInverseVectorL(Matrix& A, vector<double>& b){
	vector<double> res(b.size());
	for(int i=0;i<b.size();i++){
		double sum = 0;
		for(int j=0;j<i;j++){
			sum += A[i][j]*res[j];
		}
		res[i] = b[i] - sum; 
	}
	return res;
}

vector<double> getInverseVectorLU(Matrix& A, vector<double>& b){
	vector<double> temp = getInverseVectorL(A, b);
	return getInverseVectorU(A, temp);
}

vector<double> getInverseVector(Matrix A, vector<double>& b){
	doLUPartition(A);
	if(isnan(A[0][0])){
		return {};
	}
	return getInverseVectorLU(A,b);
}

double getXiSquare(double (*fitFunction)(double,vector<double>&), vector<double>& x, vector<double>& y, vector<double>& a){
	double xi = 0;
	for(int k=0;k<x.size();k++){
		double f = fitFunction(x[k],a);
	
		xi += (f-y[k])*(f-y[k]);
	}
	return xi;
}

vector<double> gaussNewton(double (*fitFunction)(double,vector<double>&), vector<double> (*fitFunctionGradient)(double, vector<double>&), vector<double>& x, vector<double>& y, vector<double> a){
	double xi_square = getXiSquare(fitFunction, x, y, a);
	double lambda = GAUSSNEWTON_LAMBDA_0;
	for(int n=0;n<100000;n++){
		lambda *= GAUSSNEWTON_GAMMA;
		
		Matrix A(a.size(),vector<double>(a.size(),0));
		vector<double> b(a.size(),0);
		for(int k=0;k<x.size();k++){
			double f = fitFunction(x[k],a);
			vector<double> grad = fitFunctionGradient(x[k],a);
			for(int i=0;i<a.size();i++){
				for(int j=0;j<a.size();j++){
					A[i][j] += grad[i]*grad[j];
				}
				b[i] += (y[k]-f)*grad[i];
			}
		}
		
		//cout << "A:" << endl << A << endl;
		
		while(lambda <= GAUSSNEWTON_LAMBDA_MAX){
			//cout << "\tdo lambda: " << lambda << endl;
			Matrix A_ = A;
			for(int i=0;i<A_.size();i++){
				A_[i][i] += lambda;
			}
			vector<double> da = getInverseVector(A_,b);
			if(da.empty()){
				cerr << "Wasn't able to invert matrix: try again with different lambda" << endl;
				lambda *=1.00001;
				continue;
			}
			
			vector<double> a_new = a + da;
			double xi_square_new = getXiSquare(fitFunction, x, y, a_new);
			
			if(xi_square_new >= xi_square){
				
				lambda *= GAUSSNEWTON_DELTA;
				//cout << "adjust lambda " << lambda << "\t" << xi_square_new << endl;
				continue;
			}
				
			double relative_precision = 0;
			
			for(int i=0;i<a.size();i++){
				double rel = da[i]/a[i];
				if(rel<0) rel=-rel;
				if(rel > relative_precision){
					relative_precision = rel;
				}
			}
			
			if(relative_precision < PRECISION){
				return a;
			}
			
			xi_square = xi_square_new;
			a = a + da;
			cout << "da " << (abs(da)) << "\t" << relative_precision << endl;
			break;
		}	
		
	}
	return {};
}

int main(int argc, char** argv){
	std::cout.precision(10);
	
	ifstream fin("c_dominik.txt");
	vector<double> x,y;
	for(int i=0;!fin.eof();i++){
		double x_,y_;
		fin >> x_ >> y_;
		x.push_back(x_);
		y.push_back(y_);
		//cout << i << "\t" << x_ << "\t" << y_ << endl;
	}
	x.pop_back();
	y.pop_back();
	
	vector<double> a_0 = {650,50,4000};
	
	vector<double> a_fit = gaussNewton(gauss, gaussGradient, x, y, a_0);
	cout << a_fit << endl;
	
	/*Matrix a = {{1,1,3},{1,2,4},{5,3,4}};
	
	vector<double> ex = {1,0,0};
	vector<double> ey = {0,1,0};
	vector<double> ez = {0,0,1};
	
	auto e = ex+ey+ez;
	
	cout << getInverseVector(a,e) << endl;*/
	
	return 0;
}
