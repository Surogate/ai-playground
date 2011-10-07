/* 
 * File:   Entropy.hpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on September 19, 2011, 12:30 PM
 */

#ifndef ENTROPY_HPP
#define	ENTROPY_HPP

#include <map>
#include <vector>
#include <cmath>
#include <iostream>

template<typename Attribute>
class Entropy {
    
public:
    typedef std::map<Attribute, int> AttributeMapCount;
    
    Entropy(): _log2(std::log(2)) {}
    virtual ~Entropy() {}

public:
    template<typename Iterator>
    float entropy(Iterator begin, Iterator end, float size)
    {
        Iterator it = begin;
        float result = 0;
        while (it != end) {
            float p = it->second / (float) size;
            result -= (p * (std::log(p) / _log2));
            ++it;
        }
        return result;
    }
    
    template<typename Iterator>
    AttributeMapCount countEachAttribute(Iterator begin, Iterator end)
    {
        Iterator it = begin;
        AttributeMapCount result;

        result.clear();
        while (it != end) {
            result[*it]++;
            ++it;
        }
        return result;
    }
    
private:
    float        _log2;

};
#endif	/* ENTROPY_HPP */

