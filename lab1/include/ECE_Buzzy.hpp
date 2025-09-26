// AUTHOR: AUSTIN LOGAN BARBER
// FILE NAME: ECE_Buzzy.hpp
// DATE CREATED: 2025 SEPTEMBER 18
// DATE LAST UPDATED: 2025 SEPTEMBER 20
// PURPOSE: This is the header file for the ECE_Buzzy class.


// Header guards
#ifndef ECE_BUZZY_HPP
#define ECE_BUZZY_HPP


/******************************* I N C L U D E ********************************/
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

/******************************************************************************/
/******************************************************************************/


/********************************* U S I N G **********************************/
using namespace sf;

/******************************************************************************/
/******************************************************************************/


/********************** C L A S S   D E F I N I T I O N ***********************/

/*
 * Name: ECE_Buzzy
 * Type: class
 * Purpose: Derived Sprite object for Buzzy.
 */
class ECE_Buzzy : public Sprite 
{
    public:
        ECE_Buzzy(void); // Default constructor.
        ECE_Buzzy(Texture&, Sprite, float, float) ; // Parameterized constructor.
        ~ECE_Buzzy(); // Destructor
        float get_speed(void) const; // Returns Buzzy's speed.
        void set_speed(float); // Set Buzzy's speed.
        FloatRect get_limits(void) const; // Return the x bound
        void set_limits(FloatRect); // Set the x bound
        void move_left(float); // Move Buzzy to the left
        void move_right(float); // Move Buzzy to the right
        void reset_pos(Sprite); // Reset buzzy to center 
        unsigned int get_lives(void) const; // Return how many lives are remaining
        void decrement_lives(void); // Decrements Buzzy's lives 
        void reset_lives(void); // Reset number of lives back to 3

    private:
        Texture texture;
        float speed;
        FloatRect limits;
        unsigned int lives;
};

/******************************************************************************/
/******************************************************************************/


#endif // ECE_BUZZY_HPP
