#include <CL/opencl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int inputLength = 32;
  int inputLengthBytes;
  float *hostInput1;
  float *hostInput2;
  float *hostOutput;
  float *devOutput;

  int partitions = 1;
  int blocks = 1;
  cl_mem deviceInput1;
  cl_mem deviceInput2;
  cl_mem deviceOutput;
  cl_uint max_cus, max_subs;

  cl_platform_id cpPlatform; // OpenCL platform
  cl_device_id device_id;    // device ID
  cl_context context;        // context
  cl_command_queue * ptr_queue;    // command queue
  cl_program program;        // program
  cl_kernel kernel;          // kernel

  if (argc > 1) {
    inputLength = atoi (argv[1]);
  }
  if (argc > 3) {
    partitions = atoi (argv[2]);
    blocks = atoi (argv[3]);
  }

  inputLengthBytes = inputLength * sizeof(float);
  hostInput1 = (float *)malloc(inputLengthBytes);
  hostInput2 = (float *)malloc(inputLengthBytes);
  hostOutput       = (float *)malloc(inputLengthBytes);
  devOutput       = (float *)malloc(inputLengthBytes);
  for(int i=0; i<inputLength; i++)
  {
    hostInput1[i] = (float)(i % 11);
    hostInput2[i] = (float)(i % 31);
    hostOutput[i] = hostInput1[i] + hostInput2[i];
  }


  cl_int clerr = CL_SUCCESS;
  
  //@@ Initialize the workgroup dimensions

  
  //@@ Bind to platform
  
  clerr = clGetPlatformIDs(1, &cpPlatform, NULL);

  //@@ Get ID for the device
  
  clerr = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
 
  char deviceName[32];
  clerr = clGetDeviceInfo(device_id, CL_DEVICE_NAME, 32, deviceName, NULL);
  printf("[info] device name: %s\n", deviceName);
  clerr = clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(max_cus), &max_cus, NULL);
  printf("[info] Compute units: %d\n", max_cus);
  clerr = clGetDeviceInfo(device_id, CL_DEVICE_PARTITION_MAX_SUB_DEVICES, sizeof(max_subs), &max_subs, NULL);
  printf("[info] Max sub devices: %d\n", max_subs);
  if (partitions > max_subs)
    partitions = max_subs;
  //@@ Create Sub Devices 
  cl_device_id * sub_device_id = (cl_device_id *) malloc(partitions * sizeof(cl_device_id)); 
  cl_device_partition_property *count_splitter = (cl_device_partition_property *) malloc ((partitions + 3) * sizeof (cl_device_partition_property));;
  count_splitter[0] = CL_DEVICE_PARTITION_BY_COUNTS;
  count_splitter[partitions + 1] = CL_DEVICE_PARTITION_BY_COUNTS_LIST_END;
  count_splitter[partitions + 2] = 0;
  int sub_cus = max_cus / partitions;
  int ret_cus = max_cus % partitions;
  for (int i = 0; i < partitions; i ++) {
    count_splitter[i+1] = sub_cus;
    if ( i < ret_cus)
      count_splitter[i+1] ++;
  }
  clerr = clCreateSubDevices(device_id, count_splitter, partitions, sub_device_id, NULL);
  //@@ Create a context
  context = clCreateContext(0, partitions, sub_device_id, NULL, NULL, &clerr);
  
  //@@ Create a command queue
  ptr_queue = (cl_command_queue *) malloc( partitions * sizeof (cl_command_queue));
  for (int i = 0; i < partitions; i ++) {
    ptr_queue[i] = clCreateCommandQueue(context, sub_device_id[i], 0, &clerr);
  }
  
  //@@ Create the compute program from the source file 
  const char *filename = "kernel.cl";
  FILE *fid = fopen(filename, "r");
  size_t fileSize;
  if (fid) {
    fseek (fid, 0, SEEK_END);
    fileSize = (size_t)ftell(fid);
    fseek (fid, 0, SEEK_SET);
  }
  else {
    printf("Error: failed to open file: %s\n", filename);
    return -1;
  }
  char *source = (char *) malloc (sizeof (char) * fileSize);
  fread (source, sizeof (char), fileSize, fid);
  fclose (fid);

  program = clCreateProgramWithSource(context, 1, (const char **)&source, &fileSize, &clerr);
  
  //@@ Build the program executable

  clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  
  //@@ Create the compute kernel
  
  kernel = clCreateKernel(program, "vadd", &clerr);

  //@@ Create the input and output arrays in device memory for calculation
  
  deviceInput1 = clCreateBuffer(context, CL_MEM_READ_ONLY, inputLengthBytes, NULL, NULL);
  deviceInput2 = clCreateBuffer(context, CL_MEM_READ_ONLY, inputLengthBytes, NULL, NULL);
  deviceOutput = clCreateBuffer(context, CL_MEM_WRITE_ONLY, inputLengthBytes, NULL, NULL);

  size_t localSize = 2;
  //@@ Set kernel arguments
  clerr = clSetKernelArg(kernel, 0, sizeof(cl_mem), &deviceInput1);
  clerr |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &deviceInput2);
  clerr |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &deviceOutput);
  printf("[info] set args done!\n");
  
  int start_idx = 0;
  int end_idx = 0;
  int sub_idx = inputLength / blocks;
  int rem_idx = inputLength % blocks;
  for ( int i = 0; i < blocks; i++) {
    end_idx = start_idx + sub_idx;
    if ( i < rem_idx)
      end_idx ++;
    int sub_length = end_idx - start_idx;
    clerr = clEnqueueWriteBuffer(ptr_queue[i%partitions], deviceInput1, CL_TRUE, start_idx * sizeof(float), sub_length * sizeof (float), &hostInput1[start_idx], 0, NULL, NULL);
    clerr |= clEnqueueWriteBuffer(ptr_queue[i%partitions], deviceInput2, CL_TRUE, start_idx * sizeof(float), sub_length * sizeof (float), &hostInput2[start_idx], 0, NULL, NULL);
  
    //@@ Execute the kernel over the entire range of the data set
    
    size_t globalSize = ceil(sub_length/ (float) localSize) * localSize;
    size_t globaloffset = start_idx;
    clerr |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &end_idx);
    clerr = clEnqueueNDRangeKernel(ptr_queue[i%partitions], kernel, 1, &globaloffset, &globalSize, &localSize, 0, NULL, NULL);
  
    //@@ Read the results from the device
  
    clEnqueueReadBuffer(ptr_queue[i%partitions], deviceOutput, CL_FALSE, start_idx * sizeof(float), sub_length * sizeof (float), &devOutput[start_idx], 0, NULL, NULL);

    start_idx = end_idx;
  } 

  for ( int i = 0; i < partitions; i++) {
    clFinish(ptr_queue[i]);
  }

  // check
  bool pass = true;
  for(int i=0; i<inputLength; i++) {
    if (fabs(hostOutput[i] - devOutput[i]) > 0.000001) {
      printf("%dth element is diff, dev = %f, host = %f.\n", i, devOutput[i], hostOutput[i]);
      pass = false;
    }
  }
  if (pass)
    printf("Verification passed!\n");

  // release OpenCL resources
  clReleaseMemObject(deviceInput1);
  clReleaseMemObject(deviceInput2);
  clReleaseMemObject(deviceOutput);
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  for ( int i = 0; i < partitions; i ++)
    clReleaseCommandQueue(ptr_queue[i]);
  free(ptr_queue);
  clReleaseContext(context);
 
  free(sub_device_id);
  free(count_splitter);

  // release host memory
  if(hostInput1 != NULL) free(hostInput1);
  if(hostInput2 != NULL) free(hostInput2);
  if(hostOutput != NULL) free(hostOutput);
  if(devOutput != NULL) free(devOutput);
  if(source != NULL) free(source);

  return 0;
}
