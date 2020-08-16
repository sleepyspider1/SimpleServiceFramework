#include "Service.h"
#include "ServiceFactory.h"
#include <memory>

#if _WIN32
#include <windows.h>
#include <tchar.h>
int _tmain(int argc, TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
    auto s = CreateServiceDaemon("My Sample Service");
    if (!s->Initialize())
        return -1;

    s->Start();
    s->DoWork();
    s->Stop();
    s->Shutdown();
    return 0;
}
