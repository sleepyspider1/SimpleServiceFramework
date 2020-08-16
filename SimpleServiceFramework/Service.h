#pragma once

#include <string>
#include <Windows.h>

class Service
{
public:
    virtual bool Initialize() = 0;
    virtual bool Shutdown() = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void DoWork() = 0;
    virtual bool ShouldStop() = 0;
    virtual void OnEvent(DWORD controlCode) = 0;
};
