
/**
 *	Water Retention on Magic Squares Solver
 *
 *	Author: Johan Öfverstedt
 *	Modified: July 2012
 *	Version 0.12a
 *
 *	ms_matrix.h
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
 
#ifndef _MS_MATRIX_H_
#define _MS_MATRIX_H_

#include "minpriorityqueue.h"

class MSMatrix {
public:
	MSMatrix(unsigned int param_n, bool param_associative, bool param_semi_magic);
	~MSMatrix();

	void randomRestart();
	void doSwap(int param_index1, int param_index2);

	int getStoredViolation() { return cur_violation; }
	int violation();

	int swapDelta(int param_index1, int param_index2);

	int getValue(int param_index) { return mat[param_index]; }
	void setValue(int param_index, int param_value) { mat[param_index] = param_value; }
	int getN() { return n; }

	void consolePrint();

	/*** Water retention releated: *** */

	int getLastRetention() { return last_retention; }

	int retention();
	int swapRetentionDelta(int param_index1, int param_index2);
	void loadWaterLevels();	
	void saveWaterLevels();	

	//Sub procedures
	void drain(int param_index, int param_value);

protected:
	int n;
	int nn;
	int *mat;

	int magic_const;
	int associative_const;

	bool associative;
	bool semi_magic;

	int cur_violation;
	int *row_sum;
	int *col_sum;
	int right_diag_sum;
	int left_diag_sum;

	//Water retention related

	int *w; //Water levels
	int *w_saved; //Saved water levels
	MinPriorityQueue *q; //Priority queue
	int last_retention; //Last retention value
};

#endif