/* checkdevice.c - Code to check details of an OpenCL device
 * Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 * Sun, 19 Dec 2010 01:35:33 -0500
 */

#include "helper.h"

int main(int argc, char** argv)
{
	cl_uint platform_count;		// Number of platforms
	cl_uint device_count;		// Number of devices
	cl_uint totaldevices;		// Total number of devices

	/*** Detecting platforms and print platform info ***/
	CL_WRAPPER( clGetPlatformIDs(0, NULL, &platform_count) );
	printf("There are %d platforms found\n", platform_count);
	cl_platform_id* platforms = (cl_platform_id*) malloc(platform_count*sizeof(cl_platform_id));
	CL_WRAPPER( clGetPlatformIDs(platform_count, platforms, NULL) );
	for (cl_uint i = 0; i < platform_count; ++i) {
		char buf[1024]; // can't be too small or the list of extensions don't fit
		CL_WRAPPER( clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, sizeof(buf), buf, NULL) );
		printf("  Platform[%d] profile:    %s\n", i, buf);
		CL_WRAPPER( clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, sizeof(buf), buf, NULL) );
		printf("  Platform[%d] version:    %s\n", i, buf);
		CL_WRAPPER( clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(buf), buf, NULL) );
		printf("  Platform[%d] name:       %s\n", i, buf);
		CL_WRAPPER( clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(buf), buf, NULL) );
		printf("  Platform[%d] vendor:     %s\n", i, buf);
		CL_WRAPPER( clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, sizeof(buf), buf, NULL) );
		printf("  Platform[%d] extensions: %s\n", i, buf);
		/*** Detecting devices ***/
		CL_WRAPPER( clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &totaldevices) );
		printf("Platform[%d] has %d devices", i, totaldevices);
		CL_WRAPPER( clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_CPU, 0, NULL, &device_count) );
		printf(" (%d CPU", device_count);
		CL_WRAPPER( clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL, &device_count) );
		printf(", %d GPU", device_count);
		CL_WRAPPER( clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ACCELERATOR, 0, NULL, &device_count) );
		printf(", %d accelerator)\n", device_count);
		cl_device_id *devices = (cl_device_id*) malloc(totaldevices * sizeof(cl_device_id));
		CL_WRAPPER( clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, totaldevices, devices, NULL) );
		for (cl_uint j = 0; j < totaldevices; ++j) {
			char buf[1024]; // can't be too small or the list of extensions don't fit
			long long val;
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_TYPE, sizeof(val), &val, NULL) );
			printf("  Device %d type: 0x%llx", j, val );
			if (val & CL_DEVICE_TYPE_DEFAULT) printf(" default");
			if (val & CL_DEVICE_TYPE_CPU) printf(" CPU");
			if (val & CL_DEVICE_TYPE_GPU) printf(" GPU");
			if (val & CL_DEVICE_TYPE_ACCELERATOR) printf(" accelerator");
			printf("\n");
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(buf), buf, NULL) );
			printf("  Device %d name: %s by ", j, buf);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_VENDOR, sizeof(buf), buf, NULL) );
			printf("%s (", buf);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_VENDOR_ID, sizeof(val), &val, NULL) );
			printf("0x%llx)\n", val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, sizeof(buf), buf, NULL) );
			printf("  Device %d driver version: %s\n", j, buf);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_PROFILE, sizeof(buf), buf, NULL) );
			printf("  Device %d profile: %s\n", j, buf);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, sizeof(buf), buf, NULL) );
			printf("  Device %d version: %s\n", j, buf);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, sizeof(buf), buf, NULL) );
			printf("  Device %d extensions: %s\n", j, buf);

			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_EXECUTION_CAPABILITIES, sizeof(val), &val, NULL) );
			printf("  Device %d execution capabilities: 0x%llx", j, val );
			if (val & CL_EXEC_KERNEL) printf(" kernel");
			if (val & CL_EXEC_NATIVE_KERNEL) printf(" native");
			printf("\n");
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, sizeof(val), &val, NULL) );
			printf("  Device %d global mem cache type: 0x%llx", j, val );
			if (val == 0) printf(" None");
			if (val == 1) printf(" Read-only");
			if (val == 2) printf(" Read-write");
			printf("\n");
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_TYPE, sizeof(val), &val, NULL) );
			printf("  Device %d local mem type: 0x%llx", j, val );
			if (val == 1) printf(" Local");
			if (val == 2) printf(" Global");
			printf("\n");

			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_SINGLE_FP_CONFIG, sizeof(val), &val, NULL) );
			printf("  Device %d single FP config: 0x%llx\n", j, val );
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_DOUBLE_FP_CONFIG, sizeof(val), &val, NULL) );
			printf("  Device %d double FP config: 0x%llx\n", j, val );
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_QUEUE_PROPERTIES, sizeof(val), &val, NULL) );
			printf("  Device %d queue properties: 0x%llx\n", j, val );

			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(val), &val, NULL) );
			printf("  Device %d max compute units: %lld\n", j, val );
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(val), &val, NULL) );
			printf("  Device %d max work group size: %lld\n", j, val );
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(val), &val, NULL) );
			printf("  Device %d max work item dimensions: (", j);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(long long)*val, buf, NULL) );
			for (int k=0; k<val; ++k) {
				if (k > 0) printf(", ");
				printf("%lld", ((long long*)buf)[k]);
			};
			printf(")\n");
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, sizeof(val), &val, NULL) );
			printf("  Device %d preferred vector width: char(%lld), ", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, sizeof(val), &val, NULL) );
			printf("short(%lld), ", val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, sizeof(val), &val, NULL) );
			printf("int(%lld), ", val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, sizeof(val), &val, NULL) );
			printf("long(%lld), ", val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, sizeof(val), &val, NULL) );
			printf("float(%lld), ", val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, sizeof(val), &val, NULL) );
			printf("double(%lld)\n", val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_ADDRESS_BITS, sizeof(val), &val, NULL) );
			printf("  Device %d address bits: %lld\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_IMAGE_SUPPORT, sizeof(val), &val, NULL) );
			printf("  Device %d image support: %lld\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(val), &val, NULL) );
			printf("  Device %d max clock frequency: %lld MHz\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(val), &val, NULL) );
			printf("  Device %d max malloc size: %lld\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_MAX_READ_IMAGE_ARGS, sizeof(val), &val, NULL) );
			printf("  Device %d max read image args: %lld\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WRITE_IMAGE_ARGS, sizeof(val), &val, NULL) );
			printf("  Device %d max write image args: %lld\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_IMAGE2D_MAX_WIDTH, sizeof(val), &val, NULL) );
			printf("  Device %d max width * height for 2D image: %lld * ", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_IMAGE2D_MAX_HEIGHT, sizeof(val), &val, NULL) );
			printf("%lld\n", val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_IMAGE3D_MAX_WIDTH, sizeof(val), &val, NULL) );
			printf("  Device %d max width * height * depth for 3D image: %lld * ", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_IMAGE3D_MAX_HEIGHT, sizeof(val), &val, NULL) );
			printf("%lld * ", val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_IMAGE3D_MAX_DEPTH, sizeof(val), &val, NULL) );
			printf("%lld\n", val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_MAX_SAMPLERS, sizeof(val), &val, NULL) );
			printf("  Device %d max samplers: %lld\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(val), &val, NULL) );
			printf("  Device %d max parameter size: %lld\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_MEM_BASE_ADDR_ALIGN, sizeof(val), &val, NULL) );
			printf("  Device %d mem base addr align: %lld\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, sizeof(val), &val, NULL) );
			printf("  Device %d min data type align size: %lld\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(val), &val, NULL) );
			printf("  Device %d global mem: size %lld bytes, ", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(val), &val, NULL) );
			printf("cache size %lld bytes, ", val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(val), &val, NULL) );
			printf("cacheline size %lld bytes\n", val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(val), &val, NULL) );
			printf("  Device %d max constant buffer size: %lld\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(val), &val, NULL) );
			printf("  Device %d max constant args: %lld\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(val), &val, NULL) );
			printf("  Device %d local mem size: %lld bytes\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_ERROR_CORRECTION_SUPPORT, sizeof(val), &val, NULL) );
			printf("  Device %d error correction support: %lld\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_PROFILING_TIMER_RESOLUTION, sizeof(val), &val, NULL) );
			printf("  Device %d profiling timer resolution: %lld\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_ENDIAN_LITTLE, sizeof(val), &val, NULL) );
			printf("  Device %d little endian: %lld\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_COMPILER_AVAILABLE, sizeof(val), &val, NULL) );
			printf("  Device %d compiler available: %lld\n", j, val);
			CL_WRAPPER( clGetDeviceInfo(devices[j], CL_DEVICE_AVAILABLE, sizeof(val), &val, NULL) );
			printf("  Device %d available: %lld\n", j, val);
		};
		free(devices);
	};
	free(platforms);

	return 0;
}

