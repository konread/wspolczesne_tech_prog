#ifndef OPENCLTOOLS_H
#define OPENCLTOOLS_H

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <array>
#include "CL/cl.hpp"
#include "MatrixGenerator.h"

std::vector<cl::Device> retrieveDevices();

template <typename T, std::size_t dim>
std::array < std::array<T, dim>, dim> multiplyingMatrixInOneThread(const std::array < std::array<T, dim>, dim>& matrixA, 
	const std::array < std::array<T, dim>, dim>& matrixB)
{
	std::array < std::array<T, dim>, dim> matrixC;

	for (size_t i = 0; i < dim; ++i) {
		for (size_t j = 0; j < dim; ++j) {
			T value = 0;
			for (size_t k = 0; k < dim; ++k) {
				value += matrixA[i][k] * matrixB[k][j];
			}

			matrixC[i][j] = value;
		}
	}

	return matrixC;
}

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

template <typename T, std::size_t dim>
std::array<std::array<T, dim>, dim> performMultiplyProgram(cl::Device& device, std::array<std::array<T, dim>, dim>& matrixA,
	std::array<std::array<T, dim>, dim>& matrixB)
{
	cl::Context context({ device });
	std::string kernelCode = "kernel void multiply(global " + std::string(typeid(T).name()) + "* arrayA, "
		"global " + std::string(typeid(T).name()) + "* arrayB, "
		"global " + std::string(typeid(T).name()) + "* arrayC, "
		"size_t dim1, size_t dim2) {"
			"size_t tx = get_global_id(0);"
			"size_t ty = get_global_id(1);"
			"" + std::string(typeid(T).name()) + " value = 0;"
			"for (size_t k = 0; k < dim1; ++k){"
				"" + std::string(typeid(T).name()) + " itemA = arrayA[ty * dim1 + k];"
				"" + std::string(typeid(T).name()) + " itemB = arrayB[k * dim2 + tx];"
				"value += itemA * itemB;"
			"}"
			"arrayC[ty * dim1 + tx] = value;"
			"}";

	cl::Program::Sources sources;
	sources.push_back({ kernelCode.c_str(),kernelCode.length() });

	cl::Program program(context, sources);
	if (program.build("-cl-std=CL1.2") != CL_SUCCESS) {
		std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << "\n";
		exit(1);
	}

	cl::Buffer bufferMA(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(T) * dim * dim, matrixA.data());
	cl::Buffer bufferMB(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(T) * dim * dim, matrixB.data());

	std::array<std::array<T, dim>, dim> matrixC;
	cl::Buffer bufferMC(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(T) * dim * dim, matrixC.data());

	cl::Kernel kernel(program, "multiply");
	kernel.setArg(0, bufferMA);
	kernel.setArg(1, bufferMB);
	kernel.setArg(2, bufferMC);
	kernel.setArg(3, dim);
	kernel.setArg(4, dim);

	cl::CommandQueue queue(context, device);
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(dim, dim));
	queue.enqueueReadBuffer(bufferMC, CL_TRUE, 0, sizeof(T) * dim * dim, matrixC.data());

	return matrixC;
}


#endif // !OPENCLTOOLS_H