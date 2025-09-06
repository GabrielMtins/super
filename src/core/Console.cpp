#include "core/Console.hpp"
#include <cstdio>

Console console;

Console::Console(void) {

}

void Console::log(const std::string& msg) {
	printf("%s\n", msg.c_str());
}

void Console::error(const std::string& msg) {
	fprintf(stderr, "%s\n", msg.c_str());
}

void Console::exit(int status) {
	std::exit(status);
}
