#include "unplot2d.hpp"

// The shape of frame in the plot 2d graph is an large rectangle
// with small scale ticks and sometimes data marks on it.
// To detect it, we use the combination of the `cv::findContours`
// procedure and the `cv::approxPolyDP` procedure.
// The frame will be picked up at the top level contours.

void
unplot2d_type::frame_detect (cv::Mat& bin_img)
{
    // Since `cv::findContours` destroys the image matrix,
    // we make the copy of it.
    cv::Mat ctr_img = bin_img.clone ();
    // To avoid complex treatments for nestings contours,
    // finds with the linkage: `CV_RETR_CCOMP`.
    cv::findContours (ctr_img, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    // On `CV_RETR_CCOMP`, it is easy to walk around graphical components
    // at the top level them.
    for (int i = 0; i >= 0; i = hierarchy[i][0]) {
        // puts bounding box for all top level contours into the member `bound`.
        bound.push_back (cv::boundingRect (contours[i]));
        // checks it what is the frame.
        frame_check (bin_img, i);
    }
    // In defaults, we set the transfomation parameters for numerical
    // values of data marks as the normalized to size of the frame.
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
    // `contours[i]` is large or it is not frame. 
    double area = cv::contourArea (contours[i], false);
    if (area <= img.cols * img.rows / 4)
        return;
    // `contours[i]` can be approximated to the rectangle or it is not frame. 
    cv::vector<cv::Point> approx;
    double epsilon = 0.01 * cv::arcLength (contours[i], true);
    cv::approxPolyDP (cv::Mat (contours[i]), approx, epsilon, true);
    if (approx.size () != 4)
        return;
    // `contours[i]` has nesting components or it is not frame.
    if (hierarchy[i][2] < 0)
        return;
    for (int i1 = hierarchy[i][2]; i1 >= 0; i1 = hierarchy[i1][0]) {
        // `contours[i]` has large nesting components.
        double area1 = cv::contourArea (contours[i1], false);
        if (area1 <= img.cols * img.rows / 4)
            continue;
        // And one of them can be approximated to the rectangle too.
        cv::vector<cv::Point> approx1;
        double epsilon1 = 0.01 * cv::arcLength (contours[i1], true);
        cv::approxPolyDP (cv::Mat (contours[i1]), approx1, epsilon1, true);
        if (approx1.size () == 4) {
            // All tests are passed.
            // We put into the member `frame` and `frame_idx`.
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
