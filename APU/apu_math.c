/* @file   apu_math.c
*  @brief  Математичсекие функции
*
*  @detail Реализует функции математики.
*
*  @author Бугакова А.А.
*/


#include "apu_math.h"


double double_abs(double a)
{
    return a < 0 ? -a : a;
}

int sign(double a)
{
    if (double_abs(a) < 1e-12)
        return 0;
    if (a < 0)
        return -1;
    return 1;
}

double binpow(double a, int p)
{
    if (p < 0)
        return 1.0 / binpow(a, -p);
    if (p == 0)
        return 1;
    if (p == 1)
        return a;
    if (p % 2 == 0)
    {
        double temp = binpow(a, p / 2);
        return temp * temp;
    }
    else
        return a * binpow(a, p - 1);
}

double parab_integral(double(*func)(double t), double a, double b)
{
    int n = 100;
    double h = (b - a) / n;
    double I = func(a) + func(b) + 4 * func(a + h / 2);
    for (int i = 1; i < n; ++i)
        I = I + 4 * func(a + h * i + h / 2) + 2 * func(a + h * i);
    I = I * h / 6;
    return I;
}
