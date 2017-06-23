#pragma once

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <string>
#include <vector>

struct unplot2d_type {
    cv::vector<cv::vector<cv::Point> > contours;
    cv::vector<cv::Vec4i> hierarchy;
    cv::vector<cv::Rect> bound;
    cv::vector<cv::Rect> frame;
    cv::vector<int> frame_idx;

    cv::Mat mark;
    cv::vector<cv::Point> data_mark;

    std::vector<int> xaxis_tick;
    std::vector<int> yaxis_tick;

    cv::Rect xaxis_label_rect;
    cv::Rect yaxis_label_rect;
    std::vector<cv::Rect> scale_label_rect;
    std::string xaxis_label;
    std::string yaxis_label;
    std::vector<std::string> scale_label;

    double xaxis_grad;
    double xaxis_offset;
    double yaxis_grad;
    double yaxis_offset;
    std::vector<cv::Point2d> data;

    void frame_detect (cv::Mat& bin_img);
    void frame_check (cv::Mat& img, int const i);

    void mark_choose (cv::Mat& img);
    void data_mark_match (cv::Mat& img);

    bool ticks_xaxis_detect (cv::Mat& bin_img);
    bool ticks_yaxis_detect (cv::Mat& bin_img);
    void ticks_xaxis_score (cv::Mat& bin_img,
        std::vector<int>& peaks, std::vector<int>& peak_idx);
    void ticks_yaxis_score (cv::Mat& bin_img,
        std::vector<int>& peaks, std::vector<int>& peak_idx);
    double ticks_interval_average (int frame_size,
        std::vector<int>& peaks, std::vector<int>& peak_idx);
    int ticks_interval_min (int interval_min,
        std::vector<int>& peaks, std::vector<int>& peak_idx);

    void label_rect_yaxis_detect (cv::Mat& gray_img);
    void label_rect_yaxis_rotation (void);
    void label_rect_xaxis_detect (cv::Mat& gray_img);
    void label_rect_scale_detect (cv::Mat& gray_img);
    void label_yaxis_read (tesseract::TessBaseAPI& tess, cv::Mat& gray_img);
    void label_xaxis_read (tesseract::TessBaseAPI& tess, cv::Mat& gray_img);
    void label_scale_read (tesseract::TessBaseAPI& tess, cv::Mat& gray_img);

    void major_xaxis (void);
    void major_yaxis (void);
    void major_data_mark (void);

    bool peak2f_check (cv::Mat& img, int x, int y, float threshold);
    void peak1i_match (std::vector<int>& value, std::vector<int>& peak);
    double correlation (cv::Mat& mat0, cv::Mat& mat1);

    std::string chomp (std::string const& s);
    void tesseract_call (tesseract::TessBaseAPI& api,
                         cv::Mat& img, cv::Rect& clip,
                         std::string& output,
                         std::string const& configfile);
};

struct linear_least_square_method_type {
    int n;
    double sum_x;
    double sum_xx;
    double sum_y;
    double sum_yx;
    // for Kahan summation algorithm carry low-order bits
    double carry_x;
    double carry_xx;
    double carry_y;
    double carry_yx;
    double grad;
    double offset;
    void clear (void);
    void put (double x, double y);
    void finalize (void);
};
