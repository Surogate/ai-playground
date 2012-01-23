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

class Environnement {
public:
    Environnement();
    virtual ~Environnement();
private:
    
    Board board_;

    Board & GetBoard();
};

#endif	/* ENVIRONNEMENT_HPP */

