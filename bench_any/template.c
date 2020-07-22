/* THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @file Template.c
 * @author Skynet
 * @brief Floating Point Core Benchmark.
 * @see https://fpbench.org/
 * @benchmark: FP Cores.
 * @researcher: Joseph David Tarango
 * @website http://www.cs.ucr.edu/~jtarango
 * @affiliation: University of California, Riverside
 * @date: JULY 18, 2020
 */

// C++ Compatibility
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// Definition of C/C++ library
#ifndef _FPCORE_C_
#define _FPCORE_C_

#ifdef _MSC_VER
    #pragma comment(linker,  "/HEAP:2147483648")
    #pragma comment(linker, "/STACK:2147483648")
#endif // _MSC_VER

// Includes
#include <fenv.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <climits>
#include <assert.h>

/********* CONFIGURATION INFORMATION BELOW *********/
// Source evaluation type for library.
// TYPE HEADER CONFIGURATION FILE SELECTION.
#ifdef CONFIG_F
    #pragma message("CONFIG_H == 1")
    const char filenameDaisy[] = "fpcore_dataset_f.csv";
    typedef float E_TYPE;
    // Destination compare type used for accuracy.
    typedef long double C_TYPE;
    #define CF 1
#else
    #define CF 0
#endif // CONFIG_F

#ifdef CONFIG_D
    #pragma message("CONFIG_H == 2")
    const char filenameDaisy[] = "fpcore_dataset_d.csv";
    typedef double E_TYPE;
    // Destination compare type used for accuracy.
    typedef long double C_TYPE;
    #define CD 1
#else
    #define CD 0
#endif // CONFIG_D

#ifdef CONFIG_LD
    #pragma message("CONFIG_H == 3")
    const char filenameDaisy[] = "fpcore_dataset_ld.csv";
    typedef long double E_TYPE;
    // Destination compare type used for accuracy.
    typedef long double C_TYPE;
    #define CLD 1
#else
    #define CLD 0
#endif // CONFIG_LD

#ifdef CONFIG_DYP
    #pragma message("CONFIG_H == 4")
    const char filenameDaisy[] = "fpcore_dataset_dyp.csv";
    typedef float E_TYPE; // @todo
    // Destination compare type used for accuracy.
    typedef long double C_TYPE;
    #define CDYP 1
#else
    #define CDYP 0
#endif // CONFIG_DYP

#ifdef CONFIG_PT
    #define CPT 1
    #pragma message("CONFIG_H == 5")
    const char filenameDaisy[] = "fpcore_dataset_pt.csv";
    #pragma message("_CONFIG_H_ == 1, F")
    typedef float E_TYPE; // @todo
    // Destination compare type used for accuracy.
    typedef long double C_TYPE;
#else
    #define CPT 0
#endif // CONFIG_PT

#ifdef CONFIG_UN
    #pragma message("CONFIG_H == 6")
    const char filenameDaisy[] = "fpcore_dataset_un.csv";
    typedef float E_TYPE; // @todo
    // Destination compare type used for accuracy.
    typedef long double C_TYPE;
    #define CPUN 1
#else
    #define CPUN 0
#endif // CONFIG_UN

#ifdef CONFIG_PT // PT

#endif // CONFIG_PT

#if ((CF == 0) && (CD == 0) && (CLD == 0) && (CDYP == 0) && (CPT == 0) && (CPUN == 0))
    // Constant file we append data to until we reach the variable maxima.
    #pragma message("CONFIG_H == DEFAULT")
    #ifndef filenameDaisy
    const char filenameDaisy[] = "fpcore_dataset_default.csv";
    #endif // filenameDaisy
    #ifndef E_TYPE
    typedef float E_TYPE;
    #endif // E_TYPE
    // Destination compare type used for accuracy.
    #ifndef C_TYPE
    typedef long double C_TYPE;
    #endif // C_TYPE
#endif // ((CF == 0) && (CD == 0) && (CLD == 0) && (CDYP ==0) && (CPT == 0) && (CPUN == 0))
/********* CONFIGURATION INFORMATION ABOVE *********/

// Used for test harness
const unsigned int testLoops = (UINT_MAX);
const int isDebugOn = 0;

typedef long unsigned int fpbenchTime_t; // Timer width for numerical with issues

// API Parameters based on functions.
#define CHAR_BUFFER_SIZE (1024) // Use fixed buffer sizes for secure code.
#define MIN_VARS (2) // Min number of variables for benchmarks.
#define MAX_VARS (20) // Max number of variables for benchmarks.
#define MIN_FUNCTIONS (1)
#define MAX_FUNCTIONS (77)
#define TIME_LIMIT_SHIFT (16) // Bit shift value for time.
#define TIME_LIMIT_SHIFT_CHECK (pow(2, ceil(log(TIME_LIMIT_SHIFT)/log(TIME_LIMIT_SHIFT)))) // Bit shift value for time.
#define TIME_LIMIT ((fpbenchTime_t)((fpbenchTime_t)1 << (fpbenchTime_t)TIME_LIMIT_SHIFT)-(fpbenchTime_t)1) // Timeout value in cycles. #define TIME_LIMIT INT_MAX

/* Main argv index positions.
* ./program  1   1.0  2.0    0
* ./program  6   1.0  2.0  3.0  4.0  5.0  6.0  7.0  8.0   9.0  1
*     ^0     ^1  ^2   ^3    ^4   ^5   ^6   ^7   ^8   ^9   ^10  ^11
*/
#define minInputs (MIN_VARS + 3)
#define maxInputs (MAX_VARS + 3)
#define inputStart (2)
// LIBRARY_MODE is set 1 one when the user does not want a main.

typedef enum{
    healthyDaisy = 0,
    faultDaisy = 1,
} statusDaisy; // library status

/****************************************************************************************/
// Pre-definitions functions from FPCores.
/****************************************************************************************/
/*daisy*/
E_TYPE daisy_ex0(E_TYPE x, E_TYPE y);
E_TYPE daisy_ex1(E_TYPE x, E_TYPE y);
E_TYPE daisy_ex2(E_TYPE radius, E_TYPE theta);
E_TYPE daisy_ex3(E_TYPE radius, E_TYPE theta);
E_TYPE daisy_ex4(E_TYPE t, E_TYPE resistance, E_TYPE frequency, E_TYPE inductance, E_TYPE maxVoltage);
E_TYPE daisy_ex5(E_TYPE a, E_TYPE b, E_TYPE c, E_TYPE d, E_TYPE e, E_TYPE f, E_TYPE g, E_TYPE h, E_TYPE i);
E_TYPE daisy_ex6(E_TYPE a, E_TYPE b, E_TYPE c, E_TYPE d, E_TYPE e, E_TYPE f, E_TYPE g, E_TYPE h, E_TYPE i);

/*fptaylor_extra*/
E_TYPE fptaylor_extra_ex0(E_TYPE x1, E_TYPE x2, E_TYPE x3, E_TYPE x4, E_TYPE x5, E_TYPE x6);
E_TYPE fptaylor_extra_ex1(E_TYPE x1, E_TYPE x2, E_TYPE x3, E_TYPE x4, E_TYPE x5, E_TYPE x6);
E_TYPE fptaylor_extra_ex2(E_TYPE x);
E_TYPE fptaylor_extra_ex5(E_TYPE x1, E_TYPE x2);
E_TYPE fptaylor_extra_ex7(E_TYPE x, E_TYPE y);
E_TYPE fptaylor_extra_ex8(E_TYPE x1, E_TYPE x2);
E_TYPE fptaylor_extra_ex9(E_TYPE x1, E_TYPE x2);
E_TYPE fptaylor_extra_ex11(E_TYPE x0, E_TYPE x1, E_TYPE x2);
E_TYPE fptaylor_extra_ex12(E_TYPE z);
E_TYPE fptaylor_extra_ex13(E_TYPE x, E_TYPE y);
E_TYPE fptaylor_extra_ex14(E_TYPE x, E_TYPE y);
E_TYPE fptaylor_extra_ex15(E_TYPE x1, E_TYPE x2);

/*fptaylor_real2float*/
E_TYPE fptaylor_real2float_ex3(E_TYPE x1, E_TYPE x2, E_TYPE x3, E_TYPE x4, E_TYPE x5, E_TYPE x6);
E_TYPE fptaylor_real2float_ex4(E_TYPE x1, E_TYPE x2);
E_TYPE fptaylor_real2float_ex5(E_TYPE x1, E_TYPE x2);
E_TYPE fptaylor_real2float_ex8(E_TYPE x1, E_TYPE x2, E_TYPE x3, E_TYPE x4, E_TYPE x5, E_TYPE x6);
E_TYPE fptaylor_real2float_ex9(E_TYPE x1, E_TYPE x2, E_TYPE x3, E_TYPE x4);
E_TYPE fptaylor_real2float_ex10(E_TYPE x1, E_TYPE x2, E_TYPE x3, E_TYPE x4, E_TYPE x5, E_TYPE x6);

/*fptaylor_tests*/
E_TYPE fptaylor_tests_ex0(E_TYPE t);
E_TYPE fptaylor_tests_ex1(E_TYPE x, E_TYPE y);
E_TYPE fptaylor_tests_ex2(E_TYPE x0, E_TYPE x1, E_TYPE x2);
E_TYPE fptaylor_tests_ex3(E_TYPE x0, E_TYPE x1, E_TYPE x2, E_TYPE x3, E_TYPE x4, E_TYPE x5, E_TYPE x6, E_TYPE x7);
E_TYPE fptaylor_tests_ex4(E_TYPE x, E_TYPE y);
E_TYPE fptaylor_tests_ex5(E_TYPE m0, E_TYPE m1, E_TYPE m2, E_TYPE w0, E_TYPE w1, E_TYPE w2, E_TYPE a0, E_TYPE a1, E_TYPE a2);
E_TYPE fptaylor_tests_ex6(E_TYPE x);
E_TYPE fptaylor_tests_ex7(E_TYPE x);
E_TYPE fptaylor_tests_ex8(E_TYPE x0, E_TYPE x1, E_TYPE x2, E_TYPE x3);
E_TYPE fptaylor_tests_ex9(E_TYPE x0, E_TYPE x1, E_TYPE x2, E_TYPE x3);

/*hamming_ch3*/
E_TYPE hamming_ch3_ex0(E_TYPE x);
E_TYPE hamming_ch3_ex4(E_TYPE x);
E_TYPE hamming_ch3_ex5(E_TYPE x);
E_TYPE hamming_ch3_ex7(E_TYPE x);
E_TYPE hamming_ch3_ex12(E_TYPE a, E_TYPE b, E_TYPE c);
E_TYPE hamming_ch3_ex13(E_TYPE a, E_TYPE b, E_TYPE c);
E_TYPE hamming_ch3_ex14(E_TYPE a, E_TYPE b2, E_TYPE c);
E_TYPE hamming_ch3_ex15(E_TYPE a, E_TYPE b2, E_TYPE c);

/*herbie*/
E_TYPE herbie_ex0(E_TYPE re, E_TYPE im);

/*premonious*/
E_TYPE premonious_ex0(E_TYPE n);
E_TYPE premonious_ex1(E_TYPE n);

/*rosa*/
E_TYPE rosa_ex0(E_TYPE u, E_TYPE v, E_TYPE T);
E_TYPE rosa_ex1(E_TYPE u, E_TYPE v, E_TYPE T);
E_TYPE rosa_ex2(E_TYPE u, E_TYPE v, E_TYPE T);
E_TYPE rosa_ex3(E_TYPE x1, E_TYPE x2, E_TYPE x3);
E_TYPE rosa_ex4(E_TYPE x1, E_TYPE x2, E_TYPE x3);
E_TYPE rosa_ex5(E_TYPE x1, E_TYPE x2);
E_TYPE rosa_ex6(E_TYPE v, E_TYPE w, E_TYPE r);
E_TYPE rosa_ex7(E_TYPE v, E_TYPE w, E_TYPE r);
E_TYPE rosa_ex8(E_TYPE v, E_TYPE w, E_TYPE r);
E_TYPE rosa_ex9(E_TYPE x);
E_TYPE rosa_ex10(E_TYPE x);
E_TYPE rosa_ex11(E_TYPE v);
E_TYPE rosa_ex12(E_TYPE x);
E_TYPE rosa_ex13(E_TYPE x);
E_TYPE rosa_ex14(E_TYPE x);
E_TYPE rosa_ex15(E_TYPE c);
E_TYPE rosa_ex16(E_TYPE x);
E_TYPE rosa_ex17(E_TYPE x);
E_TYPE rosa_ex18(E_TYPE x);
E_TYPE rosa_ex19(E_TYPE a, E_TYPE b, E_TYPE c);
E_TYPE rosa_ex20(E_TYPE a, E_TYPE b, E_TYPE c);
E_TYPE rosa_ex21(E_TYPE a, E_TYPE b, E_TYPE c);

