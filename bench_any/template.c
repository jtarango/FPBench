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
 * @benchmark: Daisy FPCORE.
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
#ifndef _DAISY_C_
#define _DAISY_C_

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

// API Parameters based on functions.
#define CHAR_BUFFER_SIZE 512
#define MIN_VARS 2
#define MAX_VARS 9
#define MIN_FUNCTIONS 1
#define MAX_FUNCTIONS 7

/* Main argv index positions.
* ./program  1   1.0  2.0    0
* ./program  6   1.0  2.0  3.0  4.0  5.0  6.0  7.0  8.0   9.0  1
*     ^0     ^1  ^2   ^3    ^4   ^5   ^6   ^7   ^8   ^9   ^10  ^11
*/
#define minInputs (MIN_VARS + 3)
#define maxInputs (MAX_VARS + 3)
#define inputStart (2)
// LIBRARY_MODE is set 1 one when the user does not want a main.

// Constant file we append data to until we reach the variable maxima.
const char filenameDaisy[] = "daisy_dataset.csv";

typedef enum{
    healthyDaisy = 0,
    faultDaisy = 1,
} statusDaisy; // library status

// Source evaluation type for library.
#ifndef E_TYPE
typedef float E_TYPE;
#endif // E_TYPE

// Destination compare type used for accuracy.
#ifndef C_TYPE
typedef long double C_TYPE;
#endif // C_TYPE

// Pre-definitions Functions.
E_TYPE daisy_ex0(E_TYPE x, E_TYPE y);
E_TYPE daisy_ex1(E_TYPE x, E_TYPE y);
E_TYPE daisy_ex2(E_TYPE radius, E_TYPE theta);
E_TYPE daisy_ex3(E_TYPE radius, E_TYPE theta);
E_TYPE daisy_ex4(E_TYPE t, E_TYPE resistance, E_TYPE frequency, E_TYPE inductance, E_TYPE maxVoltage);
E_TYPE daisy_ex5(E_TYPE a, E_TYPE b, E_TYPE c, E_TYPE d, E_TYPE e, E_TYPE f, E_TYPE g, E_TYPE h, E_TYPE i);
E_TYPE daisy_ex6(E_TYPE a, E_TYPE b, E_TYPE c, E_TYPE d, E_TYPE e, E_TYPE f, E_TYPE g, E_TYPE h, E_TYPE i);
// Application Programming Interface (API)
statusDaisy daisy_API(int function, E_TYPE vars[MAX_VARS], E_TYPE *retValue, int debug, long double *runTime);
// Self Test.
void daisy_testHarness(void);
// Helper Functions.
unsigned int daisy_min_uint(unsigned int a, unsigned int b);
double daisy_rand_double(void);
double daisy_rand_gen();
double daisy_gauss_rand(int select);
int validDataInput(double randomValue);

// MAIN Functions.
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

/* Function to select min of two unsigned integers */
unsigned int daisy_min_uint(unsigned int a, unsigned int b)
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
double daisy_gauss_rand(int select)
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
double daisy_rand_double(void)
{
    static unsigned int ranOnce = 0;
    static int pingPong = 0;
    double v;
    if (ranOnce == 0){
        printf("  Random Seed Set\n");
        ranOnce++;
        srand(time(NULL)^clock()>>1); // srand(clock()) or srand(NULL)
    }
    if (pingPong >= 3) {
        pingPong = 0;
    }
    pingPong++;
    v = daisy_gauss_rand(pingPong);
    return v;
}

/* Application Program Interface
 * int function: Function selection
 * E_TYPE vars[MAX_VARS]: Double precision input set.
 * E_TYPE *retValue: Double precision return value.
 * int debug: Debug Mode (0=off,1=on)
 * long double *runTime: CPU cycles for execution.
 */
