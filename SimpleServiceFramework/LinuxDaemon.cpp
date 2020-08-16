#include "LinuxDaemon.h"

#ifdef __unix__
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

std::string LinuxDaemon::_serviceName = "Default Linux Daemon Name";

// ------------------------------------------------------------------------------------------------
LinuxDaemon::LinuxDaemon(const std::string& serviceName)
	: Service()
{
	_serviceName = serviceName;
}

// ------------------------------------------------------------------------------------------------
bool LinuxDaemon::Initialize() {
    if (!_ForkProcess())
        return false;

    _ChangeFileModeMask();
    _OpenLogFile();
    _CreateUniqueSessionId();
    _SetWorkingDirectory();
    _CloseStandardFileDescriptors();
    _DoWork();
	return true;
}

// ------------------------------------------------------------------------------------------------
bool LinuxDaemon::Shutdown() {
    closelog();
	return true;
}

// ------------------------------------------------------------------------------------------------
void LinuxDaemon::Start() {

}

// ------------------------------------------------------------------------------------------------
void LinuxDaemon::Stop() {

}

// ------------------------------------------------------------------------------------------------
bool LinuxDaemon::ShouldStop() {
	return false;
}

// ------------------------------------------------------------------------------------------------
void LinuxDaemon::OnEvent(DWORD controlCode) {

}

// ------------------------------------------------------------------------------------------------
bool LinuxDaemon::_ForkProcess() {
    pid_t pid;

    // Fork off the parent process.
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    // If we got a good PID, then we can exit the parent process.
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    return true;
}

// ------------------------------------------------------------------------------------------------
bool LinuxDaemon::_ChangeFileModeMask() {
    // So we can access any files we create. Like logs.
    umask(0);
}

// ------------------------------------------------------------------------------------------------
void LinuxDaemon::_OpenLogFile() {
    openlog("slog", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Starting linux daemon...");
}

// ------------------------------------------------------------------------------------------------
void LinuxDaemon::_CreateUniqueSessionId() {
    // Create a new SID for the child process.
    sid = setsid();
    if (sid < 0) {
        syslog(LOG_INFO, "Failed to create a unique session id for child process.");
        exit(EXIT_FAILURE);
    }
}

// ------------------------------------------------------------------------------------------------
void LinuxDaemon::_SetWorkingDirectory() {
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
        syslog(LOG_INFO, "Failed to set working directory.");
    }
}

// ------------------------------------------------------------------------------------------------
void LinuxDaemon::_CloseStandardFileDescriptors() {
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

// ------------------------------------------------------------------------------------------------
void LinuxDaemon::_DoWork() {
    while (1) {
        // 3 seconds.
        sleep(3);
    }
}

#endif