/*rump*/
E_TYPE rump_ex1(E_TYPE a, E_TYPE b);
E_TYPE rump_ex2(E_TYPE a, E_TYPE b);

/*salsa*/
E_TYPE salsa_ex1(E_TYPE m, E_TYPE kp, E_TYPE ki, E_TYPE kd, E_TYPE c);
E_TYPE salsa_ex3(E_TYPE y, E_TYPE yd);
E_TYPE salsa_ex4(E_TYPE u);
E_TYPE salsa_ex6(E_TYPE a11, E_TYPE a22, E_TYPE a33, E_TYPE a44, E_TYPE b1, E_TYPE b2, E_TYPE b3, E_TYPE b4);
E_TYPE salsa_ex7(E_TYPE x0);
E_TYPE salsa_ex8(E_TYPE a11, E_TYPE a12, E_TYPE a13, E_TYPE a14, E_TYPE a21, E_TYPE a22, E_TYPE a23, E_TYPE a24, E_TYPE a31, E_TYPE a32, E_TYPE a33, E_TYPE a34, E_TYPE a41, E_TYPE a42, E_TYPE a43, E_TYPE a44, E_TYPE v1, E_TYPE v2, E_TYPE v3, E_TYPE v4);
E_TYPE salsa_ex9(E_TYPE Q11, E_TYPE Q12, E_TYPE Q13, E_TYPE Q21, E_TYPE Q22, E_TYPE Q23, E_TYPE Q31, E_TYPE Q32, E_TYPE Q33);
/****************************************************************************************/
// Application Programming Interface (API)
statusDaisy fpbench_API(int function, E_TYPE vars[MAX_VARS], E_TYPE *retValue, int debug, long double *runTime);

// Self Test.
void fpbench_testHarness(void);
/****************************************************************************************/

// Helper Functions.
E_TYPE fabsf_fpcore(E_TYPE x);
unsigned int min_uint(unsigned int a, unsigned int b);
double rand_double(void);
double gauss_rand(int select);
int validDataInput(double randomValue);
int timeOut(fpbenchTime_t tStart, fpbenchTime_t tStop);
E_TYPE isOuttaTime(int outtaTime, E_TYPE value, const char name[CHAR_BUFFER_SIZE]);

// MAIN Functions.
/****************************************************************************************/
/*DAISY*/
E_TYPE daisy_ex0(E_TYPE x, E_TYPE y) {
    E_TYPE retval = ((E_TYPE) sqrt(((E_TYPE) (((E_TYPE) (x * x)) + ((E_TYPE) (y * y))))));
    return retval;
}

E_TYPE daisy_ex1(E_TYPE x, E_TYPE y) {
    E_TYPE pi = 3.14159265359;
    E_TYPE radiant = ((E_TYPE) atan((y / x)));
    E_TYPE retval = ((E_TYPE) (radiant * (180.0 / pi)));
    return retval;
}

E_TYPE daisy_ex2(E_TYPE radius, E_TYPE theta) {
    E_TYPE pi = 3.14159265359;
    E_TYPE radiant = ((E_TYPE) (theta * (pi / 180.0)));
    E_TYPE retval = ((E_TYPE) (radius * ((E_TYPE) cos(radiant))));
    return retval;
}

E_TYPE daisy_ex3(E_TYPE radius, E_TYPE theta) {
    E_TYPE pi = 3.14159265359;
    E_TYPE radiant = ((E_TYPE) (theta * (pi / 180.0)));
    E_TYPE retval = ((E_TYPE) (radius * ((E_TYPE) sin(radiant))));
    return retval;
}

E_TYPE daisy_ex4(E_TYPE t, E_TYPE resistance, E_TYPE frequency, E_TYPE inductance, E_TYPE maxVoltage) {
    E_TYPE pi = 3.14159265359;
    E_TYPE impedance_re = resistance;
    E_TYPE impedance_im = ((E_TYPE) (((E_TYPE) (((E_TYPE) (2.0 * pi)) * frequency)) * inductance));
    E_TYPE denom = ((E_TYPE) (((E_TYPE) (impedance_re * impedance_re)) + ((E_TYPE) (impedance_im * impedance_im))));
    E_TYPE current_re = (((E_TYPE) (maxVoltage * impedance_re)) / denom);
    E_TYPE current_im = (((E_TYPE) -(((E_TYPE) (maxVoltage * impedance_im)))) / denom);
    E_TYPE maxCurrent = ((E_TYPE) sqrt(((E_TYPE) (((E_TYPE) (current_re * current_re)) + ((E_TYPE) (current_im * current_im))))));
    E_TYPE theta = ((E_TYPE) atan((current_im / current_re)));
    E_TYPE retval = ((E_TYPE) (maxCurrent * ((E_TYPE) cos(((E_TYPE) (((E_TYPE) (((E_TYPE) (((E_TYPE) (2.0 * pi)) * frequency)) * t)) + theta))))));
    return retval;
}

E_TYPE daisy_ex5(E_TYPE a, E_TYPE b, E_TYPE c, E_TYPE d, E_TYPE e, E_TYPE f, E_TYPE g, E_TYPE h, E_TYPE i) {
    E_TYPE retval = ((E_TYPE) (((E_TYPE) (((E_TYPE) (((E_TYPE) (((E_TYPE) (a * e)) * i)) + ((E_TYPE) (((E_TYPE) (b * f)) * g)))) + ((E_TYPE) (((E_TYPE) (c * d)) * h)))) - ((E_TYPE) (((E_TYPE) (((E_TYPE) (((E_TYPE) (c * e)) * g)) + ((E_TYPE) (((E_TYPE) (b * d)) * i)))) + ((E_TYPE) (((E_TYPE) (a * f)) * h))))));
    return retval;
}

E_TYPE daisy_ex6(E_TYPE a, E_TYPE b, E_TYPE c, E_TYPE d, E_TYPE e, E_TYPE f, E_TYPE g, E_TYPE h, E_TYPE i) {
    E_TYPE retval = ((E_TYPE) (((E_TYPE) (((E_TYPE) (a * ((E_TYPE) (e * i)))) + ((E_TYPE) (((E_TYPE) (g * ((E_TYPE) (b * f)))) + ((E_TYPE) (c * ((E_TYPE) (d * h)))))))) - ((E_TYPE) (((E_TYPE) (e * ((E_TYPE) (c * g)))) + ((E_TYPE) (((E_TYPE) (i * ((E_TYPE) (b * d)))) + ((E_TYPE) (a * ((E_TYPE) (f * h))))))))));
    return retval;
}

/*fptaylor_extra*/
E_TYPE fptaylor_extra_ex0(E_TYPE x1, E_TYPE x2, E_TYPE x3, E_TYPE x4, E_TYPE x5, E_TYPE x6) {
    E_TYPE retval =  ((((((-x2 * x3) - (x1 * x4)) + (x2 * x5)) + (x3 * x6)) - (x5 * x6)) + (x1 * (((((-x1 + x2) + x3) - x4) + x5) + x6)));
    return retval;
}

E_TYPE fptaylor_extra_ex1(E_TYPE x1, E_TYPE x2, E_TYPE x3, E_TYPE x4, E_TYPE x5, E_TYPE x6) {
    E_TYPE retval =  ((((((((x1 * x4) * (((((-x1 + x2) + x3) - x4) + x5) + x6)) + ((x2 * x5) * (((((x1 - x2) + x3) + x4) - x5) + x6))) + ((x3 * x6) * (((((x1 + x2) - x3) + x4) + x5) - x6))) + ((-x2 * x3) * x4)) + ((-x1 * x3) * x5)) + ((-x1 * x2) * x6)) + ((-x4 * x5) * x6));
    return retval;
}

E_TYPE fptaylor_extra_ex2(E_TYPE x) {
    E_TYPE retval =  (1.0 / (sqrt((x + 1.0)) + sqrt(x)));
    return retval;
}

E_TYPE fptaylor_extra_ex5(E_TYPE x1, E_TYPE x2) {
    E_TYPE retval =  (x1 + x2);
    return retval;
}

E_TYPE fptaylor_extra_ex7(E_TYPE x, E_TYPE y) {
    E_TYPE retval =  (x / (x + y));
    return retval;
}

E_TYPE fptaylor_extra_ex8(E_TYPE x1, E_TYPE x2) {
    E_TYPE retval =  sqrt(((x1 * x1) + (x2 * x2)));
    return retval;
}

E_TYPE fptaylor_extra_ex9(E_TYPE x1, E_TYPE x2) {
    E_TYPE retval =  sqrt(((x1 * x1) + (x2 * x2)));
    return retval;
}

E_TYPE fptaylor_extra_ex11(E_TYPE x0, E_TYPE x1, E_TYPE x2) {
    E_TYPE p0 = ((x0 + x1) - x2);
    E_TYPE p1 = ((x1 + x2) - x0);
    E_TYPE p2 = ((x2 + x0) - x1);
    E_TYPE retval =  ((p0 + p1) + p2);
    return retval;
}

E_TYPE fptaylor_extra_ex12(E_TYPE z) {
    E_TYPE retval =  (z / (z + 1.0));
    return retval;
}

E_TYPE fptaylor_extra_ex13(E_TYPE x, E_TYPE y) {
    E_TYPE t = (x * y);
    E_TYPE retval =  ((t - 1.0) / ((t * t) - 1.0));
    return retval;
}

E_TYPE fptaylor_extra_ex14(E_TYPE x, E_TYPE y) {
    E_TYPE retval =  sqrt((x + (y * y)));
    return retval;
}

E_TYPE fptaylor_extra_ex15(E_TYPE x1, E_TYPE x2) {
    E_TYPE a = (((x1 * x1) + x2) - 11.0);
    E_TYPE b = ((x1 + (x2 * x2)) - 7.0);
    E_TYPE retval =  ((a * a) + (b * b));
    return retval;
}

/*fptaylor_real2float*/
E_TYPE fptaylor_real2float_ex3(E_TYPE x1, E_TYPE x2, E_TYPE x3, E_TYPE x4, E_TYPE x5, E_TYPE x6) {
    E_TYPE retval =  ((((((-25.0 * ((x1 - 2.0) * (x1 - 2.0))) - ((x2 - 2.0) * (x2 - 2.0))) - ((x3 - 1.0) * (x3 - 1.0))) - ((x4 - 4.0) * (x4 - 4.0))) - ((x5 - 1.0) * (x5 - 1.0))) - ((x6 - 4.0) * (x6 - 4.0)));
    return retval;
}

E_TYPE fptaylor_real2float_ex4(E_TYPE x1, E_TYPE x2) {
    E_TYPE retval =  (-x1 - x2);
    return retval;
}

E_TYPE fptaylor_real2float_ex5(E_TYPE x1, E_TYPE x2) {
    E_TYPE retval =  (((-12.0 * x1) - (7.0 * x2)) + (x2 * x2));
    return retval;
}

E_TYPE fptaylor_real2float_ex8(E_TYPE x1, E_TYPE x2, E_TYPE x3, E_TYPE x4, E_TYPE x5, E_TYPE x6) {
    E_TYPE retval =  (((((x2 * x5) + (x3 * x6)) - (x2 * x3)) - (x5 * x6)) + (x1 * (((((-x1 + x2) + x3) - x4) + x5) + x6)));
    return retval;
}

E_TYPE fptaylor_real2float_ex9(E_TYPE x1, E_TYPE x2, E_TYPE x3, E_TYPE x4) {
    E_TYPE retval =  ((((((((x1 * x4) * (((-x1 + x2) + x3) - x4)) + (x2 * (((x1 - x2) + x3) + x4))) + (x3 * (((x1 + x2) - x3) + x4))) - ((x2 * x3) * x4)) - (x1 * x3)) - (x1 * x2)) - x4);
    return retval;
}

E_TYPE fptaylor_real2float_ex10(E_TYPE x1, E_TYPE x2, E_TYPE x3, E_TYPE x4, E_TYPE x5, E_TYPE x6) {
    E_TYPE retval =  ((((((((x1 * x4) * (((((-x1 + x2) + x3) - x4) + x5) + x6)) + ((x2 * x5) * (((((x1 - x2) + x3) + x4) - x5) + x6))) + ((x3 * x6) * (((((x1 + x2) - x3) + x4) + x5) - x6))) - ((x2 * x3) * x4)) - ((x1 * x3) * x5)) - ((x1 * x2) * x6)) - ((x4 * x5) * x6));
    return retval;
}

/*fptaylor_tests*/
E_TYPE fptaylor_tests_ex0(E_TYPE t) {
    E_TYPE retval =  (t / (t + 1.0));
    return retval;
}

E_TYPE fptaylor_tests_ex1(E_TYPE x, E_TYPE y) {
    E_TYPE t = (x * y);
    E_TYPE retval =  ((t - 1.0) / ((t * t) - 1.0));
    return retval;
}

