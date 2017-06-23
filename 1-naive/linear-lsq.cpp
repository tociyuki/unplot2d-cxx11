#include "unplot2d.hpp"
#include "kahan-sum.hpp"

// for major axis between scale ticks and scale label.

void
linear_least_square_method_type::clear (void)
{
    n = 0;
    sum_x  = 0.0;
    sum_xx = 0.0;
    sum_y  = 0.0;
    sum_yx = 0.0;
    carry_x  = 0.0;
    carry_xx = 0.0;
    carry_y  = 0.0;
    carry_yx = 0.0;
}

void
linear_least_square_method_type::put (double x, double y)
{
    ++n;
    kahan_sum (x,     sum_x,  carry_x);     //sum_x  += x;
    kahan_sum (x * x, sum_xx, carry_xx);    //sum_xx += x * x;
    kahan_sum (y,     sum_y,  carry_y);     //sum_y  += y;
    kahan_sum (y * x, sum_yx, carry_yx);    //sum_yx += y * x;
}

void
linear_least_square_method_type::finalize (void)
{
    double const d = sum_xx * n - sum_x * sum_x;
    grad   = (sum_yx * n - sum_x * sum_y) / d;
    offset = (sum_xx * sum_y - sum_yx * sum_x) / d;
}
