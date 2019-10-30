CU_NAME = covariance
ALL: $(CU_NAME).cu
	/usr/local/cuda/bin/nvcc  -gencode=arch=compute_60,code=\"sm_60,compute_60\"  -m64 --compiler-options -fno-strict-aliasing  -I. -I/usr/local/cuda/include  -DUNIX -O2   -o $(CU_NAME).cu.o -c $(CU_NAME).cu
	g++ -O2 -c polybench.cpp -o polybench.o
	g++ -fPIC   -m64 -o ./$(CU_NAME) ./$(CU_NAME).cu.o polybench.o -L/usr/local/cuda/lib64 -lcudart -L/usr/local/cuda/lib64  -lcudart

clean:
	rm $(CU_NAME) $(CU_NAME).cu.o polybench.o
