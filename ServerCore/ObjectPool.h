#pragma once
#include "MemoryPool.h"
#include "Types.h"
#include <memory>

template <typename Type>
class ObjectPool
{
public:
	template <typename... Args>
	static Type* Pop(Args&&... args)
	{
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(s_pool.Pop(), s_allocSize));
		new(memory)Type(std::forward<Args>(args)...);
		return memory;
	}

	static void Push(Type* ptr)
	{
		ptr->~Type();
		s_pool.Push(MemoryHeader::DettachHeader(ptr));
	}
	
	template <typename... Args>
	static std::shared_ptr<Type> MakeShared(Args&&... args)
	{
		std::shared_ptr<Type> ptr = { Pop(forward<Args>(args)...), Push };
		return ptr;
	}

private:
	static MemoryPool s_pool;
	static int32 s_allocSize;
};

template <typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template <typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };