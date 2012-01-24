/* 
 * File:   Environnement.cpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 * 
 * Created on January 18, 2012, 7:08 PM
 */

#include "Environnement.hpp"

Environnement::Environnement() {
}

Environnement::~Environnement() {
}

void Environnement::SetBoard(Board const & board)
{
    board_ = board;
}

Board & Environnement::GetBoard()
{
    return (board_);
}

void Environnement::AddEntity(uint32_t const &key, Entity const & entity)
{
    entities_[key] = entity;
}

Entity & Environnement::GetEntity(uint32_t const &key)
{
    return (entities_[key]);
}

void Environnement::RmEntity(uint32_t const &key)
{
    if (entities_.find(key) != entities_.end())
        entities_.erase(key);
}

void Environnement::Draw(sf::RenderTarget &render)
{
    EntityMap::iterator it = entities_.begin();
    EntityMap::iterator ite = entities_.end();
    
    board_.Draw(render);
    for (; it != ite; ++it)
    {
        it->second.Draw(render);
    }
}

