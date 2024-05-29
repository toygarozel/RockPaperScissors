#pragma once

#include <string>

struct Player {
    Player(const std::string& n) : name {n}
    {}

    std::string name{""};
    std::string choice{""};
    std::size_t score{};
};