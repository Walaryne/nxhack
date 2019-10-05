#include <iostream>
#include <string>
#include <functional>
#include <filesystem>
#include <map>
#include <dlfcn.h>
#include "ProcessMemoryEditor.hpp"
#include "symbols.hpp"
#include "../StringExplosion/src/stringexplosion.hpp"


namespace fs = std::filesystem;

int main(int argc, char **argv) {
	pid_t pid = 0;

	if(argc < 2) {
		std::cerr << "PID argument required!" << std::endl;
		std::exit(1);
	} else {
		pid = std::stoi(std::string(argv[1]));
	}

	std::map<std::string, std::function<void(xeno::ProcessMemoryEditor*, std::vector<std::string>*)>> commandFunctionMap;
	std::map<std::string, void*> handleMap;

	std::string input {};
	std::vector<std::string> commandvec;

	xeno::ProcessMemoryEditor pme(pid);
	se::exploder exp(' ', true);

	void* tempHandle;
	void* tempFunction;

	if(!fs::exists("plugins") || fs::is_empty("plugins")) {
		std::cerr << "Plugins folder doesn't exist or is empty, exiting." << std::endl;
		std::exit(1);
	}

	//Bootstrap all of the commands from the plugins using known symbols.
	//Skips the C style ugliness and allows for the prettier C++ functional style to take hold.
	for(auto& p : fs::directory_iterator("plugins")) {
		tempHandle = dlopen(p.path().c_str(), RTLD_LAZY);
		handleMap[p.path().filename()] = tempHandle;
		
		registerCommand rc = (registerCommand) dlsym(tempHandle, REGISTERCOMMAND);
		autoexec ax = (autoexec) dlsym(tempHandle, AUTOEXEC);

		rc(&commandFunctionMap);
		ax(&pme);
	}

	while(true) {
		std::cout << ">: ";
		std::getline(std::cin, input);
		exp.load(input);
		try {
			commandvec = exp.explode();
		} catch(se::exploderex &e) {
			continue;
		}

		if(commandvec.front() == "exit") {
			std::exit(0);
		}

		try {
			std::function func = commandFunctionMap.at(commandvec.front());
			func(&pme, &commandvec);
		} catch(std::out_of_range &e) {
			std::cout << "Unknown Command" << "\n";
			continue;
		}
	}
	
	return 0;
}
