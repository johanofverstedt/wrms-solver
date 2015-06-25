
/**
 *	Water Retention on Magic Squares Solver
 *
 *	Author: Johan Öfverstedt
 *	Modified: July 2012
 *	Version 0.1
 *
 *	ms_matrix.cpp
 *	Magic Square Matrix Implementation with Water Retention
 *	Algorithm to calculate the Water Retention value and delta
 *
 *	Project website:
 *	sourceforge.net/projects/wrmssolver
 *
 *	Based on thesis:
 *	http://urn.kb.se/resolve?urn=urn:nbn:se:uu:diva-176018
 *
 */
 
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "ms_matrix.h"

#define MAX(x, y) (x) >= (y) ? (x) : (y)
#define MIN(x, y) (x) <= (y) ? (x) : (y)

using namespace std;

MSMatrix::MSMatrix(unsigned int param_n) {

	n = param_n;
	nn = n * n;

	magic_const = n * (nn + 1) / 2;

	mat = new int[nn];
	row_sum = new int[n];
	col_sum = new int[n];

	randomRestart();

	//Setup water retention

	q = new MinPriorityQueue(nn);

	w = new int[nn];
	w_saved = new int[nn];

}

MSMatrix::~MSMatrix() {
	
	if(mat)
		delete[] mat;
	
	if(row_sum)
		delete[] row_sum;

	if(col_sum)
		delete[] col_sum;

	delete[] w;
	delete[] w_saved;
	delete q;

}

void MSMatrix::randomRestart() {

	int inc = 1;
	for(int i = 0; i < nn; ++i) {
		mat[i] = inc++;
	}

	for(int i = 0; i < nn - 1; ++i) {
		int ri = i + rand() % (n * n - i);

		int tmp = mat[i];
		mat[i] = mat[ri];
		mat[ri] = tmp;
	}

	violation();

}

void MSMatrix::doSwap(int param_index1, int param_index2) {

	int tmp = mat[param_index1];
	mat[param_index1] = mat[param_index2];
	mat[param_index2] = tmp;

	//violation();

}

int MSMatrix::violation() {

	int s = 0;

	//Reset violation value

	cur_violation = 0;

	//Row constraints

	for(int i = 0; i < n; ++i) {
		s = 0;
		for(int j = 0; j < n; ++j) {
			s += mat[i * n + j];
		}
		row_sum[i] = s;
		cur_violation += abs(row_sum[i] - magic_const);
	}

	//Col constraints

	for(int i = 0; i < n; ++i) {
		s = 0;
		for(int j = 0; j < n; ++j) {
			s += mat[j * n + i];
		}
		col_sum[i] = s;
		cur_violation += abs(col_sum[i] - magic_const);
	}

	//Diag constraints

	right_diag_sum = 0;
	left_diag_sum = 0;

	for(int i = 0; i < n; ++i) {
		right_diag_sum += mat[i * n + i];
		left_diag_sum += mat[(n - i - 1) * n + i];
	}

	cur_violation += abs(right_diag_sum - magic_const);
	cur_violation += abs(left_diag_sum - magic_const);

	return cur_violation;

}

int MSMatrix::swapDelta(int param_index1, int param_index2) {

	int i1 = param_index1 / n;
	int j1 = param_index1 % n;
	int i2 = param_index2 / n;
	int j2 = param_index2 % n;

	int delta = 0;

	//Row constraints

	if(i1 != i2) { //Different rows
		delta += abs(row_sum[i1] - mat[param_index1] + mat[param_index2] - magic_const) - abs(row_sum[i1] - magic_const);
		delta += abs(row_sum[i2] - mat[param_index2] + mat[param_index1] - magic_const) - abs(row_sum[i2] - magic_const);
	}

	if(j1 != j2) { //Different columns
		delta += abs(col_sum[j1] - mat[param_index1] + mat[param_index2] - magic_const) - abs(col_sum[j1] - magic_const);
		delta += abs(col_sum[j2] - mat[param_index2] + mat[param_index1] - magic_const) - abs(col_sum[j2] - magic_const);
	}

	if(i1 == j1 && i2 != j2) { //First in right column but second is not
		delta += abs(right_diag_sum - mat[param_index1] + mat[param_index2] - magic_const) - abs(right_diag_sum - magic_const);
	}
	if(i1 != j1 && i2 == j2) { //First in right column but second is not
		delta += abs(right_diag_sum - mat[param_index2] + mat[param_index1] - magic_const) - abs(right_diag_sum - magic_const);
	}

	if(i1 == (n - j1 - 1) && i2 != (n - j2 - 1)) { //First in right column but second is not
		delta += abs(left_diag_sum - mat[param_index1] + mat[param_index2] - magic_const) - abs(left_diag_sum - magic_const);
	}
	if(i1 != (n - j1 - 1) && i2 == (n - j2 - 1)) { //First in right column but second is not
		delta += abs(left_diag_sum - mat[param_index2] + mat[param_index1] - magic_const) - abs(left_diag_sum - magic_const);
	}

	return delta;

}

