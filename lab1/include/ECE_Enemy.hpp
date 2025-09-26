// AUTHOR: AUSTIN LOGAN BARBER
// FILE NAME: ECE_Enemy.hpp
// DATE CREATED: 2025 SEPTEMBER 18
// DATE LAST UPDATED: 2025 SEPTEMBER 20
// PURPOSE: This is the header file for the ECE_Enemy class.


// Header guards
#ifndef ECE_ENEMY_HPP
#define ECE_ENEMY_HPP


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
 * Name: ECE_Enemy
 * Type: class
 * Purpose: Derived Sprite object for enemies.
 */
class ECE_Enemy : public Sprite 
{
    public:
        ECE_Enemy(void); // Default constructor.
        ECE_Enemy(Texture&, Sprite, Sprite, float, float, float) ; // Parameterized constructor.
        ~ECE_Enemy(); // Destructor
        float get_hspeed(void) const; // Returns the Enemy's horizontal speed.
        void set_hspeed(float); // Set the Enemy's horizontal speed.
        float get_vspeed(void) const; // Returns the Enemy's vertical speed.
        void set_vspeed(float); // Set the Enemy's horizontal speed.
        FloatRect get_limits(void) const; // Return the x bound
        void set_limits(FloatRect); // Set the x bound
        void move_left(float); // Move the Enemy to the left
        void move_right(float); // Move the Enemy to the right
        void move_up(float); // Move the Enemy up.

    private:
        Texture texture;
        float hspeed;
        float vspeed;
        FloatRect limits;
};

/******************************************************************************/
/******************************************************************************/


#endif // ECE_ENEMY_HPP
