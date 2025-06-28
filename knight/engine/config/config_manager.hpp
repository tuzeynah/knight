#pragma once

#include "../../ext/nlohmann/json.hpp"
#include "../module/module_manager.hpp"

#include <string>
#include <unordered_map>
#include <shlobj.h>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

class c_config_manager {
private:
    std::filesystem::path path;
    std::vector<std::string> configs;
public:
    void initialize();
    void open_folder();
    void save(size_t id) const;
    void load(size_t id);
    void add(std::string name);
    void rename(size_t item, std::string new_name);
    void remove(size_t id);

    constexpr auto& get_configs() { return configs; };
    constexpr auto& get_path() { return path; };
};

inline auto config_manager = c_config_manager();