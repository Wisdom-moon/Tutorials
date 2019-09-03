__kernel void vadd (__global const float *a, 
	__global float *b,
 	__global float *result, 
	unsigned int n) {

  int id = get_global_id (0);
  if (id < n) 
  {
    result[id] = a[id] + b[id];
    //printf("%d\n", id);
    //result[id] = id;
  }
}
