#include "OpenCLTools.h"
#include <array>

template <typename T, std::size_t dim>
void print(const std::array<T, dim>& array)
{
	for (auto& item: array) {
		std::cout << item << "    ";
	}
	std::cout << std::endl;
}

template <typename T, std::size_t dim>
void print(const std::array<std::array<T, dim>, dim>& matrix)
{
	for (auto& row : matrix) {
		for (auto& item: row) {
			std::cout << item << "    ";
		}std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main() {
	const size_t arraySize = 5;
	using type = int;
	MatrixGenerator<type, arraySize> generator;
	generator.initWithInteger(0, 5);

	auto devices = retrieveDevices();
	std::cout << "Using device: " << devices[0].getInfo<CL_DEVICE_NAME>() << "\n";
	auto& device = devices[0];
	auto matrix = generator.getMatrix();

	print<type, arraySize>(matrix[0]);
	print<type, arraySize>(matrix[1]);
	auto result = performAddProgram<type>(device, matrix[0], matrix[1]);
	print<type, arraySize>(result);

	/*const size_t arraySize = 3;
	MatrixGenerator<int, arraySize> generator;
	generator.initWithInteger(4, 6);

	auto devices = retrieveDevices();
	std::cout << "Using device: " << devices[0].getInfo<CL_DEVICE_NAME>() << "\n";
	auto& device = devices[0];
	
	cl::Context context({ device });
	std::string kernelCode = "kernel void multiply(global int* array){"
		"size_t id = (get_global_id(1) * get_global_size(0)) + get_global_id(0);"
		"array[id] = array[id] * 5;"
		"}";

	cl::Program::Sources sources;
	sources.push_back({ kernelCode.c_str(),kernelCode.length() });

	cl::Program program(context, sources);
	if (program.build("-cl-std=CL1.2") != CL_SUCCESS) {
		std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << "\n";
		exit(1);
	}

	auto matrix = generator.getMatrix();
	print(matrix);
	cl::Buffer buffer(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * matrix.size() * matrix.size(), matrix.data());
	cl::Kernel kernel(program, "multiply");
	kernel.setArg(0, buffer);

	cl::CommandQueue queue(context, device);
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(matrix.size(), matrix.size()));
	queue.enqueueReadBuffer(buffer, CL_TRUE, 0, sizeof(int) * matrix.size() * matrix.size(), matrix.data());
	print(matrix);*/

	return 0;
}