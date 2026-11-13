#ifndef STL_MAP_EX
#define STL_MAP_EX

#include <optional>

template <typename Map, typename Key>
auto find_value(const Map& m, const Key& k) -> std::optional<typename Map::mapped_type> {
    auto it = m.find(k);
    if (it == m.end()) return std::nullopt;
    return it->second;
}

#endif