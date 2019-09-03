#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

#include <hStreams_source.h>
#include <hStreams_app_api.h>
#include <common/COIMacros_common.h>

static HSTR_OPTIONS hstreams_options;

int main( int argc, char **argv)
{
  uint32_t places_per_domain = 7;
  uint32_t streams_per_place = 1;
  int stream = 0;
  int MAXLOGSTR = places_per_domain;
  int tasks = 1;
  int i;

  char * str = argv[1];
  int length = strlen (str);


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

  CHECK_HSTR_RESULT (hStreams_app_create_buf (str, sizeof(char) * length));
  hStreams_ThreadSynchronize ();

  uint64_t arg[2];
  arg[1] = (uint64_t) str;


  for (int i = 0; i < length; i++)
  {    
    arg[0] = (uint64_t) i;
    CHECK_HSTR_RESULT (hStreams_app_xfer_memory (&str[i], &str[i], sizeof(char), i % MAXLOGSTR, HSTR_SRC_TO_SINK, NULL));
    CHECK_HSTR_RESULT (hStreams_EnqueueCompute (i % MAXLOGSTR, "kernel", 1, 1, arg, NULL, NULL, 0));
  }
  hStreams_ThreadSynchronize ();
  hStreams_app_fini ();

  return 0;
}

