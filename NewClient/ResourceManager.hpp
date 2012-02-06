/* 
 * File:   ResourceManager.hpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on January 24, 2012, 11:15 AM
 */

#ifndef RESOURCEMANAGER_HPP
#define	RESOURCEMANAGER_HPP

#include "Singleton.hpp"

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ResourceManager : public Singleton<ResourceManager> {

public:
    SINGLETON_DEFINED(ResourceManager);

    void                AddTexture(std::string const &, std::string const &);
    sf::Texture &       GetTexture(std::string const &);
    
private:
    ResourceManager();
    virtual ~ResourceManager();

    std::map<std::string, sf::Texture> textures_;
};

#endif	/* RESOURCEMANAGER_HPP */

