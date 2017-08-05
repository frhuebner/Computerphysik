#include<iostream>
#include<cstdlib>
#include<time.h>
#include<math.h>
#include<vector>
#include<limits.h>
#include<utility>
#include<fstream>

//Stepsize for solving the ode
#define NUMEROV_STEPSIZE 1e-5
//solve the ode in [0:NUMEROV_MAX]
#define NUMEROV_MAX 10.0
//number of points in the output
#define PLOT_SAMPLES 1000
//the precision of the energy
#define ENERGY_PRECISION 0.001
//the step between 2 energies while sweeping
#define ENERGY_SWEEP_STEP 0.1
//sweeps the intervall [0:ENERGY_SWEEP_MAX] for eigenenergies
#define ENERGY_SWEEP_MAX 40

using namespace std;

typedef pair<double, bool> PDB;
typedef pair<double, int> PDI; 

/*
 * the potential for the normal oscillator
 */ 
double harmonicOscillator(double x){
	return 0.5*x*x;
}

/*
 * the potential for the anharmonic oscillator
 */
double anharmonicOscillator(double x){
	return 0.5*x*x + 1.5*x*x*x*x;
}

//the potential that is used by numerov
static double (*potential) (double) = harmonicOscillator; 

/*
 * solves the schroedingers-equation with energy e, potential potential and the 2 initial values y0, y1
 * e: energy of the particle
 * y0: value of y at 0
 * y1: value of y at NUMEROV_STEPSIZE
 * return: a list of the solution y with y[i] = y(i*NUMEROV_STEPSIZE)
 */ 
vector<double> numerov(double e, double y0,  double y1){
	//number of steps
	int samples = (int) (NUMEROV_MAX/NUMEROV_STEPSIZE);
	//the solution function
	vector<double> y(samples);
	//init with initial values
	y[0] = y0;
	y[1] = y1;
	for(int i=2;i<samples;i++){
		//do numerov, i.e. calculate s_i, s_i-1, s_i-2
		double s_i = -2*potential(NUMEROV_STEPSIZE*(i-1)) + 2*e; 
		double s_minus_1 = -2*potential(NUMEROV_STEPSIZE*(i-1)) + 2*e;
		double s_minus_2 = -2*potential(NUMEROV_STEPSIZE*(i-2)) + 2*e;
		//calculate the next y
		y[i] = (2-5.0/6*NUMEROV_STEPSIZE*NUMEROV_STEPSIZE*s_minus_1)*y[i-1] - (1+NUMEROV_STEPSIZE*NUMEROV_STEPSIZE/12*s_minus_2)*y[i-2];
		y[i] /= (1+NUMEROV_STEPSIZE*NUMEROV_STEPSIZE/12*s_i);
	}
	//return the result
	return y;
}

/*
 * calculates the norm of a function psi
 * stopping at the first local minimum after 'zeros' zeros have been found 
 * 
 * psi: wave function given as array (like numerov-output)
 * zeros: the number of zeros in the intervall (0,oo)
 * return: a pair (double, int) with the norm and the position where integrating stopped
 */ 
PDI getNorm(vector<double> psi, int zeros){
	PDI ret(0,-1);
	//the current number of zeros found
	int z = 0;
	//the integral over psi² using trapezoidal method 
	double sum = psi[0]*psi[0]/2;
	for(int i=1;i<psi.size()-1;i++){
		//stop the integration if enough zeros have been found and we are in a local minima
		if(z == zeros && psi[i]*psi[i]<psi[i+1]*psi[i+1] && psi[i-1]*psi[i-1] > psi[i]*psi[i]){
			//add the last term of the trapezoidal method 
			sum += psi[i]*psi[i]/2;
			//stop at this index
			ret.second = i;
			break;
		}
		//check if we found a zero or a sign change
		if(psi[i-1]*psi[i] < 0 || psi[i] == 0){
			//increment the number of zeros found
			if(z<zeros){
				z++;
			}
		}
		//sum over psi²
		sum += psi[i]*psi[i];
	}
	//finally calculate the norm
	ret.first = sqrt(2 * sum * NUMEROV_STEPSIZE);
	return ret;
}

/*
 * solves the ode for the even case and returns the last value of psi as an indicator if the function goes to +oo or -oo 
 * e: energy
 * return: the last value calculated
 */ 
double getLimitEven(double e){
	vector<double> psi = numerov(e, 1, 1);
	return psi[psi.size()-1];
}

/*
 * solves the ode for the odd case and returns the last value of psi as an indicator if the function goes to +oo or -oo 
 * e: energy
 * return: the last value calculated
 */ 
double getLimitOdd(double e){
	vector<double> psi = numerov(e, 0, NUMEROV_STEPSIZE);
	return psi[psi.size()-1];
}


