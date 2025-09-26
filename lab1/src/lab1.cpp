// AUTHOR: AUSTIN LOGAN BARBER
// FILE NAME: lab1.cpp
// DATE CREATED: 2025 SEPTEMBER 16
// DATE LAST UPDATED: 2025 SEPTEMBER 18
// PURPOSE: THIS FILE IS THE MAIN file.


/******************************* I N C L U D E ********************************/
#include "lab1.hpp"
#include <list>

/******************************************************************************/
/******************************************************************************/


/******************************** D E F I N E *********************************/
#define PATH_TO_BACKGROUND "../graphics/background.png"
#define PATH_TO_BULLDOG "../graphics/bulldog.png"
#define PATH_TO_LASER "../graphics/laser.png"
#define PATH_TO_BUZZY "../graphics/Buzzy_blue.png"
#define PATH_TO_TIGERS "../graphics/clemson_tigers.png"
#define PATH_TO_START_MENU "../graphics/Start_Screen.png"

#define START_MENU_SCALE (0.7f)
#define BACKGROUND_SCALE (START_MENU_SCALE)
#define BULLDOG_SCALE (0.21f)
#define LASER_SCALE (0.21f)
#define BUZZY_SCALE (0.21f)
#define CLEMSON_TIGER_SCALE (0.07f)


/******************************************************************************/
/******************************************************************************/


/********************************* U S I N G **********************************/
using namespace sf;
using namespace std;

/******************************************************************************/
/******************************************************************************/


/********************************** M A I N ***********************************/

