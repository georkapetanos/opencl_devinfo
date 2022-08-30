#include <stdio.h>
#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>

#define MAX_SOURCE_SIZE 1048576
#define LOCAL_WORK_SIZE 128

typedef struct devinfo {
	unsigned int address_bits;
	unsigned int little_endian;
	unsigned int error_correction_support;
	unsigned long global_mem_cache_size;
	unsigned int global_mem_cacheline_size;
	unsigned long global_mem_size;
	unsigned int max_clock_freq;
	unsigned int compute_units;
	char *device_extensions;
	char *opencl_version;
	char *device_name;
	char *driver_version;
	char *vendor;
} devinfoT;

int main(int argc, char *argv[]) {
	cl_int ret;
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_platforms;
	cl_uint ret_num_devices;
	
	devinfoT devinfo;
	size_t size;
	
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	if (ret != CL_SUCCESS) {
		printf("Failed to get platform ID.\n");
		goto error;
	}
	printf("%d platform(s) available.\n", ret_num_platforms);
	
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
	if (ret != CL_SUCCESS) {
		printf("Failed to get device ID.\n");
		goto error;
	}
	printf("%d device(s) available.\n", ret_num_devices);
	
	ret = clGetDeviceInfo(device_id, CL_DEVICE_ADDRESS_BITS, sizeof(unsigned int), &devinfo.address_bits, NULL);
	
	ret = clGetDeviceInfo(device_id, CL_DEVICE_ERROR_CORRECTION_SUPPORT, sizeof(unsigned int), &devinfo.error_correction_support, NULL);
	
	ret = clGetDeviceInfo(device_id, CL_DEVICE_ENDIAN_LITTLE, sizeof(unsigned int), &devinfo.little_endian, NULL);
	
	ret = clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(unsigned long), &devinfo.global_mem_cache_size, NULL);
	
	ret = clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(unsigned int), &devinfo.global_mem_cacheline_size, NULL);
	
	ret = clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(unsigned long), &devinfo.global_mem_size, NULL);
	
	ret = clGetDeviceInfo(device_id, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(unsigned int), &devinfo.max_clock_freq, NULL);
	
	ret = clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(unsigned int), &devinfo.compute_units, NULL);
	
	ret = clGetDeviceInfo(device_id, CL_DEVICE_NAME, 0, NULL, &size);
	devinfo.device_name = (char *) malloc(sizeof(char) * size);
	ret = clGetDeviceInfo(device_id, CL_DEVICE_NAME, size, devinfo.device_name, NULL);
	
	ret = clGetDeviceInfo(device_id, CL_DEVICE_VERSION, 0, NULL, &size);
	devinfo.opencl_version = (char *) malloc(sizeof(char) * size);
	ret = clGetDeviceInfo(device_id, CL_DEVICE_VERSION, size, devinfo.opencl_version, NULL);
	
	ret = clGetDeviceInfo(device_id, CL_DEVICE_VENDOR, 0, NULL, &size);
	devinfo.vendor = (char *) malloc(sizeof(char) * size);
	ret = clGetDeviceInfo(device_id, CL_DEVICE_VENDOR, size, devinfo.vendor, NULL);
	
	ret = clGetDeviceInfo(device_id, CL_DRIVER_VERSION, 0, NULL, &size);
	devinfo.driver_version = (char *) malloc(sizeof(char) * size);
	ret = clGetDeviceInfo(device_id, CL_DRIVER_VERSION, size, devinfo.driver_version, NULL);
	
	ret = clGetDeviceInfo(device_id, CL_DEVICE_EXTENSIONS, 0, NULL, &size);
	devinfo.device_extensions = (char *) malloc(sizeof(char) * size);
	ret = clGetDeviceInfo(device_id, CL_DEVICE_EXTENSIONS, size, devinfo.device_extensions, NULL);
	
	printf("OpenCL vendor = %s\n", devinfo.vendor);
	printf("Device name: %s\n", devinfo.device_name);
	printf("OpenCL driver version: %s\n", devinfo.driver_version);
	printf("OpenCL version: %s\n", devinfo.opencl_version);
	
	printf("Default address space size: %d bits\n", devinfo.address_bits);
	printf("Is little endian: %d\n", devinfo.little_endian);
	printf("Has error correction support: %d\n", devinfo.error_correction_support);
	
	printf("Global memory cache size: %ld bytes\n", devinfo.global_mem_cache_size);
	printf("Global memory cache line size: %d bytes\n", devinfo.global_mem_cacheline_size);
	printf("Global device memory size: %ld bytes\n", devinfo.global_mem_size);
	printf("Maximum configured clock frequency: %d MHz\n", devinfo.max_clock_freq);
	printf("Compute cores: %d\n", devinfo.compute_units);
	
	printf("Supported extensions: %s\n", devinfo.device_extensions);
	
	free(devinfo.device_name);
	free(devinfo.opencl_version);
	free(devinfo.vendor);
	free(devinfo.driver_version);
	free(devinfo.device_extensions);
	
	error:
	
	return 0;
}
