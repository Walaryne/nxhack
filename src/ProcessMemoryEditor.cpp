#include <sys/uio.h>
#include "ProcessMemoryEditor.hpp"

namespace xeno {

void ProcessMemoryEditor::setPid(pid_t pid) {
	_pid = pid;
	pid_t pidlist[2] {pid, 0};
	proc_t** prt = readproctab(PROC_FILLSTAT | PROC_PID, pidlist);
	_target_base_address = prt[0]->start_code;
}

pid_t ProcessMemoryEditor::getPid() {
	return _pid;
}

}