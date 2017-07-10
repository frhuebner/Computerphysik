#include<vector>
using namespace std;

/*
 * use fehlberg
 */
//the siez of the butcher tableau
#define RUNGE_KUTTA_SIZE 6
//the safety constant eta
#define RUNGE_KUTTA_ETA 0.8
//the order of correctness of the runge kutta
#define RUNGE_KUTTA_ORDER 5

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

//the precission: called epsilon in the lecture notes
static double runge_kutta_precission = 1e-12;

//the current stepsize, start with 0.001
double runge_kutta_h = 0.001;

//the table of all the a[i][j] coefficients
const double RUNGE_KUTTA_A[RUNGE_KUTTA_SIZE][RUNGE_KUTTA_SIZE] =
	{{0,0,0,0,0,0},
	{1.0/4,0,0,0,0,0},
	{3.0/32,9.0/32,0,0,0,0},
	{1932.0/2197,-7200.0/2197,7296.0/2197,0,0,0},
	{439.0/216,-8,3680.0/513,-845.0/4104,0,0},
	{-8.0/27,2,-3544.0/2565,1859.0/4104,-11.0/40,0}};
//the b coefficients for both methods
const double RUNGE_KUTTA_B[2][RUNGE_KUTTA_SIZE] = 
	{{16.0/135,0,6656.0/12825,28561.0/56430,-9.0/50,2.0/55},
	{25.0/216,0,1408.0/2565,2197.0/4104,-1.0/5,0}};
//the c coefficients
const double RUNGE_KUTTA_C[RUNGE_KUTTA_SIZE] =
	{0,1.0/4,3.0/8,12.0/13,1,1.0/2};

//the current simulation time
double runge_kutta_t;
//the current y
vector<double> runge_kutta_y;

//the function f in y' = f(t,y) where t is a double and y a vector<double>
static vector<double> (*runge_kutta_f)(double, vector<double>);

/*
 * init the runge kutta method with f,t0 and y0
 * f: the function to use for y' = f(t,y)
 * t0: the starting time
 * y0: the starting y vector
 */ 
void runge_kutta_init(vector<double> (*f)(double, vector<double>), double t0, const vector<double>& y0){
	runge_kutta_f = f;
	runge_kutta_t = t0;
	runge_kutta_y = y0; 
}

/*
 * Does one step and calculates the next h
 * optimise:
 * 		true -> stepsize-control on: if the error is to big: repeats the procedure with optimise = false and a better h
 * 		false -> stepsize-control off: ignores the error, this is for preventing infinite loops
 */ 
void runge_kutta_step(bool optimise){
	double t = runge_kutta_t;
	double h = runge_kutta_h; 
	vector<double>& y = runge_kutta_y;
	//the k's from runge kutta
	vector<double> k[RUNGE_KUTTA_SIZE];
	
	//calculate all the k[i]
	for(int i=0;i<RUNGE_KUTTA_SIZE;i++){
		//sum over all a[i][j]*k[j]
		vector<double> sum_a_k(y.size(),0);
		for(int j=0;j<i;j++){
			sum_a_k += (RUNGE_KUTTA_A[i][j]*k[j]);
		}
		//calculate k[i]
		k[i] = runge_kutta_f(t + h*RUNGE_KUTTA_C[i],y + (h*sum_a_k));
	}
	
	//calculte new value and error
	vector<double> sum_b_k(y.size(),0);
	vector<double> sum_b_k_diff(y.size(),0);
	for(int i=0;i<RUNGE_KUTTA_SIZE;i++){
		//for new value
		sum_b_k += (RUNGE_KUTTA_B[0][i]*k[i]);
		//for the error
		sum_b_k_diff += (RUNGE_KUTTA_B[0][i]-RUNGE_KUTTA_B[1][i])*k[i];
	}
	
	//control step size
	
	//dh is the error
	double dh = abs(sum_b_k_diff)*h;
	//calculate new optimal h by eta*h*(epsilon/dh)^(1/5)
	double hopt = RUNGE_KUTTA_ETA*h*pow(runge_kutta_precission/dh,1.0/RUNGE_KUTTA_ORDER);
	//if we want to control stepsize and the error is to big -> do it again with better h
	if(optimise && dh > runge_kutta_precission){
		//set new h to hopt
		runge_kutta_h = hopt;
		//do the whole thing again, but with no stepsize control to prevent infinite loops
		runge_kutta_step(false);
		return;
	}
	//else calculate the new values
	runge_kutta_t += h;
	runge_kutta_h = hopt;
	runge_kutta_y += (h*sum_b_k);
}

/*
 * does one iteration, simply calls runge_kutta_step with stepsize-control
 */ 
void runge_kutta_iterate(){
	runge_kutta_step(true);
}
