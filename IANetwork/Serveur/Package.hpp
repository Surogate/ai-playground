#ifndef _PACKAGE_HPP_
#define _PACKAGE_HPP_

#include <boost/array.hpp>
#include <stdint.h>

namespace Networking
{
	class Package
	{
	public:
		typedef boost::array<char, 64> Buffer;
	private:
		Buffer	buffer_;

	public:
		Package(void);
		Package(Package const & orig);
		~Package(void);

		Package & operator=(Package const & orig);

		Buffer & get_buffer();
		uint32_t size();
		void init_from_string(std::string & data);
	};
}

#endif