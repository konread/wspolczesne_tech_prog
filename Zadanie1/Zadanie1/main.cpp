#include "MatrixGenerator.h"
#include "OpenCLTools.h"

template<typename T>
void print(const std::vector<T> vec)
{
	for (auto& item: vec) {
		std::cout << item << "  ";
	}
	std::cout << std::endl;
}

int main() {
	const size_t arraySize = 10;
	MatrixGenerator<float> generator(arraySize);
	generator.initWithReal(0, 5);

	auto devices = retrieveDevices();
	std::cout << "Using device: " << devices[0].getInfo<CL_DEVICE_NAME>() << "\n";
	auto& device = devices[0];
	auto matrix = generator.getMatrix();

	auto result = performAddProgram<float>(device, matrix[0], matrix[1]);
	print<float>(matrix[0]);
	print<float>(matrix[1]);
	print<float>(result);
	
	return 0;
}