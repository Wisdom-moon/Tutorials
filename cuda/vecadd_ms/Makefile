CU_NAME = vectorAdd
ALL: $(CU_NAME).cu
	/usr/local/cuda/bin/nvcc  -gencode=arch=compute_75,code=\"sm_75,compute_75\"  -m64 --compiler-options -fno-strict-aliasing  -I. -I/usr/local/cuda/include  -DUNIX -O2   -o $(CU_NAME).cu.o -c $(CU_NAME).cu
	g++ -fPIC   -m64 -o ./$(CU_NAME) ./$(CU_NAME).cu.o  -L/usr/local/cuda/lib64 -lcudart -L/usr/local/cuda/lib64  -lcudart

clean:
	rm $(CU_NAME) $(CU_NAME).cu.o
