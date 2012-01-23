/* 
 * File:   Packet.hpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on January 17, 2012, 3:05 PM
 */

#ifndef PACKET_HPP
#define	PACKET_HPP

#include <stdint.h>
#include <vector>
#include <string>

class Packet {
public:
    Packet();
    Packet(const Packet& orig);
    virtual ~Packet();
    
    void Append(const void * data, std::size_t size);
    const char * GetData() const;
    std::size_t GetSize() const;
    void Clear();
    Packet & operator<<(uint8_t  value);
    Packet & operator<<(uint16_t value);
    Packet & operator<<(uint32_t value);
    Packet & operator<<(uint64_t value);
    Packet & operator<<(int8_t value);
    Packet & operator<<(int16_t value);
    Packet & operator<<(int32_t value);
    Packet & operator<<(int64_t value);
    Packet & operator<<(float value);
    Packet & operator<<(double value);
    Packet & operator<<(char * data);
    Packet & operator<<(std::string & data);
    
    Packet & operator>>(uint8_t &  value);
    Packet & operator>>(uint16_t & value);
    Packet & operator>>(uint32_t & value);
    Packet & operator>>(uint64_t & value);
    Packet & operator>>(int8_t  & value);
    Packet & operator>>(int16_t & value);
    Packet & operator>>(int32_t & value);
    Packet & operator>>(int64_t & value);
    Packet & operator>>(float  & value);
    Packet & operator>>(double & value);
    Packet & operator>>(char * data);
    Packet & operator>>(std::string & data);

	uint32_t Endianl(uint32_t value);
    uint16_t Endians(uint16_t value);

private:
    std::vector<char> data_;
    uint32_t reading_pos_;
    

    bool     CheckSize(uint32_t size);
};

#endif	/* PACKET_HPP */

