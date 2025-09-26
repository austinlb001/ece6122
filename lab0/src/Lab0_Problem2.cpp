// AUTHOR: AUSTIN LOGAN BARBER
// FILE NAME: Lab0_Problem1.cpp
// DATE CREATED: 2025 AUGUST 20
// DATE LAST UPDATED: 2025 AUGUST 20
// CLASS: ECE 6122 - Q
// PURPOSE: This file is my solution to ECE 6122 >> LAB 0 >> PROBLEM 1.

/******************************* I N C L U D E ********************************/
#include<iostream>
#include<vector>
#include<tuple>

/******************************************************************************/
/******************************************************************************/


/******************************** U S E *********************************/
using namespace std;

/******************************************************************************/
/******************************************************************************/


/***************************** P R T O T Y P E S ******************************/

// Used to determine if a number is prime.
bool isPrime(int);

// Used to determine if a character is numeric;
bool isNumeric(char);

// Get a list of primes below max.
void getPrimes(unsigned int, vector<unsigned int>&);

// Get a list of the sums of primes
void getSumPrimes(vector<unsigned int>&, vector<unsigned int>&);

// Return the prime with the longest consecutive sum or primes.
tuple<unsigned int, unsigned int, unsigned int, unsigned int>
largestSumOfPrimes(unsigned int val, vector<unsigned int> &listOfPrimes, vector<unsigned int> &listOfPrimeSums);

/******************************************************************************/
/******************************************************************************/


/********************************** M A I N ***********************************/
int main()
{
    bool run = true;                    // Should the program run again?
    string input = "";                  // Holds user input
    int val = -1;                       // Holds the integer value of user input
    unsigned int state = 0;             // For finite state machine architecture
    vector<unsigned int> primeList;    // Holds a list of prime numbers
    vector<unsigned int> sumList;      // Holds a list of sum of prime numbers
    unsigned int prime = 0;            // Largest prime number with longest consecutive sum of prime numbers
    unsigned int startIndex = 0;       // Starting index for the consecutive sum
    unsigned int lastIndex = 0;        // Last index for the consective sum
    unsigned int lengthOfSum = 0;      // Length of consecutive sum

    while (run)
    {
        // Get input from user
        if (state == 0)
        {
            cout << "Please enter a natural number (0 to quit): ";
            cin >> input;
            state = 1;
        }

        // Verify user input
        if (state == 1)
        {
            for (char &c : input)
            {
                if ((!isNumeric(c)) && (c != '\0'))
                {
                    cout << "Error! Invalid input!" << endl;
                    state = 0;
                    break;
                }
            }

            if (state != 0)
            {
                val = stoi(input);

                // Anything less than 5 is not valid because there is no sum of
                // prime numbers.
                if (val > 4)
                {
                    state = 2;
                }
                else if (val == 0)
                {
                    cout << "Program terminated." << endl
                        << "Have a nice day!" << endl;
                    run = false;
                }
                else
                {
                    cout << "Error! Invalid input!" << endl;
                    state = 0;
                }
            }
        }

        // Find the longest sum of prime numbers that add to a prime number
        if (state == 2)
        {
            getPrimes(val, primeList);
            getSumPrimes(primeList, sumList);
            tie(prime, startIndex, lastIndex, lengthOfSum) = largestSumOfPrimes(val, primeList, sumList);

            cout << "The answer is " << prime << " with " << lengthOfSum << " terms: ";
            cout << primeList[startIndex];
            for (unsigned int i = startIndex + 1; i <= lastIndex - 1; i++)
            {

                cout << " + " << primeList[i];
            }
            cout << endl;

            state = 0;
        }
    }

    // Return zero for clean exit
    return 0;
}

/******************************************************************************/
/******************************************************************************/


/***************************** F U N C T I O N S ******************************/

/*
 * Name: isPrime
 * Type: function
 * Return: bool
 * Parmeters: int
 * Purpose: Determines if a number is prime.
 *  The alogrithm used is pulled from:
 *      https://en.wikipedia.org/wiki/Primality_test#Complexity
 */
