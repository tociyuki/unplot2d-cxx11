#include "unplot2d.hpp"

void
unplot2d_type::major_xaxis (void)
{
    linear_least_square_method_type lsq;
    lsq.clear ();
    int const tick_w = (xaxis_tick[1] - xaxis_tick[0]) / 2;
    for (int i = 0; i < xaxis_tick.size (); ++i) {
        int const x = xaxis_tick[i];
        for (int j = 0; j < scale_label_rect.size (); ++j) {
            int const scale_w = scale_label_rect[j].width / 2;
            int const scale_x = scale_label_rect[j].x + scale_w;
            int const scale_y = scale_label_rect[j].y;
            if (abs (scale_x - x) < tick_w && frame[0].br ().y < scale_y) {
                lsq.put (x, std::stod (scale_label[j]));
            }
        }
    }
    lsq.finalize ();
    xaxis_grad = lsq.grad;
    xaxis_offset = lsq.offset;
}

void
unplot2d_type::major_yaxis (void)
{
    linear_least_square_method_type lsq;
    lsq.clear ();
    int const tick_h = (yaxis_tick[1] - yaxis_tick[0]) / 2;
    for (int i = 0; i < yaxis_tick.size (); ++i) {
        int const y = yaxis_tick[i];
        for (int j = 0; j < scale_label_rect.size (); ++j) {
            int const scale_h = scale_label_rect[j].height / 2;
            int const scale_x = scale_label_rect[j].br ().x;
            int const scale_y = scale_label_rect[j].y + scale_h;
            if (abs (scale_y - y) < tick_h && scale_x < frame[0].x) {
                lsq.put (y, std::stod (scale_label[j]));
            }
        }
    }
    lsq.finalize ();
    yaxis_grad = lsq.grad;
    yaxis_offset = lsq.offset;
}

void
unplot2d_type::major_data_mark (void)
{
    for (int i = 0; i < data_mark.size (); ++i) {
        double const x = xaxis_grad * data_mark[i].x + xaxis_offset;
        double const y = yaxis_grad * data_mark[i].y + yaxis_offset;
        data.push_back (cv::Point2d (x, y));
    }
    std::sort (data.begin (), data.end (), [](cv::Point2d& a, cv::Point2d& b) {
        return a.x < b.x;
    });
}
