/**
 * @file LineMandelCalculator.cc
 * @author Monika Rosinská
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over lines
 * @date 2021-10-27
 */
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <cstdlib>
//#include <tclDecls.h>

#include <omp.h>
#include <random>

#include "LineMandelCalculator.h"


LineMandelCalculator::LineMandelCalculator (unsigned matrixBaseSize, unsigned limit) :
	BaseMandelCalculator(matrixBaseSize, limit, "LineMandelCalculator")
{
    data = (int *)(_mm_malloc(height * width * sizeof(int), 64));
    std::fill_n(data, width*height, limit);

    zImag = (float *)(_mm_malloc(width * sizeof(float), 64));
    zReal = (float *)(_mm_malloc(width * sizeof(float),64));
    startImag = (float *)(_mm_malloc(height * sizeof(float), 64));
    startReal = (float *)(_mm_malloc(width * sizeof(float), 64));

    passed = (bool *)(_mm_malloc(width * sizeof(bool), 64));
    std::fill_n(passed, width, false);
}

LineMandelCalculator::~LineMandelCalculator() {
    _mm_free(data);
    _mm_free(zImag);
    _mm_free(zReal);
    _mm_free(startReal);
    _mm_free(startImag);
    _mm_free(passed);

    data = NULL;
    zImag = NULL;
    zImag = NULL;
    startReal = NULL;
    startImag = NULL;
    passed = NULL;
}

int * LineMandelCalculator::calculateMandelbrot () {
    #pragma omp for simd
    for(int i = 0; i < height; ++i){
        startImag[i] = y_start + i * dy;
    }
    #pragma omp for simd
    for(int i = 0; i < width; ++i){
        startReal[i] = x_start + i * dx;
    }

    for (int i = 0; i < height; ++i) {
        float *zImagPtr = zImag;
        float *zRealPtr = zReal;
        float *startImagPtr = startImag + i;
        float *startRealPtr = startReal;
        int *dataPtr = data;
        bool *passedPtr = passed;

#pragma omp simd aligned(zRealPtr, zImagPtr, startImagPtr, startRealPtr:64)
        for (int j = 0; j < width; ++j) {
            *(zRealPtr + j) = *(startRealPtr + j);
            *(zImagPtr + j) = *startImagPtr;
            passed[j] = false;
        }
        allPassed = 0;
        for (int l = 0; l < limit; ++l) {
            #pragma omp simd simdlen(64) aligned(passedPtr, zImagPtr, zRealPtr, startImagPtr, startRealPtr, dataPtr:64)
            for (int j = 0; j < width; ++j) {
                int index = i * width + j;

                float r2 = *(zRealPtr + j) * *(zRealPtr + j);
                float i2 = *(zImagPtr + j) * *(zImagPtr + j);

                bool pass = r2 + i2 > 4.0f;

                *(dataPtr + index) = pass && !*(passedPtr + j) ? l : *(dataPtr + index);
                *(passedPtr + j) = *(passedPtr + j) ? *(passedPtr + j) : pass;

                *(zImagPtr + j) = 2.0f * *(zRealPtr + j) * *(zImagPtr + j) + *startImagPtr;
                *(zRealPtr + j) = r2 - i2 + *(startRealPtr + j);
            }
        }
    }
    return data;
}

