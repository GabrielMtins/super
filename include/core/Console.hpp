#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <string>

class Console {
	public:
		Console(void);

		void log(const std::string& msg);
		void error(const std::string& msg);
		void exit(int status);
};

extern Console console;

#endif
