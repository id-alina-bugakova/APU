/* @file   apu_math.h
*  @brief  Объявление функций математики
*
*  @detail Файл содержит объявления функций математики (интеграл, возведение в степень).
*
*  @author Бугакова А.А.
*/

#pragma once

/* @brief  Алгоритм бинарного возведения в целую степень
*
*  @param  double a : основание степени
*  @param  int p : показатель степени
*  @return a^p
*/
double binpow(double a, int p);

/* @brief  Алгоритм вычисления интеграла методом парабол
*
*  @param  double(*func)(double t) : подынтегральная функция от t
*  @param  double a : начало интервала интегрирования
*  @param  double b : конец интервала интегрирования
*  @return Интеграл от a до b от func(t) по dt
*/
double parab_integral(double(*func)(double t), double a, double b);