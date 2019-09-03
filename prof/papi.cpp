#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <omp.h>
#include <math.h>
#include <pthread.h>

#include <papi.h>

#define SWITCH_CHAR '-'

float RandFloat(float low, float high){
    float t = (float)rand() / (float)RAND_MAX;
    return (1.0f - t) * low + t * high;
}

int main(int argc, char **argv)
{
    float *h_A, *h_B, *h_C_CPU;
    int i;

    int VECTOR_N = 256;
    int ELEMENT_N = 4096;
  int b;

    while (argc){
      if (*argv[0] == SWITCH_CHAR){
          switch (*(argv[0]+1)){
            case 'n':
                VECTOR_N = (int)atol(argv[0]+2);
                break;
            case 'e':
                ELEMENT_N= (int)atol(argv[0]+2);
                break;
            default:
                break;
          }
      }
      argc -= 1;
      argv++;
    }

    int DATA_N = VECTOR_N * ELEMENT_N;
    int DATA_SZ = DATA_N * sizeof(float);
    int RESULT_SZ = VECTOR_N  * sizeof(float);

    h_A     = (float *)malloc(DATA_SZ);
    h_B     = (float *)malloc(DATA_SZ);
    float * h_C_Ref = (float *)malloc(RESULT_SZ);

/*
    srand(123);
    for(i = 0; i < DATA_N; i++){
        h_A[i] = RandFloat(0.0f, 1.0f);
        h_B[i] = RandFloat(0.0f, 1.0f);
    }
*/

if (PAPI_library_init (PAPI_VER_CURRENT) != PAPI_VER_CURRENT)
{
    printf("Error: PAPI_library_init\n");
    exit(1);
}
if (PAPI_thread_init(pthread_self) != PAPI_OK)
{
    printf("Error: PAPI_thread_init\n");
    exit(1);
}

#pragma omp parallel
{

long long * values = (long long *)malloc(2 * sizeof (long long));
values[0] =0;
values[1] = 0;
int EventSet = PAPI_NULL;
int ret_val = 0;

if (PAPI_create_eventset(&EventSet) != PAPI_OK)
{
    printf("Error: PAPI_create_eventset\n");
    exit(1);
}

int native = 0x0;
ret_val = PAPI_event_name_to_code("L2_CODE_READ_MISS_MEM_FILL", &native);
if (ret_val != PAPI_OK)
{
    printf("Error: PAPI_event_name_to_code1 %d, native = %x\n", ret_val, native);
    exit(1);
}
ret_val = PAPI_add_event(EventSet, native);
if (ret_val != PAPI_OK)
{
    printf("Error: PAPI_add_event1 %d, native = %x\n", ret_val, native);
    exit(1);
}
/*

ret_val = PAPI_event_name_to_code("L2_CODE_READ_MISS_CACHE_FILL", &native);
if (ret_val != PAPI_OK)
{
    printf("Error: PAPI_event_name_to_code2 %d, native = %x\n", ret_val, native);
    exit(1);
}
ret_val = PAPI_add_event(EventSet, native);
if (ret_val != PAPI_OK)
{
    printf("Error: PAPI_add_event2 %d, native = %x\n", ret_val, native);
    exit(1);
}
else
  printf("native = %x success\n", native);
*/


if (PAPI_start(EventSet) != PAPI_OK)
{
    printf("Error: PAPI_start\n");
    exit(1);
}
#pragma omp for
    for(int vec = 0; vec < VECTOR_N; vec++){
        double sum = 0;
        for(int pos = 0; pos < 163840; pos++)
            sum += 0.324 * 2.798;
/*
        int vectorBase = ELEMENT_N * vec;
        int vectorEnd  = vectorBase + ELEMENT_N;

        double sum = 0;
        for(int pos = vectorBase; pos < vectorEnd; pos++)
            sum += h_A[pos] * h_B[pos];

        h_C_Ref[vec] = (float)sum;
*/

if (PAPI_read(EventSet, values) != PAPI_OK)
{
    printf("Error: PAPI_read\n");
    exit(1);
}
	printf("Thread %d in %d threads group on core %d work on %d L2_ICMM %lld\n",omp_get_thread_num(), omp_get_num_threads(), sched_getcpu(), vec, values[0]);
if (PAPI_reset(EventSet) != PAPI_OK)
{
    printf("Error: PAPI_reset\n");
    exit(1);
}
    }

}

    free(h_C_Ref);
    free(h_B);
    free(h_A);

}
