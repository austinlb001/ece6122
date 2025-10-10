// AUTHOR: AUSTIN LOGAN BARBER
// FILE NAME: lab2.cpp
// DATE CREATED: 2025 SEPTEMBER 29
// DATE LAST UPDATED: 2025 SEPTEMBER 30
// PURPOSE: This is the main file.


/******************************* I N C L U D E ********************************/
#include "lab2.hpp"


/******************************************************************************/
/******************************************************************************/


/******************************** D E F I N E *********************************/



/******************************************************************************/
/******************************************************************************/


/********************************* U S I N G **********************************/
using namespace sf;
using namespace std;

/******************************************************************************/
/******************************************************************************/


/********************************** M A I N ***********************************/

int main(int argc, char* argv[])
{
    XInitThreads();
    
    // Set frequency
    typedef chrono::duration<double, ratio<1, 4>> wait_time_t;
    
    // Variables
    int num_threads = 8;    // Number of threads to use (-n, --num-threads)
    int cell_size = 5;       // Size square cells (>= 1) (-c, --cell-size)
    int window_width = 800;  // Window width (-x, --window-width)
    int window_height = 600; // Height of window (-y, --window-height)
    int type = 0;            // 0 = std::thread; 1 = Sequential; 2 = OpenMP (-t, --type)
    
    // Process input flags
    process_flags(argc, argv, num_threads, cell_size, window_width, window_height, type);
    
    // Create a video mode object
    VideoMode video_mode(window_width, window_height);

    // Create and open a window for the game
    RenderWindow window(video_mode, "Game of Life", Style::Default);

    // Initialize grid
    random_device generator;
    uniform_int_distribution<int> dist(-1, 1);
    const Vector2f cell_size_vec(cell_size, cell_size);
    const int r_offset[8] = {-1, -1, -1, 0, 0, 1 , 1, 1}; // Neighbor coordinates rows                
    const int c_offset[8] = {-1, 0, 1, -1, 1, -1, 0, 1};  // Neighbor coordinates cols
    int num_rows = window_height/cell_size;
    int num_cols = window_width/cell_size;
    int num_cells = num_rows*num_cols;
    bool *cell_grid0 = new bool[num_cells];                // Current state
    bool *cell_grid1 = new bool[num_cells];                // Future state
    
    // alive > 0; dead < 0
    for(int i = 0; i < num_cells; i++)
    {
        if(dist(generator) > 0)
        {
            cell_grid0[i] = true;
        }
        else
        {
            cell_grid0[i] = false;
        }
        cell_grid1[i] = cell_grid0[i];
    }

    // Create thread for reading keyboard
    atomic<bool> close_window = false;
    thread keyboard_thread([&]()
    {
        while(!Keyboard::isKeyPressed(Keyboard::Escape))
        {
            this_thread::yield();
        }

        close_window.store(true);
    });

    // Allocate threads upfront
    int chunk_size = num_rows/num_threads;
    int cell_chunk_size = num_cells/num_threads;
    thread *thread_ptr = nullptr;
    if(type == 0)
    {
        thread_ptr = new thread[num_threads];
    }
    

    // Measure timing
    chrono::time_point start = chrono::high_resolution_clock::now();
    chrono::time_point stop = chrono::high_resolution_clock::now();
    chrono::microseconds duration = chrono::microseconds(0);
    unsigned int generations = 0;
    

    /* Main application */
    while (window.isOpen())
    {   
        // Close window
        if(close_window.load())
        {
            window.close();
        }
        
        // Clear everything from the last frame
        window.clear();

        // Draw cells here
        for(int i = 0; i < num_rows; i++)
        {
            for(int j = 0; j < num_cols; j++)
            {
                RectangleShape cell;
                cell.setPosition(i*cell_size, j*cell_size);
                cell.setSize(cell_size_vec);
                
                // Set color
                if(cell_grid0[i*num_cols + j] == true)
                {
                    cell.setFillColor(Color::White);
                    window.draw(cell);
                }
            }
        }

        // Show everything we just drew
        window.display();

        // Choose algorithm to run
        if(type == 0)
        {
            start = chrono::high_resolution_clock::now();
            thrd_alg(cell_grid0, cell_grid1, r_offset, c_offset, 
                num_rows, num_cols, num_cells, thread_ptr, num_threads, 
                chunk_size, cell_chunk_size);
            stop = chrono::high_resolution_clock::now();

            generations++;
            duration += chrono::duration_cast<chrono::microseconds>(stop - start);
            
            if(generations == 100)
            {   
                cout << "100 generations took " << 
                    duration.count() <<
                    " microseconds with " << num_threads << " std::threads." << endl;

                generations = 0;
                duration = chrono::microseconds(0);
            }
        }
        else if(type == 1)
        {
            start = chrono::high_resolution_clock::now();
            sequential_alg(cell_grid0, cell_grid1, r_offset, c_offset,
                num_rows, num_cols, num_cells);
            stop = chrono::high_resolution_clock::now();

            generations++;
            duration += chrono::duration_cast<chrono::microseconds>(stop - start);

            if(generations == 100)
            {
                cout << "100 generations took " << 
                    duration.count() <<
                    " microseconds with single thread." << endl;

                generations = 0;
                duration = chrono::microseconds(0);
            }
        }
        else if(type == 2)
        {
            start = chrono::high_resolution_clock::now();
            num_threads = omp_alg(cell_grid0, cell_grid1, r_offset, c_offset,
                num_rows, num_cols, num_cells);
            stop = chrono::high_resolution_clock::now();

            generations++;
            duration += chrono::duration_cast<chrono::microseconds>(stop - start);

            if(generations == 100)
            {
                cout << "100 generations took " << 
                    duration.count() << 
                    " microseconds with " << num_threads << " OMP threads." << endl;

                generations = 0;
                duration = chrono::microseconds(0);
            }
        }

        // Wait so user can see the plane change
        this_thread::sleep_for(wait_time_t(1));
    }

    // Join thread
    keyboard_thread.join();

    // Free memory
    delete[] cell_grid0;
    cell_grid0 = nullptr;
    
    delete[] cell_grid1;
    cell_grid1 = nullptr;
    
    delete[] thread_ptr;
    thread_ptr = nullptr;

    // Main returns an int; 0 is no error.
    return 0;
}


