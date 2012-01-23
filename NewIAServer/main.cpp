/* 
 * File:   main.cpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on January 17, 2012, 11:47 AM
 */

#include <iostream>
#include <string>
#include "Server.hpp"

/*
 * 
 */
int main(int argc, char** argv) {
    Server::getInstance().Start();    
    return 0;
}