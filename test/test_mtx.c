#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#include "progressbar.h"
#include "statusbar.h"

// #define RUN_PROGRESS_BAR

// [TODO] How to remain the log ?
void easysleep()
{
	// struct timespec a;
	// a.tv_sec  = 0;
	// a.tv_nsec = 1000000;
	// int rc = nanosleep( &a, NULL );
	usleep(1);
}

/* generate a random floating point number from min to max */
double* randfrom(double min, double max, uint32_t len) 
{
	time_t t;
	/* Intializes random number generator */
   	srand((unsigned) time(&t));
	
	double* numArray = (double*)malloc(len * sizeof(double));

	int64_t _max = 0;
	int64_t _min = 0;
	uint32_t index;

	for (index = 0; index < len; index++)
	{
		numArray[index] = (double)rand();
		if (numArray[index] > _max)
		{
			_max = numArray[index];
		}
		if (numArray[index] < _min)
		{
			_min = numArray[index];
		}
		// easysleep();
	}

	// min-max regulation
	for (index = 0; index < len; index++)
	{
		numArray[index] = (numArray[index] - _min) / (double)(_max - _min); // static cast
	}

    return numArray; 
}

int DOUBLE_TO_FIX(double x, int m ,int n){// return Q m.n
	// double temp ;
	double min_val, max_val;
	min_val = -pow(2, m);
	max_val =  pow(2, m) - pow(2, -n);

	if      (x>max_val) return (int) round(max_val * pow(2, n));
	else if (x<min_val) return (int) round(min_val * pow(2, n));
	else                return (int) round(x       * pow(2, n));


}

double FIX_TO_DOUBLE(int x, int Q){
	return (double)x / pow(2, Q);
}

void print_matrix_int(int const *x, int m, int n){
	int i, j;
	for(i=0;i<m;i++){
		printf("[");
		for(j=0;j<n;j++){
			printf("%d  ", x[i*n+j]);
		}
		printf("]\n");
	}
}

void print_matrix_fix(int const *x, int m, int n, int Q){
	int i, j;
	for(i=0;i<m;i++){
		printf("[");
		for(j=0;j<n;j++){
			printf("%lg  ", FIX_TO_DOUBLE(x[i*n+j],Q));
		}
		printf("]\n");
	}
}
void print_matrix_double(double const *x, int m, int n){
	int i, j;
	for(i=0;i<m;i++){
		printf("[");
		for(j=0;j<n;j++){
			printf("%lg  ", x[i*n+j]);
		}
		printf("]\n");
	}
}

