/*
 * ProcessMemoryEditor.hpp
 *
 *  Created on: Sep 15, 2019
 *      Author: walaryne
 */

#ifndef PROCESSMEMORYEDITOR_HPP_
#define PROCESSMEMORYEDITOR_HPP_
#include <iostream>
#include <sys/uio.h>
#include <proc/readproc.h>

namespace xeno {

class ProcessMemoryEditor {

public:
	ProcessMemoryEditor(pid_t pid) {
		_pid = pid;
		pid_t pidlist[2] {pid, 0};
		proc_t** prt = readproctab(PROC_FILLSTAT | PROC_PID, pidlist);
		_target_base_address = prt[0]->start_code;
	}

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
			return -1;
		} else {
			return retval;
		}
	}

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
			return -1;
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
