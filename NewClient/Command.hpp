/* 
 * File:   Command.hpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on January 18, 2012, 4:41 PM
 */

#ifndef COMMAND_HPP
#define	COMMAND_HPP

#include <SFML/Network.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <vector>
#include <stdint.h>
#include "Environnement.hpp"


class Command {
public:
    Command();
    virtual ~Command();
    
    void execute(uint8_t, sf::Packet &, Environnement &);
private:
    std::vector<boost::function<void (sf::Packet &, Environnement &)> > cmd_;
    
    void synchronize(sf::Packet&, Environnement &);
    void spawn(sf::Packet&, Environnement &);
    void move(sf::Packet&, Environnement &);
    void eat(sf::Packet&, Environnement &);
    void die(sf::Packet&, Environnement &);
    void reproduce(sf::Packet&, Environnement &);
    void grassUp(sf::Packet&, Environnement &);
    void grassDown(sf::Packet&, Environnement &);
};

#endif	/* COMMAND_HPP */

