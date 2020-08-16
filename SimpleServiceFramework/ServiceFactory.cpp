#include "ServiceFactory.h"

#if _WIN32
#include "WindowsService.h"
#else
#include "LinuxDaemon.h"
#endif

std::unique_ptr<class Service> CreateServiceDaemon(const std::string& serviceName) {
#if _WIN32
	return std::make_unique<WindowsService>(serviceName);
#else
	return std::make_unique<LinuxDaemon>(serviceName);
#endif
}
