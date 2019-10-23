#include <map>
#include <functional>
#include <vector>
#include <string>
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/keysym.h>
//#include <X11/Xresource.h>
#include "ProcessMemoryEditor.hpp"
#include "CommandRegistry.hpp"


int tempX;
int tempY;
std::pair<int, int> tempPair;

unsigned long currentTeleportSlot = 0;
std::map<unsigned long, std::pair<int, int>> teleportMap;

unsigned long addr;

xeno::ProcessMemoryEditor pme;

void autoexec(pid_t pid) {
	pme.setPid(pid);
	try {
		addr = pme.readProcessMemory<unsigned long>(0x374030, true);
	} catch(std::runtime_error& e) {
		std::cerr << e.what() << "\n";
	}
}

void teleport(std::vector<std::string>* commandvec) {
	try {
		tempX = pme.readProcessMemory<int>(addr + 0x14, false);
		tempY = pme.readProcessMemory<int>(addr + 0x18, false);
	} catch(std::runtime_error &e) {
		std::cerr << e.what() << "\n";
	}
	std::cout << "Currently at X: " << tempX << ", Y: " << tempY << "\n";
	tempX = teleportMap[currentTeleportSlot].first;
	tempY = teleportMap[currentTeleportSlot].second;
	try {
		pme.writeProcessMemory<int>(addr + 0x14, &tempX, false);
		pme.writeProcessMemory<int>(addr + 0x18, &tempY, false);
	} catch(std::runtime_error& e) {
		std::cerr << e.what() << "\n";
	}
	std::cout << "Teleported to X: " << tempX << ", Y: " << tempY << "\n";
}

void saveloc(std::vector<std::string>* commandvec) {
	try {
		tempX = pme.readProcessMemory<int>(addr + 0x14, false);
		tempY = pme.readProcessMemory<int>(addr + 0x18, false);
	} catch(std::runtime_error& e) {
		std::cerr << e.what() << "\n";
	}
	tempPair = std::make_pair(tempX, tempY);
	teleportMap[currentTeleportSlot] = tempPair;
	std::cout << "Saved location X: " << tempX << ", Y: " << tempY << " to slot " << currentTeleportSlot << "\n";
}

void slot(std::vector<std::string>* commandvec) {
	try {
		currentTeleportSlot = std::stoi(commandvec->at(1));
	} catch(std::invalid_argument& e) {
		std::cout << "Argument isn't a number.\n";
	} catch(std::out_of_range& e) {
		std::cout << "Argument is WAY too big.\n";
	}
}

void getslot(std::vector<std::string>* commandvec) {
	std::cout << "Current teleport slot: " << currentTeleportSlot << "\n";
}

void registerCommand(xeno::CommandRegistry& cr) {
	cr.addCommand("teleport", teleport);
	cr.addCommand("saveloc", saveloc);
	cr.addCommand("slot", slot);
	cr.addCommand("getslot", getslot);
}