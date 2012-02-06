/* 
 * File:   Packet.cpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 * 
 * Created on January 17, 2012, 3:05 PM
 */

#include "Packet.hpp"

Packet::Packet()
{
	std::memset(&data_, 0, ARRAY_BUFF_SIZE);
	size_ = 0U;
	reading_pos_ = 0U;
}

Packet::Packet(const Packet& orig) {
    if (this != &orig)
    {
		this->size_ = orig.size_;
		this->reading_pos_ = orig.reading_pos_;
        this->data_ = orig.data_;
		//std::memcpy(&data_, &orig.data_, orig.GetSize());
    }
}

Packet::~Packet() {
}

void Packet::Append(const void* data, std::size_t size)
{
    if (data != NULL && size > 0 && size < ARRAY_BUFF_SIZE)
    {
        std::size_t dSize = GetSize();
        //data_.resize(dSize + size);
        std::memcpy(&data_[dSize], data, size);
		size_ += size;
    }
}

const char * Packet::GetData() const
{
    return ((data_.empty()) ? (NULL) : (&data_[0]));
}

void Packet::Clear()
{
	std::memset(&data_, 0, ARRAY_BUFF_SIZE);
    //data_.clear();
    reading_pos_ = 0U;
	size_ = 0U;
}

uint32_t Packet::Endianl(uint32_t value)
{
    uint32_t ret = (value << 24) | (value << 8 & 0x00FF0000) | (value >> 8 & 0x0000FF00) | (value >> 24);
    return (ret);
}

uint16_t Packet::Endians(uint16_t value)
{
    uint16_t ret = (value << 8 & 0xFF00) | (value >> 8 & 0x00FF);
    return (ret);
}

std::size_t Packet::GetSize() const
{
    return (size_);
}

Packet & Packet::operator <<(uint8_t value)
{
    Append(&value, sizeof(value));
    return (*this);
}

Packet & Packet::operator <<(uint16_t value)
{
    uint16_t v = Endians(value);
    Append(&v, sizeof(v));
    return (*this);
}

Packet & Packet::operator <<(uint32_t value)
{
    uint32_t v = Endianl(value);
    Append(&v, sizeof(value));
    return (*this);    
}

Packet & Packet::operator <<(int8_t value)
{
    Append(&value, sizeof(value));
    return (*this);
}

Packet & Packet::operator <<(int16_t value)
{
    int16_t v = Endians(value);
    Append(&v, sizeof(v));
    return (*this);
}

Packet & Packet::operator <<(int32_t value)
{
    int32_t v = Endianl(value);
    Append(&v, sizeof(v));
    return (*this);
}

Packet & Packet::operator <<(float value)
{
    Append(&value, sizeof(value));
    return (*this);
}

Packet & Packet::operator <<(double value)
{
    Append(&value, sizeof(value));
    return (*this);
}

Packet & Packet::operator <<(char * data)
{
    uint32_t length;
    for (length = 0; data[length] != '\0'; length++);
    *this << length;
    Append(data, length * sizeof(*data));
    return (*this);
}

Packet & Packet::operator <<(std::string& data)
{
    uint32_t size = static_cast<uint32_t>(data.size());
    *this << size;
    Append(data.c_str(), size * sizeof(*data.c_str()));
    return (*this);
}

Packet & Packet::operator >>(uint8_t & value)
{
    if (CheckSize(sizeof(value)))
    {
		value = *reinterpret_cast<const uint8_t*>(GetData() + reading_pos_);
		reading_pos_ += sizeof(value);
    }
    return (*this);
}

Packet & Packet::operator >>(uint16_t & value)
{
    if (CheckSize(sizeof(value)))
    {
        value = *reinterpret_cast<const uint16_t*>(GetData() + reading_pos_);
        reading_pos_ += sizeof(value);
        value = Endians(value);
    }
    return (*this);
}

Packet & Packet::operator >>(uint32_t & value)
{
    if (CheckSize(sizeof(value)))
    {
        value = *reinterpret_cast<const uint32_t*>(GetData() + reading_pos_);
        reading_pos_ += sizeof(value);
        value = Endianl(value);
    }
    return (*this);    
}

Packet & Packet::operator >>(int8_t & value)
{
    if (CheckSize(sizeof(value)))
    {
        value = *reinterpret_cast<const int8_t*>(GetData() + reading_pos_);
        reading_pos_ += sizeof(value);
    }
    return (*this);
}

Packet & Packet::operator >>(int16_t & value)
{
    if (CheckSize(sizeof(value)))
    {
        value = *reinterpret_cast<const int16_t*>(GetData() + reading_pos_);
        reading_pos_ += sizeof(value);
        value = Endians(value);
    }
    return (*this);
}

Packet & Packet::operator >>(int32_t & value)
{
    if (CheckSize(sizeof(value)))
    {
        value = *reinterpret_cast<const int32_t*>(GetData() + reading_pos_);
        reading_pos_ += sizeof(value);
        value = Endianl(value);
    }
    return (*this);
}

Packet & Packet::operator >>(float & value)
{
    if (CheckSize(sizeof(value)))
    {
        value = *reinterpret_cast<const float*>(GetData() + reading_pos_);
        reading_pos_ += sizeof(value);
    }
    return (*this);
}

Packet & Packet::operator >>(double & value)
{
    if (CheckSize(sizeof(value)))
    {
        value = *reinterpret_cast<const double*>(GetData() + reading_pos_);
        reading_pos_ += sizeof(value);
    }
    return (*this);
}

Packet & Packet::operator >>(char * data)
{
    uint32_t length = 0;
    *this >> length;
    if (length > 0 && CheckSize(length))
    {
        std::memcpy(data, GetData() + reading_pos_, length);
        data[length] = '\0';
        reading_pos_ += length;
    }
    return (*this);
}

Packet & Packet::operator >>(std::string& data)
{
    uint32_t length = 0;
    *this >> length;
    
    data.clear();
    if (length > 0 && CheckSize(length))
    {
        data.assign(GetData() + reading_pos_, length);
        reading_pos_ += length;
    }
    return (*this);
}

bool Packet::CheckSize(uint32_t size)
{
    return (reading_pos_ + size < GetSize());
}