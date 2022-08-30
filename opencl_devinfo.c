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
	unsigned int max_constant_args;
	unsigned long max_constant_buffer_size;
	unsigned long max_mem_alloc_size;
	size_t max_parameter_size;
	char *device_extensions;
	char *opencl_version;
	char *device_name;
	char *driver_version;
	char *vendor;
} devinfoT;

typedef struct platinfo {
	devinfoT *devinfo;
} platinfoT;

int main(int argc, char *argv[]) {
	cl_int ret;
	cl_platform_id *platform_ids = NULL;
	cl_device_id *device_ids = NULL;
	cl_uint ret_num_platforms, ret_num_devices;
	platinfoT *platinfo = NULL;
	size_t size;
	int i, j;
	
	ret = clGetPlatformIDs(0, NULL, &ret_num_platforms);
	if (ret != CL_SUCCESS) {
		printf("Failed to get platform ID.\n");
		goto error;
	}
	printf("%d platform(s) available.\n", ret_num_platforms);
	platform_ids = (cl_platform_id *) malloc(sizeof(cl_platform_id) * ret_num_platforms);
	platinfo = (platinfoT *) malloc(sizeof(platinfo) * ret_num_platforms);
	ret = clGetPlatformIDs(ret_num_platforms, platform_ids, NULL);
	if (ret != CL_SUCCESS) {
		printf("Failed to get platform ID.\n");
		goto error;
	}
	
	for(i = 0; i < ret_num_platforms; i++) {
		printf("Platform %d\n", i);
		
		ret = clGetDeviceIDs(0, CL_DEVICE_TYPE_DEFAULT, 1, NULL, &ret_num_devices);
		if (ret != CL_SUCCESS) {
			printf("Failed to get device ID.\n");
			goto error;
		}
		printf("  %d device(s) available.\n", ret_num_devices);
		platinfo[i].devinfo = (devinfoT *) malloc(sizeof(devinfoT) * ret_num_devices);
		device_ids = (cl_device_id *) malloc(sizeof(cl_device_id) * ret_num_devices);
		ret = clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_DEFAULT, 1, device_ids, &ret_num_devices);
		if (ret != CL_SUCCESS) {
			printf("Failed to get device ID.\n");
			goto error;
		}
		
		for(j = 0; j < ret_num_devices; j++) {
			printf("  Device %d\n", j);	
			ret = clGetDeviceInfo(device_ids[j], CL_DEVICE_ADDRESS_BITS, sizeof(unsigned int), &platinfo[i].devinfo[j].address_bits, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_ERROR_CORRECTION_SUPPORT, sizeof(unsigned int), &platinfo[i].devinfo[j].error_correction_support, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_ENDIAN_LITTLE, sizeof(unsigned int), &platinfo[i].devinfo[j].little_endian, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(unsigned long), &platinfo[i].devinfo[j].global_mem_cache_size, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(unsigned int), &platinfo[i].devinfo[j].global_mem_cacheline_size, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(unsigned long), &platinfo[i].devinfo[j].global_mem_size, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(unsigned int), &platinfo[i].devinfo[j].max_clock_freq, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(unsigned int), &platinfo[i].devinfo[j].compute_units, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(unsigned int), &platinfo[i].devinfo[j].max_constant_args, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(unsigned long), &platinfo[i].devinfo[j].max_constant_buffer_size, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(unsigned long), &platinfo[i].devinfo[j].max_mem_alloc_size, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(size_t), &platinfo[i].devinfo[j].max_parameter_size, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_NAME, 0, NULL, &size);
			platinfo[i].devinfo[j].device_name = (char *) malloc(sizeof(char) * size);
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_NAME, size, platinfo[i].devinfo[j].device_name, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_VERSION, 0, NULL, &size);
			platinfo[i].devinfo[j].opencl_version = (char *) malloc(sizeof(char) * size);
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_VERSION, size, platinfo[i].devinfo[j].opencl_version, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_VENDOR, 0, NULL, &size);
			platinfo[i].devinfo[j].vendor = (char *) malloc(sizeof(char) * size);
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_VENDOR, size, platinfo[i].devinfo[j].vendor, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DRIVER_VERSION, 0, NULL, &size);
			platinfo[i].devinfo[j].driver_version = (char *) malloc(sizeof(char) * size);
			ret |= clGetDeviceInfo(device_ids[j], CL_DRIVER_VERSION, size, platinfo[i].devinfo[j].driver_version, NULL);
			
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_EXTENSIONS, 0, NULL, &size);
			platinfo[i].devinfo[j].device_extensions = (char *) malloc(sizeof(char) * size);
			ret |= clGetDeviceInfo(device_ids[j], CL_DEVICE_EXTENSIONS, size, platinfo[i].devinfo[j].device_extensions, NULL);
			
			if (ret != CL_SUCCESS) {
				printf("Failed to get device info.\n");
				goto error;
			}
			
			printf("    OpenCL vendor = %s\n", platinfo[i].devinfo[j].vendor);
			printf("    Device name: %s\n", platinfo[i].devinfo[j].device_name);
			printf("    OpenCL driver version: %s\n", platinfo[i].devinfo[j].driver_version);
			printf("    OpenCL version: %s\n", platinfo[i].devinfo[j].opencl_version);
			
			printf("    Default address space size: %d bits\n", platinfo[i].devinfo[j].address_bits);
			printf("    Is little endian: %d\n", platinfo[i].devinfo[j].little_endian);
			printf("    Has error correction support: %d\n", platinfo[i].devinfo[j].error_correction_support);
			
			printf("    Global memory cache size: %ld bytes\n", platinfo[i].devinfo[j].global_mem_cache_size);
			printf("    Global memory cache line size: %d bytes\n", platinfo[i].devinfo[j].global_mem_cacheline_size);
			printf("    Global device memory size: %ld bytes\n", platinfo[i].devinfo[j].global_mem_size);
			printf("    Maximum configured clock frequency: %d MHz\n", platinfo[i].devinfo[j].max_clock_freq);
			printf("    Compute cores: %d\n", platinfo[i].devinfo[j].compute_units);
			
			printf("    Max number of constant arguments: %d\n", platinfo[i].devinfo[j].max_constant_args);
			printf("    Max constant buffer allocation: %ld bytes\n", platinfo[i].devinfo[j].max_constant_buffer_size);
			printf("    Max object memory allocation: %ld bytes\n", platinfo[i].devinfo[j].max_mem_alloc_size);
			printf("    Max argument size: %ld bytes\n", platinfo[i].devinfo[j].max_parameter_size);
			
			printf("    Supported extensions: %s\n", platinfo[i].devinfo[j].device_extensions);
			printf("\n");
		}
	}
	
	for(i = 0; i < ret_num_platforms; i++) {
		for(j = 0; j < ret_num_devices; j++) {
			free(platinfo[i].devinfo[j].device_name);
			free(platinfo[i].devinfo[j].opencl_version);
			free(platinfo[i].devinfo[j].vendor);
			free(platinfo[i].devinfo[j].driver_version);
			free(platinfo[i].devinfo[j].device_extensions);
		}
	}
	
	error:
	
	return 0;
}
