#include "WindowsService.h"

#ifdef _WIN32

std::string           WindowsService::_serviceName = "Default Service Name";
SERVICE_STATUS        WindowsService::_serviceStatus = { 0 };
SERVICE_STATUS_HANDLE WindowsService::_statusHandle = NULL;
HANDLE                WindowsService::_serviceStopEvent = INVALID_HANDLE_VALUE;

// ------------------------------------------------------------------------------------------------
void WindowsService::ServiceMain(DWORD argc, LPTSTR* argv) {
	DWORD Status = E_FAIL;

	int wchars_num = MultiByteToWideChar(CP_UTF8, 0, _serviceName.c_str(), -1, NULL, 0);
	LPWSTR wstr = new WCHAR[wchars_num];
	MultiByteToWideChar(CP_UTF8, 0, _serviceName.c_str(), -1, wstr, wchars_num);
	// Register our service control handler with the SCM
	_statusHandle = RegisterServiceCtrlHandler(wstr, &WindowsService::ServiceCtrlHandler);
	delete[] wstr;

	if (_statusHandle == NULL) {
		return;
	}

	// Tell the service controller we are starting
	ZeroMemory(&_serviceStatus, sizeof(_serviceStatus));
	_serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	_serviceStatus.dwControlsAccepted = 0;
	_serviceStatus.dwCurrentState = SERVICE_START_PENDING;
	_serviceStatus.dwWin32ExitCode = 0;
	_serviceStatus.dwServiceSpecificExitCode = 0;
	_serviceStatus.dwCheckPoint = 0;

	if (SetServiceStatus(_statusHandle, &_serviceStatus) == FALSE) {
		OutputDebugString(TEXT("My Sample Service: ServiceMain: SetServiceStatus returned error"));
	}

	// Perform tasks necessary to start the service here

	 // Create a service stop event to wait on later
	_serviceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (_serviceStopEvent == NULL)
	{
		// Error creating event
		// Tell service controller we are stopped and exit
		_serviceStatus.dwControlsAccepted = 0;
		_serviceStatus.dwCurrentState = SERVICE_STOPPED;
		_serviceStatus.dwWin32ExitCode = GetLastError();
		_serviceStatus.dwCheckPoint = 1;

		if (SetServiceStatus(_statusHandle, &_serviceStatus) == FALSE) {
			OutputDebugString(TEXT("My Sample Service: ServiceMain: SetServiceStatus returned error"));
		}
		return;
	}

	// Tell the service controller we are started
	_serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	_serviceStatus.dwCurrentState = SERVICE_RUNNING;
	_serviceStatus.dwWin32ExitCode = 0;
	_serviceStatus.dwCheckPoint = 0;

	if (SetServiceStatus(_statusHandle, &_serviceStatus) == FALSE) {
		OutputDebugString(TEXT("My Sample Service: ServiceMain: SetServiceStatus returned error"));
	}

	// Start a thread that will perform the main task of the service
	HANDLE hThread = CreateThread(NULL, 0, &WindowsService::ServiceWorkerThread, NULL, 0, NULL);

	// Wait until our worker thread exits signaling that the service needs to stop
	WaitForSingleObject(hThread, INFINITE);

	// Perform any cleanup tasks
	CloseHandle(_serviceStopEvent);

	// Tell the service controller we are stopped
	_serviceStatus.dwControlsAccepted = 0;
	_serviceStatus.dwCurrentState = SERVICE_STOPPED;
	_serviceStatus.dwWin32ExitCode = 0;
	_serviceStatus.dwCheckPoint = 3;

	if (SetServiceStatus(_statusHandle, &_serviceStatus) == FALSE) {
		OutputDebugString(TEXT("My Sample Service: ServiceMain: SetServiceStatus returned error"));
	}
}

// ------------------------------------------------------------------------------------------------
DWORD WindowsService::ServiceWorkerThread(LPVOID lpParam) {
	//  Periodically check if the service has been requested to stop
	while (WaitForSingleObject(_serviceStopEvent, 0) != WAIT_OBJECT_0)
	{
		/*
		 * Perform main service function here
		 */

		 //  Simulate some work by sleeping
		Sleep(3000);
	}

	return ERROR_SUCCESS;
}

// ------------------------------------------------------------------------------------------------
WindowsService::WindowsService(const std::string& serviceName)
	: Service()
{
	_serviceName = serviceName;
}

// ------------------------------------------------------------------------------------------------
bool WindowsService::Initialize() {
	int wchars_num = MultiByteToWideChar(CP_UTF8, 0, _serviceName.c_str(), -1, NULL, 0);
	LPWSTR wstr = new WCHAR[wchars_num];
	MultiByteToWideChar(CP_UTF8, 0, _serviceName.c_str(), -1, wstr, wchars_num);

	SERVICE_TABLE_ENTRY ServiceTable[] = {
		{ wstr, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};
	delete[] wstr;

	if (StartServiceCtrlDispatcher(ServiceTable) == FALSE) {
		GetLastError();
		return false;
	}
	return true;
}

// ------------------------------------------------------------------------------------------------
bool WindowsService::Shutdown() {
	return true;
}

// ------------------------------------------------------------------------------------------------
void WindowsService::Start() {

}

// ------------------------------------------------------------------------------------------------
void WindowsService::Stop() {

}

// ------------------------------------------------------------------------------------------------
bool WindowsService::ShouldStop() {
	return false;
}

// ------------------------------------------------------------------------------------------------
void WindowsService::ServiceCtrlHandler(DWORD controlCode) {
	switch (controlCode)
	{
	case SERVICE_CONTROL_STOP:

		if (_serviceStatus.dwCurrentState != SERVICE_RUNNING)
			break;

		// Perform tasks necessary to stop the service here
		_serviceStatus.dwControlsAccepted = 0;
		_serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		_serviceStatus.dwWin32ExitCode = 0;
		_serviceStatus.dwCheckPoint = 4;

		if (SetServiceStatus(_statusHandle, &_serviceStatus) == FALSE) {
			OutputDebugString(TEXT("My Sample Service: ServiceCtrlHandler: SetServiceStatus returned error"));
		}

		// This will signal the worker thread to start shutting down
		SetEvent(_serviceStopEvent);

		break;

	default:
		break;
	}
}

// ------------------------------------------------------------------------------------------------
void WindowsService::OnEvent(DWORD controlCode) {

}

// ------------------------------------------------------------------------------------------------
void WindowsService::DoWork() {
}

#endif
