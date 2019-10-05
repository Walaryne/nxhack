#ifndef SYMBOLS_HPP_
#define SYMBOLS_HPP_

#define REGISTERCOMMAND "_Z15registerCommandPSt3mapINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESt8functionIFvPN4xeno19ProcessMemoryEditorEPSt6vectorIS5_SaIS5_EEEESt4lessIS5_ESaISt4pairIKS5_SF_EEE"
#define AUTOEXEC "_Z8autoexecPN4xeno19ProcessMemoryEditorE"

#include "ProcessMemoryEditor.hpp"
#include <map>
#include <functional>

typedef void (*registerCommand)(std::map<std::string, std::function<void(xeno::ProcessMemoryEditor*, std::vector<std::string>*)>>*);
typedef void (*autoexec)(xeno::ProcessMemoryEditor* pme);

#endif /* SYMBOLS_HPP_ */