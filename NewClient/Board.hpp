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
#include <vector>
#include "Square.hpp"

class Board {
public:
    typedef std::vector< std::vector< Square > > BoardSquare;
    Board(uint32_t size = 0);
    Board(Board const &board);
    virtual ~Board();
    Board & operator=(Board const & board);

    uint32_t GetSize();
    Square & GetSquare(int x, int y);
    void Draw(sf::RenderTarget &);

private:
    BoardSquare square_;
    uint32_t    size_;
    sf::Sprite  tile_;
};

#endif	/* BOARD_HPP */

