all:
	gcc -Wall -g opencl_devinfo.c -O3 -lOpenCL -o opencl_devinfo
	
release:
	gcc opencl_devinfo.c -O3 -lOpenCL -o opencl_devinfo
	strip --strip-unneeded opencl_devinfo
	
clean:
	rm opencl_devinfo
