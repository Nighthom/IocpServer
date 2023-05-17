#pragma once

enum
{
	SLIST_ALIGNMENT = 16, 
};


DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryHeader : SLIST_ENTRY
{
	int32 m_allocSize = 0;

	MemoryHeader(int32 allocSize) : m_allocSize(allocSize) { }

	static void* AttachHeader(MemoryHeader* header, int32 allocSize)
	{
		new(header)MemoryHeader(allocSize);
		return reinterpret_cast<void*>(--header);
	}

	static MemoryHeader* DettachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}
};

class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void Push(MemoryHeader* ptr);
	MemoryHeader* Pop();

private:
	SLIST_HEADER m_header;
	int32 m_allocSize = 0;
	std::atomic<int32> m_useCount = 0;
	std::atomic<int32> m_reserveCount = 0;
};

