// Author: Austin Logan Barber
// File Name: Lab6.cpp
// Date Created: 2025 November 21
// Date Last Updated: 2025 November 21
// Purpose: This file holds the code for ECE 6122 >> Lab 6


/******************************* I N C L U D E ********************************/
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <mpi.h>
#include <random>

/******************************************************************************/
/******************************************************************************/


/********************************* U S I N G **********************************/
using namespace std;

/******************************************************************************/
/******************************************************************************/


/**************************** P R O T O T Y P E S *****************************/

void process_flags(const int&, const char* const [], int&, int&, int& );

/******************************************************************************/
/******************************************************************************/


/********************************** M A I N ***********************************/
int main(int argc, char* argv[])
{
    /* MPI Variables */
    int rc;
    int rank;
    int numTasks;
    
    // Initialize MPI
    rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS) 
    {
        cerr << "Error starting MPI program. Terminating." << endl;
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    

    /* Application Variables */
    int evalInteg = 1; // Which integral to run
    int numRandSamples = 1000000; // Number of random samples to generate
    double localSum = 0.0;
    double globalSum = 0.0;
    double result;
    double* arrLocalSums = nullptr;

    // Process flags
    process_flags(argc, argv, evalInteg, numRandSamples, rc);
    int samplesPerTask = numRandSamples/numTasks;
    int samplesRem = numRandSamples%numTasks; // Remaining samples
    for(int i = samplesRem - 1; i > -1; --i)
    {
        if(rank == i)
        {
            ++samplesPerTask;
        }
    }

    // Allocate memory for each local sum on the root task
    if(rank == 0)
    {
        arrLocalSums = new double[numTasks];
    }


    // Generate random samples
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(0.0,1.0);
    double* function = new double[samplesPerTask];
    double number;
    for(int i = 0; i < samplesPerTask; i++)
    {
        if(evalInteg == 1)
        {
            function[i] = pow(distribution(generator), 2.0);
        }
        else
        {
            function[i] = exp(-1.0*pow(distribution(generator), 2.0));
        }
    }

    // Sum each sample
    for(int i = 0; i < samplesPerTask; ++i)
    {
        localSum += function[i];
    }

    // Gather the local sums for each task
    MPI_Gather(&localSum, 1, MPI_DOUBLE, 
        arrLocalSums, 1, MPI_DOUBLE, 
        0, MPI_COMM_WORLD);


    // Peform final calculation
    if(rank == 0)
    {
        for(int i = 0; i < numTasks; ++i)
        {
            globalSum += arrLocalSums[i];
        }
        result = globalSum/numRandSamples;
        cout << "The estimate for integral " << evalInteg << " is " << 
            fixed << setprecision(8) << result << endl << "Bye!" << endl;

        // Free the dynamically allocated memory in root
        delete[] arrLocalSums;
        arrLocalSums = nullptr;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    
    // Free the dynamically allocated memory
    delete[] function;
    function = nullptr;

    // Close MPI
    MPI_Finalize();

    // Exit Success
    return 0;
}


/******************************************************************************/
/******************************************************************************/


/***************************** F U N C T I O N S ******************************/

/*
 * Name: process_flags
 * Type: function
 * Return: void
 * Parmeters: const unsigned int&, const char* const* [], int&, int&, int&
 * Purpose: Process the flags from user input.
 */
void process_flags(const int &count, const char* const args[], int &integral, 
        int &samples, int& rc)
{
    int temp;

    for(int i = 1; i < count; i += 2)
    {   
        if(strcmp(args[i], "-P") == 0)
        {   
            try
            {
                temp = stoi(args[i + 1]);
                
                if((temp == 1) || (temp == 2))
                {
                    integral = temp;
                }
                else
                {
                    throw invalid_argument("integral to run");
                }
            }
            catch (invalid_argument &e)
            {
                cerr << "Invalid argument: " << e.what() << endl;
                cerr << "The integral to run must be 1 or 2." << endl;
                MPI_Abort(MPI_COMM_WORLD, rc);
                exit(EXIT_FAILURE);
            }
        }
        else if(strcmp(args[i], "-N") == 0)
        {
            try
            {
                temp = stoi(args[i + 1]);
                
                if(temp > 0)
                {
                    samples = temp;
                }
                else
                {
                    throw invalid_argument("number of samples");
                }
            }
            catch (invalid_argument &e)
            {
                cerr << "Invalid argument: " << e.what() << endl;
                cerr << "The number of samples to use must be" << 
                    "greater than 0" << endl;
                MPI_Abort(MPI_COMM_WORLD, rc);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            cerr << "Invalid arguments" << endl;
            MPI_Abort(MPI_COMM_WORLD, rc);
            exit(EXIT_FAILURE);
        }
    }
}

/******************************************************************************/
/******************************************************************************/

