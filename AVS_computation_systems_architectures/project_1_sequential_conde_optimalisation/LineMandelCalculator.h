/**
 * @file LineMandelCalculator.h
 * @author Monika Rosinská
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over lines
 * @date 2021-10-27
 */

#include <BaseMandelCalculator.h>

class LineMandelCalculator : public BaseMandelCalculator
{
public:
    LineMandelCalculator(unsigned matrixBaseSize, unsigned limit);
    ~LineMandelCalculator();
    int *calculateMandelbrot();

private:
    int *data;
    float *zReal;
    float *zImag;
    float *startReal;
    float *startImag;
    bool *passed;

    int allPassed;
};
