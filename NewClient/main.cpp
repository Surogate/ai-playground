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
#include "ResourceManager.hpp"

/*
 * 
 */
int main(int argc, char** argv) {

    if (argc < 2)
        return EXIT_FAILURE;
    sf::RenderWindow app(sf::VideoMode(1024, 780, 32), "IA");
    ResourceManager::getInstance().AddTexture("grass", "resources/grass.png");
    ResourceManager::getInstance().AddTexture("animals", "resources/animals.png");
    sf::Event event;
    sf::Clock clock;
    sf::View def;
    sf::View cam;
    sf::TcpSocket socket;
    Command command;
    Environnement env;
    

    def = app.GetDefaultView();
    cam = app.GetView();
    if (socket.Connect(sf::IpAddress(argv[1]), 16000) == sf::Socket::Disconnected)
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

			float timer = clock.GetElapsedTime().AsSeconds() / 1000.f;
            if (event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::Up)
                cam.SetCenter(cam.GetCenter().x, cam.GetCenter().y - 2 * timer);
            if (event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::Down)
                cam.SetCenter(cam.GetCenter().x, cam.GetCenter().y + 2 * timer);
            if (event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::Left)
                cam.SetCenter(cam.GetCenter().x - 2 * timer, cam.GetCenter().y);
            if (event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::Right)
                cam.SetCenter(cam.GetCenter().x + 2 * timer, cam.GetCenter().y);
 
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
        app.SetView(cam);
        env.Draw(app);
        app.SetView(def);
        app.Display();
    }
	std::cin.get();
    return 0;
}

