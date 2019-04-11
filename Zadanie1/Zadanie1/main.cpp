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
	/*const size_t arraySize = 5;
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
	print<type, arraySize>(result);*/
	
	auto devices = retrieveDevices();
	std::cout << "Using device: " << devices[0].getInfo<CL_DEVICE_NAME>() << "\n";
	auto& device = devices[0];

	const size_t dim = 3;
	using type = int;

	MatrixGenerator<type, dim> generator1;
	generator1.initWithInteger(4, 6);
	auto matrix1 = generator1.getMatrix();
	print<type, dim>(matrix1);

	MatrixGenerator<type, dim> generator2;
	generator2.initWithInteger(4, 6);
	auto matrix2 = generator2.getMatrix();
	print<type, dim>(matrix2);

	auto matrix3 = performMultiplyProgram<type, dim>(device, matrix1, matrix2);
	print<type, dim>(matrix3);

	return 0;
}