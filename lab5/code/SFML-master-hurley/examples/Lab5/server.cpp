// Author: Austin Logan Barber
// File Name: server.cpp
// Date Created: 2025 November 06
// Date Last Updated: 2025 November 06
// Purpose: This file provides the server code for ECE 6122 >> Lab5.


/******************************* I N C L U D E ********************************/
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <list>
#include <memory>
#include <utility>
#include <SFML/Network.hpp>


/******************************************************************************/
/******************************************************************************/


/********************************* U S I N G **********************************/
using namespace sf;
using namespace std;
/******************************************************************************/
/******************************************************************************/


/******************************* D E F I N E S ********************************/


/******************************************************************************/
/******************************************************************************/


/**************************** P R O T O T Y P E S *****************************/
void process_flags(const unsigned int&, const char* const [], unsigned short&);

/******************************************************************************/
/******************************************************************************/


/********************************** M A I N ***********************************/
int main(int argc, char* argv[])
{   
    // Network variables
    unsigned short port_num = 61000;
    TcpListener listener;
    list<unique_ptr<TcpSocket>> clients;
    SocketSelector selector;
    Packet received;
    string message;
    Socket::Status status;

    // File variables
    fstream fs;
    fs.open("server.log", fstream::out | fstream::app);
    
    // Process input flags
    process_flags(argc, argv, port_num);

    // Listen to the given port for incoming connections
    if (listener.listen(port_num) != sf::Socket::Done)
    {
        cerr << "Unable to listen on port " << port_num;
        exit(EXIT_FAILURE);
    }
    cout << "Server is listening for connections on port " << port_num << endl;
    
    // Add listerner to the selector
    selector.add(listener);

    // Application
    while(true)
    {
        if(selector.wait(Time::Zero))
        {
            if(selector.isReady(listener))
            {
                unique_ptr<TcpSocket> temp_client = make_unique<TcpSocket>();
                if(listener.accept(*temp_client) == sf::Socket::Status::Done)
                {               
                    clients.push_back(move(temp_client));
                    cout << ((clients.back())->getRemoteAddress()).toString() << ":" << (clients.back())->getRemotePort() << ": " << "connected" << endl;
                    fs << ((clients.back())->getRemoteAddress()).toString() << ":" << (clients.back())->getRemotePort() << ": " << "connected" << endl;
                    selector.add(*(clients.back()));
                }
                else
                {
                    cerr << "Unable to accept client: " << (temp_client->getRemoteAddress()).toString() << ":" << temp_client->getRemotePort() << endl;
                    fs << "Unable to accept client: " << (temp_client->getRemoteAddress()).toString() << ":" << temp_client->getRemotePort() << endl;
                }
            }

            for(list<unique_ptr<TcpSocket>>::iterator it = clients.begin(); it != clients.end(); ++it)
            {
                if(selector.isReady(**it))
                {
                    status = (*it)->receive(received);
                    if(status == sf::Socket::Done)
                    {
                        received >> message;
                        fs << ((*it)->getRemoteAddress()).toString() << ":" << (*it)->getRemotePort() << ": " << message << endl;
                    }
                    else if(status == Socket::Disconnected)
                    {
                        fs << ((*it)->getRemoteAddress()).toString() << ":" << (*it)->getRemotePort() << ": " << "disconnected" << endl;
                        cout << ((*it)->getRemoteAddress()).toString() << ":" << (*it)->getRemotePort() << ": " << "disconnected" << endl;
                        selector.remove(**it);
                        it = clients.erase(it);
                    }
                }
            }
        }
    }
    
    // Exit no error
    return 0;
}

/******************************************************************************/
/******************************************************************************/


/***************************** F U N C T I O N S ******************************/

/*
 * Name: process_flags
 * Type: function
 * Return: void
 * Parmeters: const unsigned int&, const char* const* [], unsigned int&,
 *     unsigned int&, unsigned int&, bool&
 * Purpose: Process the flags from user input.
 */
void process_flags(const unsigned int& count, const char* const args[], 
    unsigned short& port)
{
    if(count != 2)
    {
        cerr << "The server program only takes one command line argument!" << endl;
        cerr << "Press enter to exit...";
        cin.get();
        exit(EXIT_FAILURE);
    }

    int temp;
    try
    {
        temp = stoi(args[1]);

        if((temp < 61000) || (temp > 65535))
        {
            cout << "Invalid command line argument detected: " << args[1] << endl;
            cout << "Please check your values and press enter to end the program!";
            cin.get();
            exit(EXIT_FAILURE);
        }
    }
    catch(invalid_argument &e)
    {
        cout << "Invalid command line argument detected: " << args[1] << endl;
        cout << "Please check your values and press enter to end the program!";
        cin.get();
        exit(EXIT_FAILURE);
    } 

    port = (unsigned short)temp;
}

/******************************************************************************/
/******************************************************************************/
