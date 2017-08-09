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
//the lambda to start with (levenberg maquard) 
#define GAUSSNEWTON_LAMBDA_0 0.0003
//the factor for decreasing lambda at begin of each iteration (levenberg maquard)
#define GAUSSNEWTON_GAMMA 0.2
//the factor for increase lambda after a failure (levenberg maquard)
#define GAUSSNEWTON_DELTA 5
//if lambda reaches this value, the algorithm stops to avoid infinite loops
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
 * returns the value of the gauss function n*exp(-1/2*(x-mu)^2/sigma^2)
 * x: position
 * param[0]: mu
 * param[1]: sigma
 * param[2]: n
 * return: the function value
 */ 
double gauss(double x, vector<double>& param){
	return param[2]*exp(-(x-param[0])*(x-param[0])/2/param[1]/param[1]);
}

/*
 * returns the {mu, sigma, n} gradient of the gauss function f(x) = n*exp(-1/2*(x-mu)^2/sigma^2)
 * x: position
 * param[0]: mu
 * param[1]: sigma
 * param[2]: n
 * return: the gradient at x and param: {d/dmu f, d/dsigma f, d/dn f}
 */
vector<double> gaussGradient(double x, vector<double>& param){
	vector<double> grad(3);
	grad[0] = param[2]*(x-param[0])/param[1]/param[1]*exp(-(x-param[0])*(x-param[0])/2/param[1]/param[1]);
	grad[1] = param[2]*exp(-(x-param[0])*(x-param[0])/2/param[1]/param[1])*(x-param[0])*(x-param[0])/param[1]/param[1]/param[1];
	grad[2] = exp(-(x-param[0])*(x-param[0])/2/param[1]/param[1]);
	return grad;
}

/*
 * does a LU-Partition of A and stores the result in A
 * if the matrix can not be inverted A[0][0] is set to nan
 * A: the matrix (will be overwritten)
 */ 
void doLUPartition(Matrix& A){
	//go diagonal from up let to down right
	for(int d=0;d<A.size();d++){
		//go right to calculate u[i][j] in this row
		for(int j=d;j<A.size();j++){
			//caculate u
			double sum = 0;
			for(int k=0;k<d;k++){
				sum += A[d][k]*A[k][j];
			}
			A[d][j] = A[d][j] - sum;
		}
		//if u[d][d] = A[d][d] = 0 => LU-partition does not work 
		if(A[d][d] == 0){
			//write to stderr
			cerr << "Matrix can't be inverted because of a 0 in LU-partition" << endl;
			//mark A[0][0] as nan to show that it didn't worked
			A[0][0] = NAN;
			//stop here
			return;
		}
		//go down to calculate l[i][j] in this column
		for(int i=d+1;i<A.size();i++){
			//caculate l
			double sum = 0;
			for(int k=0;k<d;k++){
				sum += A[i][k]*A[k][d];
			}
			A[i][d] = (A[i][d] - sum)/A[d][d];
		}
	}
}

/*
 * returns the inverse vector of b under U assuming U is the upper part of A
 * A: the LU-partition of A
 * b: the vector to invert
 * return U^-1 * b
 */ 
vector<double> getInverseVectorU(Matrix& A, vector<double>& b){
	//the result
	vector<double> res(b.size());
	//start at the bottom and go up
	for(int i=b.size()-1;i>=0;i--){
		double sum = 0;
		for(int j=i+1;j<b.size();j++){
			sum += A[i][j]*res[j];
		}
		res[i] = (b[i] - sum)/A[i][i]; 
	}
	return res;
}

/*
 * returns the inverse vector of b under L assuming L is the lower part of A
 * A: the LU-partition of A
 * b: the vector to invert
 * return L^-1 * b
 */ 
vector<double> getInverseVectorL(Matrix& A, vector<double>& b){
	//the result
	vector<double> res(b.size());
	//start at the top and go down
	for(int i=0;i<b.size();i++){
		double sum = 0;
		for(int j=0;j<i;j++){
			sum += A[i][j]*res[j];
		}
		res[i] = b[i] - sum; 
	}
	return res;
}

