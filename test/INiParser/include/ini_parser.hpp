
#pragma once
#include <string>
#include <unordered_map>
#include <optional>

class IniParser{
    //节下的键值对
    using ini_section_t = std::unordered_map<std::string , std::string>;
    // 节对应若干个键值对
    std::unordered_map<std::string, ini_section_t> m_data;
    //先查找ini_section_t,然后再根据key,查找value

public:
    bool parse(const std::string_view view);
    std::optional<std::string> get(const std::string_view section , 
                                    const std::string_view key);

};