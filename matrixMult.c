/*
Matrix multiplication file
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "matrixMult.h"
#include "rtclock.h"

//function implementations

/*
Sets up sequential matrix multiplication and smooths the results by running
N+1 times and throwing away the first run
@param int **first, int **second are the matrices to multiply
@param int **resultSeq is the matrix to store result in 
@param int size of matrices
@return double time of calculation
*/
double seqSetUp(int **first, int **second, int **resultSeq, int size){
	double clkbegin,clkend,sequentialTime;
	int i;

	printf("=========\n");
	printf("mode: sequential\n");
	printf("thread count: 1\n");
	printf("size: %d\n", size);
	printf("=========\n");

	//fill matrices
	first = initMatrix(size);	
	second = initMatrix(size);
	resultSeq = initResult(size);

	//throw away first calculation
	seqMult(first,second,resultSeq,size);


	//do calculation 3 times to smooth results
	sequentialTime = 0.0;
	for(i=0;i<=3;i++){
		clkbegin = rtclock();
		seqMult(first, second, resultSeq, size);
		clkend = rtclock();
		sequentialTime += (clkend-clkbegin);
	}
	sequentialTime = sequentialTime/3;

	return sequentialTime;
}

/*
Prints a matrix
@param int **matrix to print, int size of matrix
@return void
*/
void printMatrix(int **matrix, int size){
	int i,j;
	for (i = 0; i <  size; i++) {
      for (j = 0; j < size; j++) {
         printf("%d ", matrix[i][j]); 
      }
      printf("\n");
	}
}

/*
Initialized array of random values between 0 and 99
@param int size of matrix
@return int** matrix of random values
*/
int** initMatrix(int size)
{
	int i,j;
	int **newMatrix = (int**) malloc(size*sizeof(int*));
	for(i=0;i<size;i++){
		newMatrix[i] = (int*) malloc(size*sizeof(int*));
	}
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			newMatrix[i][j] = rand()%100;
		}
	}
    return newMatrix;
}

/*
initializes arrays to all 0
@param int size of matrix
@return int** matrix of all 0
*/
int** initResult(int size)
{
	int i,j;
	int **newMatrix = (int**) malloc(size*sizeof(int*));
	for(i=0;i<size;i++){
		newMatrix[i] = (int*) malloc(size*sizeof(int*));
	}
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			newMatrix[i][j] = 0;
		}
	}
    return newMatrix;

}

/*
This method computes matrix multiplication sequentially 
@param int **first, int **second are two square integer matrices
that will be multiplied together
@param int size is size of both matrices
*/
void seqMult(int **first, int **second, int **resultSeq, int size)
{
	int i,j,k,sum;
	sum =0;
	for(i=0;i<size;i++){ //iterate rows
		for(j=0;j<size;j++){ //iterate columns
			for(k=0;k<size;k++){
				sum = sum + first[i][k]*second[k][j]; //update sum
			}
			resultSeq[i][j] = sum; //result element after matrix mult
			sum = 0; //for next run
		}
	}
}

/*
This method computes matrix multiplication using N number of threads
@param void* arg  slice 
@return void* NULL
*/
void* parallelMult(void* arg)
{
	int i,j,k,slice,start,end; 
	slice = (int)arg; //retrieve argument info
	start = (slice*matrixSize)/N;
	end = ((slice+1)*matrixSize)/N;

	//each thread computes 1/Nth of matrix multiplication
	for (i = start; i < end; i++){
		for(j=0; j<matrixSize; j++){
			for(k=0; k<matrixSize; k++){
				resultPar[i][j] += first[i][k]*second[k][j];
			}
		}
	}
	return NULL;
}

/*
This method compares two square matrices (of integers)of equal size
@param int **first, int **second are two squre matrices
@param int matrixSize is size of rows and columns of both matrices
@return 0 if equal matrices, 1 if not
*/
double compareMatrix(int **first, int **second, int matrixSize)
{
	int i,j;
	for(i=0; i<matrixSize; i++){
		for(j=0; j<matrixSize; j++){
			if(first[i][j] != second[i][j]){
				return 1;
			}
		}
	}
	return 0.0;
}