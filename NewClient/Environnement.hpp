/* 
 * File:   Environnement.hpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on January 18, 2012, 7:08 PM
 */

#ifndef ENVIRONNEMENT_HPP
#define	ENVIRONNEMENT_HPP

#include "Board.hpp"
#include "Entity.hpp"

#include <map>

class Environnement {
public:
    typedef std::map<uint32_t, Entity> EntityMap;
    
    Environnement();
    virtual ~Environnement();

    void SetBoard(Board const &);
    Board & GetBoard();
    void AddEntity(uint32_t const &, Entity const &);
    Entity & GetEntity(uint32_t const &);
    void RmEntity(uint32_t const &);

    void Draw(sf::RenderTarget &);
    
private:
    
    Board board_;
    EntityMap entities_;
};

#endif	/* ENVIRONNEMENT_HPP */

