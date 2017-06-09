#include "unplot2d.hpp"

// for `cv::matchTemplate` result.

bool
unplot2d_type::peak2f_check (cv::Mat& img, int x, int y, float threshold)
{
    if (img.at<float> (y, x) < threshold)
        return false;
    float m = 0.0f;
    int xp = -1, yp = -1;
    for (int dy = -3; dy < 4; ++dy) {
        for (int dx = -3; dx < 4; ++dx) {
            if (img.at<float> (y + dy, x + dx) > m) {
                m = img.at<float> (y + dy, x + dx);
                yp = y + dy;
                xp = x + dx;
            }
        }
    }
    return xp == x && yp == y;
}
