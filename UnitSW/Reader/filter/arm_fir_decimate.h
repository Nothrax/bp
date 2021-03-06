//
// Created by Jakub Trubka on 04.02.20.
//
#pragma once

#ifndef BP_ARM_FIR_DECIMATE_H
#define BP_ARM_FIR_DECIMATE_H

#endif //BP_ARM_FIR_DECIMATE_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

typedef enum
{
    ARM_MATH_SUCCESS = 0,                /**< No error */
    ARM_MATH_ARGUMENT_ERROR = -1,        /**< One or more arguments are incorrect */
    ARM_MATH_LENGTH_ERROR = -2,          /**< Length of data buffer is incorrect */
    ARM_MATH_SIZE_MISMATCH = -3,         /**< Size of matrices is not compatible with the operation. */
    ARM_MATH_NANINF = -4,                /**< Not-a-number (NaN) or infinity is generated */
    ARM_MATH_SINGULAR = -5,              /**< Generated by matrix inversion if the input matrix is singular and cannot be inverted. */
    ARM_MATH_TEST_FAILURE = -6           /**< Test Failed  */
} arm_status;

typedef struct
{
    uint8_t M;                  /**< decimation factor. */
    uint16_t numTaps;           /**< number of coefficients in the filter. */
    q31_t *pCoeffs;             /**< points to the coefficient array. The array is of length numTaps.*/
    q31_t *pState;              /**< points to the state variable array. The array is of length numTaps+blockSize-1. */
} arm_fir_decimate_instance_q31;


arm_status arm_fir_decimate_init_q31(
        arm_fir_decimate_instance_q31 * S,
        uint16_t numTaps,
        uint8_t M,
        q31_t * pCoeffs,
        q31_t * pState,
        uint32_t blockSize);

void arm_fir_decimate_q31(
        const arm_fir_decimate_instance_q31 * S,
        q31_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize);
