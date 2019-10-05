#include <map>
#include <functional>
#include <vector>
#include <string>
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/keysym.h>
//#include <X11/Xresource.h>
#include "ProcessMemoryEditor.hpp"


int tempX;
int tempY;
std::pair<int, int> tempPair;

unsigned long currentTeleportSlot = 0;
std::map<unsigned long, std::pair<int, int>> teleportMap;

unsigned long addr;

void autoexec(xeno::ProcessMemoryEditor* pme) {
	addr = pme->readProcessMemory<unsigned long>(0x374030, true);
}

void teleport(xeno::ProcessMemoryEditor* pme, std::vector<std::string>* commandvec) {
	tempX = pme->readProcessMemory<int>(addr + 0x14, false);
	tempY = pme->readProcessMemory<int>(addr + 0x18, false);
	std::cout << "Currently at X: " << tempX << ", Y: " << tempY << "\n";
	tempX = teleportMap[currentTeleportSlot].first;
	tempY = teleportMap[currentTeleportSlot].second;
	pme->writeProcessMemory<int>(addr + 0x14, &tempX, false);
	pme->writeProcessMemory<int>(addr + 0x18, &tempY, false);
	std::cout << "Teleported to X: " << tempX << ", Y: " << tempY << "\n";
}

void saveloc(xeno::ProcessMemoryEditor* pme, std::vector<std::string>* commandvec) {
	tempX = pme->readProcessMemory<int>(addr + 0x14, false);
	tempY = pme->readProcessMemory<int>(addr + 0x18, false);
	tempPair = std::make_pair(tempX, tempY);
	teleportMap[currentTeleportSlot] = tempPair;
	std::cout << "Saved location X: " << tempX << ", Y: " << tempY << " to slot " << currentTeleportSlot << "\n";
}

void slot(xeno::ProcessMemoryEditor* pme, std::vector<std::string>* commandvec) {
	currentTeleportSlot = std::stoi(commandvec->at(1));
}

void getslot(xeno::ProcessMemoryEditor* pme, std::vector<std::string>* commandvec) {
	std::cout << "Current teleport slot: " << currentTeleportSlot << "\n";
}

void registerCommand(std::map<std::string, std::function<void(xeno::ProcessMemoryEditor*, std::vector<std::string>*)>>* commandFunctionMap) {
	commandFunctionMap->operator[]("teleport") = std::function(teleport);
	commandFunctionMap->operator[]("saveloc") = std::function(saveloc);
	commandFunctionMap->operator[]("slot") = std::function(slot);
	commandFunctionMap->operator[]("getslot") = std::function(getslot);
}