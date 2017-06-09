#include "unplot2d.hpp"

// Matchs peaks in data.
//
// see Savitzky, Golay, ``Smoothing and Differentiation of Data by Simplified
// Least Squares Procedures'', Analytical Chemistry (1964)
void
unplot2d_type::peak1i_match (std::vector<int>& value, std::vector<int>& peaks)
{
    int const q = 4;
    int sign = -1;
    int span = 0;
    for (int i = q + 1; i <= value.size () - q; ++i) {
        double p = 0.0;
        for (int j = 1; j <= q; ++j) {
            p += (double)(value[i + j - 1] - value[i - j - 1]) * j;
        }
        int pos = 0;
        int left_sign = sign;
        sign = p < 0 ? -1 : p > 0 ? +1 : 0;
        if (sign > 0) {
            span = 0;
        }
        else if (sign == 0) {
            ++span;
        }
        else if (left_sign == 0) {
            pos = i - 1 - span / 2;
        }
        else if (left_sign > 0) {
            pos = i;
        }
        else {
            span = 0;
        }
        if (left_sign >= 0 && sign < 0) {
            int peak_pos = pos;
            for (int k = pos - q; k <= pos + q; ++k) {
                if (value[k - 1] > value[peak_pos - 1]) {
                    peak_pos = k;
                }
            }
            if (value[peak_pos - 1] > 3) {
                peaks.push_back (peak_pos - 1);
            }
            i += q;
        }
    }
}
