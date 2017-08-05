#include<vector>
using namespace std;

//the safety constant eta for embedded Runge-Kutta
#define RUNGE_KUTTA_ETA 0.8


/*
 ***********************************************************************
 * overload operators for vectors
 */ 
 
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
 * overloads the += operator
 * a = a + b componentwise
 */ 
void operator+=(vector<double>& a,const vector<double>& b){
	for(int i=0;i<a.size();i++){
		a[i] += b[i];
	}
}

/*
 * overloads operator - for vectors
 * return: a-b componentwise
 */ 
const vector<double> operator-(const vector<double>& a,const vector<double>& b){
	vector<double> ret(a.size());
	for(int i=0;i<a.size();i++){
		ret[i] = a[i] - b[i];
	}
	return ret;
}

/*
 * overloads operator * for double and vector
 * return: l*a componentwise
 */
const vector<double> operator*(double l, const vector<double>& a){
	vector<double> ret(a.size());
	for(int i=0;i<a.size();i++){
		ret[i] = l*a[i];
	}
	return ret;
}

/*
 * returns the dotproduct of 2 vectors
 * return: a*b = a[0]*b[0] + a[1]*b[1] + ...
 */ 
double dotproduct(const vector<double>& a, const vector<double>& b){
	double sum = 0;
	for(int i=0;i<a.size();i++){
		sum+=a[i]*b[i];
	}
	return sum;
}

/*
 * returns the crossproduct between 2 vectors
 * return: a x b 
 */
const vector<double> crossproduct(const vector<double>& a, const vector<double>& b){
	vector<double> ret = {a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]};
	return ret;
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
 ***********************************************************************
 * Define Algorithms
 */ 

/*
 * FEHLBERG
 */ 
//the table of all the a[i][j] coefficients of fehlberg
vector<vector<double> > FEHLBERG_A =
	{{0,0,0,0,0,0},
	{1.0/4,0,0,0,0,0},
	{3.0/32,9.0/32,0,0,0,0},
	{1932.0/2197,-7200.0/2197,7296.0/2197,0,0,0},
	{439.0/216,-8,3680.0/513,-845.0/4104,0,0},
	{-8.0/27,2,-3544.0/2565,1859.0/4104,-11.0/40,0}};
//the b coefficients of fehlberg
vector<vector<double> > FEHLBERG_B = 
	{{16.0/135,0,6656.0/12825,28561.0/56430,-9.0/50,2.0/55},
	{25.0/216,0,1408.0/2565,2197.0/4104,-1.0/5,0}};
//the c coefficients of fehlberg
vector<double> FEHLBERG_C =
	{0,1.0/4,3.0/8,12.0/13,1,1.0/2};


/*
 * CLASSIC RUNGE-KUTTA
 */ 
//the table of all the a[i][j] coefficients of classic runge-kutta
vector<vector<double> > CLASSIC_A =
	{{0,0,0,0},
	{1.0/2,0,0,0},
	{0,1.0/2,0,0},
	{0,0,1,0}};
//the b coefficients of classic runge-kutta
//secon line is 0 because it's no embedded
vector<vector<double> > CLASSIC_B = 
	{{1.0/6,1.0/3,1.0/3,1.0/6},
	{0,0,0,0}};
//the c coefficients of classic runge-kutta
vector<double> CLASSIC_C =
	{0,1.0/2,1.0/2,1};


/*
 ***********************************************************************
 * ODE-Solver 
 */ 



struct RungeKuttaAlgorithm{
	//true if embedded runge-kutta
	bool embedded;
	//the size of the butcher tableau
	int size;
	//the order of correctness of the runge kutta
	int order;
	vector<vector<double> > a;
	vector<vector<double> > b;
	vector<double> c;
};



class RungeKutta{
	//see implementation for detailed description of functions
	public:
		//the current simulation time
		double t;
		
		//the current y
		vector<double> y;
		
		//construct and init with all needed parameters
		RungeKutta(vector<double> (*f_)(double, vector<double>), double t0, const vector<double>& y0, RungeKuttaAlgorithm algo);
		
		//set stepsize
		void setStepSize(double h_);
		
