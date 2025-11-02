// Author: Austin Logan Barber
// File Name: Lab4.cu
// Date Created: 2025 October 28
// Date Last Updated: 2025 October 28
// Purpose: This is the main file for ECE 6122 >> Lab 4.

/******************************* I N C L U D E ********************************/
#include <cuda.h>
#include <cuda_runtime.h>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

/******************************************************************************/
/******************************************************************************/


/******************************* D E F I N E S ********************************/
// Degrees Celcius
#define AMBIENT_TEMP (20.0f)

// Degrees Celcius
#define HEAT_SOURCE_TEMP (100.0f)

#define BLOCK_SIZE_2D (32)
#define BLOCK_SIZE_1D (256)

/******************************************************************************/
/******************************************************************************/


/********************************* U S I N G **********************************/
using namespace std;

/******************************************************************************/
/******************************************************************************/


/**************************** P R O T O T Y P E S *****************************/
void process_flags(const unsigned int&, const vector<string>, unsigned int&,
    unsigned int&, unsigned int&, bool&);
__global__ void relaxation(float*, float*, int);
__global__ void copy_elements(float*, float*, int);

/******************************************************************************/
/******************************************************************************/


/********************************** M A I N ***********************************/
int main(int argc, char* argv[])
{
    /* Variables for controlling the application */
    unsigned int state = 2;  // 0, get user input; 1, process user input; 2, calculations
    bool run_program = true; // Keep running the program while true

    /* Variables for writing to a text file */
    fstream fs;
    fs.open("finalTemperatures.csv", fstream::out | fstream::app);
    
    /* Variables for calculations */
    unsigned int element_shape = 256;       // Size of grid to split
    unsigned int iterations = 10000;        // Number of iterations to perform for relaxtion
    float plate_size = 10.0;                // Length of each side of plate (feet)
    float delta = 0.0;                      // Distance between each point (vertically and horizontally) (feet)
    float heat_source_start_j = 3.0;        // Location of where heat source starts (feet)
    unsigned int heat_source_start_jdx = 0; // Index of where heat source starts
    float heat_source_end_j = 7.0;          // Location of where the heat source ends (feet)
    unsigned int heat_source_end_jdx = 0;   // Index of where heat source ends

    /* Timing variables */
    chrono::time_point start = chrono::high_resolution_clock::now();
    chrono::time_point stop = chrono::high_resolution_clock::now();
    chrono::milliseconds duration = chrono::milliseconds(0);

    /* Memory variables */
    float *h_h; // Holds the temperature variables on the host
    float *d_h; // Holds the temperature variabls on the device
    float *d_g; // Temporary temperature vaiables on the device

    /* Variables for getting user input */
    string stdin_input = "";    // User input for after first iteration
    const string DELIMETER = " ";     // Delimeter used to split string
    vector<string> split_input; // Holds the split user input
    size_t position;            // Where the delimeter is found
    
    // Process command line flags
    for(int i = 1; i < argc; i++)
    {
        split_input.push_back(argv[i]);
    }
    process_flags(split_input.size(), split_input, state, element_shape, iterations, run_program);
    
    /* Main application */
    while(run_program)
    {   
        // Get user input after first iteration
        if(state == 0)
        {
            stdin_input.clear();
            cout << "Enter flags to execute: ";
            getline(cin, stdin_input);
            state = 1;
        }
        // Process user input
        else if(state == 1)
        {
            split_input.clear();
            position = stdin_input.find(DELIMETER);
            while(position != string::npos)
            {
                split_input.push_back(stdin_input.substr(0, position));
                stdin_input.erase(0, position + DELIMETER.length());
                position = stdin_input.find(DELIMETER);
            }
            split_input.push_back(stdin_input);

            process_flags(split_input.size(), split_input, state, element_shape, iterations, run_program);
        }
        // Main algorithm
        else if(state == 2)
        {
            // Calculations for problem setup
            delta = plate_size / element_shape; // Calculate distance between each point
            heat_source_start_jdx = (heat_source_start_j / delta) + 1; // Starting point heat source
            heat_source_end_jdx = heat_source_end_j / delta; // End point heat source

            // Allocate memory
            h_h = (float*)malloc(sizeof(float)*element_shape*element_shape);
            cudaMalloc((void**)&d_h, sizeof(float)*element_shape*element_shape);
            cudaMalloc((void**)&d_g, sizeof(float)*element_shape*element_shape);

            // Initialize the top of the temperature grid
            for(unsigned int j = 0; j < element_shape; j++)
            {
                if((j < heat_source_start_jdx) || (j > heat_source_end_jdx))
                {
                    h_h[j] = AMBIENT_TEMP;
                }
                else
                {
                    h_h[j] = HEAT_SOURCE_TEMP;
                } 
            }

            // Initialize the bottom of the temperature grid
            // Leave this separate since the bottom and top sides do not have any contiguous memory
            for(unsigned int j = 0; j < element_shape; j++)
            {
                h_h[(element_shape - 1)*element_shape + j] = AMBIENT_TEMP;
            }    

            // Initialize the side of the temperature grid
            // Combine left and right to tak advantage of the minimal contiguous memory
            for(unsigned int i = element_shape; i < element_shape*element_shape; i += element_shape)
            {
                h_h[i] = AMBIENT_TEMP; // left side
                h_h[i + (element_shape - 1)] = AMBIENT_TEMP; // right side
            }
            
            // Copy initalized arrays to the GPU
            cudaMemcpy(d_h, h_h, sizeof(float)*element_shape*element_shape, cudaMemcpyHostToDevice);

            
            /* Perform the calculation
             * Algorithm will split in two places to keep the threads from 
             *     overwriting numbers that are needed for "this time frame's" instance
             */
            dim3 threads_per_block_2d(BLOCK_SIZE_2D, BLOCK_SIZE_2D, 1);
            dim3 block_per_grid_2d((element_shape + BLOCK_SIZE_2D - 1)/BLOCK_SIZE_2D, 
                (element_shape + BLOCK_SIZE_2D - 1)/BLOCK_SIZE_2D);
            
            dim3 threads_per_block_1d(BLOCK_SIZE_1D, 1, 1);
            dim3 block_per_grid_1d((element_shape*element_shape + BLOCK_SIZE_1D - 1)/BLOCK_SIZE_1D);

            // Initalize d_g
            copy_elements<<<block_per_grid_1d, threads_per_block_1d>>>(d_g, d_h, element_shape*element_shape);

            // Iterations
            start = chrono::high_resolution_clock::now();
            for(unsigned int i = 0; i < iterations; i++)
            {
                // Implement kernel for calculation
                relaxation<<<block_per_grid_2d, threads_per_block_2d>>>(d_h, d_g, element_shape);

                // Implement kernel for data transfer
                copy_elements<<<block_per_grid_1d, threads_per_block_1d>>>(d_h, d_g, element_shape*element_shape);
            }
            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Calculation duration (milliseconds): " << duration.count() << endl;

            // Copy solved arrays to host
            cudaMemcpy(h_h, d_h, sizeof(float)*element_shape*element_shape, cudaMemcpyDeviceToHost);

            // Go to printing the algorithm
            state = 3;
        }
        // Print to csv
        else if(state == 3)
        {
            // Write results to a csv file
            for(unsigned int i = 0; i < element_shape; i++)
            {
                for(unsigned int j = 0; j < element_shape; j++)
                {
                    fs << setw(6) << setprecision(2) << fixed << h_h[i*element_shape + j] << ", ";
                }
                fs << endl;
            }
            fs << endl << endl;
            

            /* Free memory */
            free(h_h);
            cudaFree(d_h);
            cudaFree(d_g);
            
            /* Go to state zero for user input */
            state = 0;
        }
        else
        {
            cout << "Oh snap! Something impossible happened! Check your hardware..." << endl;
            break;
        }
    }

    // Close output file
    fs.close();

    // Exit success
    return 0;
}

