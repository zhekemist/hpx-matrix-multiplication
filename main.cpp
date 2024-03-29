#include <hpx/hpx_main.hpp>
#include <hpx/iostream.hpp>

#include "matrix.h"

template <typename T> Matrix<T> operator*(Matrix<T> left, Matrix<T> right) {
    std::vector<hpx::future<T>> result_elements(left.rows * right.cols);

    hpx::experimental::for_loop(
        hpx::execution::par, 0, result_elements.size(), [&](size_t pos) {
            size_t row = pos / right.cols, col = pos % right.cols;

            auto row_begin = left.row_begin(row);
            auto row_end = left.row_begin(row + 1);
            auto col_begin = right.column_begin(col);

            result_elements.at(pos) = hpx::transform_reduce(hpx::execution::par(hpx::execution::task),
                                                            row_begin, row_end, col_begin, T(0));
        });

    return {left.rows, right.cols, hpx::unwrap(result_elements)};
}

int main() {

    Matrix a{5, 4};
    Matrix b{4, 6};

    fill_matrix(a, 10, 99);
    fill_matrix(b, 1, 9);

    hpx::cout << "Matrix A: " << std::endl;
    a.print();
    hpx::cout << "Matrix B: " << std::endl;
    b.print();

    Matrix c = a * b;

    hpx::cout << "Result: " << std::endl;
    c.print();

    return 0;
}
