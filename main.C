#include "main.h"
#include <ap_int.h>
#include <ap_fixed.h> // Using fixed-point arithmetic for resource optimization
#include <hls_stream.h>
#include <iostream>  // For debug prints
#include "hls_math.h"
#include <fstream>
#include <iomanip>
#include <cstring>
#include <string>
#include "fit.h"

using namespace std;
//pseudo code time
//First open the file
//Next count the number of rows in the file (expect each row to have 4 ints)
//I need to bring in the .txt file and partition it into 6 differently sized arrays
//I know when each array ends because the last value is 1
//Once I have these four arrays, I want to partition them into 4 1D arrays
//I want to use these 1D arrays to get my summations
//The summations can get a, b, uncertainties, and chi squared

extern "C"{
void functionname() {
	int **xs;
	int **ys;
	int **sigmays;
	int **lasts;
	int i;
	int rows = 0;

	//Opens the txt file
	FILE *data = fopen("data.txt", "r");

	//Counts the number of rows
	while(fscanf(data,"%d %d %d %d")==4) {
		rows++;
	}

	//Allocates memory dynamically for each array
	xs = (int **)malloc(rows * sizeof(int *));
	ys = (int **)malloc(rows * sizeof(int *));
	sigmays = (int **)malloc(rows * sizeof(int *));
	lasts = (int **)malloc(rows * sizeof(int *));
	
	//Start back at begining of txt file
	rewind(data);

	//Takes each line of 4 ints and writes them into the arrays
	while(fscanf(data,"%d %d %d %d",&xs[i],&ys[i],&sigmays[i],&lasts[i])==4) {
		i++;
	}

	//Closes the txt file
	fclose(data);
	
	//Now we have four separate arrays of x's, y's, sigma y's, and lasts
	//Need to separate these into the number of  events in the file
	


	//Who knows what this does????
	fit(tempxs, tempys, tempsigmas, templasts, tempN);

	int N = sizeof(xs);
	int count = 0;
	int **indices;

	//Counts number of events and finds the indices of the ends of those events
	for(i=0;i<N;i++){
		if(lasts[i]==1){
			count++;
			indices[i];
		}
	}

	//Separates the arrays into smaller arrays of each event
	void arraycuts(int *arr){
		for(i=0;i<count;i++){
			memcpy(xse, xs[indices[i]],(indices[i+1]-[i]) * sizeof(int));
			memcpy(yse, ys[indices[i]],(indices[i+1]-[i]) * sizeof(int));
			memcpy(sigmayse, sigmays[indices[i]],(indices[i+1]-[i]) * sizeof(int));
		}

	}

	int sumxs;
	int sumys;
	int sumxxs;
	int sumxys;

	//Create Summations
	for(i=0;i<N;i++){
		sumxs += tempxs[i];
		sumys += tempyx[i];
		sumxxs += tempxs[i]*tempxs[i];
		sumxys += tempxs[i]*tempys[i];
	}

	int bigs;
	int bigsx;
	int bigsy;

	//Calculate Variance
	for(i=0;i<(N-1);i++){
		bigs += 1/(sigmays[i]*sigmays[i]);
		bigsx += xs[i]/(sigmays[i]*sigmays[i]);
		bigsy += ys[i]/(sigmays[i]*sigmays[i]);
	}

	int bigstt;

	for(i=0;i<(N-1);i++){
		bigstt += (1/sigmays[i])*(xs[i] - bigsx/bigs) * (1/sigmays[i])*(xs[i] - bigsx/bigs);
	}

	
	int var_a = (1/bigs) * (1 + (sumxs*sumxs)/(bigs * bigstt));
	int var_b = 1/bigstt

	int param_b;
	int param_a;

	for(i=0;i<(N-1);i++){
		param_b += (1/bigstt) * (((1/sigmays[i])*(xs[i] - bigsx/bigs))*ys[i])/sigmays[i]
	}

	param_a = (bigsy - bigsx * param_b)/bigs;

	int chi_squared;

	for(i=0;i<N;i++){
		chi_squared += (1/(N-2)) * ((ys[i] - param_a - param_b * xs[i])/sigmays[i]) * ((ys[i] - param_a - param_b * xs[i])/sigmays[i]);
		//Does N need to be degrees of freedom?
	}


	return 0;
	}

	
}
