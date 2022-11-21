#include <stdio.h>
#include <time.h>
#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>
#define DATA_SIZE 1073741824

int main(int argc, char *argv[]) {
	unsigned char *host_array = NULL;
	struct timespec tv1, tv2;
	size_t size;
	char *device_name;
	int nof_platform = -1, nof_device = -1;
	cl_int ret;
	cl_platform_id *platform_ids = NULL;
	cl_device_id *device_ids = NULL;
	cl_uint ret_num_platforms;
	cl_uint ret_num_devices;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_mem device_array = NULL;
	
	if(argc != 3) {
		printf("Usage: ./bandwidth_test platform_number device_number\n");
		goto error;
	}
	nof_platform = atoi(argv[1]);
	nof_device = atoi(argv[2]);
	printf("Selected platform, device: %d,%d\n", nof_platform, nof_device);
	
	host_array = malloc(DATA_SIZE * sizeof(char));
	
	ret = clGetPlatformIDs(0, NULL, &ret_num_platforms);
	if (ret != CL_SUCCESS) {
		printf("Failed to get platform ID.\n");
		goto error;
	}
	if(ret_num_platforms <= nof_platform) {
		printf("Invalid platform number.\n");
		goto error;
	}
	printf("%d platform(s) available.\n", ret_num_platforms);
	platform_ids = (cl_platform_id *) malloc(sizeof(cl_platform_id) * ret_num_platforms);
	ret = clGetPlatformIDs(ret_num_platforms, platform_ids, NULL);
	if (ret != CL_SUCCESS) {
		printf("Failed to get platform ID.\n");
		goto error;
	}
	
	ret = clGetDeviceIDs(0, CL_DEVICE_TYPE_DEFAULT, 1, NULL, &ret_num_devices);
	if (ret != CL_SUCCESS) {
		printf("Failed to get device ID.\n");
		goto error;
	}
	if(ret_num_devices <= nof_device) {
		printf("Invalid device number.\n");
		goto error;
	}
	printf("%d device(s) available.\n", ret_num_devices);
	device_ids = (cl_device_id *) malloc(sizeof(cl_device_id) * ret_num_devices);
	ret = clGetDeviceIDs(platform_ids[nof_platform], CL_DEVICE_TYPE_DEFAULT, 1, device_ids, &ret_num_devices);
	if (ret != CL_SUCCESS) {
		printf("Failed to get device ID.\n");
		goto error;
	}
	
	ret |= clGetDeviceInfo(device_ids[nof_device], CL_DEVICE_NAME, 0, NULL, &size);
	device_name = (char *) malloc(sizeof(char) * size);
	ret |= clGetDeviceInfo(device_ids[nof_device], CL_DEVICE_NAME, size, device_name, NULL);
	printf("Selected device name: %s\n", device_name);
	
	context = clCreateContext(NULL, 1, &device_ids[nof_device], NULL, NULL, &ret);
	if (ret != CL_SUCCESS) {
		printf("Failed to create OpenCL context.\n");
		goto error;
	}
	command_queue = clCreateCommandQueue(context, device_ids[nof_device], 0, &ret);
	if (ret != CL_SUCCESS) {
		printf("Failed to create command queue %d\n", (int) ret);
		goto error;
	}
	
	device_array = clCreateBuffer(context, CL_MEM_READ_WRITE, DATA_SIZE * sizeof(char), NULL, &ret);
	clock_gettime(CLOCK_MONOTONIC_RAW, &tv1);
	ret |= clEnqueueWriteBuffer(command_queue, device_array, CL_TRUE, 0, DATA_SIZE * sizeof(void), (void *)host_array, 0, NULL, NULL);
	clock_gettime(CLOCK_MONOTONIC_RAW, &tv2);
	printf ("Transfered %d GiB\nBandwidth measured: %.5g MiB/sec\n", DATA_SIZE / 1024 / 1024 / 1024, DATA_SIZE / ((double) (tv2.tv_nsec - tv1.tv_nsec) / 1000000000.0 + (double) (tv2.tv_sec - tv1.tv_sec)) / 1048576);
	if (ret != CL_SUCCESS) {
		printf("Failed to copy date from host to device: %d\n", (int) ret);
		goto error;
	}	
	
	free(host_array);
	free(device_name);
	free(platform_ids);
	free(device_ids);
error:

	clFlush(command_queue);
	clFinish(command_queue);
	
	clReleaseMemObject(device_array);

	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);

	return 0;
}