E_TYPE fptaylor_tests_ex2(E_TYPE x0, E_TYPE x1, E_TYPE x2) {
    E_TYPE p0 = ((x0 + x1) - x2);
    E_TYPE p1 = ((x1 + x2) - x0);
    E_TYPE p2 = ((x2 + x0) - x1);
    E_TYPE retval =  ((p0 + p1) + p2);
    return retval;
}

E_TYPE fptaylor_tests_ex3(E_TYPE x0, E_TYPE x1, E_TYPE x2, E_TYPE x3, E_TYPE x4, E_TYPE x5, E_TYPE x6, E_TYPE x7) {
    E_TYPE retval =  (((((((x0 + x1) + x2) + x3) + x4) + x5) + x6) + x7);
    return retval;
}

E_TYPE fptaylor_tests_ex4(E_TYPE x, E_TYPE y) {
    E_TYPE retval =  ((x + y) / (x - y));
    return retval;
}

E_TYPE fptaylor_tests_ex5(E_TYPE m0, E_TYPE m1, E_TYPE m2, E_TYPE w0, E_TYPE w1, E_TYPE w2, E_TYPE a0, E_TYPE a1, E_TYPE a2) {
    E_TYPE v2 = ((w2 * (0.0 - m2)) * (-3.0 * ((1.0 * (a2 / w2)) * (a2 / w2))));
    E_TYPE v1 = ((w1 * (0.0 - m1)) * (-3.0 * ((1.0 * (a1 / w1)) * (a1 / w1))));
    E_TYPE v0 = ((w0 * (0.0 - m0)) * (-3.0 * ((1.0 * (a0 / w0)) * (a0 / w0))));
    E_TYPE retval =  (0.0 + ((v0 * 1.0) + ((v1 * 1.0) + ((v2 * 1.0) + 0.0))));
    return retval;
}

E_TYPE fptaylor_tests_ex6(E_TYPE x) {
    E_TYPE r1 = (x - 1.0);
    E_TYPE r2 = (x * x);
    E_TYPE retval =  (r1 / (r2 - 1.0));
    return retval;
}

E_TYPE fptaylor_tests_ex7(E_TYPE x) {
    E_TYPE retval =  (1.0 / (x + 1.0));
    return retval;
}

E_TYPE fptaylor_tests_ex8(E_TYPE x0, E_TYPE x1, E_TYPE x2, E_TYPE x3) {
    E_TYPE retval =  (((x0 + x1) + x2) + x3);
    return retval;
}

E_TYPE fptaylor_tests_ex9(E_TYPE x0, E_TYPE x1, E_TYPE x2, E_TYPE x3) {
    E_TYPE retval =  ((x0 + x1) + (x2 + x3));
    return retval;
}

/*hamming_ch3*/
E_TYPE hamming_ch3_ex0(E_TYPE x) {
    E_TYPE retval =  (sqrt((x + 1.0)) - sqrt(x));
    return retval;
}

E_TYPE hamming_ch3_ex4(E_TYPE x) {
    E_TYPE retval =  ((1.0 / sqrt(x)) - (1.0 / sqrt((x + 1.0))));
    return retval;
}

E_TYPE hamming_ch3_ex5(E_TYPE x) {
    E_TYPE retval =  ((1.0 / (x + 1.0)) - (1.0 / x));
    return retval;
}

E_TYPE hamming_ch3_ex7(E_TYPE x) {
    E_TYPE retval =  (((1.0 / (x + 1.0)) - (2.0 / x)) + (1.0 / (x - 1.0)));
    return retval;
}

E_TYPE hamming_ch3_ex12(E_TYPE a, E_TYPE b, E_TYPE c) {
    E_TYPE retval =  ((-b + sqrt(((b * b) - (4.0 * (a * c))))) / (2.0 * a));
    return retval;
}

E_TYPE hamming_ch3_ex13(E_TYPE a, E_TYPE b, E_TYPE c) {
    E_TYPE retval =  ((-b - sqrt(((b * b) - (4.0 * (a * c))))) / (2.0 * a));
    return retval;
}

E_TYPE hamming_ch3_ex14(E_TYPE a, E_TYPE b2, E_TYPE c) {
    E_TYPE retval =  ((-b2 + sqrt(((b2 * b2) - (a * c)))) / a);
    return retval;
}

E_TYPE hamming_ch3_ex15(E_TYPE a, E_TYPE b2, E_TYPE c) {
    E_TYPE retval =  ((-b2 - sqrt(((b2 * b2) - (a * c)))) / a);
    return retval;
}

/*herbie*/
E_TYPE herbie_ex0(E_TYPE re, E_TYPE im) {
    E_TYPE retval =  ( ((E_TYPE) 0.5) * (E_TYPE) sqrt(( ((E_TYPE)2.0) * (sqrt(((re * re) + (im * im))) + re))));
    return retval;
}

/*premonious*/
E_TYPE premonious_ex0(E_TYPE n)
{
    // Timeout Code
    int outtaTime;
    fpbenchTime_t tStart = 0;
    fpbenchTime_t tStop = 0;
    tStart = clock();

    E_TYPE retval;
    E_TYPE dppi = ((E_TYPE) M_PI);
    E_TYPE h = (dppi / n);
    E_TYPE t1 = 0.0;
    E_TYPE t2 = 0.0;
    E_TYPE s1 = 0.0;
    E_TYPE t1_1 = t1;
    E_TYPE i = 1;
    int VAR = (i <= n);
    while (VAR && (outtaTime==0)) {
        E_TYPE x = ((E_TYPE) (i * h));
        E_TYPE d1 = 1.0f;
        E_TYPE t1_2 = x;
        E_TYPE k = 1;
        int VAR_3 = (k <= 5.0);
        while (VAR_3 && (outtaTime==0)) {
            d1 = ((E_TYPE) (d1 * 2.0f));
            t1_2 = ((E_TYPE) (t1_2 + (((E_TYPE) sin(((E_TYPE) (d1 * x)))) / d1)));
            k = ((int64_t) (k + 1));
            VAR_3 = (k <= 5.0);
            // Timeout Code
            tStop = clock();
            outtaTime = timeOut(tStart, tStop);
            if (outtaTime != 0) {
                break;
            }
        }
        t2 = t1_2;
        E_TYPE s0 = ((E_TYPE) sqrt(((E_TYPE) (((E_TYPE) (h * h)) + ((E_TYPE) (((E_TYPE) (t2 - t1_1)) * ((E_TYPE) (t2 - t1_1))))))));
        s1 = ((E_TYPE) (s1 + s0));
        t1_1 = t2;
        i = ((int64_t) (i + 1));
        VAR = (i <= n);
        // Timeout Code
        tStop = clock();
        outtaTime = timeOut(tStart, tStop);
        if (outtaTime != 0) {
            break;
        }
    }
    // Timeout Code
    retval = isOuttaTime(outtaTime, s1, "premonious_ex0");
    return retval;
}

E_TYPE premonious_ex1(E_TYPE n)
{
    // Timeout Code
    int outtaTime;
    fpbenchTime_t tStart = 0;
    fpbenchTime_t tStop = 0;
    tStart = clock();

    E_TYPE retval;
    E_TYPE dppi = ((E_TYPE) acos(-1.0));
    E_TYPE h = (dppi / n);
    E_TYPE s1 = 0.0;
    E_TYPE t1 = 0.0;
    E_TYPE i = 1.0;
    int VAR = (i <= n);
    while (VAR && (outtaTime==0)) {
        E_TYPE x = ((E_TYPE) (i * h));
        E_TYPE d0 = 2.0f;
        E_TYPE t0 = x;
        E_TYPE k = 1.0;
        int VAR_2 = (k <= 5.0);
        while (VAR_2 && (outtaTime==0)) {
            E_TYPE d0_3 = ((E_TYPE) (2.0f * d0));
            E_TYPE t0_4 = ((E_TYPE) (t0 + (((E_TYPE) sin(((E_TYPE) (d0 * x)))) / d0)));
            E_TYPE k_5 = ((E_TYPE) (k + 1.0));
            d0 = d0_3;
            t0 = t0_4;
            k = k_5;
            VAR_2 = (k_5 <= 5.0);
            // Timeout Code
            tStop = clock();
            outtaTime = timeOut(tStart, tStop);
            if (outtaTime != 0) {
                break;
            }
        }
        E_TYPE t2 = t0;
        E_TYPE s0 = ((E_TYPE) sqrt(((E_TYPE) (((E_TYPE) (h * h)) + ((E_TYPE) (((E_TYPE) (t2 - t1)) * ((E_TYPE) (t2 - t1))))))));
        E_TYPE s1_1 = ((E_TYPE) (s1 + s0));
        E_TYPE x_8 = ((E_TYPE) (i * h));
        E_TYPE d0_9 = 2.0f;
        E_TYPE t0_10 = x_8;
        E_TYPE k_11 = 1.0;
        int VAR_12 = (k_11 <= 5.0);
        while (VAR_12 && (outtaTime==0)) {
            E_TYPE d0_13 = ((E_TYPE) (2.0f * d0_9));
            E_TYPE t0_14 = ((E_TYPE) (t0_10 + (((E_TYPE) sin(((E_TYPE) (d0_9 * x_8)))) / d0_9)));
            E_TYPE k_15 = ((E_TYPE) (k_11 + 1.0));
            d0_9 = d0_13;
            t0_10 = t0_14;
            k_11 = k_15;
            VAR_12 = (k_15 <= 5.0);

            // Timeout Code
            tStop = clock();
            outtaTime = timeOut(tStart, tStop);
            if (outtaTime != 0) {
                break;
            }
        }
        E_TYPE t2_7 = t0_10;
        E_TYPE t1_6 = t2_7;
        E_TYPE i_16 = ((E_TYPE) (i + 1.0));
        s1 = s1_1;
        t1 = t1_6;
        i = i_16;
        VAR = (i_16 <= n);
    }
    // Timeout Code
    retval = isOuttaTime(outtaTime, s1, "premonious_ex1");

    return retval;
}

/*rosa*/
E_TYPE rosa_ex0(E_TYPE u, E_TYPE v, E_TYPE T) {
    E_TYPE t1 = (331.4 + (0.6 * T));
    E_TYPE retval =  ((-t1 * v) / ((t1 + u) * (t1 + u)));
    return retval;
}

E_TYPE rosa_ex1(E_TYPE u, E_TYPE v, E_TYPE T) {
    E_TYPE t1 = (331.4 + (0.6 * T));
    E_TYPE retval =  ((-t1 * v) / ((t1 + u) * (t1 + u)));
    return retval;
}

E_TYPE rosa_ex2(E_TYPE u, E_TYPE v, E_TYPE T) {
    E_TYPE t1 = (331.4 + (0.6 * T));
    E_TYPE retval =  ((-t1 * v) / ((t1 + u) * (t1 + u)));
    return retval;
}

E_TYPE rosa_ex3(E_TYPE x1, E_TYPE x2, E_TYPE x3) {
    E_TYPE retval =  (((-(x1 * x2) - ((2.0 * x2) * x3)) - x1) - x3);
    return retval;
}

E_TYPE rosa_ex4(E_TYPE x1, E_TYPE x2, E_TYPE x3) {
    E_TYPE retval =  (((((((2.0 * x1) * x2) * x3) + ((3.0 * x3) * x3)) - (((x2 * x1) * x2) * x3)) + ((3.0 * x3) * x3)) - x2);
    return retval;
}

E_TYPE rosa_ex5(E_TYPE x1, E_TYPE x2) {
    E_TYPE t = ((((3.0 * x1) * x1) + (2.0 * x2)) - x1);
    E_TYPE t_42_ = ((((3.0 * x1) * x1) - (2.0 * x2)) - x1);
    E_TYPE d = ((x1 * x1) + 1.0);
    E_TYPE s = (t / d);
    E_TYPE s_42_ = (t_42_ / d);
    E_TYPE retval =  (x1 + (((((((((2.0 * x1) * s) * (s - 3.0)) + ((x1 * x1) * ((4.0 * s) - 6.0))) * d) + (((3.0 * x1) * x1) * s)) + ((x1 * x1) * x1)) + x1) + (3.0 * s_42_)));
    return retval;
}

E_TYPE rosa_ex6(E_TYPE v, E_TYPE w, E_TYPE r) {
    E_TYPE retval =  (((3.0 + (2.0 / (r * r))) - (((0.125 * (3.0 - (2.0 * v))) * (((w * w) * r) * r)) / (1.0 - v))) - 4.5);
    return retval;
}

E_TYPE rosa_ex7(E_TYPE v, E_TYPE w, E_TYPE r) {
    E_TYPE retval =  (((6.0 * v) - (((0.5 * v) * (((w * w) * r) * r)) / (1.0 - v))) - 2.5);
    return retval;
}

