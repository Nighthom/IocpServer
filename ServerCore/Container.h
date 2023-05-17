#pragma once

#include <array>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "Allocator.h"

template <typename Type, size_t Size>
using Array = std::array<Type, Size>;

template <typename Type>
using Vector = std::vector<Type, StlAllocator<Type>>;

template <typename Type>
using List = std::list<Type, StlAllocator<Type>>;

template <typename Type>
using Deque = std::deque<Type, StlAllocator<Type>>;

template <typename Type, typename Container = Deque<Type>>
using Queue = std::queue<Type, Container>;

template <typename Type, typename Container = Deque<Type>, typename Pred = less<Type>>
using PriorityQueue = std::priority_queue<Type, Container, Pred>;

template <typename Type, typename Container = Deque<Type>>
using Stack = std::stack<Type, Container>;

template <typename Key, typename Type, typename Pred = less<Key>>
using Map = std::map<Key, Type, Pred, StlAllocator<std::pair<const Key, Type>>>;

template <typename Key, typename Pred = less<Key>>
using Set = std::set<Key, Pred, StlAllocator<Key>>;

template <typename Key, typename Type, typename Hasher = hash<Key>, typename KeyEq = equal_to<Key>>
using HashMap = std::unordered_map<Key, Type, Hasher, KeyEq, StlAllocator<std::pair<const Key, Type>>>;

template <typename Key, typename Hasher = hash<Key>, typename KeyEq = equal_to<Key>>
using HashSet = std::unordered_set<Key, Hasher, KeyEq, StlAllocator<Key>>;