/*
 * returns the inverse vector of b under A assuming A is LU partition of A
 * A: the LU-partition of A
 * b: the vector to invert
 * return A^-1 * b
 */ 
vector<double> getInverseVectorLU(Matrix& A, vector<double>& b){
	vector<double> temp = getInverseVectorL(A, b);
	return getInverseVectorU(A, temp);
}

/*
 * returns the inverse vector of b under A
 * returns {} if inverting failed
 * A: the Matrix
 * b: the vector to invert
 * return A^-1 * b
 */ 
vector<double> getInverseVector(Matrix A, vector<double>& b){
	//LU-partition first
	doLUPartition(A);
	//if LU didn't work -> return {}
	if(isnan(A[0][0])){
		return {};
	}
	//else return the inverse vector
	return getInverseVectorLU(A,b);
}

/*
 * returns the inverse Matrix of A
 * returns {} if inverting failed
 * A: the Matrix to invert
 * return A^-1
 */
Matrix getInverseMatrix(Matrix A){
	//LU-partition first
	doLUPartition(A);
	//check if successfull, else return {}
	if(isnan(A[0][0])){
		return {};
	}
	//B = A^-1
	Matrix B = Matrix(A.size(),vector<double>(A.size()));
	//the j'th unit vector
	vector<double> e_j(A.size());
	//start with e_1
	e_j[0] = 1;
	//invert all the e_j and write it in the j'th column of B
	for(int j=0;j<A.size();j++){
		//get the inverse of e_j
		vector<double> inverse = getInverseVectorLU(A,e_j);
		//store it in the j'th column of B
		for(int i=0;i<inverse.size();i++){
			B[i][j] = inverse[i];
		}
		//calculate the next e_j by shifting the 1
		e_j[j] = 0;
		if(j!=A.size()-1){
			e_j[j+1] = 1;
		}
	}
	return B;
}

/*
 * returns Xi² of the function to fit with current parameters
 * fitfunction: the function to fit
 * x: the x values
 * y: the y values
 * a: the current parameters
 * return Xi² = sum over (y-f(x))²
 */ 
double getXiSquare(double (*fitFunction)(double,vector<double>&), vector<double>& x, vector<double>& y, vector<double>& a){
	double xi = 0;
	for(int k=0;k<x.size();k++){
		//add the k'th quadratic error to xi
		double f = fitFunction(x[k],a);
		xi += (f-y[k])*(f-y[k]);
	}
	return xi;
}

/*
 * returns a list of of parameters such that fitfunction fits the data best (relative error < PRECISION)
 * uses Gaus-Newton and Levenberg-Marquard
 * if does not converge -> return {}
 * fitfunction: the function to fit
 * fitfunctionGradient: the parameter-gradient of fitfunction
 * x: the x values
 * y: the y values
 * a: the parameters to start with
 * return a list of parameters a
 */