void test_mtx_mpy(uint16_t nrow, uint16_t ncol, uint16_t ncolOther)
{
	printf("---------------test_mtx_mpy----------------\n");

    size_t sizeXmtx = sizeof(double) * nrow * ncol;
    size_t sizeYmtx = sizeof(double) * ncol * ncolOther;
    size_t sizeOutmtx = sizeof(double) * nrow * ncolOther;
    
    double* ptrXmtx = (double *)malloc(sizeXmtx);
    double* ptrYmtx = (double *)malloc(sizeYmtx);
	ptrXmtx = randfrom(-1, 1, nrow * ncol);
	ptrYmtx = randfrom(-1, 1, ncol * ncolOther);

	// https://docs.w3cub.com/cpp/memory/c/aligned_alloc
	// https://en.cppreference.com/w/cpp/memory/c/aligned_alloc
    int* ptrXmtxQ31 = (int *)aligned_alloc(8, sizeXmtx); // make the data to __attribute__((aligned(8)))
    int* ptrYmtxQ31 = (int *)aligned_alloc(8, sizeYmtx);

    double* out_float = (double *)malloc(sizeOutmtx);
    memset(out_float, 0, sizeOutmtx);
    
    int* out_fix = (int *)malloc(sizeOutmtx);
    memset(out_fix, 0, sizeOutmtx);

    printf("Successfully allocated memory for matrices\n");

	int i=0, j=0, k=0;
#ifdef RUN_PROGRESS_BAR
	progressbar *progress = progressbar_new("matrix X generating...",nrow*ncol);
#endif
	for(i = 0; i < nrow; i++){
		for(j = 0; j < ncol; j++){
			ptrXmtxQ31[i*ncol+j] = DOUBLE_TO_FIX(ptrXmtx[i*ncol+j], 0, 31);

#ifdef RUN_PROGRESS_BAR
			progressbar_inc(progress);
#endif
		}
	}
#ifdef RUN_PROGRESS_BAR
	progressbar_finish(progress);
#endif

#ifdef RUN_PROGRESS_BAR
	progressbar *progress = progressbar_new("matrix Y generating...",nrow*ncol);
#endif		

	for(i = 0; i < ncol; i++){
		for(j = 0; j < ncolOther; j++){
			ptrYmtxQ31[i*ncol+j] = DOUBLE_TO_FIX(ptrYmtx[i*ncol+j], 0, 31);
#ifdef RUN_PROGRESS_BAR
			progressbar_inc(progress);
#endif
		}
	}
#ifdef RUN_PROGRESS_BAR
	progressbar_finish(progress);
#endif

    printf("Successfully generated random matrices\n");
	
	clock_t start, end;
    double cpu_time_used;

#ifdef RUN_PROGRESS_BAR
	progress = progressbar_new("matrix calculating...",nrow*ncol*ncolOther);		
#endif

	start = clock();
	for(i = 0; i < nrow; i++){
		for(j = 0; j < ncolOther; j++){
            for(k = 0; k < ncol; k++){
                out_float[i*ncolOther+j] += ptrXmtx[i*nrow + k] * ptrYmtx[ncolOther*k + j];
#ifdef RUN_PROGRESS_BAR
				progressbar_inc(progress);
#endif
            }
		}
	}
#ifdef RUN_PROGRESS_BAR
	progressbar_finish(progress);
#endif
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	
	printf("-------------------------------------------\n");
	printf("  [simple, double] %d rows, %f seconds\n", nrow, cpu_time_used);
	printf("-------------------------------------------\n");


#ifdef RUN_PROGRESS_BAR
	progress = progressbar_new("matrix calculating...",nrow*ncol*ncolOther);		
#endif

	start = clock();
	for(i = 0; i < nrow; i++){
		for(j = 0; j < ncolOther; j++){
            for(k = 0; k < ncol; k++){
				out_fix[i*ncolOther+j] += ptrXmtxQ31[i*ncol+k] * ptrYmtxQ31[ncol*k + j];
#ifdef RUN_PROGRESS_BAR
				progressbar_inc(progress);
#endif
            }
		}
	}
#ifdef RUN_PROGRESS_BAR
	progressbar_finish(progress);
#endif
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	
	printf("-------------------------------------------\n");
	printf("  [simple, int32] %d rows, %f seconds\n", nrow, cpu_time_used);
	printf("-------------------------------------------\n");

	// start = clock();

	// // custom_mtx_mpy(out_fix, ptrXmtxQ31, ptrYmtxQ31);

	// end = clock();
	// cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	
	// printf("-------------------------------------------\n");
	// printf("  [custom, int32] %d rows, %f seconds\n", nrow, cpu_time_used);
	// printf("-------------------------------------------\n");
		
	// printf("  out_float\n");
	// print_matrix_double(out_float, nrow, ncolOther);

	// printf("  out_fix=\n");
	// print_matrix_fix(out_fix, nrow, ncolOther, 30);
    

    if (ptrXmtx==NULL) 
        printf("Memory is not allocated\n");
    else
    {
        free(ptrXmtx);
    }
    if (ptrYmtx==NULL) 
        printf("Memory is not allocated\n");
    else
    {
        free(ptrYmtx);
    }
    if (ptrXmtxQ31==NULL)
        printf("Memory is not allocated\n");
    else
    {
        free(ptrXmtxQ31);
    }
    if (ptrYmtxQ31==NULL)
        printf("Memory is not allocated\n");
    else
    {
        free(ptrYmtxQ31);
    }
    if (out_float==NULL)
        printf("Memory is not allocated\n");
    else
    {
        free(out_float);
    }
    if (out_fix==NULL)
        printf("Memory is not allocated\n");
    else
    {
        free(out_fix);
    }
	printf("Completed!\n");
}

void test_rand(size_t size)
{
	double* numArray = randfrom(-1, 1, (uint32_t)size);

	for (int i = 0; i < size; i++)
	{
		printf("%f ", numArray[i]);
	}
	printf("\n");

	free(numArray);
}

void main_mtx()
{

	// [TODO] randfrom test, limitation for usleep

	uint16_t row = 0;
	uint8_t i = 0;
	for(i=0; i<20; i++) {
		printf("  %d th\n", i+1);
		row = (uint16_t)(trunc(pow((double)(2), (double)(i+1))));
		test_mtx_mpy(row, row, row); // test matrix
		// test_rand((size_t)row); // test rand
	}
	// test_mtx_trans();
}
