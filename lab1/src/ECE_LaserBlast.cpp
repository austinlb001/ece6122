// AUTHOR: AUSTIN LOGAN BARBER
// FILE NAME: ECE_LaserBlast.cpp
// DATE CREATED: 2025 SEPTEMBER 18
// DATE LAST UPDATED: 2025 SEPTEMBER 20
// PURPOSE: THIS FILE IS THE MAIN file.


/******************************* I N C L U D E ********************************/
#include "ECE_LaserBlast.hpp"

/******************************************************************************/
/******************************************************************************/


/******************  C L A S S   I M P L E M E N T A T I O N ******************/

/*
 * Name: ECE_LaserBlast
 * Type: Default class constructor
 * Parameters: N/A
 * Return: ECE_Buzzy
 * Purpose: Provides the default contstructor for the ECE_LaserBlast class.
 */
ECE_LaserBlast::ECE_LaserBlast(void) {}

/*
 * Name: ECE_LaserBlast
 * Type: Default class constructor
 * Parameters: const String &, Sprite, Sprite, bool, float, float
 * Return: ECE_Buzzy
 * Purpose: Provides the parameterized contstructor for the ECE_Buzzy class.
 */
ECE_LaserBlast::ECE_LaserBlast(Texture& texture, Sprite background,
    Sprite soldier, bool buzzy_or_enemy, float scale, float vtime) 
{
    // Create LaserBlast Sprite
    this->setTexture(texture);
    this->setScale(scale, scale);

    // Calculate where to fire laser from
    FloatRect background_size = background.getGlobalBounds();
    FloatRect soldier_size = soldier.getGlobalBounds();
    FloatRect laser_size = this->getGlobalBounds();

    // If Buzzy fires
    if(buzzy_or_enemy == true)
    {
        // Set the initial position
        this->setPosition(soldier_size.left + soldier_size.width/2 - laser_size.width/2, soldier_size.top + soldier_size.height);

        // Set the limits
        (this->limits) = FloatRect(soldier_size.left + soldier_size.width/2 - laser_size.width/2, 
            soldier_size.top + soldier_size.height,
            laser_size.width, 
            background_size.height - soldier_size.height);
    }
    else
    {
        // Set the initial position
        this->setPosition(soldier_size.left + soldier_size.width/2 - laser_size.width/2, soldier_size.top - laser_size.height);
    
        // Set the limits
        (this->limits) = FloatRect(soldier_size.top + soldier_size.width/2 - laser_size.width/2, 
            background_size.top,
            laser_size.width, 
            soldier_size.top - background_size.top);
    }

    
    // Calculate LaserBlast's speed
    (this->speed) = background_size.height/vtime;

    // Does this laser belong to buzzy or enemy?
    (this->is_buzzy) = buzzy_or_enemy;
}

/*
 * Name: ECE_LaserBlast
 * Type: Class destructor
 * Parameters: void
 * Return: ECE_Buzzy
 * Purpose: Provides the default destructor for the ECE_Buzzy class.
 */
ECE_LaserBlast::~ECE_LaserBlast(void) {}


/*
 * Name: get_speed
 * Type: Member function
 * Parameters: void
 * Return: float
 * Purpose: Returns the speed of the laser blast.
 */
float ECE_LaserBlast::get_speed(void) const
{
    return (this->speed);
}


/*
 * Name: set_speed
 * Type: Member function
 * Parameters: float
 * Return: void
 * Purpose: Sets the speed of the laser blast.
 */
void ECE_LaserBlast::set_speed(float speed) 
{
    (this->speed) = speed;
}


/*
 * Name: get_owner
 * Type: Member function
 * Parameters: void
 * Return: bool
 * Purpose: Returns who owns the laser blast.
 */
bool ECE_LaserBlast::get_owner(void) const
{
    // Buzzy = true; Enemy = false
    return (this->is_buzzy);
}


/*
 * Name: move_up
 * Type: Member function
 * Parameters: float
 * Return: void
 * Purpose: Move the laser up. 
 */
void ECE_LaserBlast::move_up(float dt) 
{
    this->move(0.0, (this->speed)*dt*(-1.0));  
}


/*
 * Name: move_right
 * Type: Member function
 * Parameters: float
 * Return: void
 * Purpose: Moves Buzzy to the right. 
 */
void ECE_LaserBlast::move_down(float dt) 
{
    this->move(0.0, (this->speed)*dt);       
}


/*
 * Name: in_bounds
 * Type: Member function
 * Parameters: void
 * Return: bool
 * Purpose: Checks to see if the laser is in bounds. 
 */
bool ECE_LaserBlast::in_bounds(void) const
{
    FloatRect laser_size = this->getGlobalBounds();
    
    if((this->is_buzzy) == true)
    {
        if((laser_size.top + laser_size.height) < 
        ((this->limits).top + (this->limits).height) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(laser_size.top > (this->limits).top)
        {
            return true;
        }
        else
        {
            return false;
        }
    }    
}


/*
 * Name: get_is_buzzy
 * Type: Member function
 * Parameters: void
 * Return: bool
 * Purpose: Does this bullet belong to buzzy. 
 */
bool ECE_LaserBlast::get_is_buzzy(void) const
{
    return (this->is_buzzy);
}


/******************************************************************************/
/******************************************************************************/


