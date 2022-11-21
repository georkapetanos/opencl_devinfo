all:
	gcc -Wall -g opencl_devinfo.c -O3 -lOpenCL -o opencl_devinfo
	gcc -Wall -g bandwidth_test.c -O3 -lOpenCL -o bandwidth_test
	
release:
	gcc opencl_devinfo.c -O3 -lOpenCL -o opencl_devinfo
	gcc bandwidth_test.c -O3 -lOpenCL -o bandwidth_test
	strip --strip-unneeded opencl_devinfo bandwidth_test
	
clean:
	rm opencl_devinfo
	rm bandwidth_test
