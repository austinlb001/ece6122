// AUTHOR: AUSTIN LOGAN BARBER
// FILE NAME: lib.cpp
// DATE CREATED: 2025 SEPTEMBER 30
// DATE LAST UPDATED: 2025 SEPTEMBER 30
// PURPOSE: This file contains helper functions.



/******************************* I N C L U D E ********************************/
#include "lib.hpp"

/******************************************************************************/
/******************************************************************************/


/********************************* U S I N G **********************************/
using namespace std;

/******************************************************************************/
/******************************************************************************/


/***************************** F U N C T I O N S ******************************/

/*
 * Name: in_bounds
 * Type: function
 * Return: bool
 * Parmeters: unsigned int&, unsigned int&, const unsigned int&, const unsigned int&
 * Purpose: Prints the help menu.
 */
 bool in_bounds(int row_idx, int col_idx, 
    const int& nrows, const int& ncols)
{
    if((row_idx < 0) || (row_idx >= nrows))
    {
        return false;
    }

    if((col_idx < 0) || (col_idx >= ncols))
    {
        return false;
    }
    
    return true;
}


/*
 * Name: print_help_menu
 * Type: function
 * Return: void
 * Parmeters: void
 * Purpose: Prints the help menu.
 */
 void print_help_menu(void)
 {
    cout << "-n, --num-threads: the number of threads to use (>= 2)" << endl;
    cout << "-c, --cell-size: used to denote the square size of each cell (>= 1)" << endl;
    cout << "-x, --window-width: sets the window width" << endl;
    cout << "-y, --window-height: sets the window height" << endl;
    cout << "-t, --type: is either SEQ, THRD, or OMP where SEQ is sequential, " <<
        "THRD is std::thread, and OMP is OpenMP" << endl;
 }


 /*
 * Name: process_flags
 * Type: function
 * Return: void
 * Parmeters: const unsigned int&, const char* const* [], unsigned int&, unsigned int&, 
 *     unsigned int&, unsigned int&, unsigned int&
 * Purpose: Process the flags from user input.
 */
 void process_flags(const int &count, const char* const args[], int &nthreads, 
        int &csize, int &width, int &height, int &type)
{
    int temp;

    for(int i = 1; i < count; i += 2)
    {   
        if((strcmp(args[i], "-n") == 0) || 
            (strcmp(args[i], "--num-threads") == 0))
        {
            string_to_int(args[i], args[i + 1], temp);
            
            try
            {
                if(temp > 1)
                {
                    nthreads = temp;
                }
                else
                {
                    throw invalid_argument("number of threads");
                }
            }
            catch (invalid_argument &e)
            {
                cout << "Invalid argument: " << e.what() << endl;
                cout << "Number of threads must be greater than 2" << endl;
                print_help_menu();
                exit(EXIT_FAILURE);
            }
        }
        else if((strcmp(args[i], "-c") == 0) || 
            (strcmp(args[i], "--cell-size") == 0))
        {
            string_to_int(args[i], args[i + 1], temp);
            
            try
            {
                if(temp > 0)
                {
                    csize = temp;
                }
                else
                {
                    throw invalid_argument("cell size");
                }
            }
            catch (invalid_argument &e)
            {
                cout << "Invalid argument: " << e.what() << endl;
                cout << "Cell size must be greater than 0" << endl;
                print_help_menu();
                exit(EXIT_FAILURE);
            }
        }
        else if((strcmp(args[i], "-x") == 0) || 
            (strcmp(args[i], "--window-width") == 0))
        {
            string_to_int(args[i], args[i + 1], temp);
            
            try
            {
                if(temp > 0)
                {
                    width = temp;
                }
                else
                {
                    throw invalid_argument("window width");
                }
            }
            catch (invalid_argument &e)
            {
                cout << "Invalid argument: " << e.what() << endl;
                cout << "Window width must be greater than 0" << endl;
                print_help_menu();
                exit(EXIT_FAILURE);
            }
        }
        else if((strcmp(args[i], "-y") == 0) || 
            (strcmp(args[i], "--window-height") == 0))
        {
            string_to_int(args[i], args[i + 1], temp);
            
            try
            {
                if(temp > 0)
                {
                    height = temp;
                }
                else
                {
                    throw invalid_argument("window height");
                }
            }
            
            catch (invalid_argument &e)
            {
                cout << "Invalid argument: " << e.what() << endl;
                cout << "Window height must be greater than 0" << endl;
                print_help_menu();
                exit(EXIT_FAILURE);
            }
        }
        else if((strcmp(args[i], "-t") == 0) || 
            (strcmp(args[i], "--type") == 0))
        {
            try
            {
                if(strcmp(args[i + 1], "THRD") == 0) 
                {
                    type = 0;
                }
                else if(strcmp(args[i + 1], "SEQ") == 0)
                {
                    type = 1;
                }
                else if (strcmp(args[i + 1], "OMP") == 0)
                {
                    type = 2;
                }
                else
                {
                    throw invalid_argument("thread type");
                }
            }
            catch(invalid_argument &e)
            {
                cout << "Invalid argument: " << e.what() << endl;
                cout << "Thread type must be SEQ, THRD, or OMP" << endl;
                print_help_menu();
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            cout << "Invalid arguments" << endl;
            print_help_menu();
            exit(EXIT_FAILURE);
        }
    }
}


 /*
 * Name: string_to_int
 * Type: function
 * Return: void
 * Parmeters: void
 * Purpose: Prints the help menu.
 */
void string_to_int(const char arr[], const char arr1[], int& result)
{
    try
    {
        result = stoi(arr1);
    }
    catch(invalid_argument &e)
    {
        cerr << "Invalid argument: " << e.what() << endl;
        cerr << "Unable to parse input: " << arr << " " << arr1 << endl;
    }
    catch(logic_error &e)
    {
        cerr << "Logic error: " << e.what() << endl;
        cerr << "Unable to parse input: " << arr << " " << arr1 << endl;
    }
}





/******************************************************************************/
/******************************************************************************/
