#!/bin/bash
nvcc -arch=sm_61 -o HelloWorld Source/HelloWorld.cu
#nvcc -arch=sm_86 -o HelloWorld Source/HelloWorld.cu

#-arch=sm_61 比 -arch=compute_61快, 为什么?