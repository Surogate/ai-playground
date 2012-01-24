/* 
 * File:   Board.cpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 * 
 * Created on January 18, 2012, 6:52 PM
 */

#include "Board.hpp"
#include "ResourceManager.hpp"

Board::Board(uint32_t size) {
    tile_.SetTexture(ResourceManager::getInstance().GetTexture("grass"), true);
    size_ = 0;
    if (size != 0) {
        size_ = size;
        square_.resize(size_);
        for (uint32_t i = 0; i < size_; i++)
            square_[i].resize(size_);
    }
}

Board::Board(Board const &board)
{
  if (this != &board)
  {
      tile_.SetTexture(ResourceManager::getInstance().GetTexture("grass"), true);
      size_ = board.size_;
      square_.resize(size_);
      for (uint32_t i = 0; i < size_; i++)
        square_[i].resize(size_);

      for (uint32_t x = 0; x < size_; x++)
      {
          for (uint32_t y = 0; y < size_; y++)
          {
                square_[x][y] = board.square_[x][y];
          }
      }
  }
}

Board & Board::operator=(Board const & board)
{
  if (this != &board)
  {
      tile_.SetTexture(ResourceManager::getInstance().GetTexture("grass"), true);
      size_ = board.size_;
      square_.resize(size_);
      for (uint32_t i = 0; i < size_; i++)
        square_[i].resize(size_);
      for (uint32_t x = 0; x < size_; x++)
      {
          for (uint32_t y = 0; y < size_; y++)
          {
              square_[x][y] = board.square_[x][y];
          }
      }
  }
  return (*this);
}

Board::~Board() {

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
            tile_.SetTextureRect(sf::IntRect(0, 0, 32, 32));
            tile_.SetPosition(x * 32, y * 32);
            render.Draw(tile_, sf::RenderStates::Default);
            if (GetSquare(x, y).hasHerbs) {
                tile_.SetTextureRect(sf::IntRect(32, 0, 32, 32));
                render.Draw(tile_, sf::RenderStates::Default);
            }
        }
    }
}


