#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "main.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "../dsp/arm_math.h"
#include <unistd.h>
#include <stdint.h>
#include "../dsp/math_helper.h"

#include <fstream>

#define SEMIHOSTING 1
#if defined(SEMIHOSTING)
#endif


#define TEST_LENGTH_SAMPLES 500
/*

This SNR is a bit small. Need to understand why
this example is not giving better SNR ...

*/
#define SNR_THRESHOLD_F32 75.0f
#define BLOCK_SIZE 1

#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)
/* Must be a multiple of 16 */
#define NUM_TAPS_ARRAY_SIZE 73
#else
#define NUM_TAPS_ARRAY_SIZE 73
#endif


/* -------------------------------------------------------------------
 * The input signal and reference output (computed with MATLAB)
 * are defined externally in arm_fir_lpf_data.c.
 * ------------------------------------------------------------------- */



/* -------------------------------------------------------------------
 * Declare Test output buffer
 * ------------------------------------------------------------------- */

static float32_t testOutput[TEST_LENGTH_SAMPLES];

/* -------------------------------------------------------------------
 * Declare State buffer of size (numTaps + blockSize - 1)
 * ------------------------------------------------------------------- */
#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)
static float32_t firStateF32[2 * BLOCK_SIZE + NUM_TAPS_ARRAY_SIZE - 1];
#else
// static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS_ARRAY_SIZE - 1];
static float32_t firStateF32[NUM_TAPS_ARRAY_SIZE];

#endif

/* ----------------------------------------------------------------------
** FIR Coefficients buffer generated using fir1() MATLAB function.
** fir1(28, 6/24)
** ------------------------------------------------------------------- */
#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)
const float32_t firCoeffs32[NUM_TAPS_ARRAY_SIZE] = {
    -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
    -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
    +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
    +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f, 0.0f, 0.0f, 0.0f};
#else
const float32_t firCoeffs32[NUM_TAPS_ARRAY_SIZE] = {
 0.01178717, 0.01193876, 0.01208714, 0.01223223, 0.01237393,
       0.01251217, 0.01264687, 0.01277794, 0.01290531, 0.01302892,
       0.01314868, 0.01326452, 0.01337638, 0.01348419, 0.0135879 ,
       0.01368743, 0.01378273, 0.01387374, 0.01396042, 0.0140427 ,
       0.01412055, 0.01419391, 0.01426274, 0.01432701, 0.01438666,
       0.01444168, 0.01449202, 0.01453766, 0.01457856, 0.01461471,
       0.01464608, 0.01467266, 0.01469443, 0.01471137, 0.01472348,
       0.01473075, 0.01473318, 0.01473075, 0.01472348, 0.01471137,
       0.01469443, 0.01467266, 0.01464608, 0.01461471, 0.01457856,
       0.01453766, 0.01449202, 0.01444168, 0.01438666, 0.01432701,
       0.01426274, 0.01419391, 0.01412055, 0.0140427 , 0.01396042,
       0.01387374, 0.01378273, 0.01368743, 0.0135879 , 0.01348419,
       0.01337638, 0.01326452, 0.01314868, 0.01302892, 0.01290531,
       0.01277794, 0.01264687, 0.01251217, 0.01237393, 0.01223223,
       0.01208714, 0.01193876, 0.01178717};
#endif

/* ------------------------------------------------------------------
 * Global variables for FIR LPF Example
 * ------------------------------------------------------------------- */

uint32_t blockSize = BLOCK_SIZE;
// uint32_t numBlocks = TEST_LENGTH_SAMPLES / BLOCK_SIZE;
float32_t snr;

// SECTION - main
int main()
{
Timer timer;

        // ─── Fir ─────────────────────────────────────────────────────────────

    arm_fir_instance_f32 S;
    // arm_status status;
    float32_t *out;

    /* Initialize input and output buffer pointers */
    out = &testOutput[0];
    /* Call FIR init function to initialize the instance structure. */
    arm_fir_init_f32(&S, NUM_TAPS_ARRAY_SIZE, (float32_t *)&firCoeffs32[0], &firStateF32[0], blockSize);


    int numRows;


    std::vector<double> sin_noisy;
    std::vector<double> FirFilter;
    std::vector<double> time;
        

    // LINK -  fir filter
    numRows = 1000;
    double flag_time = 1.0;
    for (int j = 0; j < numRows; j++)
    {
        //wait 1 ms
        usleep(1000);
        // Get and display the elapsed time
        double elapsed_time = timer.elapsed();
        double t = elapsed_time/1.0e6;;
        if (t <= flag_time)
        {
            float32_t y = sin(2*PI*2*t)+0.2*sin(2*PI*50*t);
            sin_noisy.push_back(static_cast<double>(y));
            arm_fir_f32(&S, &y , out , blockSize);
            FirFilter.push_back(static_cast<double>((double)*out));
            time.push_back(static_cast<double>(t));
        }else
        {
            break;
        }
        
        


    }
    // store to txt file for plot by gnuplot
    storeDataToFile(time, sin_noisy , FirFilter );

    std::pair<double, double> result_raw = calculateMeanAndVariance(sin_noisy);
    std::pair<double, double> result_fir = calculateMeanAndVariance(FirFilter);

    std::cout << "Mean SIN: " << result_raw.first << std::endl;
    std::cout << "Variance SIN: " << result_raw.second << std::endl;

    std::cout << "Mean FIR: " << result_fir.first << std::endl;
    std::cout << "Variance FIR: " << result_fir.second << std::endl;

    plot_gnu();

    return 0;
}

//! SECTION