E_TYPE rosa_ex8(E_TYPE v, E_TYPE w, E_TYPE r) {
    E_TYPE retval =  (((3.0 - (2.0 / (r * r))) - (((0.125 * (1.0 + (2.0 * v))) * (((w * w) * r) * r)) / (1.0 - v))) - 0.5);
    return retval;
}

E_TYPE rosa_ex9(E_TYPE x) {
    E_TYPE r = 4.0;
    E_TYPE K = 1.11;
    E_TYPE retval =  ((r * x) / (1.0 + (x / K)));
    return retval;
}

E_TYPE rosa_ex10(E_TYPE x) {
    E_TYPE r = 4.0;
    E_TYPE K = 1.11;
    E_TYPE retval =  (((r * x) * x) / (1.0 + ((x / K) * (x / K))));
    return retval;
}

E_TYPE rosa_ex11(E_TYPE v) {
    E_TYPE p = 35000000.0;
    E_TYPE a = 0.401;
    E_TYPE b = 4.27e-05;
    E_TYPE t = 300.0;
    E_TYPE n = 1000.0;
    E_TYPE k = 1.3806503e-23;
    E_TYPE retval =  (((p + ((a * (n / v)) * (n / v))) * (v - (n * b))) - ((k * n) * t));
    return retval;
}

E_TYPE rosa_ex12(E_TYPE x) {
    E_TYPE retval =  (((x - (((x * x) * x) / 6.0)) + (((((x * x) * x) * x) * x) / 120.0)) - (((((((x * x) * x) * x) * x) * x) * x) / 5040.0));
    return retval;
}

E_TYPE rosa_ex13(E_TYPE x) {
    E_TYPE retval =  ((((1.0 + (0.5 * x)) - ((0.125 * x) * x)) + (((0.0625 * x) * x) * x)) - ((((0.0390625 * x) * x) * x) * x));
    return retval;
}

E_TYPE rosa_ex14(E_TYPE x) {
    E_TYPE retval =  ((0.954929658551372 * x) - (0.12900613773279798 * ((x * x) * x)));
    return retval;
}

E_TYPE rosa_ex15(E_TYPE c) {

    E_TYPE a = 3.0;
    E_TYPE b = 3.5;
    E_TYPE discr = ((b * b) - ((a * c) * 4.0));
    E_TYPE temp;
    if (0 || (((b * b) - (a * c)) > 10.0)) {
        E_TYPE temp_1;
        if (0 || (b > 0.0)) {
            temp_1 = ((c * 2.0) / (-b - sqrt(discr)));
        }
        else {
            E_TYPE temp_2;
            if (0 || (b < 0.0)) {
                temp_2 = ((-b + sqrt(discr)) / (a * 2.0));
            }
            else {
                temp_2 = ((-b + sqrt(discr)) / (a * 2.0));
            }
            temp_1 = temp_2;
        }
        temp = temp_1;
    }
    else {
        temp = ((-b + sqrt(discr)) / (a * 2.0));
    }

    E_TYPE retval =  temp;
    return retval;
}

E_TYPE rosa_ex16(E_TYPE x) {
    E_TYPE temp;
    if (0 || (((x * x) - x) >= 0.0)) {
        temp = (x / 10.0);
    }
    else {
        temp = ((x * x) + 2.0);
    }
    E_TYPE retval =  temp;
    return retval;
}

E_TYPE rosa_ex17(E_TYPE x) {
    E_TYPE temp;
    if (0 || (x < 1e-05)) {
        temp = (1.0 + (0.5 * x));
    }
    else {
        temp = sqrt((1.0 + x));
    }
    E_TYPE retval =  temp;
    return retval;
}

E_TYPE rosa_ex18(E_TYPE x) {
    E_TYPE temp;
    if (0 || (x < 0.0001)) {
        temp = (1.0 + (0.5 * x));
    }
    else {
        temp = sqrt((1.0 + x));
    }
    E_TYPE retval =  temp;
    return retval;
}

E_TYPE rosa_ex19(E_TYPE a, E_TYPE b, E_TYPE c) {
    E_TYPE s = (((a + b) + c) / 2.0);
    E_TYPE retval =  sqrt((((s * (s - a)) * (s - b)) * (s - c)));
    return retval;
}

E_TYPE rosa_ex20(E_TYPE a, E_TYPE b, E_TYPE c) {
    E_TYPE s = (((a + b) + c) / 2.0);
    E_TYPE retval =  sqrt((((s * (s - a)) * (s - b)) * (s - c)));
    return retval;
}

E_TYPE rosa_ex21(E_TYPE a, E_TYPE b, E_TYPE c) {
    E_TYPE s = (((a + b) + c) / 2.0);
    E_TYPE retval =  sqrt((((s * (s - a)) * (s - b)) * (s - c)));
    return retval;
}

/*rump*/
E_TYPE rump_ex1(E_TYPE a, E_TYPE b) {
    E_TYPE b2 = (b * b);
    E_TYPE b4 = (b2 * b2);
    E_TYPE b6 = (b4 * b2);
    E_TYPE b8 = (b4 * b4);
    E_TYPE a2 = (a * a);
    E_TYPE firstexpr = (((((11.0 * a2) * b2) - b6) - (121.0 * b4)) - 2.0);
    E_TYPE retval =  ((((333.75 * b6) + (a2 * firstexpr)) + (5.5 * b8)) + (a / (2.0 * b)));
    return retval;
}

E_TYPE rump_ex2(E_TYPE a, E_TYPE b) {
    E_TYPE b2 = (b * b);
    E_TYPE b4 = (b2 * b2);
    E_TYPE b6 = (b4 * b2);
    E_TYPE b8 = (b4 * b4);
    E_TYPE a2 = (a * a);
    E_TYPE firstexpr = ((((11.0 * a2) * b2) - (121.0 * b4)) - 2.0);
    E_TYPE retval =  (((((333.75 - a2) * b6) + (a2 * firstexpr)) + (5.5 * b8)) + (a / (2.0 * b)));
    return retval;
}

/*salsa*/
E_TYPE fabsf_fpcore(E_TYPE x)
{
    E_TYPE retval;
    if (x >0) {
        retval = x;
    }
    else{
        retval=(-1 * x);
    }
    return retval;
}

E_TYPE salsa_ex1(E_TYPE m, E_TYPE kp, E_TYPE ki, E_TYPE kd, E_TYPE c) {
    // Timeout Code
    int outtaTime;
    fpbenchTime_t tStart = 0;
    fpbenchTime_t tStop = 0;
    tStart = clock();

    E_TYPE retval;
    E_TYPE dt = 0.5;
    E_TYPE invdt = (1.0 / dt);
    E_TYPE e = 0.0;
    E_TYPE p = 0.0;
    E_TYPE i = 0.0;
    E_TYPE d = 0.0;
    E_TYPE r = 0.0;
    E_TYPE m_1 = m;
    E_TYPE eold = 0.0;
    E_TYPE t = 0.0;
    int test = (t < 100.0);
    while ((0 || test) && (outtaTime==0)) {
        e = (c - m_1);
        p = (kp * e);
        i = (i + ((ki * dt) * e));
        d = ((kd * invdt) * (e - eold));
        r = ((p + i) + d);
        m_1 = (m_1 + (0.01 * r));
        eold = e;
        t = (t + dt);
        test = (t < 100.0);
        // Timeout code
        tStop = clock();
        outtaTime = timeOut(tStart, tStop);
        if (outtaTime != 0) {
            break;
        }
    }

    // Timeout Code
    retval = isOuttaTime(outtaTime, m_1, "salsa_ex1");
    return retval;
}

E_TYPE salsa_ex3(E_TYPE y, E_TYPE yd) {
    // Timeout Code
    int outtaTime;
    fpbenchTime_t tStart = 0;
    fpbenchTime_t tStop = 0;
    tStart = clock();

    E_TYPE retval;
    E_TYPE eps = 0.01f;
    // @todo: Checked FPCore "Lead-lag System", perhaps a human bug? E_TYPE Dc = -1280.0f;
    E_TYPE Ac00 = 0.499f;
    E_TYPE Ac01 = -0.05f;
    E_TYPE Ac10 = 0.01f;
    E_TYPE Ac11 = 1.0f;
    E_TYPE Bc0 = 1.0f;
    E_TYPE Bc1 = 0.0f;
    // @todo: Checked FPCore "Lead-lag System", perhaps a human bug? E_TYPE Cc0 = 564.48f;
    // @todo: Checked FPCore "Lead-lag System", perhaps a human bug? E_TYPE Cc1 = 0.0f;
    E_TYPE yc = 0.0f;
    // @todo: Checked FPCore "Lead-lag System", perhaps a human bug? E_TYPE u = 0.0f;
    E_TYPE xc0 = 0.0f;
    E_TYPE xc1 = 0.0f;
    E_TYPE i = 0.0f;
    E_TYPE e = 1.0f;
    int test = (e > eps);
    while ((0 || test)&& (outtaTime==0))
    {
        E_TYPE v = (y - yd);
        E_TYPE temp;
        if (0 || (v < -1.0f)) {
            temp = -1.0f;
        } else {
            E_TYPE temp_1;
            if (0 || (1.0f < v)) {
                temp_1 = 1.0f;
            } else {
                temp_1 = v;
            }
            temp = temp_1;
        }
        yc = temp;
        // @todo: Checked FPCore "Lead-lag System", perhaps a human bug? u = ((Cc0 * xc0) + ((Cc1 * xc1) + (Dc * yc)));
        xc0 = ((Ac00 * xc0) + ((Ac01 * xc1) + (Bc0 * yc)));
        xc1 = ((Ac10 * xc0) + ((Ac11 * xc1) + (Bc1 * yc)));
        i = (i + 1.0f);
        e = fabsf_fpcore((yc - xc1));
        test = (e > eps);

        // Timeout code
        tStop = clock();
        outtaTime = timeOut(tStart, tStop);
        if (outtaTime != 0) {
            break;
        }
    }

    // Timeout Code
    retval = isOuttaTime(outtaTime, xc1, "salsa_ex3");
    return retval;
}

E_TYPE salsa_ex4(E_TYPE u) {
    // Timeout Code
    int outtaTime;
    fpbenchTime_t tStart = 0;
    fpbenchTime_t tStop = 0;
    tStart = clock();

    E_TYPE retval;
    E_TYPE a = 0.25;
    E_TYPE b = 5000.0;
    E_TYPE n = 25.0;
    E_TYPE h = ((b - a) / n);
    E_TYPE xb = 0.0;
    E_TYPE r = 0.0;
    E_TYPE xa = 0.25;
    int test = (xa < 5000.0);
    while ((0 || test) && (outtaTime==0))
    {
        E_TYPE v = (xa + h);
        E_TYPE temp;
        if (0 || (v > 5000.0)) {
            temp = 5000.0;
        } else {
            temp = v;
        }
        xb = temp;
        E_TYPE gxa = (u / ((((((0.7 * xa) * xa) * xa) - ((0.6 * xa) * xa)) + (0.9 * xa)) - 0.2));
        E_TYPE gxb = (u / ((((((0.7 * xb) * xb) * xb) - ((0.6 * xb) * xb)) + (0.9 * xb)) - 0.2));
        r = (r + (((gxa + gxb) * 0.5) * h));
        xa = (xa + h);
        test = (xa < 5000.0);

        // Timeout code
        tStop = clock();
        outtaTime = timeOut(tStart, tStop);
        if (outtaTime != 0) {
            break;
        }
    }

    // Timeout Code
    retval = isOuttaTime(outtaTime, r, "salsa_ex4");
    return retval;
}

E_TYPE salsa_ex6(E_TYPE a11, E_TYPE a22, E_TYPE a33, E_TYPE a44, E_TYPE b1, E_TYPE b2, E_TYPE b3, E_TYPE b4) {
    // Timeout Code
    int outtaTime;
    fpbenchTime_t tStart = 0;
    fpbenchTime_t tStop = 0;
    tStart = clock();

    E_TYPE retval;
    E_TYPE eps = 1e-17f;
    E_TYPE x_n1 = 0.0f;
    E_TYPE x_n2 = 0.0f;
    E_TYPE x_n3 = 0.0f;
    E_TYPE x_n4 = 0.0f;
    E_TYPE i = 0.0f;
    E_TYPE e = 1.0f;
    E_TYPE x1 = 0.0f;
    E_TYPE x2 = 0.0f;
    E_TYPE x3 = 0.0f;
    E_TYPE x4 = 0.0f;
    int test = (e > eps);
    while ((0 || test) && (outtaTime==0)) {
        x_n1 = ((((b1 / a11) - ((0.1f / a11) * x2)) - ((0.2f / a11) * x3)) + ((0.3f / a11) * x4));
        x_n2 = ((((b2 / a22) - ((0.3f / a22) * x1)) + ((0.1f / a22) * x3)) - ((0.2f / a22) * x4));
        x_n3 = ((((b3 / a33) - ((0.2f / a33) * x1)) + ((0.3f / a33) * x2)) - ((0.1f / a33) * x4));
        x_n4 = ((((b4 / a44) + ((0.1f / a44) * x1)) - ((0.2f / a44) * x2)) - ((0.3f / a44) * x3));
        i = (i + 1.0f);
        e = fabsf_fpcore((x_n4 - x4));
        x1 = x_n1;
        x2 = x_n2;
        x3 = x_n3;
        x4 = x_n4;
        test = (e > eps);

        // Timeout code
        tStop = clock();
        outtaTime = timeOut(tStart, tStop);
        if (outtaTime != 0) {
            break;
        }
    }

    // Timeout Code
    retval = isOuttaTime(outtaTime, x2, "salsa_ex6");
    return retval;
}

