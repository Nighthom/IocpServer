#include "pch.h"
#include "CoreGlobal.h"
#include "Memory.h"

CoreGlobal g_coreGlobal;
unique_ptr<Memory> g_memory;
unique_ptr<IocpCore> g_iocpCore;

CoreGlobal::CoreGlobal()
{
	g_memory = make_unique<Memory>();
	g_iocpCore = make_unique<IocpCore>();
	SocketUtils::Init();
}

CoreGlobal::~CoreGlobal()
{
	SocketUtils::Clear();
}
