// AUTHOR: AUSTIN LOGAN BARBER
// FILE NAME: ECE_Enemy.cpp
// DATE CREATED: 2025 SEPTEMBER 18
// DATE LAST UPDATED: 2025 SEPTEMBER 18
// PURPOSE: THIS FILE IS THE MAIN file.


/******************************* I N C L U D E ********************************/
#include "ECE_Enemy.hpp"

/******************************************************************************/
/******************************************************************************/


/******************  C L A S S   I M P L E M E N T A T I O N ******************/


/*
 * Name: ECE_Enemy
 * Type: Default class constructor
 * Parameters: void
 * Return: ECE_Enemy
 * Purpose: Provides the default constructor for the ECE_Enemy class.
 */
ECE_Enemy::ECE_Enemy(void) {}

/*
 * Name: ECE_Enemy
 * Type: Default class constructor
 * Parameters: Texture, Sprite, Sprite, float, float, float
 * Return: ECE_Enemy
 * Purpose: Provides the parameterized contstructor for the ECE_Enemy class.
 */
ECE_Enemy::ECE_Enemy(Texture& texture, Sprite background, Sprite buzzy, 
    float scale, float htime, float vtime) 
{
    // Create the Buzzy Sprite
    this->setTexture(texture);
    this->setScale(scale, scale);

    // Calculate where to center buzzy
    FloatRect background_size = background.getGlobalBounds();
    FloatRect enemy_size = this->getGlobalBounds();
    FloatRect buzzy_size = buzzy.getGlobalBounds();

    // Set the Enemy's bounds
    (this->limits) = FloatRect(background_size.left, 
        background_size.top + buzzy_size.height, 
        background_size.width, background_size.height - buzzy_size.height);

    // Calculate Buzzy's speed
    (this->hspeed) = (this->limits).width/htime;
    (this->vspeed) = (this->limits).height/vtime;
}

/*
 * Name: ECE_Enemy
 * Type: Class destructor
 * Parameters: void
 * Return: ECE_Enemy
 * Purpose: Provides the default destructor for the ECE_Enemy class.
 */
ECE_Enemy::~ECE_Enemy(void) {}


/*
 * Name: get_hspeed
 * Type: Member function
 * Parameters: void
 * Return: float
 * Purpose: Returns buzzy's speed.
 */
float ECE_Enemy::get_hspeed(void) const
{
    return (this->hspeed);
}


/*
 * Name: set_hspeed
 * Type: Member function
 * Parameters: float
 * Return: void
 * Purpose: Sets buzzy's speed.
 */
void ECE_Enemy::set_hspeed(float hspeed) 
{
    (this->hspeed) = hspeed;
}


/*
 * Name: get_vspeed
 * Type: Member function
 * Parameters: void
 * Return: float
 * Purpose: Returns buzzy's speed.
 */
float ECE_Enemy::get_vspeed(void) const
{
    return (this->vspeed);
}


/*
 * Name: set_hspeed
 * Type: Member function
 * Parameters: float
 * Return: void
 * Purpose: Sets buzzy's speed.
 */
void ECE_Enemy::set_vspeed(float vspeed) 
{
    (this->vspeed) = vspeed;
}


/*
 * Name: get_limit
 * Type: Member function
 * Parameters: void
 * Return: FloatRect
 * Purpose: Returns the lower and upper x bounds. 
 */
FloatRect ECE_Enemy::get_limits(void) const
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
void ECE_Enemy::set_limits(FloatRect bounds) 
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
void ECE_Enemy::move_left(float dt) 
{
    // Get Buzzy's rectangle bounds
    FloatRect enemy_bounds = this->getGlobalBounds();

    if(enemy_bounds.left > (this->limits).left)
    {
        this->move((this->hspeed)*dt*(-1.0), 0.0);
    }  
}


/*
 * Name: move_right
 * Type: Member function
 * Parameters: float
 * Return: void
 * Purpose: Moves Buzzy to the right. 
 */
void ECE_Enemy::move_right(float dt) 
{
    // Get Buzzy's rectangle bounds
    FloatRect enemy_bounds = this->getGlobalBounds();

    if((enemy_bounds.left + enemy_bounds.width) < 
        ((this->limits).left + (this->limits).width))
    {
        this->move((this->hspeed)*dt, 0.0);
    }
}


/*
 * Name: move_right
 * Type: Member function
 * Parameters: float
 * Return: void
 * Purpose: Moves Buzzy to the right. 
 */
void ECE_Enemy::move_up(float dt) 
{
    // Get Buzzy's rectangle bounds
    FloatRect enemy_bounds = this->getGlobalBounds();

    if((enemy_bounds.left + enemy_bounds.width) < 
        ((this->limits).left + (this->limits).width))
    {
        this->move(0.0, -(this->vspeed)*dt);
    }
}



/******************************************************************************/
/******************************************************************************/


