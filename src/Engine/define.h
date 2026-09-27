#ifndef ENGINE_H_DEFINED
#define ENGINE_H_DEFINED

#include <windows.h>

#include <string>
#include <unordered_map>
#include <vector>
#include <random>

#include "Core/Utils.hpp"
#include "Core/MathUtils.hpp"

using namespace MathUtils;

using String  = std::string;
using WString = std::wstring;

template <typename Type>
using Vector  = std::vector<Type>;

template<typename Key, typename Value, typename Hash = std::hash<Key>, typename Equality = std::equal_to<Key>>
using UnorderedMap = std::unordered_map<Key, Value, Hash, Equality>;

template <typename Type>
using List = std::list<Type>;

template <typename First, typename Second>
using Pair = std::pair<First, Second>;

#endif