statusDaisy daisy_API(int function, E_TYPE vars[maxInputs+1], E_TYPE *retValue, int debug, long double *runTime)
{
    FILE * appendingDaisyFileContext;
    char *content = (char *)malloc(sizeof(char)*CHAR_BUFFER_SIZE);
    long unsigned int fileByteSize;
    /* Check if file exists and open for writing.*/
    appendingDaisyFileContext = fopen(filenameDaisy, "a+");
    if(access(filenameDaisy, F_OK ) != -1) // If file does not exist, create it
    {
        appendingDaisyFileContext = fopen(filenameDaisy, "a");
    }
    else { // Write new to file.
        appendingDaisyFileContext = fopen(filenameDaisy, "w");
    }
    fileByteSize = ftell(appendingDaisyFileContext);
    if (fileByteSize > LONG_MAX){
        printf("  File %s is %ld bytes of %ld byte maxima.\n", filenameDaisy, fileByteSize, LONG_MAX);
    }

    /*Time variables*/
    clock_t tStart = 0;
    clock_t tStop = 0;
    clock_t tUsed = 0;
    E_TYPE *inputs = (E_TYPE *)malloc(sizeof(E_TYPE)*maxInputs+1); // E_TYPE inputs[MAX_VARS];
    E_TYPE retVal;
    int validCase;
    unsigned int i;
    statusDaisy status = healthyDaisy;

    // Set Data to debug pattern  then copy values
    memset(&retVal, 0xAA, sizeof(retVal));
    memset(inputs, 0, (sizeof(E_TYPE)*(maxInputs)));
    memcpy(inputs, &vars, (sizeof(E_TYPE)*(maxInputs)));
    for (i = 0; i < MAX_VARS; i++) {
        inputs[i] = vars[i];
    }

    if (debug > 0) {
        printf(" Daisy FP Core Benchmark API Function %d\n", function);
    }
    tStart = clock();
    switch(function) {
        case 1:
            retVal = daisy_ex1(inputs[0], inputs[1]);
            break;
        case 2:
            retVal = daisy_ex2(inputs[0], inputs[1]);
            break;
        case 3:
            retVal = daisy_ex3(inputs[0], inputs[1]);
            break;
        case 4:
            retVal = daisy_ex4(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4]);
            break;
        case 5:
            retVal = daisy_ex5(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7], inputs[8]);
            break;
        case 6:
            retVal = daisy_ex6(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], inputs[7], inputs[8]);
            break;
        default:
            printf("  Error in Daisy API usage.\n");
            status = faultDaisy;
    }
    tStop = clock();
    memcpy(retValue, &retVal, daisy_min_uint(sizeof(retValue), sizeof(E_TYPE))); // Copy resultant
    tUsed = (tStop-tStart);
    *runTime = (long double) tUsed;
    validCase = (function >= MIN_FUNCTIONS && function <= MAX_FUNCTIONS);
    if (validCase) {
        if (fileByteSize < LONG_MAX) {
            // Prepare content for file
            memset(content, '\0', (sizeof(char) * (CHAR_BUFFER_SIZE - 1)));

            snprintf(content, ((sizeof(char) * CHAR_BUFFER_SIZE) - 1),
                     "daisy, %d, %.32Lf, %.32Lf, %.32Lf, %.32Lf, %.32Lf, %.32Lf, %.32Lf, %.32Lf, %.32Lf, %.32Lf\n",
                     function,
                     (C_TYPE) inputs[0], (C_TYPE) inputs[1], (C_TYPE) inputs[2], (C_TYPE) inputs[3],
                     (C_TYPE) inputs[4], (C_TYPE) inputs[5], (C_TYPE) inputs[6], (C_TYPE) inputs[7],
                     (C_TYPE) inputs[8],
                     (C_TYPE) retVal);
            fputs(content, appendingDaisyFileContext); // Write content to file
        }
        if (debug > 0) {
            // Debug information
            printf("  Inputs: %.32Lf, %.32Lf, %.32Lf, %.32Lf, %.32Lf, %.32Lf, %.32Lf, %.32Lf, %.32Lf\n",
                   (C_TYPE) inputs[0], (C_TYPE) inputs[1], (C_TYPE) inputs[2], (C_TYPE) inputs[3],
                   (C_TYPE) inputs[4], (C_TYPE) inputs[5], (C_TYPE) inputs[6], (C_TYPE) inputs[7],
                   (C_TYPE) inputs[8]);
            printf("  Result: %.32Lf\n", (C_TYPE) retVal);
            printf("  Cycle Time: %Lg\n", (long double) (tUsed));
            printf("  File %s is %ld bytes of %ld byte maxima.\n", filenameDaisy, fileByteSize, LONG_MAX);
            printf("  File write content: %s", content);
        }

    }
    fclose(appendingDaisyFileContext);
    return status;
}

/* Test harness for program API*/
void daisy_testHarness(void)
{
    unsigned int i;
    unsigned int j;
    unsigned int validData;
    const unsigned int testLoops = 1;
    E_TYPE *inputs = (E_TYPE *)malloc(sizeof(E_TYPE)*(maxInputs+1)); // E_TYPE inputs[MAX_VARS];
    E_TYPE retVal;
    double randomValue;
    long double runTime;
    long double totalTime = 0;

    statusDaisy status = healthyDaisy;
    printf("----------------------\n");
    printf(" Daisy Test Harness...\n");
    for (j = 0; j < (unsigned int) testLoops; j++) {
        for (i = 0; i < MAX_VARS; i++) {
            randomValue = 0;
            validData = validDataInput(randomValue);
            while ( validData != 1 ) {
                randomValue = daisy_rand_double();
                validData = validDataInput(randomValue);
            }
            inputs[i] = randomValue;
        }

        for (i = MIN_FUNCTIONS; i < MAX_FUNCTIONS; i++) {
            status = daisy_API(i, inputs, &retVal, 1, &runTime);
            totalTime += runTime;
        }
    }

    if (status == healthyDaisy){
        printf(" Daisy Passed Test Harness\n");
    }
    else{
        printf(" Daisy Failed Test Harness\n");
    }
    printf(" Cycle Time: %Lg\n", totalTime);
    printf("----------------------\n");
}

/* Main function for user input */
#ifndef LIBRARY_MODE
int main(int argc, char *argv[CHAR_BUFFER_SIZE])
#elif (LIBRARY_MODE >= 0)
int daisy_main(int argc, char *argv[512]);
#else // LIBRARY_MODE is defined
int daisy_main(int argc, char *argv[512]);
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
    E_TYPE *inputs = (E_TYPE *)malloc(sizeof(E_TYPE)*(maxInputs+1)); // E_TYPE inputs[MAX_VARS];
    E_TYPE retVal;
    long double runTime;
    long double userInput;
    unsigned int i;
    unsigned int argCTop;
    unsigned int validInputs;
    unsigned int functionSelect;
    unsigned int debug;

    statusDaisy status = healthyDaisy;

    printf("Daisy FP Core Benchmark\n");
    printf(" Number of arguments count argc = %d.\n", argc);
    printf(" The argument supplied are: ");
    for(i = 0; i < (unsigned int) daisy_min_uint(argc, maxInputs); i++){
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
        memset(&retVal, 0, sizeof(E_TYPE));
        memset(inputs, 0, (sizeof(E_TYPE)*(maxInputs)));

        functionSelect= (unsigned int) atoi(argv[1]);
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
            daisy_testHarness();
        }
        status = daisy_API(functionSelect, inputs, &retVal, debug, &runTime);
    }
    printf(" Done\n");
    return status;
}
#endif // _DAISY_C_

// C++ Compatibility
#ifdef __cplusplus
}
#endif // __cplusplus
