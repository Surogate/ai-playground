/* 
 * File:   Entity.hpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on January 18, 2012, 6:55 PM
 */

#ifndef ENTITY_HPP
#define	ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <string>

class Entity {
public:
    enum Type {
        SHEEP,
        WOLF
    };

    Entity();
    Entity(Type const &, sf::Vector2f const &);
    Entity(Entity const &);
    
    virtual ~Entity();
    Entity & operator=(Entity const &);
    
    void Draw(sf::RenderTarget &);
    
    sf::Vector2f const & GetPosition() const;
    void SetPosition(sf::Vector2f &);
    
private:
    Type                type_;
    sf::Sprite          sprite_;
    sf::Vector2f        position_;
};

#endif	/* ENTITY_HPP */

