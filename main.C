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
//#include "fit.h"

using namespace std;

extern "C"{
	void functionname(int *arrxs,int *arrys,int *arrsigmas,int *arrlasts,const int size) {
		int i;
		int j;
		int k;
		int events = 0;
		int eventindices[6];
		int eventsizes[6];
		int number=0;
		//Determine size of each event and the index where the end of each event occurs
		for(i=0;i<size;i++){
			if(arrlasts[i]==1){
				eventindices[i]=i;
				eventsizes[i]=number;
				number -= eventsizes[i];
			}
		}

		int start = 0;
		//Slices arrays into new 2D arrays separated by event
		#pragma HLS pipeline
		for(i=0;i<6;i++){
			int xsarray[6][eventsizes[i]];
			int ysarray[6][eventsizes[i]];
			int sigmasarray[6][eventsizes[i]];
			for(j=0;j<eventsizes[i];j++){
				#pragma HLS unroll
				xsarray[i][j] = arrxs[start+j];
				ysarray[i][j] = arrys[start+j];
				sigmasarray[i][j] = arrsigmas[start+j];
			}
			//memcpy(xsarray[i],arrxs[start],eventsizes[i]*sizeof(int));
			//memcpy(ysarray[i],arrys[start],eventsizes[i]*sizeof(int));
			//memcpy(sigmasarray[i],arrsigmas[start],eventsizes[i]*sizeof(int));
			start += eventsizes[i]+1;
		}

		//Allocate memory for an array of sums for each input column excepts lasts
		int sumxs[6];
		int sumys[6];
		int sumsigmas[6];
		int bigs[6];
		int bigsx[6];
		int bigsy[6];
		int bigstt[6];
		int param_a[6];
		int param_b[6];
		int variance_a[6];
		int variance_b[6];
		int chi_squared[6];
		int xsarray[6][size];
		int ysarray[6][size];
		int sigmasarray[6][size];


		//Calculate array of sums
		for(i=0;i<6;i++){
			for(j=0;j<eventsizes[i];j++){
				sumxs[i] += xsarray[i][j];
				sumys[i] += ysarray[i][j];
				sumsigmas[i] += sigmasarray[i][j];
			}

		//Calculate Big S values
			for(j=0;j<(eventsizes[i]-1);j++){
				bigs[i] += 1/(sigmasarray[i][j] * sigmasarray[i][j]);
				bigsx[i] += xsarray[i][j]/(sigmasarray[i][j] * sigmasarray[i][j]);
				bigsy[i] += ysarray[i][j]/(sigmasarray[i][j] * sigmasarray[i][j]);
			}

			for(j=0;j<(eventsizes[i]-1);j++){
				bigstt[i] += (1/sigmasarray[i][j]) * (xsarray[i][j] - bigsx[i]/bigs[i]) * (1/sigmasarray[i][j]) * (xsarray[i][j] - bigsx[i]/bigs[i]);
			}

		//Calculate array of parameters a and b
			for(j=0;j<(eventsizes[i]-1);j++){
				param_b[i] = (1/bigstt[i]) * (((1/sigmasarray[i][j]) * (xsarray[i][j] - bigsx[i]/bigs[i]) * ysarray[i][j]) / sigmasarray[i][j]);
			}

			for(j=0;j<(eventsizes[i]-1);j++){
				param_a[i] = (1/bigs[i]) * (bigsy[i] - (bigsx[i] * param_b[i]));
			}

		//Calculate array of variance on a and b
			variance_a[i] =(1/bigs[i]) * (1 + (bigsx[i] * bigsx[i])/(bigs[i] * bigstt[i]));
			variance_b[i] =1/bigstt[i];

		//Calculate array of chi squared
			for(j=0;j<eventsizes[i];j++){
				chi_squared[i] += (1/4) * (ysarray[i][j] - param_a[i] - (param_b[i] * xsarray[i][j])) * (ysarray[i][j] - param_a[i] - (param_b[i] * xsarray[i][j]));
			}

			std::cout << "Parameter a is: " << param_a[i] << std::endl;
			std::cout << "Parameter b is: " << param_b[i] << std::endl;
			std::cout << "Uncertainty of a is: " << variance_a[i] << std::endl;
			std::cout << "Uncertainty of b is: " << variance_b[i] << std::endl;
			std::cout << "Chi Squared Value is: " << chi_squared[i] << std::endl;

			//fprintf(stdout,"Parameter a is: %d",param_a[i]);
			//fprintf(stdout,"Parameter b is: %d",param_b[i]);
			//fprintf(stdout,"Uncertainty of a is: %d",variance_a[i]);
			//fprintf(stdout,"Uncertainty of b is: %d",variance_b[i]);
			//fprintf(stdout,"Chi Squared Value is: %d",chi_squared[i]);
		}

	}

}

int main(){
	functionname(tempxs,tempys,tempsigmas,templasts,tempN);
	return 0;
}