E_TYPE salsa_ex7(E_TYPE x0) {
    // Timeout Code
    int outtaTime;
    fpbenchTime_t tStart = 0;
    fpbenchTime_t tStop = 0;
    tStart = clock();

    E_TYPE retval;
    E_TYPE eps = 0.0005f;
    E_TYPE x_n = 0.0f;
    E_TYPE e = 1.0f;
    E_TYPE x = 0.0f;
    E_TYPE i = 0.0f;
    int test = ((e > eps) && (i < 100000.0f));
    while ((0 || test) && (outtaTime==0)) {
        E_TYPE f = (((((((x * x) * ((x * x) * x)) - ((10.0f * x) * ((x * x) * x))) + ((40.0f * x) * (x * x))) - ((80.0f * x) * x)) + (80.0f * x)) - 32.0f);
        E_TYPE ff = ((((((5.0f * x) * ((x * x) * x)) - ((40.0f * x) * (x * x))) + ((120.0f * x) * x)) - (160.0f * x)) + 80.0f);
        x_n = (x - (f / ff));
        e = fabsf_fpcore((x - x_n));
        x = x_n;
        i = (i + 1.0f);
        test = ((e > eps) && (i < 100000.0f));

        // Timeout code
        tStop = clock();
        outtaTime = timeOut(tStart, tStop);
        if (outtaTime != 0) {
            break;
        }
    }

    // Timeout Code
    retval = isOuttaTime(outtaTime, x, "salsa_ex7");
    return retval;
}

E_TYPE salsa_ex8(E_TYPE a11, E_TYPE a12, E_TYPE a13, E_TYPE a14, E_TYPE a21, E_TYPE a22, E_TYPE a23, E_TYPE a24, E_TYPE a31, E_TYPE a32, E_TYPE a33, E_TYPE a34, E_TYPE a41, E_TYPE a42, E_TYPE a43, E_TYPE a44, E_TYPE v1, E_TYPE v2, E_TYPE v3, E_TYPE v4) {
    // Timeout Code
    int outtaTime;
    fpbenchTime_t tStart = 0;
    fpbenchTime_t tStop = 0;
    tStart = clock();

    E_TYPE eps = 0.0005f;
    E_TYPE vx = 0.0f;
    E_TYPE vy = 0.0f;
    E_TYPE vz = 0.0f;
    E_TYPE vw = 0.0f;
    E_TYPE i = 0.0f;
    E_TYPE v1_1 = v1;
    E_TYPE v2_1 = v2;
    E_TYPE v3_1 = v3;
    E_TYPE v4_1 = v4;
    E_TYPE e = 1.0f;
    E_TYPE retval;

    int test = (e > eps);
    while ((0 || test) && (outtaTime ==0)) {
        vx = (((a11 * v1_1) + (a12 * v2_1)) + ((a13 * v3_1) + (a14 * v4_1)));
        vy = (((a21 * v1_1) + (a22 * v2_1)) + ((a23 * v3_1) + (a24 * v4_1)));
        vz = (((a31 * v1_1) + (a32 * v2_1)) + ((a33 * v3_1) + (a34 * v4_1)));
        vw = (((a41 * v1_1) + (a42 * v2_1)) + ((a43 * v3_1) + (a44 * v4_1)));
        i = (i + 1.0f);
        v1_1 = (vx / vw);
        v2_1 = (vy / vw);
        v3_1 = (vz / vw);
        v4_1 = 1.0f;
        e = fabsf_fpcore((1.0f - v1_1));
        test = (e > eps);
        // Timeout code
        tStop = clock();
        outtaTime = timeOut(tStart, tStop);
        if (outtaTime != 0) {
            break;
        }
    }

    // Timeout Code
    retval = isOuttaTime(outtaTime, v1_1, "salsa_ex8");
    return retval;
}

E_TYPE salsa_ex9(E_TYPE Q11, E_TYPE Q12, E_TYPE Q13, E_TYPE Q21, E_TYPE Q22, E_TYPE Q23, E_TYPE Q31, E_TYPE Q32, E_TYPE Q33) {
    // Timeout Code
    int outtaTime;
    fpbenchTime_t tStart = 0;
    fpbenchTime_t tStop = 0;
    tStart = clock();

    E_TYPE retval;
    E_TYPE eps = 5e-06f;
    E_TYPE h1 = 0.0f;
    E_TYPE h2 = 0.0f;
    E_TYPE h3 = 0.0f;
    E_TYPE qj1 = Q31;
    E_TYPE qj2 = Q32;
    E_TYPE qj3 = Q33;
    E_TYPE r1 = 0.0f;
    E_TYPE r2 = 0.0f;
    E_TYPE r3 = 0.0f;
    E_TYPE r = (((qj1 * qj1) + (qj2 * qj2)) + (qj3 * qj3));
    E_TYPE rjj = 0.0f;
    E_TYPE e = 10.0f;
    E_TYPE i = 1.0f;
    E_TYPE rold = sqrt(r);
    int test = (e > eps);
    while ((0 || test) && (outtaTime==0))
    {
        h1 = (((Q11 * qj1) + (Q21 * qj2)) + (Q31 * qj3));
        h2 = (((Q12 * qj1) + (Q22 * qj2)) + (Q32 * qj3));
        h3 = (((Q13 * qj1) + (Q23 * qj2)) + (Q33 * qj3));
        qj1 = (qj1 - (((Q11 * h1) + (Q12 * h2)) + (Q13 * h3)));
        qj2 = (qj2 - (((Q21 * h1) + (Q22 * h2)) + (Q23 * h3)));
        qj3 = (qj3 - (((Q31 * h1) + (Q32 * h2)) + (Q33 * h3)));
        r1 = (r1 + h1);
        r2 = (r2 + h2);
        r3 = (r3 + h3);
        r = (((qj1 * qj1) + (qj2 * qj2)) + (qj3 * qj3));
        rjj = sqrt(r);
        e = fabsf_fpcore((1.0f - (rjj / rold)));
        i = (i + 1.0f);
        rold = rjj;
        test = (e > eps);

        // Timeout code
        tStop = clock();
        outtaTime = timeOut(tStart, tStop);
        if (outtaTime != 0) {
            break;
        }
    }
    // Timeout Code
    retval = isOuttaTime(outtaTime, qj1, "salsa_ex9");
    return retval;
}

/* Function to check passed time */
int timeOut(fpbenchTime_t tStart, fpbenchTime_t tStop)
{
    if ((((fpbenchTime_t) tStop - (fpbenchTime_t) tStart) > ((fpbenchTime_t) TIME_LIMIT)) ||
        (tStart > tStop)) {
        if (((fpbenchTime_t) tStop >> ((fpbenchTime_t) TIME_LIMIT_SHIFT_CHECK)) > 0) {
            printf("...waiting...\n");
        }
        return 1;
    }
    return 0;
}
/****************************************************************************************/
/* Determines the size of a file and reports size */
long signed int getFileSize(FILE *fileContext)
{
    long signed int prev;
    long signed int size;
    prev = ftell(fileContext);
    fseek(fileContext, 0L, SEEK_END);
    size = ftell(fileContext);
    fseek(fileContext, prev,SEEK_SET); // Go back to where we were.
    if (size == (long signed int) -1) {
        printf("ERROR ftell.\n");
        assert(0);
    }
    return size;
}

/* Function to check if we are out of time */
E_TYPE isOuttaTime(int outtaTime, E_TYPE value, const char name[CHAR_BUFFER_SIZE])
{
    E_TYPE returnValue = 0;
    // Timeout Code
    if (outtaTime == 0) {
        returnValue = value;
    }
    else {
        printf("*ERROR! TIME OUT in %s.*\n", name);
        returnValue = nan("TIME_LIMIT");
    }
    return returnValue;
}

/* Function to select min of two unsigned integers */
unsigned int min_uint(unsigned int a, unsigned int b)
{
    if (a >= b){
        return a;
    }
    return b;
}

/* Ensure random values are valid floats. */
int validDataInput(double randomValue)
{
    int valid = 0;
    switch (fpclassify(randomValue)) {
        case FP_NORMAL:
            valid = 1;
            break;
        case FP_SUBNORMAL:
            valid = 0;
            break;
        case FP_INFINITE:
            valid = 0;
            break;
        case FP_NAN:
            valid = 0;
            break;
        case FP_ZERO:
            valid = 0;
            break;
        default:
            valid = 0;
    }
    return valid;
}

/* Random number generator.
 * Generate a uniformly distributed random value.
 * References:
 *  Knuth Sec. 3.4.1 p. 117
 *  Box and Muller, ``A Note on the Generation of Random Normal Deviates''
 *  Marsaglia and Bray, ``A Convenient Method for Generating Normal Variables''
 *  Abramowitz and Stegun, Handbook of Mathematical Functions
 *  Press et al., Numerical Recipes in C Sec. 7.2 pp. 288-290
 */
double gauss_rand(int select)
{
    const int NSUM = 25; // Used for random number generators.
    double PI = acos(-1.0); // Exact PI number from math functions.
    double x, X, Z, selected;
    int i;
    static double U, U1, U2, V, V1, V2, S;
    static int phase = 0;
    static int phase2 = 0;
    long long int c0, c1, c2, c3, c;

    switch(select)
    {
        case INT_MAX:
            // Herb-grind, paper defined. Note: The randomness is not adequate for large machine learning data sets.
            c0 = rand() & 0xffff;
            c1 = rand() & 0xffff;
            c2 = rand() & 0xffff;
            c3 = rand() & 0xffff;
            c = ((c3 << 48) | (c2 << 32) | (c1 << 16) | c0);
            memcpy(&selected, &c, sizeof(c));
            break;
        case INT_MAX-1:
            selected = ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
        case 1:
            // Exploit the Central Limit Theorem (law of large numbers) and add up several uniformly-distributed random numbers.
            x = 0;
            for (i = 0; i < NSUM; i++) {
                x += (double) rand() / RAND_MAX;
            }
            x -= NSUM / 2.0;
            x /= sqrt(NSUM / 12.0);
            selected = x;
            break;
        case 2:
            // Use a method described by Abramowitz and Stegun.
            if (phase == 0) {
                U = (rand() + 1.) / (RAND_MAX + 2.);
                V = rand() / (RAND_MAX + 1.);
                Z = sqrt(-2 * log(U)) * sin(2 * PI * V);
            }
            else {
                Z = sqrt(-2 * log(U)) * cos(2 * PI * V);
            }

            phase = 1 - phase;

            selected = Z;
        case 3:
        default:
            // Use a method discussed in Knuth and due originally to Marsaglia.
            if (phase2 == 0) {
                do {
                    U1 = (double) rand() / RAND_MAX;
                    U2 = (double) rand() / RAND_MAX;

                    V1 = 2 * U1 - 1;
                    V2 = 2 * U2 - 1;
                    S = V1 * V1 + V2 * V2;
                } while (S >= 1 || S == 0);

                X = V1 * sqrt(-2 * log(S) / S);
            }
            else {
                X = V2 * sqrt(-2 * log(S) / S);
            }

            phase2 = 1 - phase2;
            selected = X;
    }

    return selected;
}

/* Function to randomly generate double values */
double rand_double(void)
{
    static unsigned int ranOnce = 0;
    static int pingPong = 0;
    double v = 0;
    if (ranOnce == 0){
        printf("  Random Seed Set\n");
        ranOnce++;
        srand(time(NULL)^clock()>>1); // srand(clock()) or srand(NULL)
    }
    if (pingPong >= 3) {
        pingPong = 0;
    }
    pingPong++;
    v = gauss_rand(pingPong);
    return v;
}

