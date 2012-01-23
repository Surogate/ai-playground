
#include <fstream>

#include <boost/chrono.hpp>

#include "Logger.hpp"

namespace Logique {

	boost::chrono::system_clock::time_point Logger::start;

	Logger::Logger() : _file() 
	{}

	Logger::Logger(const std::string& file) : _file(file) 
	{}

	void Logger::wipeFile() {
		std::ofstream descriptor(_file.c_str(), std::ios::trunc);

		start = boost::chrono::system_clock::now();
	}

	void Logger::dump(const std::string& value) {
		std::ofstream descriptor(_file.c_str(), std::ios::app | std::ios::out);

		if (descriptor.good()) {
			boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
			descriptor << sec.count() << ";" << value << std::endl;
		}
	}

	
}