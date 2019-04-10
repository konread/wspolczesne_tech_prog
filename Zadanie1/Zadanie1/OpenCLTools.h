#ifndef OPENCLTOOLS_H
#define OPENCLTOOLS_H

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <vector>
#include "CL/cl.hpp"

std::vector<cl::Device> retrieveDevices();

template<typename T>
std::vector<T> performAddProgram(cl::Device& device, const std::vector<T>& arrayA, const std::vector<T>& arrayB)
{
	cl::Context context({ device });

	std::string kernelCode = "void kernel add(global const " + std::string(typeid(T).name()) + "* arrayA,"
		"global const " + std::string(typeid(T).name()) + "* arrayB, global " + std::string(typeid(T).name()) + "* arrayC){"
		"arrayC[get_global_id(0)] = arrayA[get_global_id(0)] + arrayB[get_global_id(0)];"
		"}";

	cl::Program::Sources sources;
	sources.push_back({ kernelCode.c_str(),kernelCode.length() });

	cl::Program program(context, sources);
	if (program.build({ device }) != CL_SUCCESS) {
		std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << "\n";
		exit(1);
	}

	size_t arraySize = arrayA.size();

	// create buffers on the device
	cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(T) * arraySize);
	cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(T) * arraySize);
	cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(T) * arraySize);

	//create queue to which we will push commands for the device.
	cl::CommandQueue queue(context, device);

	//write arrays A and B to the device
	queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(T) * arraySize, arrayA.data());
	queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(T) * arraySize, arrayB.data());

	//run the kernel
	cl::Kernel kernel(program, "add");
	kernel.setArg(0, buffer_A);
	kernel.setArg(1, buffer_B);
	kernel.setArg(2, buffer_C);
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(arraySize), cl::NullRange);
	queue.finish();

	std::vector<T> result(arraySize);
	queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(T) * arraySize, result.data());
	
	return result;
}

#endif // !OPENCLTOOLS_H

