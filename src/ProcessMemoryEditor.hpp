/*
 * ProcessMemoryEditor.hpp
 *
 *  Created on: Sep 15, 2019
 *      Author: walaryne
 */

#ifndef PROCESSMEMORYEDITOR_HPP_
#define PROCESSMEMORYEDITOR_HPP_
#include <iostream>
#include <cerrno>
#include <cstring>
#include <sys/uio.h>
#include <proc/readproc.h>

namespace xeno {


/*
* The process memory editor is a class providing two templated functions,
* readProcessMemory and writeProcessMemory.
*
* These two functions accept similar arguments (in fact, the only difference
* is that the write requires a value passed as a pointer for writing)
*
* They allow you to write any type to the memory of another process,
* and read from an address returning any type (hopefully a correct one!).
*
* This allows for lots of possible uses, from game hacking to monitoring memory.
*/
class ProcessMemoryEditor {

public:
	//Constructor taking the PID of another process as an argument
	//ProcessMemoryEditor(pid_t pid)
	ProcessMemoryEditor(pid_t pid) {
		_pid = pid;
		pid_t pidlist[2] {pid, 0};
		proc_t** prt = readproctab(PROC_FILLSTAT | PROC_PID, pidlist);
		_target_base_address = prt[0]->start_code;
	}

	/*
	* Read from an address in another process' memory, returning any type you wish.
	*
	* The offset is the location in the other process' memory you'd like to read from.
	*
	* The boolean relativeToBase, specifies whether the offset given should
	* be added to the target process' base address before a read is attempted.
	*
	* T readProcessMemory<typename T>(unsigned long offset, bool relativeToBase) 
	*/
	template<typename READTYPE>
	READTYPE readProcessMemory(unsigned long offset, bool relativeToBase) {
		struct iovec local;
		struct iovec remote;
		READTYPE retval;
		ssize_t nread;

		local.iov_base = &retval;
		local.iov_len = sizeof(retval);
		if(relativeToBase) {
			remote.iov_base = (void*)(_target_base_address + offset);
		} else {
			remote.iov_base = (void*)(offset);
		}
		remote.iov_len = sizeof(retval);

		nread = process_vm_readv(_pid, &local, 1, &remote, 1, 0);
		if(nread != sizeof(retval)) {
			std::cerr << "Error: No bytes could be read from given location -- hint: use sudo?" << std::endl;
			throw std::runtime_error("process_vm_readv: " + std::string(std::strerror(errno)));
		} else {
			return retval;
		}
	}

	/*
	* Write to an address in another process' memory, using any type you wish.
	*
	* The offset is the location in the other process' memory you'd like to write to.
	*
	* The boolean relativeToBase, specifies whether the offset given should
	* be added to the target process' base address before a write is attempted.
	*
	* int writeProcessMemory<typename T>(unsigned long offset, bool relativeToBase) 
	*/
	template<typename WRITETYPE>
	int writeProcessMemory(unsigned long offset, WRITETYPE* value, bool relativeToBase) {
		struct iovec local;
		struct iovec remote;
		ssize_t nread;

		local.iov_base = value;
		local.iov_len = sizeof(*value);
		if(relativeToBase) {
			remote.iov_base = (void*)(_target_base_address + offset);
		} else {
			remote.iov_base = (void*)(offset);
		}
		remote.iov_len = sizeof(*value);

		nread = process_vm_writev(_pid, &local, 1, &remote, 1, 0);
		if(nread != sizeof(*value)) {
			std::cerr << "Error: No bytes could be read from given location -- hint: use sudo?" << std::endl;
			throw std::runtime_error("process_vm_writev: " + std::string(std::strerror(errno)));
		} else {
			return 0;
		}
	}

private:
	pid_t _pid;
	unsigned long _target_base_address;
};

}




#endif /* PROCESSMEMORYEDITOR_HPP_ */
