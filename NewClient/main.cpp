/* 
 * File:   main.cpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on January 18, 2012, 3:04 PM
 */

#include <iostream>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Command.hpp"
#include "Environnement.hpp"
/*
 * 
 */
int main(int argc, char** argv) {

    sf::RenderWindow app(sf::VideoMode(640, 480, 32), "IA");
    sf::Event event;
    sf::Clock clock;
    sf::TcpSocket socket;
    Command command;
    Environnement env;
    
    if (socket.Connect(sf::IpAddress("192.168.0.102"), 16000) == sf::Socket::Disconnected)
    {
        std::cout << "Connection fail." << std::endl;
        return EXIT_FAILURE;
    }
    socket.SetBlocking(false);
    while (app.IsOpen())
    {
        while (app.PollEvent(event))
        {
            if (event.Type == sf::Event::Closed ||
                (event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::Escape))
                app.Close();
        }
        sf::Packet packet;
        sf::Socket::Status res = socket.Receive(packet);
        if (res == sf::Socket::Done)
        {
                uint8_t cmd;
                packet >> cmd;
                command.execute(cmd, packet, env);
        }
        else if (res == sf::Socket::Disconnected)
        {
            std::cout << "Disconnect from server." << std::endl;
            app.Close();
        }
        app.Clear(sf::Color::Black);

        app.Display();
    }
    return 0;
}

