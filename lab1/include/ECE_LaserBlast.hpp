// AUTHOR: AUSTIN LOGAN BARBER
// FILE NAME: ECE_LastBlast.hpp
// DATE CREATED: 2025 SEPTEMBER 18
// DATE LAST UPDATED: 2025 SEPTEMBER 20
// PURPOSE: This is the header file for the ECE_LaserBlast class.


// Header guards
#ifndef ECE_LASERBLAST_HPP
#define ECE_LASERBLAST_HPP


/******************************* I N C L U D E ********************************/
#include <SFML/Graphics.hpp>
#include <string>

/******************************************************************************/
/******************************************************************************/


/********************************* U S I N G **********************************/
using namespace sf;

/******************************************************************************/
/******************************************************************************/


/********************** C L A S S   D E F I N I T I O N ***********************/

/*
 * Name: ECE_LaserBlast
 * Type: class
 * Purpose: Derived Sprite object for the laser blast.
 */
class ECE_LaserBlast : public Sprite 
{
    public:
        ECE_LaserBlast(void); // Default constructor
        ECE_LaserBlast(Texture&, Sprite, Sprite, bool, float, float); // Parameterized constructor
        ~ECE_LaserBlast(); // Destructor
        float get_speed(void) const; // Return speed of laser blast
        void set_speed(float speed); // Set speed of laser blast
        bool get_owner(void) const; // Returns who owns the laser blast
        void move_up(float dt); // Move the laser up
        void move_down(float dt); // Move the last down
        bool in_bounds(void) const; // Returns if the laser is in the bounds
        bool get_is_buzzy(void) const; // Returns if the laser is in the bounds

    private:
        Texture texture;
        float speed;
        bool is_buzzy;
        FloatRect limits;
};

/******************************************************************************/
/******************************************************************************/


#endif // ECE_LASERBLAST_HPP
