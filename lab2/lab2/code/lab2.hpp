// AUTHOR: AUSTIN LOGAN BARBER
// FILE NAME: lab2.hpp
// DATE CREATED: 2025 SEPTEMBER 29
// DATE LAST UPDATED: 2025 SEPTEMBER 30
// PURPOSE: This file is the main header file.

// Header guards
#ifndef LAB2_HPP
#define LAB2_HPP


/******************************* I N C L U D E ********************************/
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include "lib.hpp"
#include <mutex>
#include <omp.h>
#include <random>
#include <SFML/Graphics.hpp>
#include <thread>

/******************************************************************************/
/******************************************************************************/


/**************************** P R O T O T Y P E S *****************************/
void col_alg(bool *, bool *, const int *, const int *, 
    const int &, const int &, int);

void copy_cells(bool *, bool *, const int, const int);

void sequential_alg(bool *, bool *, const int*, const int *, const int &, 
    const int &, const int &);

int omp_alg(bool *, bool *, const int*, const int *, const int &, const int &, 
    const int&);

void thrd_alg(bool *, bool *, const int*, const int *, const int &, const int &, 
    const int &, std::thread *, const int &, const int &, const int &);

void thrd_sub_alg(bool *, bool *, const int *, const int *, const int &, 
    const int &, const int, const int);

/******************************************************************************/
/******************************************************************************/



#endif // LAB2_HPP
