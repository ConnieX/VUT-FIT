/**
 * @file BatchMandelCalculator.h
 * @author Monika Rosinská
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over small batches
 * @date 2021-11-09
 */
#ifndef BATCHMANDELCALCULATOR_H
#define BATCHMANDELCALCULATOR_H

#include <BaseMandelCalculator.h>

class BatchMandelCalculator : public BaseMandelCalculator
{
public:
    BatchMandelCalculator(unsigned matrixBaseSize, unsigned limit);
    ~BatchMandelCalculator();
    int * calculateMandelbrot();

private:
    int *data;
    float *zReal;
    float *zImag;
    float *startReal;
    float *startImag;
    bool *passed;

    int chunkSize;
    int allPassed;
};

#endif
