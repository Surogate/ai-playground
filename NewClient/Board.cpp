/* 
 * File:   Board.cpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 * 
 * Created on January 18, 2012, 6:52 PM
 */

#include "Board.hpp"

Board::Board(uint32_t size) {
    if (size != 0) {
        size_ = size;
        *square_ = new Square[size_];
        for (uint32_t i = 0; i < size_; i++)
            square_[i] = new Square[size_];
    }
}

Board::~Board() {
    for (uint32_t i = 0; i < size_; i++)
        delete[] square_[i];
    delete[] square_;
}

uint32_t Board::GetSize()
{
    return size_;
}

Square & Board::GetSquare(int x, int y) {
    return (this->square_[x][y]);
}

void Board::Draw(sf::RenderTarget &render) {
    if (size_ == 0) return ;
    for (uint32_t x = 0; x < size_; x++) {
        for (uint32_t y = 0; y < size_; y++) {
            if (GetSquare(x, y).hasHerbs) {
                sf::RectangleShape shape;
                shape = sf::RectangleShape(sf::Vector2f(5.0f, 5.0f));
                shape.SetPosition(sf::Vector2f(5.0f * x, 5.0f * y));
                shape.SetFillColor(sf::Color::Green);
                render.Draw(shape, sf::RenderStates::Default);
            }
        }
    }
}


