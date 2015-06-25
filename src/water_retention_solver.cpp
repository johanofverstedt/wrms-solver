
/**
 *	Water Retention on Magic Squares Solver
 *
 *	Author: Johan Öfverstedt
 *	Modified: July 2012
 *	Version 0.1
 *
 *	water_retention_solver.cpp
 *	Water Retention Solver implementing two of the three different
 *	algorithms from the thesis, the naive algorithm and the retention
 *	algorithm extended with an improvement discovered after the thesis
 *	was finished. This solver implements a second tabu list which lets
 *	all bad swaps found tabu for O(n^2) iterations since if they are
 *	bad moves one iteration, they are not likely to be good the next.
 *
 *	Project website:
 *	sourceforge.net/projects/wrmssolver
 *
 *	Based on thesis:
 *	http://urn.kb.se/resolve?urn=urn:nbn:se:uu:diva-176018
 *
 */

#include <iostream>
#include <time.h>
#include "ms_matrix.h"

#define MAX(x, y) (x) >= (y) ? (x) : (y)

using namespace std;

/**
 *	The Naive Algorithm Implementation
 */

int tabuNaive(MSMatrix *param_mat, int param_tabulength, int param_iterations, bool param_terminate_on_first_solution) {
	
	int it = 0;

	int n = param_mat->getN();
	int nn = n * n;
	int nn_minus_one = nn - 1;

	int *tabulist = new int[nn];

	for(int i = 0; i < nn; ++i)
		tabulist[i] = 0;

	while((param_mat->violation() > 0 || !param_terminate_on_first_solution) && it < param_iterations) {

		int best_delta = -1;
		int sel_ind1 = -1;
		int sel_ind2 = -1;

		for(int i1 = 0; i1 < nn_minus_one; ++i1) {
			if(tabulist[i1] > it)
				continue;

			for(int i2 = i1 + 1; i2 < nn; ++i2) {
				if(tabulist[i2] > it)
					continue;

				int delta = param_mat->swapDelta(i1, i2);

				if(sel_ind1 == -1 || best_delta > delta) {
					sel_ind1 = i1;
					sel_ind2 = i2;
					best_delta = delta;
				} else if(delta == best_delta) {
					if(rand() % 10 < 2) {
						sel_ind1 = i1;
						sel_ind2 = i2;
						best_delta = delta;
					}
				}

			}
		}

		if(sel_ind1 != -1) {
			param_mat->doSwap(sel_ind1, sel_ind2);
			param_mat->violation();
		}

		++it;
		tabulist[sel_ind1] = it + param_tabulength;
		tabulist[sel_ind2]= it + param_tabulength;
	}

	cout << "Iterations: " << it << endl;

	delete[] tabulist;

	if(param_mat->violation() > 0)
		return -1;

	int ret = param_mat->retention();
	
	//Return retention;
	return ret;

}

/**
 *	The Improved Retention Algorithm Implemented
 *	- The improvements of the algorithm from the version in the thesis
 *	- are marked by *** IMPROVEMENT comments.
 */

