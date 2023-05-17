#pragma once
#include <memory>

extern std::unique_ptr<class Memory> g_memory;
extern std::unique_ptr<class IocpCore> g_iocpCore;

class CoreGlobal
{
public:
	CoreGlobal();
	~CoreGlobal();
};

extern CoreGlobal g_coreGlobal;
