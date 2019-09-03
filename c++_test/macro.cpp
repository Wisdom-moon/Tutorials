#define cutilSafeCall(err)           __cudaSafeCall      (err, __FILE__, __LINE__)
inline void __cudaSafeCall (cudaError err) {
    if( cudaSuccess != err) {
                FPRINTF((stderr, "%s(%i) : cudaSafeCall() Runtime API error : %s.\n",
                file, line, cudaGetErrorString( err) ));
        exit(-1);
    }
}

