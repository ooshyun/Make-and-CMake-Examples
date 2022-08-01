#ifdef __NATUREDSP_SIGNAL_H__
#include "NatureDSP_Signal.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h> // usleep

#include "progressbar.h"
#include "statusbar.h"

// #define RUN_PROGRESS_BAR

#define RUN_RANDOM_VAR_Q0_31

// #define RUN_STATIC_ARRAY
#ifdef RUN_STATIC_ARRAY
#define SIZE 2
#endif

// [TODO] How to remain the log ?
// void easysleep()
// {
// 	// struct timespec a;
// 	// a.tv_sec  = 0;
// 	// a.tv_nsec = 1000000;
// 	// int rc = nanosleep( &a, NULL );
// 	usleep(1);
// }

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
	printf("  Array\n");
	for (index = 0; index < len; index++)
	{
		numArray[index] = (numArray[index] - _min) / (double)(_max - _min); // static cast
#ifdef RUN_RANDOM_VAR_Q0_31
        if(numArray[index] == 1){
            numArray[index] -= 1;
        }
        if(numArray[index] == -1){
            numArray[index] += 1;
        }
#endif
		printf("%f, ", numArray[index]);
	}
	printf("\n-------------------------------------------\n");

    return numArray; 
}

void custom_mtx_mpy(void* out, void* in1, void* in2, int N, int M, int P)
{
    // mtx_mpy((int*)out, (int*)in1, (int*)in2, N, M, P);
	return;
}

int cal_len_bin(int number)
{
	uint8_t count=0;
	int buffer = number;
	while(buffer!=1)
	{
		buffer /= 2;
		count += 1; 
	}
	return count;
}

// int DOUBLE_TO_FIX(double x, int m ,int n);
// double FIX_TO_DOUBLE(int x, int Q);
// void print_matrix_int(int const *x, int m, int n);
// void print_matrix_fix(int const *x, int m, int n, int Q);
// void print_matrix_double(double const *x, int m, int n);

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

	int m = cal_len_bin(ncol);
	if(m >= 29)
	{
		printf("The overlapped column is over the range\n");
		return;
	}
	int n = 31-m; // signed, 32bit, -1 <= random number <= 1

#ifdef RUN_RANDOM_VAR_Q0_31
    int m_Q0_31 = 0;
    int n_Q0_31 = 31;
	m = m_Q0_31;
	n = n_Q0_31;
#endif

    printf("(%d, %d) x (%d, %d) -> Q %d.%d\n", nrow, ncol, ncol, ncolOther, m, n);
    printf("-------------------------------------------\n");
    size_t sizeXmtxDouble = sizeof(double) * nrow * ncol;
    size_t sizeYmtxDouble = sizeof(double) * ncol * ncolOther;
    size_t sizeOutmtxDouble = sizeof(double) * nrow * ncolOther;
    size_t sizeXmtxInt = sizeof(int) * nrow * ncol;
    size_t sizeYmtxInt = sizeof(int) * ncol * ncolOther;
    size_t sizeOutmtxInt = sizeof(int) * nrow * ncolOther;

    double* ptrXmtx = (double *)malloc(sizeXmtxDouble);
    double* ptrYmtx = (double *)malloc(sizeYmtxDouble);
    ptrXmtx = randfrom(-1, 1, nrow * ncol);
    ptrYmtx = randfrom(-1, 1, ncol * ncolOther);
    
	double* out_float = (double *)malloc(sizeOutmtxDouble);
    memset(out_float, 0, sizeOutmtxDouble);
    int* out_fix = (int *)malloc(sizeOutmtxInt);
    memset(out_fix, 0, sizeOutmtxInt);

    // https://docs.w3cub.com/cpp/memory/c/aligned_alloc
    // https://en.cppreference.com/w/cpp/memory/c/aligned_alloc
    int* ptrXmtxQ31 = (int *)aligned_alloc(8, sizeXmtxInt); // make the data to __attribute__((aligned(8)))
    int* ptrYmtxQ31 = (int *)aligned_alloc(8, sizeYmtxInt);

