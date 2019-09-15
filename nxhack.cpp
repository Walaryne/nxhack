#include <iostream>
#include <string>
#include <map>
#include <proc/readproc.h>
#include <sys/uio.h>
#include "ProcessMemoryEditor.hpp"

enum commands {
	ERROR = -1,
	SAVELOC,
	TELEPORT,
	EXIT
};

const std::map<std::string, commands> commandmap {
	{"saveloc", SAVELOC},
	{"teleport", TELEPORT},
	{"exit", EXIT}
};

commands readCommandMap(const std::map<std::string, commands> *cm, std::string k) {
	try {
		return cm->at(k);
	} catch(std::out_of_range &e) {
		return ERROR;
	}
}

unsigned long getTargetBaseAddress(pid_t pid) {
	pid_t pidlist[2] {pid, 0};
	proc_t** prt = readproctab(PROC_FILLSTAT | PROC_PID, pidlist);
	return prt[0]->start_code;
}

int main(int argc, char **argv) {
	pid_t pid = 0;

	if(argc < 2) {
		std::cerr << "PID argument required!" << std::endl;
		std::exit(1);
	} else {
		pid = std::stoi(std::string(argv[1]));
	}

	xeno::ProcessMemoryEditor pme(pid);

	auto addr = pme.readProcessMemory<unsigned long>(0x374030, true);

	int savedPlayerX;
	int savedPlayerY;
	int currentPlayerX;
	int currentPlayerY;

	while(true) {
		std::cout << ">: ";
		std::string input {};
		std::cin >> input;
		switch(readCommandMap(&commandmap, input)) {
		case(commands::SAVELOC):
				savedPlayerX = pme.readProcessMemory<int>(addr + 0x14, false);
				savedPlayerY = pme.readProcessMemory<int>(addr + 0x18, false);
				std::cout << "Saved location X: " << savedPlayerX << ", Y: " << savedPlayerY << std::endl;
				break;

		case(commands::TELEPORT):
				currentPlayerX = pme.readProcessMemory<int>(addr + 0x14, false);
				currentPlayerY = pme.readProcessMemory<int>(addr + 0x18, false);
				std::cout << "Currently at X: " << currentPlayerX << ", Y: " << currentPlayerY << std::endl;
				pme.writeProcessMemory<int>(addr + 0x14, &savedPlayerX, false);
				pme.writeProcessMemory<int>(addr + 0x18, &savedPlayerY, false);
				std::cout << "Teleported to X: " << savedPlayerX << ", Y: " << savedPlayerY << std::endl;
				break;
		case(commands::EXIT):
				std::exit(0);
		default:
				std::cout << "Unknown Command" << std::endl;
				break;
		}
	}

	return 0;
}