vector<double> gaussNewton(double (*fitFunction)(double,vector<double>&), vector<double> (*fitFunctionGradient)(double, vector<double>&), vector<double>& x, vector<double>& y, vector<double> a){
	//store current xi²
	double xi_square = getXiSquare(fitFunction, x, y, a);
	//start with lambda = lambda_0
	double lambda = GAUSSNEWTON_LAMBDA_0;
	//iterate till error < precision, but stop after 100000 iterations
	for(int n=0;n<100000;n++){
		//decrease lambda
		lambda *= GAUSSNEWTON_GAMMA;
		
		//calculate A
		Matrix A(a.size(),vector<double>(a.size(),0));
		//... and b (called beta in script)
		vector<double> b(a.size(),0);
		//sum over all data
		for(int k=0;k<x.size();k++){
			//the value of f at (x[k],a)
			double f = fitFunction(x[k],a);
			//... and the gradient here
			vector<double> grad = fitFunctionGradient(x[k],a);
			for(int i=0;i<a.size();i++){
				for(int j=0;j<a.size();j++){
					//calculate A
					A[i][j] += grad[i]*grad[j];
				} 
				//calculate b
				b[i] += (y[k]-f)*grad[i];
			}
		}
		
		//Levenberg-Marquard: adjust lambda till xi^2 gets smaller, stop after lambda > GAUSSNEWTON_LAMBDA_MAX
		while(lambda <= GAUSSNEWTON_LAMBDA_MAX){
			//A_ = A + lambda * 1
			Matrix A_ = A;
			//add lambda
			for(int i=0;i<A_.size();i++){
				A_[i][i] += lambda;
			}
			//calculate the change of a
			vector<double> da = getInverseVector(A_,b);
			//if da is empty -> wasn't able to invert -> try with slighty different matrix
			if(da.empty()){
				cerr << "Wasn't able to invert matrix: try again with different lambda" << endl;
				//increment lambda slightly
				lambda *=1.00001;
				continue;
			}
			
			//else calculate the new a
			vector<double> a_new = a + da;
			//... and the new xi²
			double xi_square_new = getXiSquare(fitFunction, x, y, a_new);
			
			//if not better -> repeat with bigger lambda
			if(xi_square_new >= xi_square){
				//increase lambda
				lambda *= GAUSSNEWTON_DELTA;
				continue;
			}
			
			//now everything is fine, but check if precision is reached
			//find the maximum of relative deviation
			double relative_precision = 0;
			for(int i=0;i<a.size();i++){
				//relative deviation of the i'th parameter
				double rel = da[i]/a[i];
				//get the absolute value
				if(rel<0) rel=-rel;
				//if this one is greater than all the others before -> store it
				if(rel > relative_precision){
					relative_precision = rel;
				}
			}
			
			//if precision is reached -> return result
			if(relative_precision < PRECISION){
				return a;
			}
			
			//else accept new parameter
			xi_square = xi_square_new;
			a = a + da;
			//output info over current step
			cout << "da " << (abs(da)) << "\t" << relative_precision << endl;
			break;
		}	
		
	}
	//if it converged -> return {}
	return {};
}

/*
 * returns the error of the fittet parameters
 * if matrix couldn't be inverted return {}
 * fitfunctionGradient: the parameter-gradient of fitfunction
 * x: the x values
 * a: the parameters
 * return a list of standart deviations sigma[i]
 */

vector<double> calculateErrors(vector<double> (*fitFunctionGradient)(double, vector<double>&), vector<double>& x, vector<double>& a){
	//Normalmatrix
	Matrix N(a.size(),vector<double>(a.size(),0));
	for(int k=0;k<x.size();k++){
		//neglecting second order term
		vector<double> grad = fitFunctionGradient(x[k],a);
		for(int i=0;i<N.size();i++){
			for(int j=0;j<N.size();j++){
				N[i][j] += grad[i]*grad[j];
			}
		}
	}
	//invert N to get the covariance matrix
	Matrix C = getInverseMatrix(N);
	//if something went wrong -> return {}
	if(C.empty()){
		return {};
	}
	
	//calculate sigma[i] = sqrt(C[i][i])
	vector<double> sigma(C.size());
	for(int i=0;i<C.size();i++){
		sigma[i] = sqrt(C[i][i]);
	}
	return sigma;
}

int main(int argc, char** argv){
	std::cout.precision(10);
	
	//load the file
	ifstream fin("profile_cmb.dat");
	//read the x and y values
	vector<double> x,y;
	for(int i=0;!fin.eof();i++){
		double x_,y_;
		fin >> x_ >> y_;
		x.push_back(x_);
		y.push_back(y_);
	}
	x.pop_back();
	y.pop_back();
	
	//start with this a
	vector<double> a_0 = {650,50,4000};
	//the fittet parameters
	vector<double> a_fit = gaussNewton(gauss, gaussGradient, x, y, a_0);
	//the standart deviations of them
	vector<double> error = calculateErrors(gaussGradient, x, a_fit);
	
	cout << "fit parameters:" << endl;
	cout << "mu\t" << a_fit[0] << "\t+-" << error[0] << endl;
	cout << "sigma\t" << a_fit[1] << "\t+-" << error[1] << endl;
	cout << "N\t" << a_fit[2] << "\t+-" << error[2] << endl;
	
	return 0;
}
