/*
This is the main file for matrix multiplication
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "rtclock.h"
#include "matrixMult.h"

int main(int argc, char **argv)
{
	int i;
	double clkbegin,clkend;
	double sequentialTime,parallelTime;
	char *mode;
	if(argv[1] == NULL){
		printf("Usage: ./mmm <mode> [num threads] <size> \n");
		return 0;
	}

	mode = argv[1];

	//SEQUENTIAL MODE
	if(strcmp(mode,"S")==0){
		//check if valid arguments
		if(argv[2] == NULL){
			printf("Usage: ./mmm <mode> [num threads] <size> \n");
			return 0;
		}
		matrixSize = atoi(argv[2]);
		sequentialTime = seqSetUp(first,second,resultSeq,matrixSize);
		printf("Sequential Time: %f sec \n",sequentialTime);

	}else if(strcmp(mode,"P")==0){ //PARALLEL MODE
		//check if valid arguments
		if(argv[3]==NULL){
			printf("Error: parallel mode requires [num threads]\n");
			return 0;
		}
		//parse command arguments
		N = atoi(argv[2]);
		matrixSize = atoi(argv[3]);
		printf("=========\n");
		printf("mode: parallel\n");
		printf("thread count: %d\n",N);
		printf("size: %d\n", matrixSize);
		printf("=========\n");

		//fill matrices
		first = initMatrix(matrixSize);
		second = initMatrix(matrixSize);
		resultSeq = initResult(matrixSize);
		resultPar = initResult(matrixSize);

		//throw away first run
		seqMult(first,second,resultSeq,matrixSize);

		//run three more times
		sequentialTime = 0.0;
		for(i=0;i<=3;i++){
			clkbegin = rtclock();
			seqMult(first,second,resultSeq,matrixSize);
			clkend = rtclock();
			sequentialTime += clkend-clkbegin;
		}
		sequentialTime = sequentialTime/3;
		printf("Sequential Time: %f\n",sequentialTime);



		//run parallel now
		clkbegin = rtclock();
		//allocate space for storing N threads
		pthread_t *threads = (pthread_t*) malloc(sizeof(pthread_t)*N);

		//spawn off N threads -- each calling Multiplication
		for (i=0; i<N; i++){
			pthread_create(&threads[i], NULL, parallelMult, (void*)i);
		}

		//wait for all threads to finish
		for (i=0;i<N;i++){
			pthread_join(threads[i],NULL);
		}
		clkend = rtclock();


		//print results
		parallelTime = clkend-clkbegin;
		printf("Parallel Time: %f\n", parallelTime);
		printf("Speedup: %f\n", sequentialTime/parallelTime);

		//verify matrices are same
		double equalMatrix = compareMatrix(resultSeq,resultPar,matrixSize);
		printf("Verifying... largest error between parallel and sequential matrix: %f\n", equalMatrix);
	}


	//free space
	free(first);
	free(second);
	free(resultSeq);
	free(resultPar);

	return 0;
}

