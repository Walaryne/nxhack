#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <proc/readproc.h>
#include <sys/uio.h>
#include "ProcessMemoryEditor.hpp"
#include "../StringExplosion/src/stringexplosion.hpp"

enum commands {
	ERROR = -1,
	SAVELOC,
	TELEPORT,
	SLOT,
	GETSLOT,
	EXIT
};


const std::map<std::string, commands> commandmap {
	{"saveloc", SAVELOC},
	{"teleport", TELEPORT},
	{"slot", SLOT},
	{"getslot", GETSLOT},
	{"exit", EXIT}
};

commands readCommandMap(const std::map<std::string, commands> *cm, std::string k) {
	try {
		return cm->at(k);
	} catch(std::out_of_range &e) {
		return ERROR;
	}
}

unsigned long getTargetBaseAddress(pid_t pid) noexcept {
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
	se::exploder exp(' ', true);

	int tempX;
	int tempY;
	std::pair<int, int> tempPair;
	unsigned long currentTeleportSlot = 0;

	std::map<unsigned long, std::pair<int, int>> teleportMap;

	std::string input {};
	std::vector<std::string> commandvec;

	auto addr = pme.readProcessMemory<unsigned long>(0x374030, true);

	while(true) {
		std::cout << ">: ";
		std::getline(std::cin, input);
		exp.load(input);
		try {
			commandvec = exp.explode();
		} catch(se::exploderex &e) {
			continue;
		}
		switch(readCommandMap(&commandmap, commandvec.front())) {
		case(commands::SAVELOC):
				tempX = pme.readProcessMemory<int>(addr + 0x14, false);
				tempY = pme.readProcessMemory<int>(addr + 0x18, false);
				tempPair = std::make_pair(tempX, tempY);
				teleportMap[currentTeleportSlot] = tempPair;
				std::cout << "Saved location X: " << tempX << ", Y: " << tempY << " to slot " << currentTeleportSlot << "\n";
				break;

		case(commands::TELEPORT):
				tempX = pme.readProcessMemory<int>(addr + 0x14, false);
				tempY = pme.readProcessMemory<int>(addr + 0x18, false);
				std::cout << "Currently at X: " << tempX << ", Y: " << tempY << "\n";
				tempX = teleportMap[currentTeleportSlot].first;
				tempY = teleportMap[currentTeleportSlot].second;
				pme.writeProcessMemory<int>(addr + 0x14, &tempX, false);
				pme.writeProcessMemory<int>(addr + 0x18, &tempY, false);
				std::cout << "Teleported to X: " << tempX << ", Y: " << tempY << "\n";
				break;

		case(commands::SLOT):
				currentTeleportSlot = std::stoi(commandvec.at(1));
				break;

		case(commands::GETSLOT):
				std::cout << "Current teleport slot: " << currentTeleportSlot << "\n";
				break;

		case(commands::EXIT):
				std::exit(0);
		default:
				std::cout << "Unknown Command" << "\n";
				break;
		}
	}

	return 0;
}
