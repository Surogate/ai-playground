/* 
 * File:   Data.hpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on October 3, 2011, 5:13 PM
 */

#ifndef DATA_HPP
#define	DATA_HPP

#include <iostream>
#include <stdint.h>
#include <vector>


enum Attributes
{
    OUTLOOK,
    TEMPERATURE,
    HUMIDITY,
    WIND,
    PLAYBALL
};

enum Wind
{
    Strong,
    Weak
};

enum Outlook
{
  Sunny,
  Rain,
  Overcast
};

enum Temperature
{
    Hot,
    Cool,
    Mild
};

enum Humidity
{
    High,
    Normal
};

enum PlayBall
{
    Yes,
    No
};

struct Data
{
    std::vector<Outlook> _outlook;
    std::vector<Temperature> _temperature;
    std::vector<Humidity> _humidity;
    std::vector<Wind> _wind;
    std::vector<PlayBall> _playball;

    Data() : _outlook(14), _temperature(14), _humidity(14), _wind(14), _playball(14)
    {
        _outlook[0] = Sunny;    _temperature[0] = Hot;  _humidity[0] = High; _wind[0] = Weak; _playball[0] = No;
        _outlook[1] = Sunny;    _temperature[1] = Hot;  _humidity[1] = High; _wind[1] = Strong; _playball[1] = No;
        _outlook[2] = Overcast; _temperature[2] = Hot;  _humidity[2] = High; _wind[2] = Weak; _playball[2] = Yes;
        _outlook[3] = Rain;     _temperature[3] = Mild; _humidity[3] = High; _wind[3] = Weak; _playball[3] = Yes;
        _outlook[4] = Rain;     _temperature[4] = Cool; _humidity[4] = Normal; _wind[4] = Weak; _playball[4] = Yes;
        _outlook[5] = Rain;     _temperature[5] = Cool; _humidity[5] = Normal; _wind[5] = Strong; _playball[5] = No;
        _outlook[6] = Overcast; _temperature[6] = Cool; _humidity[6] = Normal; _wind[6] = Strong; _playball[6] = Yes;
        _outlook[7] = Sunny;    _temperature[7] = Mild; _humidity[7] = High; _wind[7] = Weak; _playball[7] = No;
        _outlook[8] = Sunny;    _temperature[8] = Cool; _humidity[8] = Normal; _wind[8] = Weak; _playball[8] = Yes;
        _outlook[9] = Rain;     _temperature[9] = Mild; _humidity[9] = Normal; _wind[9] = Weak; _playball[9] = Yes;
        _outlook[10] = Sunny;   _temperature[10] = Mild; _humidity[10] = Normal; _wind[10] = Strong; _playball[10] = Yes;
        _outlook[11] = Overcast; _temperature[11] = Mild;_humidity[11] = High; _wind[11] = Strong; _playball[11] = Yes;
        _outlook[12] = Overcast; _temperature[12] = Hot; _humidity[12] = Normal; _wind[12] = Weak; _playball[12] = Yes;
        _outlook[13] = Rain;    _temperature[13] = Mild; _humidity[13] = High; _wind[13] = Strong; _playball[13] = No;
    }
    
    Data(Data const & orig)
    {
        if (this != &orig)
        {
            _outlook = orig._outlook;
            _temperature = orig._temperature;
            _humidity = orig._humidity;
            _wind = orig._wind;
            _playball = orig._playball;
        }
    }

    virtual ~Data()
    {

    }
    
    Data & operator=(Data const & orig)
    {
        if (this != &orig)
        {
            _outlook = orig._outlook;
            _temperature = orig._temperature;
            _humidity = orig._humidity;
            _wind = orig._wind;
            _playball = orig._playball;
        }
        return (*this);
    }
    
    bool empty()
    {
        return (_outlook.empty() && _temperature.empty() && _humidity.empty() && _wind.empty() && _playball.empty());
    }
    
    void clear()
    {
        _outlook.clear();
        _temperature.clear();
        _humidity.clear();
        _wind.clear();
        _playball.clear();
    }
    
    template<typename T>
    void cleanFromOutlook(Data & data, T value)
    {
        for (uint32_t i = 0; i < _outlook.size(); i++)
        {
            if (typeid(T) == typeid(Outlook) &&_outlook[i] == value)
            {
                data._outlook.push_back(_outlook[i]);
                data._temperature.push_back(_temperature[i]);
                data._humidity.push_back(_humidity[i]);
                data._wind.push_back(_wind[i]);
                data._playball.push_back(_playball[i]);
            }
        }
    }

    template<typename T>
    void cleanFromTemperature(Data & data, T value) 
    {
        for (uint32_t i = 0; i < _temperature.size(); i++) 
        {
            if (typeid(T) == typeid(Temperature) && _temperature[i] == value)
            {
                data._outlook.push_back(_outlook[i]);
                data._temperature.push_back(_temperature[i]);
                data._humidity.push_back(_humidity[i]);
                data._wind.push_back(_wind[i]);
                data._playball.push_back(_playball[i]);
            }
        }
    }

   template<typename T>
   void cleanFromHumidity(Data & data, T value) 
    {
        for (uint32_t i = 0; i < _humidity.size(); i++) 
        {
            if (typeid(T) == typeid(Humidity) && _humidity[i] == value)
            {
                data._outlook.push_back(_outlook[i]);
                data._temperature.push_back(_temperature[i]);
                data._humidity.push_back(_humidity[i]);
                data._wind.push_back(_wind[i]);
                data._playball.push_back(_playball[i]);
            }
        }
    }

    template<typename T>
    void cleanFromWind(Data & data, T value)
    {
        for (uint32_t i = 0; i < _wind.size(); i++)
        {
            if (typeid(T) == typeid(Wind) && _wind[i] == value)
            {
                data._outlook.push_back(_outlook[i]);
                data._temperature.push_back(_temperature[i]);
                data._humidity.push_back(_humidity[i]);
                data._wind.push_back(_wind[i]);
                data._playball.push_back(_playball[i]);
            }
        }

    }

    template <typename T>
    void cleanFromPlayBall(Data & data, T value)
    {
        for (uint32_t i = 0; i < _playball.size(); i++)
        {
            if (typeid(T)== typeid(PlayBall) && _playball[i] == value) {
                data._outlook.push_back(_outlook[i]);
                data._temperature.push_back(_temperature[i]);
                data._humidity.push_back(_humidity[i]);
                data._wind.push_back(_wind[i]);
                data._playball.push_back(_playball[i]);
            }
        }

    }
    
    template<Attributes A, typename T, T value>
    Data * clean()
    {
        Data * data = new Data();
        data->clear();
        switch(A)
        {
            case OUTLOOK:
                cleanFromOutlook<T>(*data, value);
                break;
            case TEMPERATURE:
                cleanFromTemperature<T>(*data, value);
                break;
            case HUMIDITY:
                cleanFromHumidity<T>(*data, value);
                break;
            case WIND:
                cleanFromWind<T>(*data, value);
                break;
            default:
                cleanFromPlayBall<T>(*data, value);
        }
        return (data);
    }
    
    bool checkOnlyPlayball(PlayBall value)
    {
        for (uint32_t i = 0; i < _playball.size(); i++)
        {
            if (_playball[i] == value) continue;
            else
                return false;
        }
        return true;
    }
    
};


#endif	/* DATA_HPP */

