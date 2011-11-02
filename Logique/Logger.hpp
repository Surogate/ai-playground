
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <sstream>

namespace Logique {

	class Logger {
	public:
		Logger();
		Logger(const std::string& file);

		void wipeFile();

		void dump(const std::string& value);

		template <typename T>
		void dump(const T& value) {
			std::stringstream convertor;
			convertor << value;
			dump(convertor.str());
		}

	private:
		std::string _file;
	};

}

#endif /* !LOGGER_HPP */

