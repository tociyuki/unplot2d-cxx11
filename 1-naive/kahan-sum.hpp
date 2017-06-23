#pragma once

// one stage of ``Kahan summation algorithm''.
static inline void
kahan_sum (double const data, double& sum, double& carry)
{
    double const y = data - carry;
    double const t = sum + y;
    carry = (t - sum) - y;
    sum = t;
}