int main(void)
{
    // Create a video mode object
    VideoMode video_mode(807, 721);

    // Create and open a window for the game
    RenderWindow window(video_mode, "Buzzy Defender", Style::Default);

    /* For the start menu */
    Texture texture_start_menu; // Create a texture to hold a graphic on the GPU
    texture_start_menu.loadFromFile(PATH_TO_START_MENU); // Load a graphic into the texture
    Sprite sprite_start_menu; // Create a sprite for the start menu background
    sprite_start_menu.setTexture(texture_start_menu); // Attach the texture to the sprite
    sprite_start_menu.setScale(START_MENU_SCALE, START_MENU_SCALE);
    sprite_start_menu.setPosition(0, 0); // Set the sprite_start_menu to cover the screen

    /* For the in-game background */
    Texture texture_background; // Create a texture to hold a graphic on the GPU
    texture_background.loadFromFile(PATH_TO_BACKGROUND); // Load a graphic into the texture
    Sprite sprite_background; // Create a sprite for the in-game background
    sprite_background.setTexture(texture_background); // Attach the texture to the sprite
    sprite_background.setPosition(0, 0); // Set the sprite_background to cover the screen
    sprite_background.setScale(BACKGROUND_SCALE, BACKGROUND_SCALE);
    FloatRect background_size = sprite_background.getGlobalBounds();

    /* Buzzy */
    Texture buzzy_texture;
    buzzy_texture.loadFromFile(PATH_TO_BUZZY);
    ECE_Buzzy buzzy(buzzy_texture, sprite_background, BUZZY_SCALE, 5.0);

    /* Lasers */
    Texture laser_texture;
    laser_texture.loadFromFile(PATH_TO_LASER);
    list<ECE_LaserBlast> lasers;
    list<ECE_LaserBlast>::iterator lasers_it;

    /* Enemies */
    Texture bulldog_texture;
    bulldog_texture.loadFromFile(PATH_TO_BULLDOG);
    Texture tiger_texture;
    tiger_texture.loadFromFile(PATH_TO_TIGERS);
    unsigned int enemy_fire = 0;
    bool travel_right = true;
    list<ECE_Enemy> enemies;
    list<ECE_Enemy>::iterator enemies_it;
    list<ECE_Enemy>::iterator enemies_it2;
    list<ECE_Enemy>::iterator enemies_left;
    list<ECE_Enemy>::iterator enemies_right;

    
    /* Define States */
    // state = 0; Start Menu
    // state = 1; Initialize the game
    // state = 2; Game mode
    unsigned int state = 0;

    /* Track time */
    Clock clock;
    Clock enemy_fire_clock;
    Time dt;
    Time enemy_dt;


    // Helper variables
    bool is_space_pressed = false;
    

    /* Main application */
    while (window.isOpen())
    {   
        
        
        /* Game mode */
        if(state == 2)
        {
            // Track duration between loops
            dt = clock.restart();
            
            /*
            ****************************************
            Handle the players input
            ****************************************
            */
            if(Keyboard::isKeyPressed(Keyboard::Left))
            {
                buzzy.move_left(dt.asSeconds());
            }
            else if(Keyboard::isKeyPressed(Keyboard::Right))
            {
                buzzy.move_right(dt.asSeconds());
            }
            
            if(Keyboard::isKeyPressed(Keyboard::Space) && !is_space_pressed)
            {
                lasers.push_back(ECE_LaserBlast(laser_texture,
                    sprite_background, buzzy, true, LASER_SCALE, 5.0));

                is_space_pressed = true;
                
                //while (Keyboard::isKeyPressed(Keyboard::Space)) { asm("NOP"); }
            }
            else if(!Keyboard::isKeyPressed(Keyboard::Space) && is_space_pressed)
            {
                is_space_pressed = false;
            }
            
            if (Keyboard::isKeyPressed(Keyboard::Escape))
            {
                state = 0;
                
                // Prevent the key from being read again
                while (Keyboard::isKeyPressed(Keyboard::Escape)) { asm("NOP"); }
            }
            
            

            /*
            ****************************************
            Update the scene
            ****************************************
            */


            /* Move laser blasts */
            lasers_it = lasers.begin();
            while(lasers_it != lasers.end())
            {
                if(lasers_it->in_bounds())
                {
                    if(lasers_it->get_owner())
                    {
                        lasers_it->move_down(dt.asSeconds());
                    }
                    else
                    {
                        lasers_it->move_up(dt.asSeconds());
                    }
                    lasers_it++;
                }
                else
                {
                    lasers_it = lasers.erase(lasers_it);
                }
            }


            /* Move Enemies */
            enemies_it = enemies.begin();
            enemies_left = enemies.begin();
            enemies_right = enemies.end();
            enemies_right--;

            // Find left and right most enemy
            enemies_it = enemies.begin();
            while(enemies_it != enemies.end())
            {
                if((enemies_it->getGlobalBounds()).left < (enemies_left->getGlobalBounds()).left)
                {
                    enemies_left = enemies_it;
                }

                if((enemies_it->getGlobalBounds()).left + (enemies_it->getGlobalBounds()).width > 
                (enemies_right->getGlobalBounds()).left + (enemies_right->getGlobalBounds()).width)
                {
                    enemies_right = enemies_it;
                }
                enemies_it++;
            }
            
            // Move the enemies
            if(travel_right)
            {
                enemies_it = enemies.begin();
                while(enemies_it != enemies.end())
                {
                    enemies_it->move_right(dt.asSeconds());
                    enemies_it->move_up(dt.asSeconds());
                    enemies_it++;
                }

                if((enemies_right->getGlobalBounds()).left + (enemies_right->getGlobalBounds()).width >=
                    (enemies_right->get_limits()).left + (enemies_right->get_limits()).width)
                {
                    travel_right = false;
                }
            }
            else
            {
                enemies_it = enemies.begin();
                while(enemies_it != enemies.end())
                {
                    enemies_it->move_left(dt.asSeconds());
                    enemies_it->move_up(dt.asSeconds());
                    enemies_it++;
                }

                if((enemies_left->getGlobalBounds()).left <= (enemies_left->get_limits()).left)
                {
                    travel_right = true;
                }
            }


            /* Delete enemies or reset buzzy */
            lasers_it = lasers.begin();
            while(lasers_it != lasers.end())
            {
                enemies_it = enemies.begin();
                while(enemies_it != enemies.end() && !lasers.empty())
                {
                    if((lasers_it->getGlobalBounds()).intersects(enemies_it->getGlobalBounds()) && 
                        (lasers_it->get_is_buzzy()))
                    {
                        enemies_it = enemies.erase(enemies_it);
                        lasers_it = lasers.erase(lasers_it);

                        if(enemies.empty())
                        {
                            state = 0;
                        }
                    }
                    enemies_it++;
                }

                if((lasers_it->getGlobalBounds()).intersects(buzzy.getGlobalBounds()) && 
                    !(lasers_it->get_is_buzzy()))
                {
                    buzzy.decrement_lives();
                    buzzy.reset_pos(sprite_background);

                    if(buzzy.get_lives() == 0)
                    {
                        state = 0;
                    }
                }

                lasers_it++;
            }


            /* Enemy fire */
            if((enemy_fire_clock.getElapsedTime().asSeconds() >= 2) && !enemies.empty())
            {
                srand(time(0));
                enemy_fire = rand() % enemies.size();
                enemies_it = enemies.begin();
                advance(enemies_it, enemy_fire);
                lasers.push_back(ECE_LaserBlast(laser_texture,
                        sprite_background, *enemies_it, false, LASER_SCALE, 5.0));
                enemy_dt = enemy_fire_clock.restart();
            } 
                

            /*
            ****************************************
            Draw the Window
            ****************************************
            */
            
            // Draw the window
            window.clear();
            window.draw(sprite_background);
            window.draw(buzzy);
            for(ECE_Enemy e : enemies)
            {
                window.draw(e);
            }
            for(ECE_LaserBlast l : lasers)
            {
                window.draw(l);
            }
            window.display();
        }
        /* Initilize State */
        else if(state == 1)
        {
            enemies.clear();
            lasers.clear();
            
            enemies.push_back(ECE_Enemy(bulldog_texture, sprite_background, buzzy, 
            BULLDOG_SCALE, 10.0, 60.0));
            enemies_it = enemies.begin();
            enemies_it->setPosition(background_size.left, 
                background_size.top + background_size.height - (enemies_it->getGlobalBounds()).height);
            
            // First row
            for(unsigned int i = 1; i < 8; i++)
            {
                FloatRect prev = enemies_it->getGlobalBounds();
                if(i % 2 == 0)
                {
                    enemies.push_back(ECE_Enemy(bulldog_texture, sprite_background, buzzy, 
                        BULLDOG_SCALE, 10.0, 60.0));
                }
                else
                {
                    enemies.push_back(ECE_Enemy(tiger_texture, sprite_background, buzzy, 
                        CLEMSON_TIGER_SCALE, 10.0, 60.0));
                }
                enemies_it++;
                enemies_it->setPosition(prev.left + prev.width + 20, prev.top);
            }

            // Second Row
            enemies.push_back(ECE_Enemy(tiger_texture, sprite_background, buzzy, 
            CLEMSON_TIGER_SCALE, 10.0, 60.0));
            enemies_it = enemies.begin();
            enemies_it2 = enemies.end();
            enemies_it2--;
            enemies_it2->setPosition(background_size.left + (enemies_it->getGlobalBounds()).width, 
                (enemies_it->getGlobalBounds()).top - (enemies_it2->getGlobalBounds()).height - 40);
            for(unsigned int i = 1; i < 4; i++)
            {
                FloatRect prev = enemies_it2->getGlobalBounds();
                if(i % 2 == 1)
                {
                    enemies.push_back(ECE_Enemy(bulldog_texture, sprite_background, buzzy, 
                        BULLDOG_SCALE, 10.0, 60.0));
                }
                else
                {
                    enemies.push_back(ECE_Enemy(tiger_texture, sprite_background, buzzy, 
                        CLEMSON_TIGER_SCALE, 10.0, 60.0));
                }
                enemies_it2++;
                enemies_it2->setPosition(prev.left + prev.width + 20, prev.top);
            }

            buzzy.reset_lives();
            buzzy.reset_pos(sprite_background);

            state = 2;

            // Restart clocks
            enemy_dt = enemy_fire_clock.restart(); 
            dt = clock.restart();
        }
        /* Start menu */
        else if(state == 0)
        {
            if (Keyboard::isKeyPressed(Keyboard::Enter))
            {
                state = 1;
                
                // Prevent the key from being read again
                // TODO: Make this more efficient
                while (Keyboard::isKeyPressed(Keyboard::Enter)) { asm("NOP"); }
            }
            else if(Keyboard::isKeyPressed(Keyboard::Escape))
            {
                window.close();
            }
            
            // Clear everything from the last frame
            window.clear();

            // Draw our game scene here
            window.draw(sprite_start_menu);

            // Show everything we just drew
            window.display();
        }

    }

    // Main returns an int; 0 is no error.
    return 0;
}


/******************************************************************************/
/******************************************************************************/

