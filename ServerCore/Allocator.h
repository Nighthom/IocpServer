#pragma once

class PoolAllocator
{
public:
	static void* Allocate(int32 allocSize);
	static void Release(void* ptr);
};


template <typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator() { }
	
	template<typename Other>
	StlAllocator(const StlAllocator<Other>&) { }

	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(count);
		return static_cast<T*>(PoolAllocator::Allocate(size));
	}

	void deallocate(T* ptr, size_t count)
	{
		PoolAllocator::Release(ptr);
	}
};
