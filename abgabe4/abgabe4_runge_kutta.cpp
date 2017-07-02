#include<vector>
using namespace std;

/*
 * use fehlberg
 */
#define RUNGE_KUTTA_SIZE 6
#define RUNGE_KUTTA_ETA 0.8
#define RUNGE_KUTTA_ORDER 5

ostream& operator<<(ostream& o,const vector<double>& a){
	for(int i=0;i<a.size();i++){
		o << a[i] << (i==a.size()-1?"":"\t");
	}
	return o;
}

const vector<double> operator+(const vector<double>& a,const vector<double>& b){
	vector<double> ret(a.size());
	for(int i=0;i<a.size();i++){
		ret[i] = a[i] + b[i];
	}
	return ret;
}

void operator+=(vector<double>& a,const vector<double>& b){
	for(int i=0;i<a.size();i++){
		a[i] += b[i];
	}
}

const vector<double> operator-(const vector<double>& a,const vector<double>& b){
	vector<double> ret(a.size());
	for(int i=0;i<a.size();i++){
		ret[i] = a[i] - b[i];
	}
	return ret;
}

const vector<double> operator*(double l, const vector<double>& a){
	vector<double> ret(a.size());
	for(int i=0;i<a.size();i++){
		ret[i] = l*a[i];
	}
	return ret;
}

double dotproduct(const vector<double>& a, const vector<double>& b){
	double sum = 0;
	for(int i=0;i<a.size();i++){
		sum+=a[i]*b[i];
	}
	return sum;
}

const vector<double> crossproduct(const vector<double>& a, const vector<double>& b){
	vector<double> ret = {a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]};
	return ret;
}

double abs(vector<double>& a){
	double ret = 0;
	for(int i=0;i<a.size();i++){
		ret += a[i]*a[i];
	}
	return sqrt(ret);
}

static double runge_kutta_precission = 1e-12;

double runge_kutta_h = 0.001;

const double RUNGE_KUTTA_A[RUNGE_KUTTA_SIZE][RUNGE_KUTTA_SIZE] =
	{{0,0,0,0,0,0},
	{1.0/4,0,0,0,0,0},
	{3.0/32,9.0/32,0,0,0,0},
	{1932.0/2197,-7200.0/2197,7296.0/2197,0,0,0},
	{439.0/216,-8,3680.0/513,-845.0/4104,0,0},
	{-8.0/27,2,-3544.0/2565,1859.0/4104,-11.0/40,0}};
const double RUNGE_KUTTA_B[2][RUNGE_KUTTA_SIZE] = 
	{{16.0/135,0,6656.0/12825,28561.0/56430,-9.0/50,2.0/55},
	{25.0/216,0,1408.0/2565,2197.0/4104,-1.0/5,0}};
const double RUNGE_KUTTA_C[RUNGE_KUTTA_SIZE] =
	{0,1.0/4,3.0/8,12.0/13,1,1.0/2};

double runge_kutta_t;
vector<double> runge_kutta_y;

static vector<double> (*runge_kutta_f)(double, vector<double>);

void runge_kutta_init(vector<double> (*f)(double, vector<double>), double t0, const vector<double>& y0){
	runge_kutta_f = f;
	runge_kutta_t = t0;
	runge_kutta_y = y0; 
}

void runge_kutta_step(bool optimise){
	double t = runge_kutta_t;
	double h = runge_kutta_h; 
	vector<double>& y = runge_kutta_y;
	vector<double> k[RUNGE_KUTTA_SIZE];
	
	for(int i=0;i<RUNGE_KUTTA_SIZE;i++){
		vector<double> sum_a_k(y.size(),0);
		for(int j=0;j<i;j++){
			sum_a_k += (RUNGE_KUTTA_A[i][j]*k[j]);
		}
		k[i] = runge_kutta_f(t + h*RUNGE_KUTTA_C[i],y + (h*sum_a_k));
	}
	
	//calculte new value and error
	vector<double> sum_b_k(y.size(),0);
	vector<double> sum_b_k_diff(y.size(),0);
	for(int i=0;i<RUNGE_KUTTA_SIZE;i++){
		sum_b_k += (RUNGE_KUTTA_B[0][i]*k[i]);
		sum_b_k_diff += (RUNGE_KUTTA_B[0][i]-RUNGE_KUTTA_B[1][i])*k[i];
	}
	
	//control step size
	double dh = abs(sum_b_k_diff)*h;
	double hopt = RUNGE_KUTTA_ETA*h*pow(runge_kutta_precission/dh,1.0/RUNGE_KUTTA_ORDER);
	if(optimise && dh > runge_kutta_precission){
		runge_kutta_h = hopt;
		runge_kutta_step(false);
		return;
	}
	runge_kutta_t += h;
	runge_kutta_h = hopt;
	runge_kutta_y += (h*sum_b_k);
}

void runge_kutta_iterate(){
	runge_kutta_step(true);
}
