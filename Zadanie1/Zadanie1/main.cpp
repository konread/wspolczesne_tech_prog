#include "MatrixGenerator.h"

int main() {
	MatrixGenerator<float> generator(3);
	generator.initWithReal(0, 25);
	generator.print();

	return 0;
}