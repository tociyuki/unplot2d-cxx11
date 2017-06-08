#include "unplot2d.hpp"

double
unplot2d_type::correlation (cv::Mat& mat0, cv::Mat& mat1)
{
    double den = 0.0, num0 = 0.0, num1 = 0.0;
    for (int dy = 0; dy < mat0.rows; ++dy)
        for (int dx = 0; dx < mat0.cols; ++dx) {
            double const z0 = mat0.at<unsigned char> (dy, dx);
            double const z1 = mat1.at<unsigned char> (dy, dx);
            den  += z0 * z1;
            num0 += z0 * z0;
            num1 += z1 * z1;
        }
    return den / std::sqrt (num0 * num1);
}
