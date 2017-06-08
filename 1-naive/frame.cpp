#include "unplot2d.hpp"

void
unplot2d_type::frame_detect (cv::Mat& bin_img)
{
    cv::Mat ctr_img = bin_img.clone ();
    cv::findContours (ctr_img, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    for (int i = 0; i >= 0; i = hierarchy[i][0]) {
        bound.push_back (cv::boundingRect (contours[i]));
        frame_check (bin_img, i);
    }
    if (frame.size () == 2) {
        xaxis_grad = 1.0 / (double)frame[1].width;
        xaxis_offset = - (double)frame[1].x / (double)frame[1].width;
        yaxis_grad = - 1.0 / (double)frame[1].height;
        yaxis_offset = (double)frame[1].br ().y / (double)frame[1].height;
    }
}

void
unplot2d_type::frame_check (cv::Mat& img, int const i)
{
    double area = cv::contourArea (contours[i], false);
    if (area <= img.cols * img.rows / 4)
        return;
    cv::vector<cv::Point> approx;
    double epsilon = 0.01 * cv::arcLength (contours[i], true);
    cv::approxPolyDP (cv::Mat (contours[i]), approx, epsilon, true);
    if (approx.size () != 4)
        return;
    if (hierarchy[i][2] < 0)
        return;
    for (int i1 = hierarchy[i][2]; i1 >= 0; i1 = hierarchy[i1][0]) {
        double area1 = cv::contourArea (contours[i1], false);
        if (area1 <= img.cols * img.rows / 4)
            continue;
        cv::vector<cv::Point> approx1;
        double epsilon1 = 0.01 * cv::arcLength (contours[i1], true);
        cv::approxPolyDP (cv::Mat (contours[i1]), approx1, epsilon1, true);
        if (approx1.size () == 4) {
            frame.push_back (cv::boundingRect (contours[i]));
            cv::Rect r0 = cv::boundingRect (approx);
            cv::Rect r1 = cv::boundingRect (approx1);
            cv::Rect r ((r0.x + r1.x) / 2, (r0.y + r1.y) / 2,
                        (r0.width + r1.width) / 2,
                        (r0.height + r1.height) / 2);
            frame.push_back (r);
            frame_idx.push_back (i);
            frame_idx.push_back (i1);
            return;
        }
    }
    return;
}
