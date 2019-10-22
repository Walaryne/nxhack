#ifndef COMMANDREGISTRY_HPP_
#define COMMANDREGISTRY_HPP_

#include <functional>
#include <string>
#include <map>


namespace xeno {

class CommandRegistry {
	public:
	void addCommand(std::string string, std::function<void(std::vector<std::string>*)> function);
	void removeCommand(std::string string);
	std::function<void(std::vector<std::string>*)> getFunction(std::string string);

	private:
	std::map<std::string, std::function<void(std::vector<std::string>*)>> _commandFunctionMap;
};

}

#endif //COMMANDREGISTRY_HPP_