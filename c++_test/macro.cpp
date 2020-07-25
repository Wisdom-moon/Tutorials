/*
Predefined Macros:
__DATE__  A string containing the current date
__FILE__  A string containing the file name
__LINE__  An integer representing the current line number
__STDC__  If follows ANSI standard C, then the value is a nonzero integer
__TIME__  A string containing the current date.
*/


#define stack(id)  __stack##id  //id = 0, __stack0
#define stack(id)  __stack#id  //id = 0, __stack "0"

extern int  __stack0;

#define cutilSafeCall(err)           __cudaSafeCall      (err, __FILE__, __LINE__)
inline void __cudaSafeCall (cudaError err) {
    if( cudaSuccess != err) {
                FPRINTF((stderr, "%s(%i) : cudaSafeCall() Runtime API error : %s.\n",
                file, line, cudaGetErrorString( err) ));
        exit(-1);
    }
}

int main ()
{
  int i = stack(0);

  return i;
}
