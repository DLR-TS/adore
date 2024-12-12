#ifndef MATRIX_CONVERSION_H
#define MATRIX_CONVERSION_H

#include <vector>
#include <Eigen/Dense>

namespace util {
    template<typename T>
    std::vector<std::vector<T>> toVector(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& eigenMatrix);

    template<typename T>
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> toEigen(const std::vector<std::vector<T>>& stdVector);
}

#endif // MATRIX_CONVERSION_H