/*
 * Name: col_alg
 * Type: function
 * Return: void
 * Parmeters: bool *, bool *, const int&, const int&, int
 * Purpose: Algorithm that iterates over the columns. Easier for std::thread.
 */
void col_alg(bool *arr0, bool *arr1, const int *c, const int *r, 
    const int &nrows, const int &ncols, int idx)
{
    for(int j = 0; j < ncols; j++)
    {   
        int neighbors_alive = 0;
        for(int k = 0, l = 0; k < 8; k++, l++)
        {
            if(in_bounds(idx + r[k], j + c[l], nrows, ncols))
            {
                if(arr0[(idx + r[k])*ncols + (j + c[l])] == true)
                {
                    neighbors_alive++;
                    
                }
            }
        }

        // Implement rules
        if((arr0[idx*ncols + j] == true) && ((neighbors_alive == 2) || (neighbors_alive == 3)))
        {
            arr1[idx*ncols + j] = true;
        }
        else if((arr0[idx*ncols + j] == false) && (neighbors_alive == 3))
        {
            arr1[idx*ncols + j] = true;
        }
        else
        {
            arr1[idx*ncols + j] = false;
        }
    }
}


/*
 * Name: sequential_alg
 * Type: function
 * Return: void
 * Parmeters: bool *, bool *, const int&, const int&, const int&, int
 * Purpose: Sequential algorithm.
 */
void sequential_alg(bool *arr0, bool *arr1, const int *r, const int *c, 
    const int &nrows, const int &ncols, const int &ncells)
{
    for(int i = 0; i < nrows; i++)
    {   
        col_alg(arr0, arr1, r, c, nrows, ncols, i);
    }

    for(int i = 0; i < ncells; i++)
    {   
        arr0[i] = arr1[i];
    }
}


/*
 * Name: omp_alg
 * Type: function
 * Return: unsigned int
 * Parmeters: bool *, bool *, const int&, const int&, const int&, int
 * Purpose: OpenMP algorithm.
 */
int omp_alg(bool *arr0, bool *arr1, const int *r, const int *c,
    const int &nrows, const int &ncols, const int &ncells)
{
    int nthreads = 0;    

    #pragma omp parallel
    {
        #pragma omp master
        {
            nthreads = omp_get_num_threads();
        }
        
        #pragma omp for schedule(dynamic)
        for(int i = 0; i < nrows; i++)
        {   
            col_alg(arr0, arr1, r, c, nrows, ncols, i);
        }
    
        #pragma omp barrier
        #pragma omp for
        for(int i = 0; i < ncells; i++)
        {   
            arr0[i] = arr1[i];
        }
    }

    return nthreads;
}


/*
 * Name: thrd_alg
 * Type: function
 * Return: void
 * Parmeters: bool *, bool *, const int&, const int&, const int&, thread *
 * Purpose: std::thread algorithm.
 * Note: No atomic variables are needed because no same memory location will try
 *          to be accessed at the same time.
 */
void thrd_alg(bool *arr0, bool *arr1, const int *r, const int *c, 
    const int &nrows, const int &ncols, const int &ncells, thread *thrd_ptr, 
    const int &nthreads, const int &chunk, const int &cell_chunk)
{   
    int start = 0;
    int end = 0;
    
    for(int i = 0; i < nthreads; i++)
    {   
        start = i*chunk;
        
        if(i == (nthreads - 1))
        {
            end = nrows;
        }
        else
        {
            end = (i + 1)*chunk;
        }

        thrd_ptr[i] = thread(thrd_sub_alg, arr0, arr1, r, c, nrows, ncols, start, end);
    }

    // Join threads
    for(int i = 0; i < nthreads; i++)
    {
        thrd_ptr[i].join();
    }

    
    for(int i = 0; i < nthreads; i++)
    {   
        start = i*cell_chunk;
        
        if(i == (nthreads - 1))
        {
            end = ncells;
        }
        else
        {
            end = (i + 1)*cell_chunk;
        }
        
        thrd_ptr[i] = thread(copy_cells, arr0, arr1, start, end);
    }

    // Join threads
    for(int i = 0; i < nthreads; i++)
    {
        thrd_ptr[i].join();
    }
}


/*
 * Name: thrd_sub_alg
 * Type: function
 * Return: void
 * Parmeters: bool *, bool *, const int *, const int *, const int &, 
 *              const int &, const int, const int
 * Purpose: This is a helper function for because std::thread has to be special.
 */
void thrd_sub_alg(bool *arr0, bool *arr1, const int *r, const int *c, 
    const int &nrows, const int &ncols, const int start, const int end)
{
    for(int i = start; i < end; i++)
    {
        col_alg(arr0, arr1, r, c, nrows, ncols, i);
    }
}


/*
 * Name: copy_cells
 * Type: function
 * Return: void
 * Parmeters: bool *, bool *, const int&, const int&, const int&, 
 * Purpose: This is a helper function for because std::thread has to be extra special.
 */
void copy_cells(bool *arr0, bool *arr1, const int start, const int end)
{
    for(int i = start; i < end; i++)
    {
        arr0[i] = arr1[i];
    }
}


/******************************************************************************/
/******************************************************************************/
