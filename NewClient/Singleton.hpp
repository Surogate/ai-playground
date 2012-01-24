/* 
 * File:   Singleton.hpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on January 24, 2012, 11:12 AM
 */

#ifndef SINGLETON_HPP
#define	SINGLETON_HPP

#define SINGLETON_DEFINED(name) friend class Singleton< name >

template <typename T>
class Singleton {
protected:

    inline Singleton() {
    }

    virtual ~Singleton() {
    }

public:

    static T & getInstance() {
        static T instance;

        return instance;
    }
};

#endif	/* SINGLETON_HPP */

