#include <algorithm>
#include "unplot2d.hpp"

// The ticks are detected with the peak position of projection histogram
// in the narrow area at the X- or Y-axis.
//
//  1. Calculates the projection histogram.
//  2. Matchs the peaks in the histogram.
//  3. Scores the ticks for small pieces at the peaks.
//     The small piece may be some kind of data mark.
//     To ignore data marks, we check repitition.
//     We do not use the periodical tests unsuitable
//     for the logarithmic-scale axis.
//  4. Finds minimum interval from them.
//  5. Puts tick positions by interpolation.

bool
unplot2d_type::ticks_xaxis_detect (cv::Mat& bin_img)
{
    // narrow area at the X-axis.
    // 24px == 2mm * 300px / 25.4mm
    cv::Rect axis_rect (frame[0].x, frame[1].br ().y - 24,
        frame[0].width, frame[0].br ().y - frame[1].br ().y + 24);
    cv::Mat axis = bin_img (axis_rect);
    // takes a projection histogram in it.
    std::vector<int> histogram (axis.cols, 0);
    for (int i = 0; i < axis.cols; ++i) {
        histogram[i] = cv::countNonZero (axis (cv::Rect (i, 0, 1, axis.rows)));
    }
    // matchs peaks in the projection histogram.
    std::vector<int> peaks;
    peak1i_match (histogram, peaks);
    std::vector<int> peak_idx;
    // Scores the ticks for small pieces at the peaks by repitition check.
    ticks_xaxis_score (bin_img, peaks, peak_idx);
    // finds minimum interval.
    double const interval = ticks_interval_average (frame[1].width, peaks, peak_idx);
    // interpolate.
    double const x0 = peaks[peak_idx[0]] + frame[0].x;
    int const i0 = (int)(((double)frame[1].x - x0) / interval);
    for (int i = i0; x0 + interval * i <= (double)frame[1].br ().x + 8; ++i) {
        int pos = cvRound (x0 + interval * i);
        if (pos >= frame[1].x) {
            xaxis_tick.push_back (pos);
        }
    }
    return true;
}

bool
unplot2d_type::ticks_yaxis_detect (cv::Mat& bin_img)
{
    cv::Rect axis_rect (frame[0].x, frame[0].y,
        frame[1].x + 24 - frame[0].x, frame[0].height);
    cv::Mat axis = bin_img (axis_rect);
    std::vector<int> histogram (axis.rows, 0);
    for (int i = 0; i < axis.rows; ++i) {
        histogram[i] = cv::countNonZero (axis (cv::Rect (0, i, axis.cols, 1)));
    }
    std::vector<int> peaks;
    peak1i_match (histogram, peaks);
    std::vector<int> peak_idx;
    ticks_yaxis_score (bin_img, peaks, peak_idx);
    double const interval = ticks_interval_average (frame[1].height, peaks, peak_idx);
    double const y0 = peaks[peak_idx[0]] + frame[0].y;
    int const i0 = (int)(((double)frame[1].y - y0) / interval);
    for (int i = i0; y0 + interval * i <= (double)frame[1].br ().y + 8; ++i) {
        int pos = cvRound (y0 + interval * i);
        if (pos >= frame[1].y) {
            yaxis_tick.push_back (pos);
        }
    }
    return true;
}

void
unplot2d_type::ticks_xaxis_score (cv::Mat& bin_img,
    std::vector<int>& peaks, std::vector<int>& peak_idx)
{
    for (int i = 0; i < peaks.size (); ++i) {
        // small piece at the peak[i].
        // 24px == 2mm * 300px / 25.4mm
        // 36px == 3mm * 300px / 25.4mm
        cv::Rect tr (frame[0].x + peaks[i] - 18, frame[1].br ().y - 24,
                     36, frame[0].br ().y - frame[1].br ().y + 24);
        cv::Mat tpl = bin_img (tr);
        int score = 0;
        for (int j = 0; j < peaks.size (); ++j) {
            if (i == j) continue;
            // good correlation for each small pieces at other peaks.
            cv::Rect ir (frame[0].x + peaks[j] - 18, frame[1].br ().y - 24,
                         tr.width, tr.height);
            cv::Mat roi = bin_img (ir);
            double corr = correlation (tpl, roi);
            if (corr > 0.92) {
                ++score;
            }
        }
        // good repitition.
        if (score * 3 > peaks.size ()) {
            peak_idx.push_back (i);
        }
    }
}

void
unplot2d_type::ticks_yaxis_score (cv::Mat& bin_img,
    std::vector<int>& peaks, std::vector<int>& peak_idx)
{
    for (int i = 0; i < peaks.size (); ++i) {
        cv::Rect tr (frame[0].x, frame[0].y + peaks[i] - 18,
                     frame[1].x + 24 - frame[0].x, 36);                     
        cv::Mat tpl = bin_img (tr);
        int score = 0;
        for (int j = 0; j < peaks.size (); ++j) {
            if (i == j) continue;
            cv::Rect ir (frame[0].x, frame[0].y + peaks[j] - 18, tr.width, tr.height);
            cv::Mat roi = bin_img (ir);
            double corr = correlation (tpl, roi);
            if (corr > 0.92) {
                ++score;
            }
        }
        if (score * 3 > peaks.size ()) {
            peak_idx.push_back (i);
        }
    }
}

// Calculates the average interval as the tick period.
double
unplot2d_type::ticks_interval_average (int frame_size,
    std::vector<int>& peaks, std::vector<int>& peak_idx)
{
    int interval_min = ticks_interval_min (frame_size, peaks, peak_idx);
    int interval_sum = 0;
    int interval_count = 0;
    for (int j = 1; j < peak_idx.size (); ++j) {
        int const i0 = peak_idx[j - 1];
        int const i1 = peak_idx[j];
        int q = peaks[i1] - peaks[i0];
        if (q - interval_min < 2) {
            interval_sum += q;
            ++interval_count;
        }
        else {
            for (int i = 2; interval_min * i < frame_size; ++i) {
                if (abs (interval_min * i - q) < 2 * i) {
                    interval_sum += q;
                    interval_count += i;
                    break;
                }
            }
        }
    }
    return (double)interval_sum / (double)interval_count;
}

// Finds minimum interval between choosen peaks.
int
unplot2d_type::ticks_interval_min (int value,
    std::vector<int>& peaks, std::vector<int>& peak_idx)
{
    for (int j = 1; j < peak_idx.size (); ++j) {
        int const i0 = peak_idx[j - 1];
        int const i1 = peak_idx[j];
        int q = peaks[i1] - peaks[i0];
        value = std::min (value, q);
    }
    return value;
}