/******************************************************************************/
/******************************************************************************/


/***************************** F U N C T I O N S ******************************/

/*
 * Name: relaxation
 * Type: __global__ function
 * Return: void
 * Parmeters: float* a, float* b, int n
 * Purpose: Applies the relaxation method to the given system.
 */
 __global__ void relaxation(float* a, float* b, int n)
 {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if((row > 0) && (col > 0) && (row < (n - 1)) && (col < (n - 1)))
    {
        b[row*n + col] = 0.25*(a[(row - 1)*n + col] + a[(row + 1)*n + col] + 
            a[row*n + (col - 1)] + a[row*n + (col + 1)]);
    }
 }


 /*
 * Name: copy
 * Type: __global__ function
 * Return: void
 * Parmeters: float* a, float* b, int n
 * Purpose: transfers the data from b to a.
 */
 __global__ void copy_elements(float* dest, float* source, int n)
 {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;

    if(tid < n)
    {
        dest[tid] = source[tid];
    }
 }


/*
 * Name: process_flags
 * Type: function
 * Return: void
 * Parmeters: const unsigned int&, const char* const* [], unsigned int&,
 *     unsigned int&, unsigned int&, bool&
 * Purpose: Process the flags from user input.
 */
 void process_flags(const unsigned int& count, const vector<string> args, unsigned int& state,
    unsigned int& shape, unsigned int& iters, bool& run)
 {
    unsigned int i = 0;
    while(i < count)
    {   
        if(args[i] == "-N")
        {
            try
            {
                shape = stoul(args.at(i + 1));

                try
                {
                    if(shape < 1)
                    {
                        throw invalid_argument("Number of nodes");
                    }
                    state = 2;
                }
                catch (invalid_argument &e)
                {
                    cout << "Invalid argument: " << e.what() << endl;
                    cout << "Number of nodes must be greater than 0" << endl;
                    state = 0;
                }
            }
            catch(out_of_range &e)
            {
                cerr << "Out of range error: " << e.what() << endl;
                cerr << "Unable to parse input: -N" << endl;
                state = 0;
            }
            catch(invalid_argument &e)
            {
                cerr << "Invalid argument: " << e.what() << endl;
                cerr << "Unable to parse input: -N " << args[i + 1] << endl;
                state = 0;
            }
            catch(logic_error &e)
            {
                cerr << "Logic error: " << e.what() << endl;
                cerr << "Unable to parse input: -N " << args[i + 1] << endl;
                state = 0;
            }
            
            i += 2;           
        }
        else if(args[i] == "-I")
        {
            try
            {
                iters = stoul(args.at(i + 1));

                try
                {
                    if(iters < 1)
                    {
                        throw invalid_argument("Number of iterations");
                    }

                    state = 2;
                }
                catch (invalid_argument &e)
                {
                    cout << "Invalid argument: " << e.what() << endl;
                    cout << "Number of nodes must be greater than 0" << endl;
                    state = 0;
                }
            }
            catch(out_of_range &e)
            {
                cerr << "Out of range error: " << e.what() << endl;
                cerr << "Unable to parse input: -I" << endl;
                state = 0;
            }
            catch(invalid_argument &e)
            {
                cerr << "Invalid argument: " << e.what() << endl;
                cerr << "Unable to parse input: -I " << args[i + 1] << endl;
                state = 0;
            }
            catch(logic_error &e)
            {
                cerr << "Logic error: " << e.what() << endl;
                cerr << "Unable to parse input: -I " << args[i + 1] << endl;
                state = 0;
            }

            i += 2;
        }
        else if(args[i] == "-q")
        {
            run = false;

            i++;
        }
        else
        {
            cerr << "Unable to match input: " << args[i] << endl;
            i++;
            state = 0;
        }
    }
 }

/******************************************************************************/
/******************************************************************************/