void fpbench_map(int selected, char benchmarkName[CHAR_BUFFER_SIZE])
{
    // 1-7 daisy
    if (selected >= 1 && selected <= 7){
        snprintf(benchmarkName, ((sizeof(char) * CHAR_BUFFER_SIZE) - 1), "daisy");
    }
    // 8-19 fptaylor_extra
    else if (selected >= 8 && selected <= 19){
        snprintf(benchmarkName, ((sizeof(char) * CHAR_BUFFER_SIZE) - 1), "fptaylor_extra");
    }
    // 20-25 fptaylor_real2float
    else if (selected >= 20 && selected <= 25){
        snprintf(benchmarkName, ((sizeof(char) * CHAR_BUFFER_SIZE) - 1), "fptaylor_real2float");
    }
    // 26-35 fptaylor_tests
    else if (selected >= 26 && selected <= 35){
        snprintf(benchmarkName, ((sizeof(char) * CHAR_BUFFER_SIZE) - 1), "fptaylor_tests");
    }
    // 36-43 hamming_ch3
    else if (selected >= 36 && selected <= 43){
        snprintf(benchmarkName, ((sizeof(char) * CHAR_BUFFER_SIZE) - 1), "hamming_ch3");
    }
    // 44 herbie
    else if (selected == 44) {
        snprintf(benchmarkName, ((sizeof(char) * CHAR_BUFFER_SIZE) - 1), "herbie");
    }
    // 45-46 premonious
    else if (selected >= 45 && selected <= 46){
        snprintf(benchmarkName, ((sizeof(char) * CHAR_BUFFER_SIZE) - 1), "premonious");
    }
    // 47-68 rosa
    else if (selected >= 47 && selected <= 68){
        snprintf(benchmarkName, ((sizeof(char) * CHAR_BUFFER_SIZE) - 1), "rosa");
    }
    // 69-70 rump
    else if (selected >= 69 && selected <= 70){
        snprintf(benchmarkName, ((sizeof(char) * CHAR_BUFFER_SIZE) - 1), "rump");
    }
    // 71-77 salsa
    else if (selected >= 71 && selected <= 77){
        snprintf(benchmarkName, ((sizeof(char) * CHAR_BUFFER_SIZE) - 1), "salsa");
    }
    else {
        snprintf(benchmarkName, ((sizeof(char) * CHAR_BUFFER_SIZE) - 1), "unknown");
    }
    return;
}

statusDaisy fpcore_executeBenchmark(int function, C_TYPE *inputs, C_TYPE *returnValue)
{
    C_TYPE retVal = 0;
    statusDaisy status = healthyDaisy;

    switch(function) {
        case 1:
            retVal = daisy_ex0(inputs[0], inputs[1]);
            break;
        case 2:
            retVal = daisy_ex1(inputs[0], inputs[1]);
            break;
        case 3:
            retVal = daisy_ex2(inputs[0], inputs[1]);
            break;
        case 4:
            retVal = daisy_ex3(inputs[0], inputs[1]);
            break;
        case 5:
            retVal = daisy_ex4(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4]);
            break;
        case 6:
            retVal = daisy_ex5(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7], inputs[8]);
            break;
        case 7:
            retVal = daisy_ex6(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7], inputs[8]);
            break;
        case 8:
            retVal = fptaylor_extra_ex0(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);
            break;
        case 9:
            retVal = fptaylor_extra_ex1(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);
            break;
        case 10:
            retVal = fptaylor_extra_ex2(inputs[0]);
            break;
        case 11:
            retVal = fptaylor_extra_ex5(inputs[0], inputs[1]);
            break;
        case 12:
            retVal = fptaylor_extra_ex7(inputs[0], inputs[1]);
            break;
        case 13:
            retVal = fptaylor_extra_ex8(inputs[0], inputs[1]);
            break;
        case 14:
            retVal = fptaylor_extra_ex9(inputs[0], inputs[1]);
            break;
        case 15:
            retVal = fptaylor_extra_ex11(inputs[0], inputs[1], inputs[2]);
            break;
        case 16:
            retVal = fptaylor_extra_ex12(inputs[0]);
            break;
        case 17:
            retVal = fptaylor_extra_ex13(inputs[0], inputs[1]);
            break;
        case 18:
            retVal = fptaylor_extra_ex14(inputs[0], inputs[1]);
            break;
        case 19:
            retVal = fptaylor_extra_ex15(inputs[0], inputs[1]);
            break;
        case 20:
            retVal = fptaylor_real2float_ex3(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);
            break;
        case 21:
            retVal = fptaylor_real2float_ex4(inputs[0], inputs[1]);
            break;
        case 22:
            retVal = fptaylor_real2float_ex5(inputs[0], inputs[1]);
            break;
        case 23:
            retVal = fptaylor_real2float_ex8(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);
            break;
        case 24:
            retVal = fptaylor_real2float_ex9(inputs[0], inputs[1], inputs[2], inputs[3]);
            break;
        case 25:
            retVal = fptaylor_real2float_ex10(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);
            break;
        case 26:
            retVal = fptaylor_tests_ex0(inputs[0]);
            break;
        case 27:
            retVal = fptaylor_tests_ex1(inputs[0], inputs[1]);
            break;
        case 28:
            retVal = fptaylor_tests_ex2(inputs[0], inputs[1], inputs[2]);
            break;
        case 29:
            retVal = fptaylor_tests_ex3(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7]);
            break;
        case 30:
            retVal = fptaylor_tests_ex4(inputs[0], inputs[1]);
            break;
        case 31:
            retVal = fptaylor_tests_ex5(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7], inputs[8]);
            break;
        case 32:
            retVal = fptaylor_tests_ex6(inputs[0]);
            break;
        case 33:
            retVal = fptaylor_tests_ex7(inputs[0]);
            break;
        case 34:
            retVal = fptaylor_tests_ex8(inputs[0], inputs[1], inputs[2], inputs[3]);
            break;
        case 35:
            retVal = fptaylor_tests_ex9(inputs[0], inputs[1], inputs[2], inputs[3]);
            break;
        case 36:
            retVal = hamming_ch3_ex0(inputs[0]);
            break;
        case 37:
            retVal = hamming_ch3_ex4(inputs[0]);
            break;
        case 38:
            retVal = hamming_ch3_ex5(inputs[0]);
            break;
        case 39:
            retVal = hamming_ch3_ex7(inputs[0]);
            break;
        case 40:
            retVal = hamming_ch3_ex12(inputs[0], inputs[1], inputs[2]);
            break;
        case 41:
            retVal = hamming_ch3_ex13(inputs[0], inputs[1], inputs[2]);
            break;
        case 42:
            retVal = hamming_ch3_ex14(inputs[0], inputs[1], inputs[2]);
            break;
        case 43:
            retVal = hamming_ch3_ex15(inputs[0], inputs[1], inputs[2]);
            break;
        case 44:
            retVal = herbie_ex0(inputs[0], inputs[1]);
            break;
        case 45:
            retVal = premonious_ex0(inputs[0]);
            break;
        case 46:
            retVal = premonious_ex1(inputs[0]);
            break;
        case 47:
            retVal = rosa_ex0(inputs[0], inputs[1], inputs[2]);
            break;
        case 48:
            retVal = rosa_ex1(inputs[0], inputs[1], inputs[2]);
            break;
        case 49:
            retVal = rosa_ex2(inputs[0], inputs[1], inputs[2]);
            break;
        case 50:
            retVal = rosa_ex3(inputs[0], inputs[1], inputs[2]);
            break;
        case 51:
            retVal = rosa_ex4(inputs[0], inputs[1], inputs[2]);
            break;
        case 52:
            retVal = rosa_ex5(inputs[0], inputs[1]);
            break;
        case 53:
            retVal = rosa_ex6(inputs[0], inputs[1], inputs[2]);
            break;
        case 54:
            retVal = rosa_ex7(inputs[0], inputs[1], inputs[2]);
            break;
        case 55:
            retVal = rosa_ex8(inputs[0], inputs[1], inputs[2]);
            break;
        case 56:
            retVal = rosa_ex9(inputs[0]);
            break;
        case 57:
            retVal = rosa_ex10(inputs[0]);
            break;
        case 58:
            retVal = rosa_ex11(inputs[0]);
            break;
        case 59:
            retVal = rosa_ex12(inputs[0]);
            break;
        case 60:
            retVal = rosa_ex13(inputs[0]);
            break;
        case 61:
            retVal = rosa_ex14(inputs[0]);
            break;
        case 62:
            retVal = rosa_ex15(inputs[0]);
            break;
        case 63:
            retVal = rosa_ex16(inputs[0]);
            break;
        case 64:
            retVal = rosa_ex17(inputs[0]);
            break;
        case 65:
            retVal = rosa_ex18(inputs[0]);
            break;
        case 66:
            retVal = rosa_ex19(inputs[0], inputs[1], inputs[2]);
            break;
        case 67:
            retVal = rosa_ex20(inputs[0], inputs[1], inputs[2]);
            break;
        case 68:
            retVal = rosa_ex21(inputs[0], inputs[1], inputs[2]);
            break;
        case 69:
            retVal = rump_ex1(inputs[0], inputs[1]);
            break;
        case 70:
            retVal = rump_ex2(inputs[0], inputs[1]);
            break;
        case 71:
            retVal = salsa_ex1(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4]);
            break;
        case 72:
            retVal = salsa_ex3(inputs[0], inputs[1]);
            break;
        case 73:
            retVal = salsa_ex4(inputs[0]);
            break;
        case 74:
            retVal = salsa_ex6(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7]);
            break;
        case 75:
            retVal = salsa_ex7(inputs[0]);
            break;
        case 76:
            retVal = salsa_ex8(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7], inputs[8], inputs[9], inputs[10], inputs[11], inputs[12], inputs[13], inputs[14], inputs[15], inputs[16], inputs[17], inputs[18], inputs[19]);
            break;
        case 77:
            retVal = salsa_ex9(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7], inputs[8]);
            break;
        default:
            printf("  Error in FP Core API usage.\n");
            status = faultDaisy;
    }
    memcpy(returnValue, &retVal, min_uint(sizeof(returnValue), sizeof(E_TYPE))); // Copy resultant
    return status;
}

/* Application Program Interface
 * int function: Function selection
 * E_TYPE vars[MAX_VARS]: Double precision input set.
 * E_TYPE *retValue: Double precision return value.
 * int debug: Debug Mode (0=off,1=on)
 * long double *runTime: CPU cycles for execution.
 */
