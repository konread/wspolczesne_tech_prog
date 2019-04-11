#ifndef OPENCLTOOLS_H
#define OPENCLTOOLS_H

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <array>
#include "CL/cl.hpp"
#include "MatrixGenerator.h"

std::vector<cl::Device> retrieveDevices();

template <typename T, std::size_t dim>
std::array<T, dim> performAddProgram(cl::Device& device, const std::array<T, dim>& arrayA, const std::array<T, dim>& arrayB)
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

	// create buffers on the device
	cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(T) * dim);
	cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(T) * dim);
	cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(T) * dim);

	//create queue to which we will push commands for the device.
	cl::CommandQueue queue(context, device);

	//write arrays A and B to the device
	queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(T) * dim, arrayA.data());
	queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(T) * dim, arrayB.data());

	//run the kernel
	cl::Kernel kernel(program, "add");
	kernel.setArg(0, buffer_A);
	kernel.setArg(1, buffer_B);
	kernel.setArg(2, buffer_C);
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(dim), cl::NullRange);
	queue.finish();

	std::array<T, dim> result;
	queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(T) * dim, result.data());
	
	return result;
}

#endif // !OPENCLTOOLS_H