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
  cl_mem deviceInput1;
  cl_mem deviceInput2;
  cl_mem deviceOutput;

  cl_platform_id cpPlatform; // OpenCL platform
  cl_device_id device_id;    // device ID
  cl_context context;        // context
  cl_command_queue queue;    // command queue
  cl_program program;        // program
  cl_kernel kernel;          // kernel

  if (argc > 1) {
    inputLength = atoi (argv[1]);
  }

  inputLengthBytes = inputLength * sizeof(float);

  cl_int clerr = CL_SUCCESS;
  
  //@@ Initialize the workgroup dimensions

  size_t localSize = 64;
  size_t globalSize = ceil(inputLength / (float) localSize) * localSize;
  
  //@@ Bind to platform
  
  clerr = clGetPlatformIDs(1, &cpPlatform, NULL);

  //@@ Get ID for the device
  
  //clerr = clGetDeviceIDs(cpPlatform[1], CL_DEVICE_TYPE_ACCELERATOR, 1, &device_id, NULL);
  clerr = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
 
  char deviceName[32];
  clerr = clGetDeviceInfo(device_id, CL_DEVICE_NAME, 32, deviceName, NULL);
  printf("[info] device name: %s\n", deviceName);
  //@@ Create a context
  
  context = clCreateContext(0, 1, &device_id, NULL, NULL, &clerr);
  
  //@@ Create a command queue

  queue = clCreateCommandQueue(context, device_id, 0, &clerr);
  
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
  
  deviceInput1 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR, inputLengthBytes, NULL, NULL);
  deviceInput2 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR, inputLengthBytes, NULL, NULL);
  deviceOutput = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_ALLOC_HOST_PTR, inputLengthBytes, NULL, NULL);

  hostInput1 = (float *)clEnqueueMapBuffer(queue, deviceInput1, CL_TRUE, CL_MAP_WRITE, 0, inputLengthBytes, 0, NULL, NULL, NULL);
  hostInput2 = (float *)clEnqueueMapBuffer(queue, deviceInput2, CL_TRUE, CL_MAP_WRITE, 0, inputLengthBytes, 0, NULL, NULL, NULL);
  devOutput = (float *)clEnqueueMapBuffer(queue, deviceOutput, CL_TRUE, CL_MAP_WRITE, 0, inputLengthBytes, 0, NULL, NULL, NULL);
  hostOutput       = (float *)malloc(inputLengthBytes);
  for(int i=0; i<inputLength; i++)
  {
    hostInput1[i] = (float)(i % 11);
    hostInput2[i] = (float)(i % 31);
    hostOutput[i] = hostInput1[i] + hostInput2[i];
  }

  //@@ Write data set into the input array in device memory
  
  clerr = clEnqueueWriteBuffer(queue, deviceInput1, CL_TRUE, 0, inputLengthBytes, hostInput1, 0, NULL, NULL);
  clerr |= clEnqueueWriteBuffer(queue, deviceInput2, CL_TRUE, 0, inputLengthBytes, hostInput2, 0, NULL, NULL);
  clFinish(queue);

  //@@ Set the arguments to compute kernel

  clerr = clSetKernelArg(kernel, 0, sizeof(cl_mem), &deviceInput1);
  clerr |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &deviceInput2);
  clerr |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &deviceOutput);
  clerr |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &inputLength);
  printf("[info] set args done!\n");
  
  //@@ Execute the kernel over the entire range of the data set
  
  clerr = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL);

  //@@ Wait for the command queue to get serviced before reading back results
  
  clFinish(queue);
  printf("[info] kernel executed done!\n"); 

  //@@ Read the results from the device

  //clEnqueueReadBuffer(queue, deviceInput2, CL_FALSE, 0, inputLengthBytes, devOutput, 0, NULL, NULL);
  clEnqueueReadBuffer(queue, deviceOutput, CL_FALSE, 0, inputLengthBytes, devOutput, 0, NULL, NULL);
  clFinish(queue);
  

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

  clEnqueueUnmapMemObject(queue, deviceInput1, hostInput1, 0, NULL, NULL);
  clEnqueueUnmapMemObject(queue, deviceInput2, hostInput2, 0, NULL, NULL);
  clEnqueueUnmapMemObject(queue, deviceOutput, hostOutput, 0, NULL, NULL);
  // release OpenCL resources
  clReleaseMemObject(deviceInput1);
  clReleaseMemObject(deviceInput2);
  clReleaseMemObject(deviceOutput);
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(queue);
  clReleaseContext(context);

  // release host memory
  if(hostOutput != NULL) free(hostOutput);
  if(source != NULL) free(source);

  return 0;
}
