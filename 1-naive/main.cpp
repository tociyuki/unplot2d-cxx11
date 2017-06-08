#include <cstdlib>
#include <iostream>
#include "unplot2d.hpp"

void show_matching_position (cv::Mat& img, unplot2d_type& unplot2d);

int
main (int argc, char* argv[])
{
    unplot2d_type unplot2d;
    cv::Mat gray_img;
    cv::Mat bin_img;

    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " image-file-name" << std::endl;
        return EXIT_FAILURE;
    }
    cv::Mat img = cv::imread (argv[1], cv::IMREAD_UNCHANGED);
    if (! img.data)
        return EXIT_FAILURE;

    cv::cvtColor (img, gray_img, CV_BGR2GRAY);
    cv::threshold (gray_img, bin_img, 0, 255, cv::THRESH_BINARY|cv::THRESH_OTSU);
    bin_img = ~bin_img;

    unplot2d.frame_detect (bin_img);
    if (unplot2d.frame.size () != 2) {
        std::cerr << "cannot detect plot frame" << std::endl;
        return EXIT_FAILURE;
    }

    unplot2d.mark_choose (bin_img);
    unplot2d.data_mark_match (bin_img);

    unplot2d.ticks_xaxis_detect (bin_img);
    unplot2d.ticks_yaxis_detect (bin_img);

    unplot2d.label_rect_yaxis_detect (gray_img);
    unplot2d.label_rect_xaxis_detect (gray_img);
    unplot2d.label_rect_scale_detect (gray_img);

    tesseract::TessBaseAPI tess;
    unplot2d.label_yaxis_read (tess, gray_img);
    unplot2d.label_xaxis_read (tess, gray_img);
    unplot2d.label_scale_read (tess, gray_img);

    unplot2d.major_xaxis ();
    unplot2d.major_yaxis ();
    unplot2d.major_data_mark ();

    std::cout << "\"" << unplot2d.xaxis_label << "\","
              << "\"" << unplot2d.yaxis_label << "\"" << std::endl;
    for (int i = 0; i < unplot2d.data.size (); ++i) {
        std::cout << unplot2d.data[i].x << "," << unplot2d.data[i].y << std::endl;
    }

    show_matching_position (img, unplot2d);
    cv::waitKey ();

    return EXIT_SUCCESS;
}

void
show_matching_position (cv::Mat& img, unplot2d_type& unplot2d)
{
    cv::Mat dst = img.clone ();
    cv::rectangle (dst, unplot2d.frame[1], cv::Scalar (0, 0, 255), 1, 8);
    for (int x : unplot2d.xaxis_tick) {
        cv::line (dst, cv::Point (x - 1, unplot2d.frame[1].br ().y - 48),
                       cv::Point (x - 1, unplot2d.frame[1].br ().y - 32),
                       cv::Scalar (0, 0, 255), 2, 8);
    }
    for (int y : unplot2d.yaxis_tick) {
        cv::line (dst, cv::Point (unplot2d.frame[1].x + 32, y - 1),
                       cv::Point (unplot2d.frame[1].x + 48, y - 1),
                       cv::Scalar (0, 0, 255), 2, 8);
    }
    if (! unplot2d.yaxis_label.empty ()) {
        cv::rectangle (dst, unplot2d.yaxis_label_rect, cv::Scalar (0, 0, 255), 1, 8);
    }
    if (! unplot2d.xaxis_label.empty ()) {
        cv::rectangle (dst, unplot2d.xaxis_label_rect, cv::Scalar (0, 0, 255), 1, 8);
    }
    for (int i = 0; i < unplot2d.scale_label.size (); ++i) {
        cv::rectangle (dst, unplot2d.scale_label_rect[i], cv::Scalar (0, 0, 255), 1, 8);
    }
    for (int i = 0; i < unplot2d.data_mark.size (); ++i) {
        cv::Rect r (unplot2d.data_mark[i].x - unplot2d.mark.cols / 2,
                    unplot2d.data_mark[i].y - unplot2d.mark.rows / 2,
                    unplot2d.mark.cols, unplot2d.mark.rows);
        cv::rectangle (dst, r, cv::Scalar (0, 255, 0), 1, 8);
    }
    //cv::imwrite ("result.png", dst);
    cv::namedWindow ("UNPLOT2D", cv::WINDOW_NORMAL);
    cv::imshow ("UNPLOT2D", dst);
}