void MSMatrix::consolePrint() {

	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < n; ++j) {
			cout << mat[i * n + j] << "\t";
		}
		cout << endl;
	}

}

int MSMatrix::retention() {

	//Init corners:
	w[0] = mat[0];
	w[n - 1] = mat[n - 1];
	w[nn - n] = mat[nn - n];
	w[nn - 1] = mat[nn - 1];

	//Init edges:
	for(int i = 1; i < n - 1; ++i) {
		w[i] = mat[i];
		w[nn - i - 1] = mat[nn - i - 1];
		w[i * n] = mat[i * n];
		w[n - 1 + i * n] = mat[n - 1 + i * n];

		q->enqueue(i, w[i]);
		q->enqueue(nn - i - 1, w[nn - i - 1]);
		q->enqueue(i * n, w[i * n]);
		q->enqueue(n - 1 + i * n, w[n - 1 + i * n]);
	}
	
	//Init middle:
	for(int i = 1; i < n - 1; ++i) {
		for(int j = 1; j < n - 1; ++j) {
			w[i * n + j] = nn;
		}
	}

	while(q->size() > 0) {

		int ind = 0;
		int val = 0;

		q->dequeue(&ind, &val);

		//int indmod = ind % n;
		//int inddiv = ind / n;
		//if(indmod > 1)
			drain(ind - 1, val);
		//if(indmod < n - 1)
			drain(ind + 1, val);
		//if(inddiv > 0)
			drain(ind - n, val);
		//if(inddiv < n - 1)
			drain(ind + n, val);

	}

	last_retention = 0;

	for(int i = 0; i < nn; ++i)
		last_retention += w[i] - mat[i];

	return last_retention;

}

int MSMatrix::swapRetentionDelta(int param_index1, int param_index2) {

	if(w[param_index1] > mat[param_index1] && w[param_index2] > mat[param_index2]) {
		if(w[param_index2] > mat[param_index1] && w[param_index1] > mat[param_index2]) {
			return 0;
		}
	}

	int max_w1 = 0;
	if(param_index1 - 1 >= 0)
		max_w1 = MAX(max_w1, w[param_index1 - 1]);
	if(param_index1 + 1 < nn)
		max_w1 = MAX(max_w1, w[param_index1 + 1]);
	if(param_index1 - n >= 0)
		max_w1 = MAX(max_w1, w[param_index1 - n]);
	if(param_index1 + n < nn)
		max_w1 = MAX(max_w1, w[param_index1 + n]);

	int max_w2 = 0;
	if(param_index2 - 1 >= 0)
		max_w2 = MAX(max_w2, w[param_index2 - 1]);
	if(param_index2 + 1 < nn)
		max_w2 = MAX(max_w2, w[param_index2 + 1]);
	if(param_index2 - n >= 0)
		max_w2 = MAX(max_w2, w[param_index2 - n]);
	if(param_index2 + n < nn)
		max_w2 = MAX(max_w2, w[param_index2 + n]);

	if(w[param_index1] == mat[param_index1] && w[param_index2] == mat[param_index2]) {
		
		if(mat[param_index2] > max_w1 && mat[param_index1] > max_w2)
			return 0;

	}

	int pre = last_retention;
	int delta = 0;

	doSwap(param_index1, param_index2);

	retention();

	delta = last_retention - pre;

	doSwap(param_index1, param_index2);

	loadWaterLevels();

	last_retention = pre;

	return delta;

}

void MSMatrix::saveWaterLevels() {

	for(int i = 0; i < nn; ++i)
		w_saved[i] = w[i];

}

void MSMatrix::loadWaterLevels() {

	for(int i = 0; i < nn; ++i)
		w[i] = w_saved[i];

}

void MSMatrix::drain(int param_index, int param_value) {

	if(param_index < 0 || param_index >= nn)
		return;

	int tmp = MAX(mat[param_index], param_value);
	if(tmp < w[param_index]) {
		w[param_index] = tmp;
		q->enqueue(param_index, tmp);
	}

}