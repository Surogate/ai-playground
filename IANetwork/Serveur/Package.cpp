#include "Package.hpp"
#include <iostream>

namespace Networking
{
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

	void Package::init(std::string & data)
	{
		uint32_t i = 0;
		for (; i < size() && i < data.size(); i++)
		{
			buffer_[i] = data.at(i);
		}
		if (i < size())
		{
			for (; i < size(); i++)
				buffer_[i] = 0;
		}
	}
}