bool isPrime(int num)
{
    if (num < 2)
    {
        return false;
    }
    else if ((num == 2) || (num == 3))
    {
        return true;
    }
    else if ((num % 2 == 0) || (num % 3 == 0))
    {
        return false;
    }
    else
    {
        for (int i = 5; i*i <= num; i += 6)
        {
            if ((num % i == 0) || (num % (i + 2) == 0))
            {
                return false;
            }
        }

        return true;
    }
}


/*
 * Name: isNumeric
 * Type: function
 * Return: bool
 * Parmeters: int
 * Purpose: Determines if character is numeric.
 */
bool isNumeric(char c)
{
    // Check if "c" is a number. Reference ASCII table
    if ((c > 47) && (c < 58))
    {
        return true;
    }

    return false;
}


/*
 * Name: getPrimes
 * Type: function
 * Return: void
 * Parmeters: int, vector<unsigned int>
 * Purpose: Gets a list of primes.
 */
void getPrimes(unsigned int max, vector<unsigned int> &listOfPrimes)
{
    unsigned int nextPrime = max; // Holds the value of the next prime number after max
    bool found = false; // Found the next prime number after max?
    while(!found)
    {
        if (isPrime(nextPrime))
        {
            found = true;
        }
        else
        {
            nextPrime++;
        }
    }

    if(listOfPrimes.empty())
    {
        listOfPrimes.push_back(2);
        listOfPrimes.push_back(3);

        for(unsigned int i = 5; i <= nextPrime; i += 6)
        {
            if(isPrime(i))
            {
                listOfPrimes.push_back(i);
            }

            if(isPrime(i + 2))
            {
                listOfPrimes.push_back(i + 2);
            }
        }
    }
    else
    {
        for(unsigned int i = listOfPrimes.back() + 1; i <= nextPrime; i++)
        {
            if(isPrime(i))
            {
                listOfPrimes.push_back(i);
            }
        }
    }
}


/*
 * Name: getSumOfPrimes
 * Type: function
 * Return: void
 * Parmeters: int, vector<unsigned int>
 * Purpose: Gets a list of the sum primes.
 */
void getSumPrimes(vector<unsigned int> &listOfPrimes, vector<unsigned int> &listOfPrimeSums)
{
    if(listOfPrimeSums.empty())
    {
        listOfPrimeSums.push_back(0);
        for (unsigned int i = 0; i < listOfPrimes.size(); i++)
        {
            listOfPrimeSums.push_back(listOfPrimeSums[i] + listOfPrimes[i]);
        }
    }
    else
    {
        for (unsigned int i = listOfPrimeSums.size() - 1; i < listOfPrimes.size(); i++)
        {
            listOfPrimeSums.push_back(listOfPrimeSums[i] + listOfPrimes[i]);
        }
    }
}


/*
 * Name: largestSumOfPrimesistOfPrimeSums.push_back(listOfPrimeSums[i] + listOfPrimes[i + 2]);
         }
 * Type: function
 * Return: unsigned int
 * Parmeters: int, vector<unsigned int>, vector<unsigned int>
 * Purpose: Finds the largest sum or primes.
 */
tuple<unsigned int, unsigned int, unsigned int, unsigned int>
largestSumOfPrimes(unsigned int val, vector<unsigned int> &listOfPrimes, vector<unsigned int> &listOfPrimeSums)
{
    unsigned int length = 0;            // Number of consecutive primes that add to a prime number
    unsigned int prime = 0;             // Longest sum of consecutive prime numbers
    unsigned int start = 0;             // Starting index of consecutive sum
    unsigned int end = 0;               // Last index of consecutive sum
    unsigned int consecutiveSum = 0;    // Holds the consecutive sum

    for (unsigned int i = 2; listOfPrimes[i] <= val; i++)
    {
        for(unsigned int j = 0; j < i; j++)
        {
            if (listOfPrimeSums[i] - listOfPrimeSums[j] > val)
            {
                break;
            }

            consecutiveSum = listOfPrimeSums[i] - listOfPrimeSums[j];

            if (isPrime(consecutiveSum))
            {
                if (length < i - j + 1)
                {
                    prime = consecutiveSum;
                    start = j;
                    end = i;
                    length = i - j;
                }
            }
        }
    }
    return {prime, start, end, length};
}


/******************************************************************************/
/******************************************************************************/