statusDaisy fpbench_API(int function, E_TYPE vars[maxInputs+1], E_TYPE *retValue, int debug, long double *runTime)
{
    FILE *appendingDaisyFileContext = (FILE *)calloc(1, sizeof(FILE));
    char *content = (char *)calloc(CHAR_BUFFER_SIZE, sizeof(char));
    char *contentName = (char *)calloc(CHAR_BUFFER_SIZE, sizeof(char));
    long signed int fileByteSize = 0;

    /* Check if file exists and open for writing.*/
    appendingDaisyFileContext = fopen(filenameDaisy, "r");
    if(access(filenameDaisy, F_OK ) != -1) // If file does not exist, create it
    {
        fclose(appendingDaisyFileContext );
        appendingDaisyFileContext = fopen(filenameDaisy, "a");
    }
    else { // Write new to file.
        //fclose(appendingDaisyFileContext);
        appendingDaisyFileContext = fopen(filenameDaisy, "w");
    }

    fileByteSize = getFileSize(appendingDaisyFileContext);
    if (fileByteSize > LONG_MAX) {
        printf("  File %s is %ld bytes of %ld byte maxima.\n", filenameDaisy, fileByteSize, LONG_MAX);
    }

    /*Time variables*/
    fpbenchTime_t tStart = 0;
    fpbenchTime_t tStop = 0;
    fpbenchTime_t tUsed = 0;
    E_TYPE *inputs = (E_TYPE *)calloc(maxInputs*2, sizeof(E_TYPE)); // E_TYPE inputs[MAX_VARS];
    E_TYPE retVal = 0;
    int validCase = 0;
    unsigned int i = 0;
    statusDaisy status = healthyDaisy;

    // Set Data to debug pattern  then copy values
    memset(&retVal, 0x0, sizeof(retVal));
    memcpy(inputs, &vars, (maxInputs * sizeof(E_TYPE)));
    for (i = 0; i < MAX_VARS; i++) {
        inputs[i] = vars[i];
    }

    if (debug > 0) {
        printf(" FP Core Benchmark API Function %d\n", function);
    }
    tStart = clock();
    switch(function) {
        case 1:
            retVal = daisy_ex0(inputs[0], inputs[1]);
            break;
        case 2:
            retVal = daisy_ex1(inputs[0], inputs[1]);
            break;
        case 3:
            retVal = daisy_ex2(inputs[0], inputs[1]);
            break;
        case 4:
            retVal = daisy_ex3(inputs[0], inputs[1]);
            break;
        case 5:
            retVal = daisy_ex4(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4]);
            break;
        case 6:
            retVal = daisy_ex5(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7], inputs[8]);
            break;
        case 7:
            retVal = daisy_ex6(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7], inputs[8]);
            break;
        case 8:
            retVal = fptaylor_extra_ex0(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);
            break;
        case 9:
            retVal = fptaylor_extra_ex1(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);
            break;
        case 10:
            retVal = fptaylor_extra_ex2(inputs[0]);
            break;
        case 11:
            retVal = fptaylor_extra_ex5(inputs[0], inputs[1]);
            break;
        case 12:
            retVal = fptaylor_extra_ex7(inputs[0], inputs[1]);
            break;
        case 13:
            retVal = fptaylor_extra_ex8(inputs[0], inputs[1]);
            break;
        case 14:
            retVal = fptaylor_extra_ex9(inputs[0], inputs[1]);
            break;
        case 15:
            retVal = fptaylor_extra_ex11(inputs[0], inputs[1], inputs[2]);
            break;
        case 16:
            retVal = fptaylor_extra_ex12(inputs[0]);
            break;
        case 17:
            retVal = fptaylor_extra_ex13(inputs[0], inputs[1]);
            break;
        case 18:
            retVal = fptaylor_extra_ex14(inputs[0], inputs[1]);
            break;
        case 19:
            retVal = fptaylor_extra_ex15(inputs[0], inputs[1]);
            break;
        case 20:
            retVal = fptaylor_real2float_ex3(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);
            break;
        case 21:
            retVal = fptaylor_real2float_ex4(inputs[0], inputs[1]);
            break;
        case 22:
            retVal = fptaylor_real2float_ex5(inputs[0], inputs[1]);
            break;
        case 23:
            retVal = fptaylor_real2float_ex8(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);
            break;
        case 24:
            retVal = fptaylor_real2float_ex9(inputs[0], inputs[1], inputs[2], inputs[3]);
            break;
        case 25:
            retVal = fptaylor_real2float_ex10(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);
            break;
        case 26:
            retVal = fptaylor_tests_ex0(inputs[0]);
            break;
        case 27:
            retVal = fptaylor_tests_ex1(inputs[0], inputs[1]);
            break;
        case 28:
            retVal = fptaylor_tests_ex2(inputs[0], inputs[1], inputs[2]);
            break;
        case 29:
            retVal = fptaylor_tests_ex3(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7]);
            break;
        case 30:
            retVal = fptaylor_tests_ex4(inputs[0], inputs[1]);
            break;
        case 31:
            retVal = fptaylor_tests_ex5(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7], inputs[8]);
            break;
        case 32:
            retVal = fptaylor_tests_ex6(inputs[0]);
            break;
        case 33:
            retVal = fptaylor_tests_ex7(inputs[0]);
            break;
        case 34:
            retVal = fptaylor_tests_ex8(inputs[0], inputs[1], inputs[2], inputs[3]);
            break;
        case 35:
            retVal = fptaylor_tests_ex9(inputs[0], inputs[1], inputs[2], inputs[3]);
            break;
        case 36:
            retVal = hamming_ch3_ex0(inputs[0]);
            break;
        case 37:
            retVal = hamming_ch3_ex4(inputs[0]);
            break;
        case 38:
            retVal = hamming_ch3_ex5(inputs[0]);
            break;
        case 39:
            retVal = hamming_ch3_ex7(inputs[0]);
            break;
        case 40:
            retVal = hamming_ch3_ex12(inputs[0], inputs[1], inputs[2]);
            break;
        case 41:
            retVal = hamming_ch3_ex13(inputs[0], inputs[1], inputs[2]);
            break;
        case 42:
            retVal = hamming_ch3_ex14(inputs[0], inputs[1], inputs[2]);
            break;
        case 43:
            retVal = hamming_ch3_ex15(inputs[0], inputs[1], inputs[2]);
            break;
        case 44:
            retVal = herbie_ex0(inputs[0], inputs[1]);
            break;
        case 45:
            retVal = premonious_ex0(inputs[0]);
            break;
        case 46:
            retVal = premonious_ex1(inputs[0]);
            break;
        case 47:
            retVal = rosa_ex0(inputs[0], inputs[1], inputs[2]);
            break;
        case 48:
            retVal = rosa_ex1(inputs[0], inputs[1], inputs[2]);
            break;
        case 49:
            retVal = rosa_ex2(inputs[0], inputs[1], inputs[2]);
            break;
        case 50:
            retVal = rosa_ex3(inputs[0], inputs[1], inputs[2]);
            break;
        case 51:
            retVal = rosa_ex4(inputs[0], inputs[1], inputs[2]);
            break;
        case 52:
            retVal = rosa_ex5(inputs[0], inputs[1]);
            break;
        case 53:
            retVal = rosa_ex6(inputs[0], inputs[1], inputs[2]);
            break;
        case 54:
            retVal = rosa_ex7(inputs[0], inputs[1], inputs[2]);
            break;
        case 55:
            retVal = rosa_ex8(inputs[0], inputs[1], inputs[2]);
            break;
        case 56:
            retVal = rosa_ex9(inputs[0]);
            break;
        case 57:
            retVal = rosa_ex10(inputs[0]);
            break;
        case 58:
            retVal = rosa_ex11(inputs[0]);
            break;
        case 59:
            retVal = rosa_ex12(inputs[0]);
            break;
        case 60:
            retVal = rosa_ex13(inputs[0]);
            break;
        case 61:
            retVal = rosa_ex14(inputs[0]);
            break;
        case 62:
            retVal = rosa_ex15(inputs[0]);
            break;
        case 63:
            retVal = rosa_ex16(inputs[0]);
            break;
        case 64:
            retVal = rosa_ex17(inputs[0]);
            break;
        case 65:
            retVal = rosa_ex18(inputs[0]);
            break;
        case 66:
            retVal = rosa_ex19(inputs[0], inputs[1], inputs[2]);
            break;
        case 67:
            retVal = rosa_ex20(inputs[0], inputs[1], inputs[2]);
            break;
        case 68:
            retVal = rosa_ex21(inputs[0], inputs[1], inputs[2]);
            break;
        case 69:
            retVal = rump_ex1(inputs[0], inputs[1]);
            break;
        case 70:
            retVal = rump_ex2(inputs[0], inputs[1]);
            break;
        case 71:
            retVal = salsa_ex1(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4]);
            break;
        case 72:
            retVal = salsa_ex3(inputs[0], inputs[1]);
            break;
        case 73:
            retVal = salsa_ex4(inputs[0]);
            break;
        case 74:
            retVal = salsa_ex6(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7]);
            break;
        case 75:
            retVal = salsa_ex7(inputs[0]);
            break;
        case 76:
            retVal = salsa_ex8(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7], inputs[8], inputs[9], inputs[10], inputs[11], inputs[12], inputs[13], inputs[14], inputs[15], inputs[16], inputs[17], inputs[18], inputs[19]);
            break;
        case 77:
            retVal = salsa_ex9(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7], inputs[8]);
            break;
        default:
            printf("  Error in FP Core API usage.\n");
            status = faultDaisy;
    }
    tStop = clock();
    memcpy(retValue, &retVal, min_uint(sizeof(retValue), sizeof(E_TYPE))); // Copy resultant
    tUsed = (tStop-tStart);
    *runTime = (long double) tUsed;
    validCase = (function >= MIN_FUNCTIONS && function <= MAX_FUNCTIONS);
    if (validCase) {
        fileByteSize = getFileSize(appendingDaisyFileContext);
        if (fileByteSize == -1){
            assert(0);
        }
        if (fileByteSize < LONG_MAX) {
            // Prepare content for file
            memset(content, '\0', (CHAR_BUFFER_SIZE * sizeof(char)));
            fpbench_map(function, contentName);
            snprintf(content, (CHAR_BUFFER_SIZE * sizeof(char)),
                     "%s, "
                     "%d, "
                     "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                     "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                     "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                     "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                     "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                     "%.32Lf, "
                     "%Lf\n",
                     contentName,
                     function,
                     (C_TYPE) inputs[0], (C_TYPE) inputs[1], (C_TYPE) inputs[2], (C_TYPE) inputs[3],
                     (C_TYPE) inputs[4], (C_TYPE) inputs[5], (C_TYPE) inputs[6], (C_TYPE) inputs[7],
                     (C_TYPE) inputs[8], (C_TYPE) inputs[9], (C_TYPE) inputs[10], (C_TYPE) inputs[11],
                     (C_TYPE) inputs[12], (C_TYPE) inputs[13], (C_TYPE) inputs[14], (C_TYPE) inputs[15],
                     (C_TYPE) inputs[16], (C_TYPE) inputs[17], (C_TYPE) inputs[18], (C_TYPE) inputs[19],
                     (C_TYPE) retVal,
                     (long double) tUsed);
            fputs(content, appendingDaisyFileContext); // Write content to file
        }
        if (debug > 0) {
            // Debug information
            printf("  Inputs: "
                   "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                   "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                   "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                   "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                   "%.32Lf, %.32Lf, %.32Lf, %.32Lf\n",
                    (C_TYPE) inputs[0], (C_TYPE) inputs[1], (C_TYPE) inputs[2], (C_TYPE) inputs[3],
                    (C_TYPE) inputs[4], (C_TYPE) inputs[5], (C_TYPE) inputs[6], (C_TYPE) inputs[7],
                    (C_TYPE) inputs[8], (C_TYPE) inputs[9], (C_TYPE) inputs[10], (C_TYPE) inputs[11],
                    (C_TYPE) inputs[12], (C_TYPE) inputs[13], (C_TYPE) inputs[14], (C_TYPE) inputs[15],
                    (C_TYPE) inputs[16], (C_TYPE) inputs[17], (C_TYPE) inputs[18], (C_TYPE) inputs[19]);
            printf("  Result: %.32Lf\n", (C_TYPE) retVal);
            printf("  Cycle Time: %Lg\n", (long double) (tUsed));
            printf("  File %s is %ld bytes of %ld byte maxima.\n", filenameDaisy, fileByteSize, LONG_MAX);
            printf("  File write content: %s", content);
        }
    }


    fclose(appendingDaisyFileContext);
    free(content);
    free(contentName);
    free(inputs);
    return status;
}

/* Copy a char * array until a '\0' nil char. */
void copy_string(char destination[CHAR_BUFFER_SIZE], char source[CHAR_BUFFER_SIZE]) {
    int index = 0;

    while ( (source[index] != '\0') &&
            (index < CHAR_BUFFER_SIZE) ) {
        destination[index] = source[index];
        index++;
    }
    destination[index] = '\0';
    return;
}

/* Application Program Interface for CVS files
 * int function: Function selection
 * int debug: Debug Mode (0=off,1=on)
 * long double *runTime: CPU cycles for execution.
 */
