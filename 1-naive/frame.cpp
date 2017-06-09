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
    // bounding rectagle of approx may be outside of frame ractangle.
    cv::Rect outer_rect = cv::boundingRect (approx);
    // check bounding rectangle of nesting components.
    cv::Rect inner_rect;
    for (int i1 = hierarchy[i][2]; i1 >= 0; i1 = hierarchy[i1][0]) {
        cv::Rect r = cv::boundingRect (contours[i1]);
        // we correct nesting components in the outer_rect.
        if ((outer_rect & r) == r) {
            if (inner_rect.width == 0)
                inner_rect = r;
            else
                inner_rect |= r;
        }
    }
    // inner_rect must be almost same as the outer_rect or it is not frame.
    if (outer_rect.width - inner_rect.width > 24 || outer_rect.height - inner_rect.height > 24) {
        return;
    }
    // Now we believe catching the frame.
    // We need center_rect to major data marks later.
    cv::Rect center_rect ((outer_rect.x + inner_rect.x) / 2,
        (outer_rect.y + inner_rect.y) / 2,
        (outer_rect.width + inner_rect.width) / 2,
        (outer_rect.height + inner_rect.height) / 2);
    // frame[0] is bounding box of frame with ticks and data marks on it.
    frame.push_back (cv::boundingRect (contours[i]));
    // frame[1] is center_rect.
    frame.push_back (center_rect);
    // for shortcut frame[0] comarisons and debug uses.
    frame_idx.push_back (i);
    frame_idx.push_back (hierarchy[i][2]);
    return;
}
