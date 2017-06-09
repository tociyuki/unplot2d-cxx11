#include "unplot2d.hpp"

// Reads text of labels by tesseract-OCR library.
// Only Y-axis label is drawn by pi/2 counterclockwise rotation.

void
unplot2d_type::label_yaxis_read (tesseract::TessBaseAPI& tess, cv::Mat& gray_img)
{
    if (yaxis_label_rect.width > 0) {
        cv::Mat clockwise_gray_img;
        cv::transpose (gray_img, clockwise_gray_img);
        cv::flip (clockwise_gray_img, clockwise_gray_img, 1);
        cv::Rect clip (yaxis_label_rect.br ().y, yaxis_label_rect.tl ().x,
            yaxis_label_rect.height, yaxis_label_rect.width);
        tesseract_call (tess, clockwise_gray_img, clip, yaxis_label, "");
    }
}

void
unplot2d_type::label_xaxis_read (tesseract::TessBaseAPI& tess, cv::Mat& gray_img)
{
    if (xaxis_label_rect.width > 0) {
        tesseract_call (tess, gray_img, xaxis_label_rect, xaxis_label, "");
    }
}

void
unplot2d_type::label_scale_read (tesseract::TessBaseAPI& tess, cv::Mat& gray_img)
{
    for (int i = 0; i < scale_label_rect.size (); ++i) {
        std::string s;
        tesseract_call (tess, gray_img, scale_label_rect[i], s, "digits");
        scale_label.push_back (s);
    }
}

void
unplot2d_type::tesseract_call (tesseract::TessBaseAPI& api,
                             cv::Mat& img, cv::Rect& clip,
                             std::string& output,
                             std::string const& configfile)
{
    api.Init(nullptr, "eng");
    if (! configfile.empty ()) {
        api.ReadConfigFile (configfile.c_str ());
    }
    api.SetImage((unsigned char*)img.data, img.cols, img.rows,
        img.channels(), img.step1());
    api.SetPageSegMode (tesseract::PSM_SINGLE_BLOCK);
    api.SetRectangle (clip.x, clip.y, clip.width, clip.height);
    api.Recognize (nullptr);
    char* s = api.GetUTF8Text ();
    output.assign (chomp (s));
    delete[] s;
    api.Clear ();
}

// tesseract basic API returns text as "foo\n\n".
// strips tail "\n".
std::string
unplot2d_type::chomp (std::string const& s)
{
    std::string t = s;
    while (! t.empty () && t.back () == '\n')
        t.pop_back ();
    return t;
}
