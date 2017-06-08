#include "unplot2d.hpp"

void
linear_least_square_method_type::clear (void)
{
    n = 0;
    sum_x  = 0.0;
    sum_xx = 0.0;
    sum_y  = 0.0;
    sum_yx = 0.0;
}

void
linear_least_square_method_type::put (double x, double y)
{
    ++n;
    sum_x  += x;
    sum_xx += x * x;
    sum_y  += y;
    sum_yx += y * x;
}

void
linear_least_square_method_type::finalize (void)
{
    double const d = sum_xx * n - sum_x * sum_x;
    grad   = (sum_yx * n - sum_x * sum_y) / d;
    offset = (sum_xx * sum_y - sum_yx * sum_x) / d;
}
