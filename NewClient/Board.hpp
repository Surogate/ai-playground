/* 
 * File:   Board.hpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on January 18, 2012, 6:52 PM
 */

#ifndef BOARD_HPP
#define	BOARD_HPP

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include "Square.hpp"

class Board {
public:
    Board(uint32_t size = 0);
    virtual ~Board();

    uint32_t GetSize();
    Square & GetSquare(int x, int y);
    void Draw(sf::RenderTarget &);

private:
    Square **   square_;
    uint32_t    size_;
};

#endif	/* BOARD_HPP */