#ifdef RUN_STATIC_ARRAY
    int XmtxQ31[SIZE][SIZE] __attribute__((aligned(8)));
    int YmtxQ31[SIZE][SIZE] __attribute__((aligned(8)));
    int out_fix_custom[SIZE][SIZE] = {0};
#else
    int* out_fix_custom = (int *)malloc(sizeOutmtxInt);
    memset(out_fix_custom, 0, sizeOutmtxInt);
#endif
    printf("Successfully allocated memory for matrices\n");

	int i=0, j=0, k=0;
	
	// number
#ifdef RUN_PROGRESS_BAR
	progressbar *progress = progressbar_new("matrix X generating...",nrow*ncol);
#endif
	for(i = 0; i < nrow; i++){
		for(j = 0; j < ncol; j++){
			ptrXmtxQ31[i*ncol+j] = DOUBLE_TO_FIX(ptrXmtx[i*ncol+j], m, n);
#ifdef RUN_STATIC_ARRAY
            XmtxQ31[i][j] = DOUBLE_TO_FIX(ptrXmtx[i*ncol+j], m, n);
#endif
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
			ptrYmtxQ31[i*ncol+j] = DOUBLE_TO_FIX(ptrYmtx[i*ncol+j], m, n);
#ifdef RUN_STATIC_ARRAY
            YmtxQ31[i][j] = DOUBLE_TO_FIX(ptrYmtx[i*ncol+j], m, n);
#endif
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
	double tmp_number = 0;
	int n_tmp_number = n;
	int m_tmp_number = m;
	for(i = 0; i < nrow; i++){
		for(j = 0; j < ncolOther; j++){
			tmp_number = 0;
			n_tmp_number = n;
			m_tmp_number = m;
            for(k = 0; k < ncol; k++){
				tmp_number += FIX_TO_DOUBLE(ptrXmtxQ31[i*ncol+k], n) * FIX_TO_DOUBLE(ptrYmtxQ31[ncol*k + j], n);
#ifdef RUN_PROGRESS_BAR
				progressbar_inc(progress);
#endif
            }
#ifdef RUN_RANDOM_VAR_Q0_31
			n_tmp_number -= 1;
			m_tmp_number += 1;
#endif
			out_fix[i*ncolOther+j] = DOUBLE_TO_FIX(tmp_number, m_tmp_number, n_tmp_number); // [TODO] Overflow
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

    start = clock();

#ifdef RUN_STATIC_ARRAY
    //custom_mtx_mpy((int*)out_fix_custom, (int*)ptrXmtxQ31, (int*)ptrYmtxQ31, nrow, ncol, ncolOther); [TODO] Test wrapping
    mtx_mpy(&out_fix_custom[0][0], &XmtxQ31[0][0], &YmtxQ31[0][0], SIZE, SIZE, SIZE);
#else
    // mtx_mpy((int*)out_fix_custom, (int*)ptrXmtxQ31, (int*)ptrYmtxQ31, nrow, ncol, ncolOther);
#endif
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("-------------------------------------------\n");
    printf("  [custom, int32] %d rows, %f seconds\n", nrow, cpu_time_used);
    printf("-------------------------------------------\n");
		
	printf("\nResult\n");
	printf("-------------------------------------------\n");

#ifdef RUN_RANDOM_VAR_Q0_31
	n -= 1;
#endif
	printf("%d\n",n);

	printf("  out_float\n");
	print_matrix_double(out_float, nrow, ncolOther);

	printf("  out_fix\n");
	print_matrix_fix(out_fix, nrow, ncolOther, n);

#ifdef RUN_STATIC_ARRAY
    print_matrix_fix(&out_fix_custom[0][0], nrow, ncolOther, n);
#else
    print_matrix_fix(out_fix_custom, nrow, ncolOther, n);    
#endif

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
	for(i=0; i<2; i++) {
		printf("  %d th\n", i+1);
		row = (uint16_t)(trunc(pow((double)(2), (double)(i+1))));
		test_mtx_mpy(row, row, row); // test matrix
		// test_rand((size_t)row); // test rand
	}
	// test_mtx_trans();
}
