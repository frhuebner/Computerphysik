#include<iostream>
#include<cstdlib>
#include<time.h>
#include<math.h>
#include<vector>
#include<limits.h>
#include<utility>
#include<fstream>

#define NUMEROV_STEPSIZE 1e-5
#define NUMEROV_MAX 10.0
#define PLOT_SAMPLES 1000
#define NORM_LIMIT 0.001
#define ENERGY_PRECISION 0.001
#define ENERGY_SWEEP_STEP 0.1
#define ENERGY_SWEEP_MAX 20

using namespace std;

typedef pair<double, bool> PDB;
typedef pair<double, int> PDI; 

double harmonicOscillator(double x){
	return 0.5*x*x;
}

static double (*potential) (double) = harmonicOscillator; 

vector<double> numerov(double e, double y0,  double y1){
	int samples = (int) (NUMEROV_MAX/NUMEROV_STEPSIZE);
	vector<double> y(samples);
	y[0] = y0;
	y[1] = y1;
	for(int i=2;i<samples;i++){
		double s_i = -2*potential(NUMEROV_STEPSIZE*(i-1)) + 2*e; 
		double s_minus_1 = -2*potential(NUMEROV_STEPSIZE*(i-1)) + 2*e;
		double s_minus_2 = -2*potential(NUMEROV_STEPSIZE*(i-2)) + 2*e;
		y[i] = (2-5.0/6*NUMEROV_STEPSIZE*NUMEROV_STEPSIZE*s_minus_1)*y[i-1] - (1+NUMEROV_STEPSIZE*NUMEROV_STEPSIZE/12*s_minus_2)*y[i-2];
		y[i] /= (1+NUMEROV_STEPSIZE*NUMEROV_STEPSIZE/12*s_i);
	}
	return y;
}

PDI getNorm(vector<double> psi, int zeros){
	PDI ret(0,-1);
	int z = 0;
	double sum = psi[0]*psi[0]/2;
	for(int i=1;i<psi.size();i++){
		if(z == zeros && psi[i]*psi[i]/(2*sum*NUMEROV_STEPSIZE) < NORM_LIMIT && psi[i-1]*psi[i-1] > psi[i]*psi[i]){
			sum += psi[i]*psi[i]/2;
			ret.second = i;
			cout << "stop at " << i*NUMEROV_STEPSIZE << endl;
			break;
		}
		if(psi[i-1]*psi[i] < 0 || psi[i] == 0){
			if(z<zeros){
				z++;
			}
		}
		sum += psi[i]*psi[i];
	}
	cout << "square " << (2 * sum * NUMEROV_STEPSIZE) << endl;
	ret.first = sqrt(2 * sum * NUMEROV_STEPSIZE);
	return ret;
}

double getLimitEven(double e){
	vector<double> psi = numerov(e, 1, 1);
	return psi[psi.size()-1];
}

double getLimitOdd(double e){
	vector<double> psi = numerov(e, 0, NUMEROV_STEPSIZE);
	return psi[psi.size()-1];
}

double bisection(double (*f)(double x), double a, double b){
	int n = (int)log2((b-a)/ENERGY_PRECISION)+2;
	double fa = f(a);
	double m = (a+b)/2;
	for(int i=0;i<n;i++){
		m = (a+b)/2;
		double fm = f(m);
		if(fm == 0){
			return m;
		}
		if(fa*fm >0){
			fa = fm;
			a = m;
		}else{
				b = m;
		}
	}
	return m;
}

vector<PDB> sweep(){
	
	vector<PDB> ret;
	
	double limit_even = getLimitEven(ENERGY_SWEEP_STEP), limit_odd = getLimitOdd(ENERGY_SWEEP_STEP); 
	for(double e=2*ENERGY_SWEEP_STEP;e<ENERGY_SWEEP_MAX;e+=ENERGY_SWEEP_STEP){
		cout << "Looking at energy: " << e << endl;
		double le = getLimitEven(e);
		double lo = getLimitOdd(e);
		if(le == 0){
			PDB pdb(e,true);
			ret.push_back(pdb);
		}
		if(lo == 0){
			PDB pdb(e,false);
			ret.push_back(pdb);
		}
		if(limit_even * le < 0){
			double root = bisection(getLimitEven,e-ENERGY_SWEEP_STEP,e);
			PDB pdb(root,true);
			ret.push_back(pdb);
		}
		if(limit_odd * lo < 0){
			double root = bisection(getLimitOdd,e-ENERGY_SWEEP_STEP,e);
			PDB pdb(root,false);
			ret.push_back(pdb);
		}
		limit_even = le;
		limit_odd = lo;
	}
	return ret;
}

int main(int argc, char** argv){
	std::cout.precision(10);
	
	/*int e;
	cin >> e;
	
	vector<double> result = numerov(e/1000.0, 0, 10*NUMEROV_STEPSIZE);
	
	double norm = getNorm(result, e/2000);
	cout << "Norm " << norm << endl;
	
	char file[50];
	
	sprintf(file, "out_%d.txt", e);
	
	ofstream fout(file);
	int plot_step = (int)(NUMEROV_MAX/NUMEROV_STEPSIZE/PLOT_SAMPLES);
	for(int i=0;i<result.size();i+=plot_step){
		fout << (i*NUMEROV_STEPSIZE) << "\t" << (result[i]/norm) << "\t" << result[i] << endl;
	}
	fout.close();*/
	
	vector<PDB> energies = sweep();
	cout << "number\tenergy\t\tparity" << endl;
	for(int i=0;i<energies.size();i++){
		double energy = energies[i].first;
		bool parity = energies[i].second;
		
		if(i%2 == parity){
			cerr << i << "'th function has wrong parity!" << endl; 
		}
		
		vector<double> psi = numerov(energy, parity?1:0, parity?1:(NUMEROV_STEPSIZE));
		PDI pdi = getNorm(psi, i/2);
		double norm = pdi.first;
		int cutoff = pdi.second;
		char filename[50];
		sprintf(filename, "out_sweep_%d.txt", (int)round(energy*1000));
		ofstream fout(filename);
		
		int plot_step = (int)(NUMEROV_MAX/NUMEROV_STEPSIZE/PLOT_SAMPLES);
		for(int i=0;i<psi.size();i+=plot_step){
			fout << (i*NUMEROV_STEPSIZE) << "\t" << (psi[i]/norm) << "\t" << (i<=cutoff?(psi[i]/norm):0) << "\t" << psi[i] << endl;
		}
		
		fout.close();
		cout << i << ":\t" << energy << "\t" << (parity?"true":"false") << endl; 
	}
	
	return 0;
}
