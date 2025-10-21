#include <stdio.h>


int main() 
{
	cudaFree(0);

    CUcontext cuCtx = 0;  // zero means take the current context
    optixInit();
	printf("hello, optix\n");
	return 0;
}