/*
 * a bisection method that finds a zero of f in the intervall (a,b)
 * f: the function
 * a: left intervall border
 * b: right intervall border
 */
double bisection(double (*f)(double x), double a, double b){
	//calculate the number of iterations needed to get the precision needed
	int n = (int)log2((b-a)/ENERGY_PRECISION)+2;
	//the value of the function on the left side, is used later to compare the middle value 
	double fa = f(a);
	double m = (a+b)/2;
	for(int i=0;i<n;i++){
		//calculate the center of the intervall
		m = (a+b)/2;
		//get the value of the function there
		double fm = f(m);
		//if we found a zero -> return this value
		if(fm == 0){
			return m;
		}
		//if f(m) has the same sign as f(a) -> set a = m, else b = m
		if(fa*fm >0){
			fa = fm;
			a = m;
		}else{
			b = m;
		}
	}
	//return the result
	return m;
}

/*
 * sweeps the intervall [0:ENERGY_SWEEP_MAX] for eigenenergies
 * return: a list of pairs (double, bool)
 * 		double: the energy
 * 		bool: the parity of the function, true -> even, false -> odd
 */ 
vector<PDB> sweep(){
	
	vector<PDB> ret;
	
	//the current asymptotical direction of the wave function, if >0 -> +oo, if <0 -> -oo
	//if one of these swaps -> in the interval is an eigenenergy 
	double limit_even = getLimitEven(ENERGY_SWEEP_STEP), limit_odd = getLimitOdd(ENERGY_SWEEP_STEP); 
	//sweep over all energies
	for(double e=2*ENERGY_SWEEP_STEP;e<ENERGY_SWEEP_MAX;e+=ENERGY_SWEEP_STEP){
		cout << "Looking at energy: " << e << endl;
		//the the new limits of the wave function
		double le = getLimitEven(e);
		double lo = getLimitOdd(e);
		//if one of them is 0 -> found a eigenenergy
		if(le == 0){
			//add solution to list
			PDB pdb(e,true);
			ret.push_back(pdb);
		}
		if(lo == 0){
			//add solution to list
			PDB pdb(e,false);
			ret.push_back(pdb);
		}
		//if the sign of the limit has swapped -> in the intervall (e-step, e) is a eigenenergy
		if(limit_even * le < 0){
			//get a better result using bisection, ala. find the root of getLimitEven
			double root = bisection(getLimitEven,e-ENERGY_SWEEP_STEP,e);
			//add solution to list
			PDB pdb(root,true);
			ret.push_back(pdb);
		}
		if(limit_odd * lo < 0){
			//get a better result using bisection, ala. find the root of getLimitOdd
			double root = bisection(getLimitOdd,e-ENERGY_SWEEP_STEP,e);
			//add solution to list
			PDB pdb(root,false);
			ret.push_back(pdb);
		}
		//update the limits
		limit_even = le;
		limit_odd = lo;
	}
	//return the result
	return ret;
}

int main(int argc, char** argv){
	std::cout.precision(10);
	
	//sweep over all energies
	vector<PDB> energies = sweep();
	cout << "number\tenergy\t\tparity" << endl;
	//for all the energies -> calculate wave function
	for(int i=0;i<energies.size();i++){
		double energy = energies[i].first;
		bool parity = energies[i].second;
		
		//the parity has to alternate, if not -> something went wrong
		if(i%2 == parity){
			cerr << i << "'th function has wrong parity!" << endl; 
		}
		
		//get the wave function
		vector<double> psi = numerov(energy, parity?1:0, parity?1:(NUMEROV_STEPSIZE));
		//get the norm and the cutoff
		PDI pdi = getNorm(psi, i/2);
		double norm = pdi.first;
		int cutoff = pdi.second;
		//get an output file
		char filename[50];
		sprintf(filename, "out_sweep_%d.txt", (int)round(energy*1000));
		ofstream fout(filename);
		
		//output the wave function, but with less samples because it's not needed
		int plot_step = (int)(NUMEROV_MAX/NUMEROV_STEPSIZE/PLOT_SAMPLES);
		for(int i=0;i<psi.size();i+=plot_step){
			/*
			 * column 1: x value
			 * column 2: normalised wave function
			 * column 3: normalised wave function cutted of at the cutoff
			 * column 4: the unnormalised wave function
			 */ 
			fout << (i*NUMEROV_STEPSIZE) << "\t" << (psi[i]/norm) << "\t" << (i<=cutoff?(psi[i]/norm):0) << "\t" << psi[i] << endl;
		}
		
		fout.close();
		//output the energy to the console
		cout << i << ":\t" << energy << "\t" << (parity?"true":"false") << endl; 
	}
	
	return 0;
}
