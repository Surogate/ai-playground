/* 
 * File:   Gain.hpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on September 19, 2011, 6:19 PM
 */

#ifndef GAIN_HPP
#define	GAIN_HPP

#include <iostream>
#include <vector>
#include <map>
#include "Entropy.hpp"


template <typename Attribute, typename Result>
class Gain
{
    public:
        typedef std::map<Attribute, int> AttributeMap;
        typedef std::map<Result, int> ResultMap;
        typedef std::map<Attribute, ResultMap> AttributePerResultNumber;
        
    public:
        Gain(std::vector<Attribute> & attr, std::vector<Result> & res) :
                _attributePerNumber(), _resultPerNumber(), _attrPerRes(), _attribEntropy(), _resultEntropy()
        {
            _attributePerNumber = _attribEntropy.countEachAttribute(attr.begin(), attr.end());
            _resultPerNumber = _resultEntropy.countEachAttribute(res.begin(), res.end());
            _globalEntropy = _resultEntropy.entropy(_resultPerNumber.begin(), _resultPerNumber.end(), res.size());
            
            for (int i = 0; i < attr.size() && i < res.size(); i++)
            {
                _attrPerRes[attr[i]][res[i]]++;
            }
            
            typename AttributePerResultNumber::const_iterator it = _attrPerRes.begin();
            typename AttributePerResultNumber::const_iterator ite = _attrPerRes.end();
            _gain = _globalEntropy;
            while (it != ite)
            {
                _gain -= (_attributePerNumber[it->first] / (float)res.size()) * _resultEntropy.entropy(it->second.begin(), it->second.end(), _attributePerNumber[it->first]);
                ++it;
            }
        }
        
        virtual ~Gain() {}
        
        float   getGlobalEntropy()
        {
            return _globalEntropy;
        }
        
        float   result()
        {
            return _gain;
        }
        
    private:
        AttributeMap _attributePerNumber;
        ResultMap    _resultPerNumber;
        AttributePerResultNumber        _attrPerRes;
        
        
        Entropy<Attribute>      _attribEntropy;
        Entropy<Result>         _resultEntropy;
        float                   _globalEntropy;
        float                   _gain;
};

#endif	/* GAIN_HPP */

