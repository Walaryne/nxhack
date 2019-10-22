#ifndef SYMBOLS_HPP_
#define SYMBOLS_HPP_

#define REGISTERCOMMAND "_Z15registerCommandRN4xeno15CommandRegistryE"
#define AUTOEXEC "_Z8autoexeci"

#include <map>
#include <functional>
#include "ProcessMemoryEditor.hpp"
#include "CommandRegistry.hpp"

typedef void (*registerCommand)(xeno::CommandRegistry& cr);
typedef void (*autoexec)(pid_t pid);

#endif /* SYMBOLS_HPP_ */