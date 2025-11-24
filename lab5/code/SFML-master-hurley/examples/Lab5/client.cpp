// Author: Austin Logan Barber
// File Name: client.cpp
// Date Created: 2025 November 06
// Date Last Updated: 2025 November 06
// Purpose: This file provides the client code for ECE 6122 >> Lab5.


//******************************* I N C L U D E ********************************/
#include <iostream>
#include <cstdlib>
#include <SFML/Network.hpp>


/******************************************************************************/
/******************************************************************************/


/********************************* U S I N G **********************************/
using namespace sf;
using namespace std;
/******************************************************************************/
/******************************************************************************/


/**************************** P R O T O T Y P E S *****************************/
void process_flags(const unsigned int&, const char* const [], IpAddress&, 
    unsigned short&);

/******************************************************************************/
/******************************************************************************/


/********************************** M A I N ***********************************/
int main(int argc, char* argv[])
{
    // Network variables
    unsigned short port_num = 61000;
    IpAddress server = IpAddress::LocalHost;
    TcpSocket socket;
    string message;
    Packet packet_message;
    
    // Process input flags
    process_flags(argc, argv, server, port_num);

    // Connect to the server
    if(socket.connect(server, port_num) != sf::Socket::Done)
    {
        cout << "Failed to connect to the server at " << server.toString() << " on " <<
            port_num << ". Please check your values and press enter to end the program...";
        cin.get();
        exit(EXIT_FAILURE);
    } 
    std::cout << "Connected to server " << server.toString() << ":" << port_num << std::endl;

    // Application
    while(true)
    {
        packet_message.clear();
        cout << "Please enter a message: ";
        getline(cin, message);
        packet_message << message;
        socket.send(packet_message);
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
    IpAddress& addr, unsigned short& port)
 {
    if(count != 3)
    {
        cerr << "The client program only takes two command line arguments!" << endl;
        cerr << "Press enter to exit...";
        cin.get();
        exit(EXIT_SUCCESS);
    }

    addr = args[1];
    if(addr == sf::IpAddress::None)
    {
        cerr << "Invalid command line argument detected: " << args[1] << endl;
        cerr << "Please check your values and press enter to end the program!";
        cin.get();
        exit(EXIT_FAILURE);
    }

    int temp;
    try
    {
        temp = stoi(args[2]);

        if((temp < 61000) || (temp > 65535))
        {
            cout << "Invalid command line argument detected: " << args[2] << endl;
            cout << "Please check your values and press enter to end the program!";
            cin.get();
            exit(EXIT_FAILURE);
        }
    }
    catch(invalid_argument &e)
    {
        cout << "Invalid command line argument detected: " << args[2] << endl;
        cout << "Please check your values and press enter to end the program!";
        cin.get();
        exit(EXIT_FAILURE);
    } 

    port = (unsigned short)temp;
 }


/******************************************************************************/
/******************************************************************************/
