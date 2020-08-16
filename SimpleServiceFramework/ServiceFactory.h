#pragma once

#include <memory>
#include <string>

std::unique_ptr<class Service> CreateServiceDaemon(const std::string& serviceName);
