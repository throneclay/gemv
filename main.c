#include <stdio.h>
#include <sys/time.h>
#include <hbwmalloc.h>
#include <memkind.h>
#include <mkl.h>

double seconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
}

memkind_t kind;
void *bench_malloc(size_t size)
{
    int err;
    void *ptr = NULL;

    if (memkind_check_available(MEMKIND_HBW) == 0){
        kind = MEMKIND_HBW;
    } else {
        fprintf(stderr, "WARNING: There's NO high bandwidth memory using standard memory\n");
        kind = MEMKIND_DEFAULT;
    }

    err = memkind_posix_memalign(kind, &ptr, 4096, size);
    if (err){
        fprintf(stderr, "ERROR: Unable to allocate MCDRAM memory\n");
        exit(-err);
    }

    return ptr;
}
void bench_free(void *ptr)
{
    memkind_free(kind, ptr);
}


int main(int argc, char* argv[])
{
	int M=512,N=512,K=512;
	double st=0,et=0;
	char ta = 'N',tb = 'N';
	int ITER = 10;
	int flag=-1;

	if(argc==6)
	{
		M = atoi(argv[1]);
		N = atoi(argv[2]);
		K = atoi(argv[3]);
		ITER = atoi(argv[4]);
		flag = atoi(argv[5]);
		printf("M = %d N = %d K = %d ITER = %d flag=%d [0,1]\n",M,N,K,ITER,flag);
	}else
	{
		printf("%s M N K ITER mode(0:sgemv, 1:dgemv)\n" ,argv[0]);
		exit(-1);
	}
	double ngops = 2.0 * M * N * K / 1E9;		
	int lda = 1>M?1:M;
	int ldb = 1>K?1:K;
	int ldc = 1>M?1:M;

	int hbw_f = hbw_check_available();

	if(flag==0){
		float* a,*b,*c;
		float alpha = 1.0;
		float beta = 1.0;

		a = (float*)bench_malloc(sizeof(float)*M*K);
		b = (float*)bench_malloc(sizeof(float)*K*N);
		c = (float*)bench_malloc(sizeof(float)*M*N);
		// warm up
		sgemm(&ta, &tb, &M, &N, &K, &alpha,
					a, &lda, b, &ldb, &beta, c, &ldc);
		sgemm(&ta, &tb, &M, &N, &K, &alpha,
					a, &lda, b, &ldb, &beta, c, &ldc);

		st = seconds();

		for(int i=0;i<ITER;i++)
		sgemm(&ta, &tb, &M, &N, &K, &alpha,
					a, &lda, b, &ldb, &beta, c, &ldc);

		et = seconds();
		printf("GFLOPS: %lf\n",ngops*ITER/(et-st));
		bench_free(a);
		bench_free(b);
		bench_free(c);
	}
	else
	{
		double* a,*b,*c;
		double alpha = 1.0;
		double beta = 1.0;

		a = (double*)bench_malloc(sizeof(double)*M*K);
		b = (double*)bench_malloc(sizeof(double)*K*N);
		c = (double*)bench_malloc(sizeof(double)*M*N);
		// warm up
		dgemm(&ta, &tb, &M, &N, &K, &alpha,
					a, &lda, b, &ldb, &beta, c, &ldc);
		dgemm(&ta, &tb, &M, &N, &K, &alpha,
					a, &lda, b, &ldb, &beta, c, &ldc);

		st = seconds();

		for(int i=0;i<ITER;i++)
		dgemm(&ta, &tb, &M, &N, &K, &alpha,
					a, &lda, b, &ldb, &beta, c, &ldc);

		et = seconds();
		printf("GFLOPS: %lf\n",ngops*ITER/(et-st));
		bench_free(a);
		bench_free(b);
		bench_free(c);

	}
	return 0;	
}

