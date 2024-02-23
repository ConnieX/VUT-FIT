/**
 * @file BatchMandelCalculator.cc
 * @author Monika Rosinská
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over small batches
 * @date 2021-11-09
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <cstdlib>

#include <stdlib.h>
#include <stdexcept>

#include "BatchMandelCalculator.h"

#include <random>

BatchMandelCalculator::BatchMandelCalculator (unsigned matrixBaseSize, unsigned limit) :
	BaseMandelCalculator(matrixBaseSize, limit, "BatchMandelCalculator")
{
    chunkSize = 64;
    data = (int *)(_mm_malloc(height * width * sizeof(int), 64));
    std::fill_n(data, width*height, limit);

    zImag = (float *)(_mm_malloc(chunkSize*chunkSize * sizeof(float), 64));
    zReal = (float *)(_mm_malloc(chunkSize*chunkSize * sizeof(float),64));
    startImag = (float *)(_mm_malloc(height* sizeof(float), 64));
    startReal = (float *)(_mm_malloc(width * sizeof(float), 64));

    passed = (bool *)(_mm_malloc(chunkSize*chunkSize * sizeof(bool), 64));
    std::fill_n(passed, chunkSize*chunkSize, false);
}

BatchMandelCalculator::~BatchMandelCalculator() {
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

int * BatchMandelCalculator::calculateMandelbrot () {
    int allPassedGoal = chunkSize*chunkSize;

    #pragma omp for simd
    for(int i = 0; i < height; ++i){
        startImag[i] = y_start + i * dy;
    }
    #pragma omp for simd
    for(int i = 0; i < width; ++i){
        startReal[i] = x_start + i * dx;
    }

    int *dataPtr = data;

    bool *passedPtr = passed;

    float *zImagPtr = zImag;
    float *zRealPtr = zReal;
    float *startImagPtr = startImag;
    float *startRealPtr = startReal;
    for (int yTop = 0; yTop < height; yTop += chunkSize){
        for(int xTop = 0; xTop < width; xTop += chunkSize){
            #pragma omp for simd
            for(int j = 0; j < chunkSize*chunkSize; j++) {
                zReal[j] = startReal[xTop+j%chunkSize];
                zImag[j] = startImag[yTop+j/chunkSize];
                passed[j] = false;
            }
            allPassed = 0;

            for(int l = 0; (l < limit); l++){
                for(int yChunk = 0; yChunk < chunkSize; yChunk++){
                #pragma omp for simd simdlen(64) reduction(+:allPassed) aligned(dataPtr,passedPtr,zImagPtr,zRealPtr,startImagPtr,startRealPtr:64)
                    for(int xChunk = 0; xChunk < chunkSize; xChunk++){
                        int chunkIndex = yChunk*chunkSize + xChunk;
                        int index = (yTop + yChunk)*width + xTop + xChunk;

                        float r2 = *(zRealPtr + chunkIndex) * *(zRealPtr + chunkIndex);
                        float i2 = *(zImagPtr + chunkIndex) * *(zImagPtr + chunkIndex);

                        bool pass = r2 + i2 > 4.0f;
                        *(dataPtr+index) = (pass && !*(passedPtr + chunkIndex)) ? l : *(dataPtr+index);
                        allPassed += (pass && !*(passedPtr + chunkIndex)) ? 1 : 0;
                        *(passedPtr + chunkIndex) = *(passedPtr + chunkIndex) ? *(passedPtr + chunkIndex) : pass;

                        *(zImagPtr + chunkIndex) = 2.0f * *(zRealPtr + chunkIndex) * *(zImagPtr + chunkIndex) + *(startImagPtr+ + yTop + yChunk);
                        *(zRealPtr + chunkIndex) = r2 - i2 + *(startRealPtr + xTop + xChunk);
                    }
                }
                if (allPassed == allPassedGoal){
                    break;
                }
            }
        }
    }
    return data;
}
