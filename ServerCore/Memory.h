#pragma once
#include "MemoryPool.h"

class Memory
{
	enum
	{
		POOL_SIZE = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096,
	};

public:
	Memory();
	~Memory();

	void* Allocate(int32 size);
	void Release(void* ptr);
private:
	vector<MemoryPool*> m_pools;
	MemoryPool* m_poolTable[MAX_ALLOC_SIZE + 1];
};

template <typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* ptr = static_cast<Type*>(PoolAllocator::Allocate(sizeof(Type)));
	new(ptr)Type(std::forward<Args>(args)...);
	return ptr;
}

template <typename Type>
void xdelete(Type* ptr)
{
	ptr->~Type();
	PoolAllocator::Release(ptr);
}

template <typename Type, typename... Args>
shared_ptr<Type> MakeShared(Args&&... args)
{
	return shared_ptr<Type>{ xnew<Type>(std::forward<Args>(args)...), xdelete<Type> };
}
