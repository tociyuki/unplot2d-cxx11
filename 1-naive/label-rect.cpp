#include <algorithm>
#include "unplot2d.hpp"

void
unplot2d_type::label_rect_yaxis_detect (cv::Mat& gray_img)
{
    std::vector<cv::Rect> vbox;
    for (int i = 0; i < bound.size (); ++i) {
        if ((frame[0] & bound[i]) == bound[i])
            continue;
        int found = -1;
        for (int j = 0; j < vbox.size (); ++j) {
            if (std::max (vbox[j].x, bound[i].x)
                    < std::min (bound[i].br ().x, vbox[j].br ().x)) {
                found = j;
                break;
            }
        }
        if (found < 0) {
            vbox.push_back (bound[i]);
        }
        else {
            vbox[found] |= bound[i];
        }
    }
    for (int i = 0; i < vbox.size (); ++i) {
        if (yaxis_label_rect.width == 0 || vbox[i].x < yaxis_label_rect.x)
            yaxis_label_rect = vbox[i];
    }
    label_rect_yaxis_rotation ();
}

void
unplot2d_type::label_rect_yaxis_rotation (void)
{
    if (yaxis_label_rect.width <= 0)
        return;
    for (int i = 0; i < bound.size (); ++i) {
        if (frame[0].x <= bound[i].x)
            continue;
        if ((yaxis_label_rect & bound[i]) == bound[i]) {
            int w = bound[i].width;
            int h = bound[i].height;
            if (h < 2 * w && h > w) {
                yaxis_label_rect.x = yaxis_label_rect.y = 0;
                yaxis_label_rect.width = yaxis_label_rect.height = 0;
                break;
            }
        }
    }
}

void
unplot2d_type::label_rect_xaxis_detect (cv::Mat& gray_img)
{
    std::vector<cv::Rect> hbox;
    for (int i = 0; i < bound.size (); ++i) {
        if ((frame[0] & bound[i]) == bound[i])
            continue;
        if (yaxis_label_rect.width > 0 && (yaxis_label_rect & bound[i]) == bound[i])
            continue;
        int found = -1;
        for (int j = 0; j < hbox.size (); ++j) {
            if (std::max (hbox[j].y, bound[i].y)
                    < std::min (bound[i].br ().y, hbox[j].br ().y)) {
                found = j;
                break;
            }
        }
        if (found < 0) {
            hbox.push_back (bound[i]);
        }
        else {
            hbox[found] |= bound[i];
        }
    }
    for (int i = 0; i < hbox.size (); ++i) {
        if (xaxis_label_rect.y < hbox[i].y)
            xaxis_label_rect = hbox[i];
    }
}

void
unplot2d_type::label_rect_scale_detect (cv::Mat& gray_img)
{
    std::vector<cv::Rect> hbox;
    int grif_height = 0;
    for (int i = 0; i < bound.size (); ++i) {
        if ((frame[0] & bound[i]) == bound[i])
            continue;
        if (yaxis_label_rect.width > 0 && (yaxis_label_rect & bound[i]) == bound[i])
            continue;
        if (xaxis_label_rect.width > 0 && (xaxis_label_rect & bound[i]) == bound[i])
            continue;
        grif_height = std::max (grif_height, bound[i].height);
        cv::Rect sqrect (bound[i].x, bound[i].y, grif_height, bound[i].height);
        int found = -1;
        for (int j = 0; j < hbox.size (); ++j) {
            cv::Rect a = sqrect & hbox[j];
            if (a.width > 0 && a.height > 0) {
                found = j;
                break;
            }
        }
        if (found < 0) {
            hbox.push_back (sqrect);
            scale_label_rect.push_back (bound[i]);
        }
        else {
            hbox[found] |= sqrect;
            scale_label_rect[found] |= bound[i];
        }
    }
}
