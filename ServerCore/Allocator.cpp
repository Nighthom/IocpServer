#include "pch.h"
#include "Allocator.h"
#include "Memory.h"

void* PoolAllocator::Allocate(int32 allocSize)
{
	return g_memory->Allocate(allocSize);
}

void PoolAllocator::Release(void* ptr)
{
	g_memory->Release(ptr);
}
