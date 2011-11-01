
#include <fstream>

#include "Logger.hpp"

namespace Logique {

	Logger::Logger() : _file() 
	{}

	Logger::Logger(const std::string& file) : _file(file) 
	{}

	void Logger::dump(const std::string& value) {
		std::ofstream descriptor(_file, std::ios::app | std::ios::out);

		if (descriptor.good()) {
			descriptor << value << std::endl;
		}
	}

}