#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

#include <hStreams_source.h>
#include <hStreams_app_api.h>
#include <common/COIMacros_common.h>

static HSTR_OPTIONS hstreams_options;

#define SWITCH_CHAR '-'

void FillVec (float * vec, int length)
{
  int i;
  const float fScale = 1.0f / (float) RAND_MAX;
  for (i = 0; i < length; i++)
  {
    vec[i] = fScale * rand();
  }
}

int main( int argc, char **argv)
{
  uint32_t places_per_domain = 1;
  uint32_t streams_per_place = 1;
  int stream = 0;
  int MAXLOGSTR = places_per_domain;
  int tasks = 1;
  int i;
  int length = 888866;

  argc --;
  argv++;
  while (argc)
  {
    if (*argv[0] == SWITCH_CHAR)
    {
      switch (*(argv[0]+1))
      {
        case 'p':
          places_per_domain= (int)atol(argv[0]+2);
          MAXLOGSTR=places_per_domain;
          break;
        case 't':
          tasks = (int) atol(argv[0]+2);
          break;
        case 'n':
          length = (int) atol(argv[0]+2);
          break;
        default:
          break;

      }
    }
    argc --;
    argv++;
  }
  hStreams_GetCurrentOptions(&hstreams_options, sizeof(hstreams_options));
  hstreams_options.verbose = 0;
  hstreams_options.phys_domains_limit = 256;
  char *libNames[20] = {NULL, NULL};
  unsigned int libNameCnt = 0;
  libNames[libNameCnt++] = "kernel.so";
  hstreams_options.libNames = libNames;
  hstreams_options.libNameCnt = (uint16_t) libNameCnt;
  hStreams_SetOptions (&hstreams_options);

  int iret = hStreams_app_init (places_per_domain, streams_per_place);
  if (iret != 0)
  {
    printf ("hstreams_app_init failed!\n");
    exit (-1);
  }

  float a = 0.5f;

  float *vecA = (float *) malloc (sizeof(float) * length);
  float *vecB = (float *) malloc (sizeof(float) * length);
  float *vecC = (float *) malloc (sizeof(float) * length);

  FillVec (vecA, length);
  FillVec (vecB, length);

//Compute the reference result.
#pragma omp parallel for
  for (int j = 0; j < length; j++)
    vecC[j] = vecA[j] * a * vecB[j];

  uint64_t arg[5];
  arg[2] = (uint64_t) *((uint64_t *) (&a));
  arg[3] = (uint64_t) vecA;
  arg[4] = (uint64_t) vecB;


  CHECK_HSTR_RESULT (hStreams_app_create_buf (vecA, sizeof(float) * length));
  CHECK_HSTR_RESULT (hStreams_app_create_buf (vecB, sizeof(float) * length));
  hStreams_ThreadSynchronize ();

  tasks = (length > tasks) ? tasks : length;
  int sub_block = length / tasks;
  int remain_index = length % tasks;
  int start_id = 0;
  int end_id = 0;

  for (int i = 0; i < tasks; i++)
  {    
    arg[0] = (uint64_t) start_id;
    end_id = start_id + sub_block;
    if ( i < remain_index)
      end_id ++;
    arg[1] = (uint64_t) end_id;

    CHECK_HSTR_RESULT (hStreams_app_xfer_memory (&vecB[start_id], &vecB[start_id], sizeof(float) * (end_id - start_id), i % MAXLOGSTR, HSTR_SRC_TO_SINK, NULL));
    CHECK_HSTR_RESULT (hStreams_app_xfer_memory (&vecA[start_id], &vecA[start_id], sizeof(float) * (end_id - start_id), i % MAXLOGSTR, HSTR_SRC_TO_SINK, NULL));
    CHECK_HSTR_RESULT (hStreams_EnqueueCompute (i % MAXLOGSTR, "kernel", 3, 2, arg, NULL, NULL, 0));

    start_id = end_id;
  }
  hStreams_ThreadSynchronize ();
  CHECK_HSTR_RESULT (hStreams_app_xfer_memory (vecB, vecB, sizeof(float) * length, 0, HSTR_SINK_TO_SRC, NULL));
  hStreams_ThreadSynchronize ();
  hStreams_app_fini ();

//Verify the results.
  for (int j = 0; j < length; j++)
    if (fabs(vecC[j] - vecB[j]) > 1.0e-5)
      printf("Result error: Ref=%f, Dev=%f\n", vecC[j], vecB[j]);

  free (vecA);
  free (vecB);
  free (vecC);
  return 0;
}

