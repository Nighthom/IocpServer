#pragma once

#include "Types.h"
#include "Container.h"
#include "CoreMacro.h"
#include "CoreGlobal.h"

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <windows.h>
#include <mutex>
#include <thread>
#include <atomic>
using namespace std;

#include "memory.h"
#include "Allocator.h"
#include "ObjectPool.h"
#include "SocketUtils.h"
#include "IocpCore.h"
