
#ifndef HPX_MATRIX_MULTIPLICATION_MATRIX_H
#define HPX_MATRIX_MULTIPLICATION_MATRIX_H

#include <iterator>
#include <random>
#include <vector>

#include <hpx/algorithm.hpp>
#include <hpx/iostream.hpp>

#include <boost/range/adaptor/strided.hpp>

template <typename T = int> struct Matrix {
    size_t rows, cols;
    std::vector<T> elements;

    Matrix(size_t rows, size_t cols)
        : rows{rows}, cols{cols}, elements(rows * cols) {}

    Matrix(size_t rows, size_t cols, const std::vector<T> &elements)
        : rows{rows}, cols{cols}, elements(elements) {}

    auto row_begin(size_t row) const {
        return this->elements.begin() + row * this->cols;
    }

    auto column_begin(size_t col) const {
        auto strided_range = std::make_pair(std::begin(this->elements) + col,std::end(this->elements))
                            | boost::adaptors::strided(this->cols);
        return strided_range.begin();
    }

    void print() {
        for (size_t row{0}; row < this->rows; ++row) {
            for (size_t col{0}; col < this->cols; ++col)
                hpx::cout << this->elements.at(row * this->cols + col) << " ";
            hpx::cout << std::endl;
        }
    }
};

std::mt19937 rnd_gen{std::random_device{}()};

template <typename T>
void fill_matrix(Matrix<T> &matrix, T lower_bound, T upper_bound) {
    hpx::generate(hpx::execution::par, std::begin(matrix.elements),
                  std::end(matrix.elements), [lower_bound, upper_bound]() {
                      return std::uniform_int_distribution{lower_bound, upper_bound}(rnd_gen);
                  });
}

#endif // HPX_MATRIX_MULTIPLICATION_MATRIX_H
