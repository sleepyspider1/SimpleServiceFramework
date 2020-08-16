#pragma once

#ifdef __unix__

#include "Service.h"
#include <Windows.h>
#include <string>

// http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html
class LinuxDaemon : public Service
{
public:
    explicit LinuxDaemon(const std::string& serviceName);
    bool Initialize() override;
    bool Shutdown() override;
    void Start() override;
    void Stop() override;
	bool ShouldStop() override;
    void OnEvent(DWORD controlCode) override;

private:
    void _ForkProcess();
    bool _ChangeFileModeMask();
    void _OpenLogFile();
    void _CreateUniqueSessionId();
    void _SetWorkingDirectory();
    void _CloseStandardFileDescriptors();
    void _DoWork();
    static std::string _serviceName;
};
#endif
