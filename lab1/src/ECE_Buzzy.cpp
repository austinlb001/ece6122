// AUTHOR: AUSTIN LOGAN BARBER
// FILE NAME: ECE_Buzzy.cpp
// DATE CREATED: 2025 SEPTEMBER 18
// DATE LAST UPDATED: 2025 SEPTEMBER 20
// PURPOSE: THIS FILE IS THE MAIN file.


/******************************* I N C L U D E ********************************/
#include "ECE_Buzzy.hpp"

/******************************************************************************/
/******************************************************************************/


/******************  C L A S S   I M P L E M E N T A T I O N ******************/

/*
 * Name: ECE_Buzzy
 * Type: Default class constructor
 * Parameters: void
 * Return: ECE_Buzzy
 * Purpose: Provides the default constructor for the ECE_Buzzy class.
 */
ECE_Buzzy::ECE_Buzzy(void) {}

/*
 * Name: ECE_Buzzy
 * Type: Default class constructor
 * Parameters: String, Sprite, float, float
 * Return: ECE_Buzzy
 * Purpose: Provides the parameterized contstructor for the ECE_Buzzy class.
 */
ECE_Buzzy::ECE_Buzzy(Texture& texture, Sprite background, float scale,
    float htime) 
{
    // Create the Buzzy Sprite
    this->setTexture(texture);
    this->setScale(scale, scale);

    // Calculate where to center buzzy
    FloatRect background_size = background.getGlobalBounds();
    FloatRect buzzy_size = this->getGlobalBounds();

    this->setPosition(background_size.width/2.0 - buzzy_size.width/2.0, 0.0);

    // Set Buzzy's bounds
    (this->limits) = FloatRect(background_size.left, background_size.top, 
        background_size.width, buzzy_size.height);

    // Calculate Buzzy's speed
    (this->speed) = (this->limits).width/htime;

    // Set number of lives
    (this->lives) = 3;
}

/*
 * Name: ECE_Buzzy
 * Type: Class destructor
 * Parameters: void
 * Return: ECE_Buzzy
 * Purpose: Provides the default destructor for the ECE_Buzzy class.
 */
ECE_Buzzy::~ECE_Buzzy(void) {}


/*
 * Name: get_speed
 * Type: Member function
 * Parameters: void
 * Return: float
 * Purpose: Returns buzzy's speed.
 */
float ECE_Buzzy::get_speed(void) const
{
    return (this->speed);
}


/*
 * Name: set_speed
 * Type: Member function
 * Parameters: float
 * Return: void
 * Purpose: Sets buzzy's speed.
 */
void ECE_Buzzy::set_speed(float speed) 
{
    (this->speed) = speed;
}


/*
 * Name: get_limit
 * Type: Member function
 * Parameters: void
 * Return: FloatRect
 * Purpose: Returns the lower and upper x bounds. 
 */
FloatRect ECE_Buzzy::get_limits(void) const
{
    return (this->limits);
}


/*
 * Name: set_limit
 * Type: Member function
 * Parameters: FloatRect
 * Return: void
 * Purpose: Returns the lower and upper x bounds. 
 */
void ECE_Buzzy::set_limits(FloatRect bounds) 
{
    (this->limits) = bounds;
}


/*
 * Name: move_left
 * Type: Member function
 * Parameters: float
 * Return: void
 * Purpose: Moves Buzzy to the left. 
 */
void ECE_Buzzy::move_left(float dt) 
{
    // Get Buzzy's rectangle bounds
    FloatRect buzzy_size = this->getGlobalBounds();

    if(buzzy_size.left > (this->limits).left)
    {
        this->move((this->speed)*dt*(-1.0), 0.0);
    }  
}


/*
 * Name: move_right
 * Type: Member function
 * Parameters: float
 * Return: void
 * Purpose: Moves Buzzy to the right. 
 */
void ECE_Buzzy::move_right(float dt) 
{
    // Get Buzzy's rectangle bounds
    FloatRect buzzy_size = this->getGlobalBounds();

    if((buzzy_size.left + buzzy_size.width) < 
        ((this->limits).left + (this->limits).width))
    {
        this->move((this->speed)*dt, 0.0);
    }
}


/*
 * Name: reset_pos
 * Type: Member function
 * Parameters: void
 * Return: void
 * Purpose: Moves Buzzy to the center. 
 */
void ECE_Buzzy::reset_pos(Sprite background) 
{
    FloatRect background_size = background.getGlobalBounds();
    FloatRect buzzy_size = this->getGlobalBounds();
    this->setPosition(background_size.width/2.0 - buzzy_size.width/2.0, 0.0);
}


/*
 * Name: get_lives
 * Type: Member function
 * Parameters: void
 * Return: unsigned int
 * Purpose: Return hown many lives Buzzy has remaining. 
 */
unsigned int ECE_Buzzy::get_lives(void) const 
{
    return (this->lives);
}


/*
 * Name: decrement_lives
 * Type: Member function
 * Parameters: void
 * Return: void
 * Purpose: Moves Buzzy to the center. 
 */
void ECE_Buzzy::decrement_lives(void) 
{
    (this->lives)--;
}


/*
 * Name: reset_lives
 * Type: Member function
 * Parameters: void
 * Return: void
 * Purpose: Moves Buzzy to the center. 
 */
void ECE_Buzzy::reset_lives(void) 
{
    (this->lives) = 3;
}


/******************************************************************************/
/******************************************************************************/