#include <stddef.h>
#include <stdint.h>

struct pocl_context {
  uint32_t work_dim;
  size_t num_groups[3];
  size_t group_id[3];
  size_t global_offset[3];
  size_t local_size[3];
};

/*
//_pocl_launcher_%s_workgroup is the default entry
void _pocl_launcher_vadd_kernel_workgroup(void** args, struct pocl_context* pc)
{
  float * a = *(float **)args[0];
  float * b = *(float **)args[1];
  float * result = *(float **)args[2];
  unsigned int n = *(unsigned int *)args[3];
  int id_offset = pc->global_offset[0] + pc->local_size[0] * pc->group_id[0];
  int i;
  if (id_offset + 63 < n)
  {
    for (i = id_offset; i < id_offset + 64; i++)
    {
      result[i] = a[i] + b[i];
    }
  }
  else
  {
    for (i = id_offset; i < n; i++)
    {
      result[i] = a[i] + b[i];
    }
  }
}
*/
void _pocl_launcher_vadd_kernel_workgroup(void** args, struct pocl_context* pc)
{
  float * a = *(float **)args[0];
  float * b = *(float **)args[1];
  float * result = *(float **)args[2];
  unsigned int n = *(unsigned int *)args[3];
  int id_offset = pc->global_offset[0] + pc->local_size[0] * pc->group_id[0];
  int num = n - id_offset;
  num = num > 64 ? 64 : num;
  
  int i ;
  for (i = id_offset; i < id_offset + num; i ++)
  {
    result[i] = a[i] + b[i];
  }
}
