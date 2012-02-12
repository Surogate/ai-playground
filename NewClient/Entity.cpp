/* 
 * File:   Entity.cpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 * 
 * Created on January 18, 2012, 6:55 PM
 */

#include "Entity.hpp"
#include "ResourceManager.hpp"

Entity::Entity() {
}

Entity::Entity(Type const & type, sf::Vector2f const & pos) {
    position_ = pos;
    position_.x *= 32;
    position_.y *= 32;
    type_ = type;
    sprite_.SetTexture(ResourceManager::getInstance().GetTexture("animals"), true);
    if (type_ == SHEEP) {
        sprite_.SetTextureRect(sf::IntRect(320, 0, 32, 32));
    } else {
        sprite_.SetTextureRect(sf::IntRect(224, 128, 32, 32));
    }
}

Entity::Entity(Entity const & entity) {
    if (this != &entity) {
        position_ = entity.position_;
        sprite_ = entity.sprite_;
        type_ = entity.type_;
        if (type_ == SHEEP) {
            sprite_.SetTextureRect(sf::IntRect(320, 0, 32, 32));
        } else {
            sprite_.SetTextureRect(sf::IntRect(224, 128, 32, 32));
        }
    }
}

Entity & Entity::operator=(Entity const & entity) {
    if (this != &entity) {
        position_ = entity.position_;
        sprite_ = entity.sprite_;
        type_ = entity.type_;
        if (type_ == SHEEP) {
            sprite_.SetTextureRect(sf::IntRect(320, 0, 32, 32));
        } else {
            sprite_.SetTextureRect(sf::IntRect(224, 128, 32, 32));
        }
    }
    return (*this);
}

Entity::~Entity() {
}

void Entity::Draw(sf::RenderTarget& render) {
    sprite_.SetPosition(position_);
    render.Draw(sprite_, sf::RenderStates::Default);
}

sf::Vector2f const & Entity::GetPosition() const {
    return (position_);
}

void Entity::SetPosition(sf::Vector2f &pos) {
    position_ = pos;
    position_.x *= 32;
    position_.y *= 32;
}
