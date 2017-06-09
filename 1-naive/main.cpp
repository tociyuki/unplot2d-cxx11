#include <cstdlib>
#include <iostream>
#include "unplot2d.hpp"

std::string encode_csv (std::string const& s);
void show_matching_position (cv::Mat& img, unplot2d_type& unplot2d);

// This program, named `unplot2d', reads numerical values of data marks
// from the plot 2d graph in an image file such as an PNG.

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

    // To recognize graphical components in the plot 2d graph,
    // we use `opencv` library. It reads an image file into the
    // 24 bit color pixel matrix.
    cv::Mat img = cv::imread (argv[1], cv::IMREAD_UNCHANGED);
    if (! img.data)
        return EXIT_FAILURE;
    // We use a gray image to recognize characters by `tesseract` OCR library.
    cv::cvtColor (img, gray_img, CV_BGR2GRAY);
    // We use a black and white binary image to detect graphical components
    // by `opencv` library. As usual way, since black means zero level of singal
    // and white means one level of signal, the binary image would be reversed
    // color.
    cv::threshold (gray_img, bin_img, 0, 255, cv::THRESH_BINARY|cv::THRESH_OTSU);
    bin_img = ~bin_img;

    // Here we start recoginitions.
    //
    // 1. Find all of contours in the image.
    // 2. The frame of plot area is choosed from them.
    //    After this step, the image separated into the two areas.
    //    The plot area contains the data marks, the frame,
    //    and the scale ticks. The text labeling area is outside of
    //    the plot area. 
    //    For details, see `frame.cpp`.
    unplot2d.frame_detect (bin_img);
    //    The information of the frame puts in the two rectangles of a vector.
    //    frame[0] is the bounding box of the contours of the frame.
    //    frame[1] is the logical verteces for the numerical plot area. 
    if (unplot2d.frame.size () != 2) {
        std::cerr << "cannot detect plot frame" << std::endl;
        return EXIT_FAILURE;
    }

    // 3. Choose single data mark in the plot area.
    //    This naive program assumes that the kind of marks is only single.
    //    The mark puts into the member `mark`.
    // 4. Pick up positions of the data marks in the plot area
    //    performing the template matching method.
    //    The positions put into the member `data_mark`.
    //    For details, see `data-mark.cpp`.
    unplot2d.mark_choose (bin_img);
    unplot2d.data_mark_match (bin_img);

    // 5. Detects ticks of scale on the X- and Y-axis.
    //    This program can detect ticks only on the left vertecs of
    //    frame contours for Y-axis, and the bottom  vertecs of frame
    //    contours for X-axis. If there are any ticks on the other side,
    //    they are ignored.
    //    The positions of ticks put into the member `xaxis_tick` and
    //    `yaxis_tick`.
    //    For details, see `ticks.cpp`.
    unplot2d.ticks_xaxis_detect (bin_img);
    unplot2d.ticks_yaxis_detect (bin_img);

    // 6. Detects rectangles of label of X- and Y-axis.
    //    It is important the calling sequences:
    //    First, we must detects the label of Y-axis.
    //    It is wrote in the counterclockwise pi/2 rotations.
    //    Second, we must detects the label of X-axis.
    //    Finally, we detects the labels of scales on axis.
    //    The rectangle of Y-axis label puts into the member `yaxis_label_rect`.
    //    The rectangle of X-axis label puts into the member `xaxis_label_rect`.
    //    The rectangles of scale labels put into the member `scale_label`.
    //    For details, see `label-rect.cpp`
    unplot2d.label_rect_yaxis_detect (gray_img);
    unplot2d.label_rect_xaxis_detect (gray_img);
    unplot2d.label_rect_scale_detect (gray_img);

    // 7. Recognizes text of the labels.
    //    The string for Y-axis label puts into the member `yaxis_label`.
    //    The string for X-axis label puts into the member `xaxis_label`.
    //    The string for scale labels put into the member `scale_label`.
    //    For details, see `label-text.cpp`.
    tesseract::TessBaseAPI tess;
    unplot2d.label_yaxis_read (tess, gray_img);
    unplot2d.label_xaxis_read (tess, gray_img);
    unplot2d.label_scale_read (tess, gray_img);

    // 8. Majors numerical correspondings of ticks and data marks.
    //    The numerical transformation parameters from image coordinates
    //    to numerical values for X-axis put into the member `xaxis_grad`
    //    and `xaxis_offset`: `a * x + b` where a is `xaxis_grad`, and
    //    b is `xaxis_offset` respectively.
    //    For Y-axis put into the member `yaxis_grad` and `yaxis_offset`
    //    in the same bases.
    //    The numerical values of data marks put into the member `data`.
    //    For details, see `major.cpp`.
    unplot2d.major_xaxis ();
    unplot2d.major_yaxis ();
    unplot2d.major_data_mark ();

    // Now, we have numerical values for data marks.
    // Prints them by CSV format.
    std::cout << "\"" << encode_csv (unplot2d.xaxis_label) << "\","
              << "\"" << encode_csv (unplot2d.yaxis_label) << "\"" << std::endl;
    for (int i = 0; i < unplot2d.data.size (); ++i) {
        std::cout << unplot2d.data[i].x << "," << unplot2d.data[i].y << std::endl;
    }

    show_matching_position (img, unplot2d);
    cv::waitKey ();

    return EXIT_SUCCESS;
}

// encode string for CSV format.
// substitute "\"" => "\"\"".
std::string
encode_csv (std::string const& s)
{
    std::string t;
    for (auto c : s) {
        if ('"' == c)
            t += "\"\"";
        else
            t.push_back (c);
    }
    return t;
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
