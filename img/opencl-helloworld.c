/* helloworld.c - Hello World program for OpenCL, modified from an example from the web
 * Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 * Sun, 19 Dec 2010 01:35:33 -0500
 */

#include "helper.h"

const char *KernelSource =
"__kernel square(__global float* input,    \n" \
"                __global float* output,   \n" \
"                const unsigned int count) \n" \
"{                                         \n" \
"   int i = get_global_id(0);              \n" \
"   if (i < count)                         \n" \
"       output[i] = input[i] * input[i];   \n" \
"}                                         \n";

int main(int argc, char** argv)
{
	const int count = 4096;		// Size of data
	float data[count];		// Input data set
	float results[count];		// Output data set

	cl_device_id device_id;		// Compute device
	cl_context context;		// Compute context
	cl_command_queue commands;	// Compute command queue
	cl_program program;		// Compute program
	cl_kernel kernel;		// Compute kernel

	cl_mem input;			// Device memory used for the input array
	cl_mem output;			// Device memory used for the output array

	/*** Fill input data with random float values ***/
	for(int i = 0; i < count; i++)
		data[i] = rand() / (float)RAND_MAX;

	/*** Prepare a compute device ***/
	const int gpu = 1;
	printf("Connecting to the 1st %s device\n", gpu ? "GPU" : "CPU");
	CL_WRAPPER( clGetDeviceIDs(NULL, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL) );
	printf("Creating context on the compute device\n");
	CL_ASSIGN( context = clCreateContext(0, 1, &device_id, NULL, NULL, &err) );

	const int do_profiling = 0;
	cl_command_queue_properties qprop = 0;
	if (do_profiling) {
		qprop |= CL_QUEUE_PROFILING_ENABLE;
	}
	printf("Creating command queue %s profiling\n", do_profiling ? "with" : "without");
	CL_ASSIGN( commands = clCreateCommandQueue(context, device_id, qprop, &err) );

	/*** Prepare kernel code ***/
	printf("Making program object with one kernel:\n%s", KernelSource);
	CL_ASSIGN( program = clCreateProgramWithSource(context, 1, (const char**)&KernelSource, NULL, &err) );

	printf("Compiling and linking program executable\n");
	char const *options = NULL;
	err = clBuildProgram(program, 0, NULL, options, NULL, NULL);
	if (err != CL_SUCCESS) {
		size_t len;
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
		char* buffer = (char*) malloc(len);
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		printf("%s\n", buffer);
		printf("Error %d: Failed to build program executable (%s)\n", err, cl_error_to_str(err));
		free(buffer);
		abort();
	}

	/*** Preparing kernel function and I/O memory ***/
	char* kernel_name = "square";  // name of the function in the kernel code
	printf("Extracting the __kernel function named `%s'\n", kernel_name);
	CL_ASSIGN( kernel = clCreateKernel(program, kernel_name, &err) );

	printf("Preparing input and output memory in device\n");
	CL_ASSIGN( input = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * count, NULL, NULL) );
	CL_ASSIGN( output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * count, NULL, NULL) );

	printf("Transferring data to input memory in device\n");
	CL_WRAPPER( clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, sizeof(float) * count, data, 0, NULL, NULL) );

	/*** Prepare to run: Set arguments, set workgroup size ***/
	printf("Setting arguments to kernel\n");
	CL_WRAPPER(clSetKernelArg(kernel, 0, sizeof(cl_mem), &input) |
		   clSetKernelArg(kernel, 1, sizeof(cl_mem), &output) |
		   clSetKernelArg(kernel, 2, sizeof(unsigned int), &count)
	);

	size_t global, local; // Size of context and size of workgroup
	CL_WRAPPER( clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL) );
	printf("Setting the kernel to run on maximum workgroup size %lu over a global data set of size %u\n", local, count);

	/*** Run the kernel ***/
	global = count;
	CL_WRAPPER( clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL) );
	printf("Kernel running\n");
	clFinish(commands); // Wait for the command queue finish

	/*** Retrieve output from device memory, and verify result ***/
	printf("Retrieving output data from device\n");
	CL_WRAPPER( clEnqueueReadBuffer(commands, output, CL_TRUE, 0, sizeof(float) * count, results, 0, NULL, NULL ) ); 

	printf("Validating results\n");
	int correct = 0;
	for (int i = 0; i < count; i++) {
		if (results[i] == data[i] * data[i])
			correct++;
	}
	printf("Computed '%d/%d' correct values!\n", correct, count);

	/*** Clean up ***/
	clReleaseMemObject(input);
	clReleaseMemObject(output);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);

	return 0;
}

