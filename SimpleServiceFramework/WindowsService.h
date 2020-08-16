#pragma once

#ifdef _WIN32

#include "Service.h"
#include <Windows.h>

// https://www.codeproject.com/Articles/499465/Simple-Windows-Service-in-Cplusplus
class WindowsService : public Service {
public:
	explicit WindowsService(const std::string& serviceName);
	bool Initialize() override;
	bool Shutdown() override;
	void Start() override;
	void Stop() override;
	bool ShouldStop() override;
	void OnEvent(DWORD controlCode) override;
	void DoWork() override;

private:
    static std::string _serviceName;
	static void ServiceMain(DWORD argc, LPTSTR* argv);
	static DWORD ServiceWorkerThread(LPVOID lpParam);
	static void ServiceCtrlHandler(DWORD CtrlCode);
	static SERVICE_STATUS        _serviceStatus;
	static SERVICE_STATUS_HANDLE _statusHandle;
	static HANDLE                _serviceStopEvent;
};

#endif
