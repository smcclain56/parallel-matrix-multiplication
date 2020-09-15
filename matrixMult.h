/**
Header file for matrix multiplication
*/


//global variables 
int **first;
int **second;
int **resultSeq;
int **resultPar;
int matrixSize;
int N;

//function declarations
void seqMult(int **first, int **second, int **resultSeq, int size);
void* parallelMult(void* arg);
int** initMatrix(int size);
int** initResult(int size);
void printMatrix(int **matrix, int size);
double compareMatrix(int **first, int **second, int matrixSize);
double seqSetUp(int **first, int **second, int **resultSeq, int size);