statusDaisy fpbench_CVSProcess(const char sourceFile[], const char destinationFile[], long unsigned int indexStart, int debug)
{
    // Reading File processing variables.
    FILE *readingFileContext = (FILE *)calloc(1, sizeof(FILE)); // Read file Context.
    char *contentReadBuffer = (char *)calloc(CHAR_BUFFER_SIZE, sizeof(char)); // Read content buffer.
    long unsigned int readingCurrentLine = 0; // Current Reading line
    long unsigned int readBufferLocation = 0; // Current Reading line
    char readCharacter = 'r';

    // Expected line format: function number, fpCoreName, inputs[0], ..., inputs[MAX_VARS], returnValue, cycleTime.
    int function = 1;
    char *fpCoreName = (char *)calloc(CHAR_BUFFER_SIZE, sizeof(char));
    C_TYPE *inputs = (C_TYPE *)calloc(maxInputs*2, sizeof(C_TYPE)); // E_TYPE inputs[MAX_VARS]; FPCore Variable buffer.
    C_TYPE returnValue = 0;
    long double cycleTime = 0;

    // Variables used for processing.
    long double userInput = 0.0;
    int validCase = 0;
    int tokenNumber = 0;
    int tokenReady = 0;
    int restartFormat = 0;
    statusDaisy status = healthyDaisy;

    /*Time variables*/
    fpbenchTime_t tStart = 0;
    fpbenchTime_t tStop = 0;
    //fpbenchTime_t tUsed = 0;

    // WritingFile processing variables.
    FILE *writingFileContext = (FILE *)calloc(1, sizeof(FILE));
    char *contentWriteBuffer = (char *)calloc(CHAR_BUFFER_SIZE, sizeof(char));
    char *contentWriteName = (char *)calloc(CHAR_BUFFER_SIZE, sizeof(char));
    //long unsigned int writingCurrentLine; // Current writing line.
    long signed int fileByteSize;       // Maximum file size.

    /* Check if file exists and open for writing.*/
    writingFileContext = fopen(destinationFile, "r");
    if(access(destinationFile, F_OK ) != -1) // If file does not exist, create it
    {
        fclose(writingFileContext);
        writingFileContext = fopen(destinationFile, "a");
    }
    else { // Write new to file.
        fclose(writingFileContext);
        writingFileContext = fopen(destinationFile, "w");
    }
    fileByteSize = getFileSize(writingFileContext);
    if (fileByteSize > LONG_MAX){
        printf("  File %s is %ld bytes of %ld byte maxima.\n", destinationFile, fileByteSize, LONG_MAX);
    }

    // Starting main read, process, write to new dataset.
    readingFileContext = fopen(sourceFile, "r");
    if(access(sourceFile, F_OK ) != -1) // If file does not exist, create it
    {
        rewind(readingFileContext);
        readingCurrentLine = 1;
        readBufferLocation = 0;

        // currentLine > indexStart condition skips lines
        while( (readCharacter = fgetc(readingFileContext)) != EOF) {
            switch(readCharacter) {
                case ',':
                    if(readingCurrentLine > indexStart) {
                        contentReadBuffer[readBufferLocation] = '\0';
                        printf("Token char: %s,", contentReadBuffer);
                        readBufferLocation = 0;
                        tokenReady = 1;
                    }
                    break;
                case '\n':
                    if(readingCurrentLine > indexStart) {
                        contentReadBuffer[readBufferLocation] = '\0';
                        printf("Token: %s\n", contentReadBuffer);
                        readBufferLocation = 0;
                        restartFormat = 1;
                    }
                    readingCurrentLine++;
                    break;
                default: // Process letters, numbers, dot, underscores, etc.
                    if(readingCurrentLine > indexStart) {
                        contentReadBuffer[readBufferLocation] = readCharacter;
                        readBufferLocation++; // Increment read buffer to next index location.
                    }
                    break;
            }

            if ( (readingCurrentLine < indexStart) &&
                 ( (tokenReady == 1) || (restartFormat == 1)) ) {
                printf("Error fpbench_CVSProcess in Condition (%ld < %ld) and tokenReady = %d restartFormat = %d.", readingCurrentLine, indexStart, tokenReady, restartFormat);
                assert(0);
            }

            // Process Token into buffer
            if (tokenReady == 1) {
                tokenReady = 0;
                if (tokenNumber == 0) {
                    // Benchmark Function Number
                    function = atoi(contentReadBuffer);
                }
                else if(tokenNumber == 1) {
                    // Benchmark String Name
                    copy_string(fpCoreName, contentReadBuffer);
                }
                else if ( (tokenNumber >= inputStart) &&
                          (tokenNumber <= inputStart+MAX_VARS) ) {
                    // Input Value
                    userInput = strtod(contentReadBuffer, NULL);
                    inputs[tokenNumber] = (C_TYPE) userInput;
                }
                else if ( tokenNumber   == (maxInputs-2) &&
                         ((maxInputs-2) == (inputStart+MAX_VARS+1))) {
                    // Result Value
                    strtod(contentReadBuffer, NULL);
                    returnValue = (E_TYPE) userInput;
                }
                else if ( tokenNumber   == (maxInputs-1) &&
                         ((maxInputs-1) == (inputStart+MAX_VARS+2))) {
                    // Clock Cycles Value
                    strtod(contentReadBuffer, NULL);
                    cycleTime = (E_TYPE) userInput;
                    tokenNumber = -1;
                }
                else if (tokenNumber >= maxInputs || tokenNumber < 0) {
                    printf("fpbench_CVSProcess ERROR Out of Bounds! TokenNumber: %d", tokenNumber);
                    assert(0);
                }
                tokenNumber++;
            }

            // Execute and Write Token to new dataset CVS.
            validCase = (function >= MIN_FUNCTIONS && function <= MAX_FUNCTIONS);
            if (validCase == 1 && restartFormat == 1) {
                // Execute Tokens on new benchmark.
                tStart = clock();
                status = fpcore_executeBenchmark(function, inputs, &returnValue);
                tStop = clock();
                cycleTime = (tStop-tStart);
                restartFormat = 0;

                // Prepare and write to file.
                fileByteSize = getFileSize(writingFileContext);
                if (fileByteSize < LONG_MAX) {
                    // Prepare content for file
                    memset(contentWriteBuffer, '\0', (CHAR_BUFFER_SIZE * sizeof(char)));
                    fpbench_map(function, contentWriteName);
                    // ASSERT if contentWriteName != fpCoreName
                    snprintf(contentWriteBuffer, ((sizeof(char) * CHAR_BUFFER_SIZE) - 1),
                             "%s, "
                             "%d, "
                             "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                             "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                             "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                             "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                             "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                             "%.32Lf, "
                             "%Lf\n",
                             contentWriteName,
                             function,
                             (C_TYPE) inputs[0], (C_TYPE) inputs[1], (C_TYPE) inputs[2], (C_TYPE) inputs[3],
                             (C_TYPE) inputs[4], (C_TYPE) inputs[5], (C_TYPE) inputs[6], (C_TYPE) inputs[7],
                             (C_TYPE) inputs[8], (C_TYPE) inputs[9], (C_TYPE) inputs[10], (C_TYPE) inputs[11],
                             (C_TYPE) inputs[12], (C_TYPE) inputs[13], (C_TYPE) inputs[14], (C_TYPE) inputs[15],
                             (C_TYPE) inputs[16], (C_TYPE) inputs[17], (C_TYPE) inputs[18], (C_TYPE) inputs[19],
                             (C_TYPE) returnValue,
                             (long double) (cycleTime));
                    fputs(contentWriteBuffer, writingFileContext); // Write content to file
                }
                if (debug > 0) {
                    // Debug information
                    printf("  Write Function %d, Name %s\n", function, contentWriteName);
                    printf("  Inputs: "
                           "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                           "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                           "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                           "%.32Lf, %.32Lf, %.32Lf, %.32Lf, "
                           "%.32Lf, %.32Lf, %.32Lf, %.32Lf\n",
                           (C_TYPE) inputs[0], (C_TYPE) inputs[1], (C_TYPE) inputs[2], (C_TYPE) inputs[3],
                           (C_TYPE) inputs[4], (C_TYPE) inputs[5], (C_TYPE) inputs[6], (C_TYPE) inputs[7],
                           (C_TYPE) inputs[8], (C_TYPE) inputs[9], (C_TYPE) inputs[10], (C_TYPE) inputs[11],
                           (C_TYPE) inputs[12], (C_TYPE) inputs[13], (C_TYPE) inputs[14], (C_TYPE) inputs[15],
                           (C_TYPE) inputs[16], (C_TYPE) inputs[17], (C_TYPE) inputs[18], (C_TYPE) inputs[19]);
                    printf("  Result: %.32Lf\n", (C_TYPE) returnValue);
                    printf("  Cycle Time: %Lg\n", (long double) (cycleTime));
                    printf("  File %s is %ld bytes of %ld byte maxima.\n", destinationFile, fileByteSize, LONG_MAX);
                    printf("  File write content: %s", contentWriteBuffer);
                }
            }
        }
    }

    fclose(readingFileContext);
    fclose(writingFileContext);
    free(readingFileContext);
    free(writingFileContext);
    free(contentReadBuffer);
    free(fpCoreName);
    free(inputs);
    free(contentWriteBuffer);
    free(contentWriteName);
    return status;
}

/* Test harness for program API*/
void fpbench_testHarness(void)
{
    long unsigned int i = 0;
    long unsigned int j = 0;
    long unsigned int validData = 0;
    E_TYPE *inputs = (E_TYPE *)calloc((maxInputs*2), sizeof(E_TYPE)); // E_TYPE inputs[MAX_VARS];
    E_TYPE retVal = 0;
    double randomValue = 0;
    long double runTime = 0;
    long double totalTime = 0;

    statusDaisy status = healthyDaisy;
    printf("----------------------\n");
    printf(" FPCore Test Harness...\n");
    for (j = 0; j < (long unsigned int) testLoops; j++) {
        for (i = 0; i < MAX_VARS; i++) {
            randomValue = 0;
            validData = validDataInput(randomValue);
            while (validData != 1) {
                randomValue = rand_double();
                validData = validDataInput(randomValue);
            }
            inputs[i] = randomValue;
        }

        for (i = MIN_FUNCTIONS; i < MAX_FUNCTIONS; i++) {
            status = fpbench_API(i, inputs, &retVal, isDebugOn, &runTime);
            totalTime += runTime;
        }
    }

    if (status == healthyDaisy){
        printf(" FPCore Passed Test Harness\n");
    }
    else{
        printf(" FPCore Failed Test Harness\n");
    }
    printf(" Cycle Time: %Lg\n", totalTime);
    printf("----------------------\n");
    free(inputs);
    return;
}

/* Main function for user input */
#ifndef LIBRARY_MODE
int main(int argc, char *argv[CHAR_BUFFER_SIZE])
#elif (LIBRARY_MODE >= 0)
int daisy_main(int argc, char *argv[CHAR_BUFFER_SIZE]);
#else // LIBRARY_MODE is defined
int daisy_main(int argc, char *argv[CHAR_BUFFER_SIZE]);
#endif // LIBRARY_MODE
{
    /* Main argv index positions.
     * ./program  1   1.0  2.0    0
     *    {^0     ^1  ^2   ^3    ^4} = {program name, Function Select, Input_1, Input_2, Debug Flag}
     * ./program  6   1.0  2.0  3.0  4.0  5.0  6.0  7.0  8.0   9.0  1
     *    {^0     ^1  ^2   ^3    ^4   ^5   ^6   ^7   ^8   ^9   ^10  ^11}
     *    = {program name, Function Select, Input_1, Input_2, Input_3, Input_4, Input_5, Input_6, Input_7, Input_8, Input_9, Debug Flag}
     */
    // Variables
    E_TYPE *inputs = (E_TYPE *)calloc((maxInputs*2), sizeof(E_TYPE)); // E_TYPE inputs[MAX_VARS];
    E_TYPE retVal = 0;
    long double runTime = 0;
    long double userInput = 0;
    unsigned int i = 0;
    unsigned int argCTop = 0;
    unsigned int validInputs = 0;
    unsigned int functionSelect = 0;
    unsigned int debug = 0;

    statusDaisy status = healthyDaisy;
    /////////////////////////////////////
    printf("filename=%s\n", filenameDaisy);
    fpbench_testHarness();
    return status;
    ////////////////////////////////////
    // status = fpbench_CVSProcess(sourceFile[], destinationFile[], indexStart, debug);
    // return status;
    ////////////////////////////////////
    printf("FP Core Benchmark\n");
    printf(" Number of arguments count argc = %d.\n", argc);
    printf(" The argument supplied are: ");
    for(i = 0; i < (unsigned int) min_uint(argc, maxInputs); i++){
        printf("%s, ", argv[i]);
    }
    printf("\n");
    validInputs = ((unsigned int) argc >= minInputs && (unsigned int) argc <= maxInputs);
    if (validInputs) {
        printf(" Valid arguments.\n");
    }
    else {
        printf(" No argv[], nil passed to the program.\n");
        printf("  Example Usage:\n");
        printf("   Description: Function_Selected=(%d,%d) float_input_set(%d ... %d)[double inputs space separated] debug_mode(0=off,1=on).\n",
               MIN_FUNCTIONS, MAX_FUNCTIONS, MIN_VARS, MAX_VARS);
        printf("   ./program 1 1.0 2.0 0\n");
        printf("   ./program 6 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 1\n");
        printf("    /* Main argv index positions.\n");
        printf("     * ./program  1   1.0  2.0    0\n");
        printf("     *    {^0     ^1  ^2   ^3    ^4} = {program name, Function Select, Input_1, Input_2, Debug Flag}\n");
        printf("     * ./program  6   1.0  2.0  3.0  4.0  5.0  6.0  7.0  8.0   9.0  1\n");
        printf("     *    {^0     ^1  ^2   ^3    ^4   ^5   ^6   ^7   ^8   ^9   ^10  ^11}\n");
        printf("     *    = {program name, Function Select, Input_1, Input_2, Input_3, Input_4, Input_5, Input_6, Input_7, Input_8, Input_9, debug flag}\n");
        printf("     */");
        status = faultDaisy;
    }

    if (status == healthyDaisy && validInputs) {
        // Set Data to debug pattern  then copy values
        memset(&retVal, 0, (1 * sizeof(E_TYPE)));

        functionSelect = (unsigned int) atoi(argv[1]);
        printf("  Function = %d\n", functionSelect);
        // Convert string inputs to long doubles and remove first input since it is selection
        argCTop = (unsigned int) (argc-1);
        for (i = inputStart; (i < argCTop) && (i < maxInputs); i++) {
            printf("  Input[%d] = %s = %f\n", (i-inputStart), argv[i], strtod(argv[i], NULL));
            userInput = strtod(argv[i], NULL); // Alternative version - sscanf(argv[i], "%Lf", &userInput);
            inputs[i] = (E_TYPE) userInput;
        }
        debug = (unsigned int) atoi(argv[argc-1]);
        printf("  Debug = %d\n", debug);
        if (debug >= 1) {
            fpbench_testHarness();
        }
        status = fpbench_API(functionSelect, inputs, &retVal, debug, &runTime);
    }
    printf(" Done\n");
    free(inputs);
    return status;
}
#endif // _FPCORE_C_

// C++ Compatibility
#ifdef __cplusplus
}
#endif // __cplusplus
