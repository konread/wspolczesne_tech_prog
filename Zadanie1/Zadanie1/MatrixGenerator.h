#ifndef MATRIXGENERATOR_H
#define MATRIXGENERATOR_H

#include <array>
#include <random>
#include <iostream>

template <typename T, std::size_t dim>
class MatrixGenerator {
public:
	MatrixGenerator();

	const std::array<std::array<T, dim>, dim>& getMatrix() const;

	void initWithReal(T from, T to);
	void initWithInteger(T from, T to);

private:
	std::array<std::array<T, dim>, dim> m_matrix;
};

template <typename T, std::size_t dim>
MatrixGenerator<T, dim>::MatrixGenerator()
{
	for (auto& array: m_matrix) {
		array.fill(0);
	}
}

template <typename T, std::size_t dim>
const std::array<std::array<T, dim>, dim>& MatrixGenerator<T,dim>::getMatrix() const
{
	return m_matrix;
}

template <typename T, std::size_t dim>
void MatrixGenerator<T, dim>::initWithReal(T from, T to)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<T> dis(from, to);

	size_t size = m_matrix.size();

	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < size; ++j) {
			m_matrix[i][j] = dis(gen);
		}
	}
}

template <typename T, std::size_t dim>
void MatrixGenerator<T, dim>::initWithInteger(T from, T to)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<T> dis(from, to);

	size_t size = m_matrix.size();

	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < size; ++j) {
			m_matrix[i][j] = dis(gen);
		}
	}
}

#endif // !MATRIXGENERATOR_H
