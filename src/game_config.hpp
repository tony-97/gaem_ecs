#pragma once

#include <fstream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct GameConfig_t
{
    explicit GameConfig_t()
    {
        json data { json::parse(std::ifstream{ "./game_config.json" }) };
    }
};
