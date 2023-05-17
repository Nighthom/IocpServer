#include "pch.h"
#include "Memory.h"

Memory::Memory()
{
	int32 size = 0;
	int32 tableCount = 0;

	for (size = 32; size <= 1024; size += 32)
	{
		MemoryPool* pool = new MemoryPool(size);

		while (tableCount <= size)
		{
			m_poolTable[tableCount] = pool;
			tableCount++;
		}
	}

	for (size = 1024; size <= 2048; size += 128)
	{
		MemoryPool* pool = new MemoryPool(size);

		while (tableCount <= size)
		{
			m_poolTable[tableCount] = pool;
			tableCount++;
		}
	}

	for (size = 2048; size <= 4096; size += 256)
	{
		MemoryPool* pool = new MemoryPool(size);

		while (tableCount <= size)
		{
			m_poolTable[tableCount] = pool;
			tableCount++;
		}
	}
}

Memory::~Memory()
{
	for(MemoryPool* pool : m_pools)
	{
		delete pool;
	}
	m_pools.clear();
}

void* Memory::Allocate(int32 size)
{
	int32 allocSize = size + sizeof(MemoryHeader);
	MemoryHeader* header = nullptr;

	if (allocSize > MAX_ALLOC_SIZE)
	{
		header = reinterpret_cast<MemoryHeader*>(_aligned_malloc(allocSize, SLIST_ALIGNMENT));
	}
	else
	{
		header = m_poolTable[allocSize]->Pop();
	}
	return MemoryHeader::AttachHeader(header, allocSize);
}

void Memory::Release(void* ptr)
{
	MemoryHeader* header = MemoryHeader::DettachHeader(ptr);
	int32 allocSize = header->m_allocSize;
	ASSERT_CRASH(allocSize > 0);

	if (allocSize > MAX_ALLOC_SIZE)
	{
		_aligned_free(header);
	}
	else
	{
		m_poolTable[allocSize]->Push(header);
	}
}
