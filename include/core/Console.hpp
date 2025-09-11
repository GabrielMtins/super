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

/**
 * Console utilizado para fazer logs. Prefira utilizá-lo no lugar do
 * printf(), pois por ele é mais fácil mudar a stream, como redirecionar
 * para um arquivo, por exemplo.
 */
extern Console console;

#endif
