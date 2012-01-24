/* 
 * File:   ResourceManager.cpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 * 
 * Created on January 24, 2012, 11:15 AM
 */

#include "ResourceManager.hpp"

ResourceManager::ResourceManager() {
}

ResourceManager::~ResourceManager() {
}

void ResourceManager::AddTexture(std::string const &key, std::string const & file) {
    sf::Texture texture;
    if (!texture.LoadFromFile(file))
        exit(EXIT_FAILURE);
    textures_[key] = texture;
}

sf::Texture & ResourceManager::GetTexture(std::string const & key) {
    return (textures_.at(key));
}