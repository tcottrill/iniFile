// New code update 6/17/2025
// Removed dependency on legacy Windows functions. 
// Added string functions for other uses. 
// Some code below was written with the assistance of ChatGPT

/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non - commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain.We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors.We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to < https://unlicense.org/>
*/


#include "iniFile.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

#define MAX_INI 255

static char m_szFileName[MAX_INI] = { 0 };

struct IniEntry {
    std::string key;
    std::string value;
    std::string original_line;
    bool is_comment = false;
};

static std::map<std::string, std::vector<IniEntry>> ini_data;

static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of("");
    size_t end = s.find_last_not_of("");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

void LoadIniFile() {
    ini_data.clear();
    std::ifstream file(m_szFileName);
    if (!file.is_open()) return;

    std::string line, section;
    while (std::getline(file, line)) {
        std::string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == ';' || trimmed[0] == '#') {
            ini_data[section].push_back({ "", "", line, true });
        } else if (trimmed.front() == '[' && trimmed.back() == ']') {
            section = trimmed.substr(1, trimmed.size() - 2);
            ini_data[section]; // Ensure section exists
        } else {
            size_t eq = trimmed.find('=');
            if (eq != std::string::npos) {
                std::string key = trim(trimmed.substr(0, eq));
                std::string value = trim(trimmed.substr(eq + 1));
                ini_data[section].push_back({ key, value, line, false });
            } else {
                ini_data[section].push_back({ "", "", line, true });
            }
        }
    }
}

void SaveIniFile() {
    std::ofstream file(m_szFileName);
    for (const auto& sec : ini_data) {
        if (!sec.first.empty())
            file << "[" << sec.first << "]";
        for (const auto& entry : sec.second) {
            if (entry.is_comment || entry.key.empty())
                file << entry.original_line << "";
            else
                file << entry.key << "=" << entry.value << "";
        }
        file << "";
    }
}

void SetIniFile(const char* szFileName) {
    strncpy(m_szFileName, szFileName, MAX_INI - 1);
    LoadIniFile();
}

std::string get_value(const char* section, const char* key, const char* defval) {
    auto it = ini_data.find(section);
    if (it != ini_data.end()) {
        for (const auto& entry : it->second) {
            if (!entry.is_comment && entry.key == key)
                return entry.value;
        }
    }
    return defval;
}

void update_or_add_entry(const char* section, const char* key, const char* value) {
    auto& entries = ini_data[section];
    for (auto& entry : entries) {
        if (!entry.is_comment && entry.key == key) {
            entry.value = value;
            entry.original_line = std::string(key) + "=" + value;
            SaveIniFile();
            return;
        }
    }
    entries.push_back({ key, value, std::string(key) + "=" + value, false });
    SaveIniFile();
}

int get_config_int(const char* section, const char* key, int defval) {
    return std::atoi(get_value(section, key, std::to_string(defval).c_str()).c_str());
}

float get_config_float(const char* section, const char* key, float defval) {
    return (float) std::atof(get_value(section, key, std::to_string(defval).c_str()).c_str());
}

bool get_config_bool(const char* section, const char* key, bool defval) {
    std::string val = get_value(section, key, defval ? "True" : "False");
    std::transform(val.begin(), val.end(), val.begin(), ::tolower);
    return (val == "true" || val == "1" || val == "yes");
}

char* get_config_string(const char* section, const char* key, const char* defval) {
    std::string val = get_value(section, key, defval);
    char* res = new char[val.size() + 1];
    std::strcpy(res, val.c_str());
    
   // Note: Caller must delete[] the returned pointer!
   return res;
}

void set_config_string(const char* section, const char* key, const char* val) {
    update_or_add_entry(section, key, val);
}

void set_config_int(const char* section, const char* key, int val) {
    set_config_string(section, key, std::to_string(val).c_str());
}

void set_config_float(const char* section, const char* key, float val) {
    set_config_string(section, key, std::to_string(val).c_str());
}

void set_config_bool(const char* section, const char* key, bool val) {
    set_config_string(section, key, val ? "True" : "False");
}


// std::string overloads
int get_config_int(const std::string& section, const std::string& key, int defaultValue) {
    return get_config_int(section.c_str(), key.c_str(), defaultValue);
}

float get_config_float(const std::string& section, const std::string& key, float defaultValue) {
    return get_config_float(section.c_str(), key.c_str(), defaultValue);
}

bool get_config_bool(const std::string& section, const std::string& key, bool defaultValue) {
    return get_config_bool(section.c_str(), key.c_str(), defaultValue);
}

std::string get_config_string(const std::string& section, const std::string& key, const std::string& defaultValue) {
    char* result = get_config_string(section.c_str(), key.c_str(), defaultValue.c_str());
    std::string value(result);
    delete[] result;
    return value;
}

void set_config_int(const std::string& section, const std::string& key, int value) {
    set_config_int(section.c_str(), key.c_str(), value);
}

void set_config_float(const std::string& section, const std::string& key, float value) {
    set_config_float(section.c_str(), key.c_str(), value);
}

void set_config_bool(const std::string& section, const std::string& key, bool value) {
    set_config_bool(section.c_str(), key.c_str(), value);
}

void set_config_string(const std::string& section, const std::string& key, const std::string& value) {
    set_config_string(section.c_str(), key.c_str(), value.c_str());
}
