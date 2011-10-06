#include "Package.hpp"
#include <iostream>

Package::Package(void) : buffer_()
{
	buffer_.assign(0);
}

Package::Package(Package const & orig)
{
	if (this != &orig)
	{
		buffer_ = orig.buffer_;
	}
}

Package::~Package(void)
{
}

Package & Package::operator=(Package const & orig)
{
	if (this != &orig)
	{
		buffer_ = orig.buffer_;
	}
	return (*this);
}

Package::Buffer & Package::get_buffer()
{
	return (buffer_);
}

uint32_t Package::size()
{
	return (buffer_.size());
}

void Package::init_from_string(std::string & data)
{
	for (uint32_t i = 0; i < data.size(); i++)
	{
		buffer_[i] = data.at(i);
	}
}