/* 
 * File:   Command.cpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 * 
 * Created on January 18, 2012, 4:41 PM
 */

#include "Command.hpp"
#include <iostream>
#include <stdint.h>

Command::Command() {
    cmd_.push_back(boost::bind(&Command::synchronize, this, _1, _2));
    cmd_.push_back(boost::bind(&Command::spawn, this, _1, _2));
    cmd_.push_back(boost::bind(&Command::move, this, _1, _2));
    cmd_.push_back(boost::bind(&Command::eat, this, _1, _2));
    cmd_.push_back(boost::bind(&Command::die, this, _1, _2));
    cmd_.push_back(boost::bind(&Command::reproduce, this, _1, _2));
    cmd_.push_back(boost::bind(&Command::grassUp, this, _1, _2));
    cmd_.push_back(boost::bind(&Command::grassDown, this, _1, _2));
}

Command::~Command() {
}

void Command::execute(uint8_t cmd, sf::Packet & packet, Environnement &env) {
    cmd_[cmd](packet, env);
}

void Command::synchronize(sf::Packet& packet, Environnement &env)
{
    std::cout << "SYNCHRONIZE" << std::endl;
    uint32_t board_size;
    packet >> board_size;
    env.SetBoard(Board(board_size));
}

void Command::spawn(sf::Packet& packet, Environnement &env) {
    std::cout << "SPAWN" << std::endl;
    uint32_t id;
    uint32_t type;
    uint32_t x;
    uint32_t y;
    
    packet >> id >> type >> x >> y;
    env.AddEntity(id, Entity(static_cast<Entity::Type>(type), sf::Vector2f(x, y)));
}

void Command::move(sf::Packet& packet, Environnement &env) {
    std::cout << "MOVE" << std::endl;    
    uint32_t id;
    uint32_t x;
    uint32_t y;

    packet >> id >> x >> y;
    sf::Vector2f v(x,y);
    env.GetEntity(id).SetPosition(v);
}

void Command::eat(sf::Packet& packet, Environnement &env) {
    std::cout << "EAT" << std::endl;
}

void Command::die(sf::Packet& packet, Environnement &env) {
    std::cout << "DIE" << std::endl;
    uint32_t id;
    
    packet >> id;
    env.RmEntity(id);
}

void Command::reproduce(sf::Packet& packet, Environnement &env) {
    std::cout << "REPRODUCE" << std::endl;
}

void Command::grassUp(sf::Packet& packet, Environnement &env) {
    std::cout << "Grass up" << std::endl;
    uint32_t x;
    uint32_t y;
    
    packet >> x >> y;
    env.GetBoard().GetSquare(x,y).hasHerbs = true;
}

void Command::grassDown(sf::Packet&  packet, Environnement &env)
{
    std::cout << "Grass down" << std::endl;
    uint32_t x;
    uint32_t y;
    
    packet >> x >> y;
    env.GetBoard().GetSquare(x,y).hasHerbs = false;

}