		//returns current step size
		double getStepSize();
		
		//set precision
		void setPrecision(double prec);
		
		//do one step
		void iterate();
		
		//the possible algorithms
		static RungeKuttaAlgorithm fehlberg;
		static RungeKuttaAlgorithm classic;
	
	private:
		//the dimension of the y vector
		int dimension;
		
		//the precision: called epsilon in the lecture notes, needed for embedded
		double precision = 1e-12;
		
		//the current stepsize, standart -> 0.001
		double h = 0.001;
		
		//the function f in y' = f(t,y) where t is a double and y a vector<double>
		vector<double> (*f)(double, vector<double>);
		
		//the coefficients to use
		RungeKuttaAlgorithm algorithm;
		
		//do one step and optimise stepsize if needed
		void step(bool optimise);
};

//represents the fehlberg algorithm
RungeKuttaAlgorithm RungeKutta::fehlberg = {true, 6, 5, FEHLBERG_A, FEHLBERG_B, FEHLBERG_C};
//represents the classic runge-kutta algorithm
RungeKuttaAlgorithm RungeKutta::classic = {false, 4, 0, CLASSIC_A, CLASSIC_B, CLASSIC_C};

/*
 * constructs the runge kutta with f,t0 and y0, h0 and prec using alorithm algo
 * f_: the function to use for y' = f(t,y)
 * t0: the starting time
 * y0: the starting y vector
 * algo: the coefficient set to work with
 */ 
RungeKutta::RungeKutta(vector<double> (*f_)(double, vector<double>), double t0, const vector<double>& y0, RungeKuttaAlgorithm algo){
	f = f_;
	t = t0;
	y = y0;
	algorithm = algo;
	dimension = y.size();
};

/*
 * sets the stepsize to h_
 */ 
void RungeKutta::setStepSize(double h_){
	h = h_;
}

/*
 * returns the current stepsize
 */ 
double RungeKutta::getStepSize(){
	return h;
}

/*
 * sets the precission to prec
 */ 
void RungeKutta::setPrecision(double prec){
	precision = prec;
}


/*
 * Does one step and calculates the next h
 * optimise:
 * 		true -> stepsize-control on: if the error is to big: repeats the procedure with optimise = false and a better h
 * 		false -> stepsize-control off: ignores the error, this is for preventing infinite loops
 */ 
void RungeKutta::step(bool optimise){
	//the k's from runge kutta
	vector<double> k[algorithm.size];
	
	//calculate all the k[i]
	for(int i=0;i<algorithm.size;i++){
		//sum over all a[i][j]*k[j]
		vector<double> sum_a_k(dimension,0);
		for(int j=0;j<i;j++){
			sum_a_k += (algorithm.a[i][j]*k[j]);
		}
		//calculate k[i]
		k[i] = f(t + h*algorithm.c[i],y + (h*sum_a_k));
	}
	
	//calculte new value and error
	vector<double> sum_b_k(dimension,0);
	vector<double> sum_b_k_diff(dimension,0);
	for(int i=0;i<algorithm.size;i++){
		//for new value
		sum_b_k += (algorithm.b[0][i]*k[i]);
		//for the error
		sum_b_k_diff += (algorithm.b[0][i]-algorithm.b[1][i])*k[i];
	}
	
	//control step size
	
	//dh is the error
	double dh = abs(sum_b_k_diff)*h;
	//calculate new optimal h by eta*h*(epsilon/dh)^(1/5)
	double hopt = RUNGE_KUTTA_ETA*h*pow(precision/dh,1.0/algorithm.order);
	//if we want to control stepsize and the error is to big -> do it again with better h
	if(optimise && dh > precision){
		//set new h to hopt
		h = hopt;
		//do the whole thing again, but with no stepsize control to prevent infinite loops
		step(false);
		return;
	}
	//else calculate the new values
	t += h;
	y += (h*sum_b_k);
	if(algorithm.embedded){
		h = hopt;
	}
}

/*
 * does one iteration, simply calls step with stepsize-control if needed
 */ 
void RungeKutta::iterate(){
	step(algorithm.embedded);
}
