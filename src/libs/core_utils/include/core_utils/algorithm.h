#pragma once

#include <unordered_set>

template <typename... Ts>
bool areUnique(Ts... ts) noexcept
{
    static_assert(sizeof...(Ts) > 2);
    return std::unordered_set{ std::forward<Ts>(ts)... }.size() == sizeof...(Ts);
}
