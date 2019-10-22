#include <functional>
#include <string>
#include <map>
#include "CommandRegistry.hpp"

namespace xeno {

void CommandRegistry::addCommand(std::string string, std::function<void(std::vector<std::string>*)> function) {
	_commandFunctionMap[string] = function;
}

void CommandRegistry::removeCommand(std::string string) {
	_commandFunctionMap.erase(string);
}

std::function<void(std::vector<std::string>*)> CommandRegistry::getFunction(std::string string) {
	return _commandFunctionMap.at(string);
}

}