int tabuRetention(MSMatrix *param_mat, int param_tabulength, int param_iterations, bool param_terminate_on_first_solution) {
	
	int it = 0;

	int n = param_mat->getN();
	int nn = n * n;
	int nn_minus_one = nn - 1;

	float weight = 0.5f;

	int best_retention = -1;
	int *best_mat = new int[nn];


	int *tabulist = new int[nn];
	if(!tabulist)
		return -1;

	//Declare the swap tabu list *** IMPROVEMENT 
	int *swap_tabulist = new int[nn * nn];
	if(!swap_tabulist) {
		delete tabulist;
		return -1;
	}

	for(int i = 0; i < nn; ++i)
		tabulist[i] = 0;

	//Initialize the swap tabu list *** IMPROVEMENT 
	for(int i = 0; i < nn * nn; ++i)
		swap_tabulist[i] = 0;

	while((param_mat->violation() > 0 || !param_terminate_on_first_solution) && it < param_iterations) {

		float best_delta = 0.0f;
		int sel_ind1 = -1;
		int sel_ind2 = -1;

		param_mat->violation();
		param_mat->retention();
		param_mat->saveWaterLevels();

		for(int i1 = 0; i1 < nn_minus_one; ++i1) {
			if(tabulist[i1] > it)
				continue;

			for(int i2 = i1 + 1; i2 < nn; ++i2) {
				if(tabulist[i2] > it)
					continue;

				//If swap is tabu, skip it *** IMPROVEMENT 
				if(swap_tabulist[i1 * nn + i2] > it)
					continue;

				float delta = (float)param_mat->swapDelta(i1, i2);
				float water_delta = (float)(-param_mat->swapRetentionDelta(i1, i2));

				//If move is bad, make it tabu for (tabu length) ^ 2 iterations *** IMPROVEMENT 
				if(water_delta > 5)
					swap_tabulist[i1 * nn + i2] = it + param_tabulength * param_tabulength;

				if(it % 10 < 5) {
					delta = 0.1f * delta + weight * water_delta;
				} else {
					delta += weight * water_delta;
				}

				if(sel_ind1 == -1 || best_delta > delta) {
					sel_ind1 = i1;
					sel_ind2 = i2;
					best_delta = delta;
				} else if(delta == best_delta) {
					if(rand() % 10 < 2) {
						sel_ind1 = i1;
						sel_ind2 = i2;
						best_delta = delta;
					}
				}

			}
		}

		if(sel_ind1 != -1) {
			param_mat->doSwap(sel_ind1, sel_ind2);
		}

		weight *= 0.99f;
		if(weight < 0.0001f)
			weight = 0.5f;

		++it;
		tabulist[sel_ind1] = it + param_tabulength;
		tabulist[sel_ind2] = it + param_tabulength;

		if(param_mat->violation() == 0) {
			int new_ret = param_mat->retention();
			if(new_ret > best_retention) {
				for(int i = 0; i < nn; ++i) {
					best_mat[i] = param_mat->getValue(i);
				}
				best_retention = new_ret;
			}
			weight = 0.5f;
		}
	}

	cout << "Iterations: " << it << endl;

	if(best_retention > -1) {
		for(int i = 0; i < nn; ++i) {
			param_mat->setValue(i, best_mat[i]);
		}
	}

	delete[] tabulist;
	delete[] swap_tabulist;
	delete[] best_mat;

	if(best_retention < 0)
		return -1;
	
	//Return retention;
	return best_retention;

}

int main(int argc, char **argv) {
	
	MSMatrix *mat;

	int n = 0;
	int runs = 0;
	int iterations = 0;
	bool terminate_on_first_solution = false;

	//Seed random generator
	srand(time(0));

	cout << "Dimension: ";

	cin >> n;

	cout << "Runs: ";

	cin >> runs;

	cout << "Iterations: ";

	cin >> iterations;

	cout << "Terminate on first magic square (Y/N): ";
	
	char str[256];
	
	cin >> str; 
	
	if(str[0] == 'y' || str[0] == 'Y')
		terminate_on_first_solution = true;
	
	int nn = n * n;

	cout << endl;

	mat = new MSMatrix(n);

	int best_retention = -1;
	int *best_mat = new int[nn];

	for(int i = 0; i < runs; ++i) {

		mat->randomRestart();

		int clock1 = clock();

		int ret = tabuRetention(mat, (2 * n) / 3, iterations, terminate_on_first_solution);
		
		int clock2 = clock();

		cout << "Run: " << (i + 1) << endl;
		cout << "Time: " << ((clock2 - clock1)/((float)CLOCKS_PER_SEC / 1000.0f)) << "ms." << endl;

		if(ret >= 0 && mat->violation() == 0) {
			cout << "Square is Magic." << endl;
			cout << "Retention: " << ret << " ( Best: " << best_retention << " )" << endl;
			if(ret > best_retention) {
				best_retention = ret;
				for(int k = 0; k < nn; ++k)
					best_mat[k] = mat->getValue(k);
			}
		} else
			cout << "TIMEOUT." << endl;

		mat->consolePrint();

	}

	cout << "Best retention found: " << best_retention << endl;

	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < n; ++j) {
			cout << best_mat[i * n + j] << "\t";
		}
		cout << endl;
	}

	//cout << "Violation: " << mat->violation();

	cout << endl;

	delete mat;

#ifdef WIN32
	system("pause");
#endif
	
	return 0;

}