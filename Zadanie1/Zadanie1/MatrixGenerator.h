#ifndef MATRIXGENERATOR_H
#define MATRIXGENERATOR_H

#include <vector>
#include <random>
#include <iostream>

template<typename T>
class MatrixGenerator {
public:
	MatrixGenerator(size_t dim);

	const std::vector<std::vector<T>> getMatrix() const;

	void initWithReal(T from, T to);
	void initWithInteger(T from, T to);

	void print();

	size_t dim() const;

private:
	std::vector<std::vector<T>> m_matrix;
};

template<typename T>
MatrixGenerator<T>::MatrixGenerator(size_t dim)
{
	for (size_t i = 0; i < dim; ++i) {
		std::vector<T> row;
		row.resize(dim);
		m_matrix.emplace_back(row);
	}
}

template<typename T>
const std::vector<std::vector<T>> MatrixGenerator<T>::getMatrix() const
{
	return m_matrix;
}

template<typename T>
void MatrixGenerator<T>::initWithReal(T from, T to)
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

template<typename T>
void MatrixGenerator<T>::initWithInteger(T from, T to)
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

template<typename T>
void MatrixGenerator<T>::print()
{
	for (auto& row : m_matrix) {
		for (auto& item : row) {
			std::cout << item << " ";
		}
		std::cout << "\n";
	}
}

template<typename T>
size_t MatrixGenerator<T>::dim() const
{
	return m_matrix.size();
}

#endif // !MATRIXGENERATOR_H
