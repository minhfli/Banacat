#pragma once

#include <string>
#include <nlohmann/json.hpp>
std::string ReadTextFile(const std::string path);

nlohmann::json ReadJsonFile(const std::string path);
