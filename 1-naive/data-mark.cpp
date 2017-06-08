#include <algorithm>
#include "unplot2d.hpp"

void
unplot2d_type::mark_choose (cv::Mat& img)
{
    for (int i = 0; i < bound.size (); ++i) {
        if (frame[0] == bound[i]
                || (frame[0] & bound[i]) != bound[i]
                || abs (bound[i].width - bound[i].height) > 8
                || std::max (bound[i].width, bound[i].height) > 36)
            continue;
        mark = img (bound[i]).clone ();
        break;
    }
}

void
unplot2d_type::data_mark_match (cv::Mat& img)
{
    int const xoffset = mark.cols / 2;
    int const yoffset = mark.rows / 2;
    cv::Mat result;
    cv::matchTemplate (img, mark, result, cv::TM_CCORR_NORMED);
    float threshold = 0.7f;
    for (int y = 0; y < result.rows; ++y) {
        for (int x = 0; x < result.cols; ++x) {
            if (frame[0].contains (cv::Point (x, y))
                    && peak2f_check (result, x, y, threshold)) {
                data_mark.push_back (cv::Point (x + xoffset, y + yoffset));
            }
        }
    }
}
