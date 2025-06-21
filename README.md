# 📝 INI File Parser (C++)

A simple, modern C++ INI file loader and writer. This library reads key-value pairs grouped by section headers and allows safe, readable configuration access and updates.

---

## 📁 Files

- `iniFile.h` – Header for INI config functions
- `iniFile.cpp` – Implementation of file parsing, reading, and writing

---

## 🚀 Usage Example

```cpp
#include "iniFile.h"
#include <iostream>

int main() {
    SetIniFile("config.ini");

    int width = get_config_int("Window", "Width", 800);
    float gamma = get_config_float("Display", "Gamma", 2.2f);
    bool fullscreen = get_config_bool("Display", "Fullscreen", false);
    std::string playerName = get_config_string("Player", "Name", "Guest");

    std::cout << "Resolution: " << width << " | Gamma: " << gamma << " | Fullscreen: " << fullscreen << "\n";

    set_config_int("Window", "Width", 1024);
    set_config_bool("Display", "Fullscreen", true);
    set_config_string("Player", "Name", "Alice");

    return 0;
}
```

---

## 🛠 API Overview

### 🔧 Setup

- `void SetIniFile(const char* szFileName);`  
  Load or create the INI file to be used by all other calls.

---

### 🧾 Read Functions

#### C-style
```cpp
int   get_config_int(const char* section, const char* key, int default);
float get_config_float(const char* section, const char* key, float default);
bool  get_config_bool(const char* section, const char* key, bool default);
char* get_config_string(const char* section, const char* key, const char* default); // must delete[]
```

#### `std::string` overloads
```cpp
int         get_config_int(const std::string&, const std::string&, int default);
float       get_config_float(const std::string&, const std::string&, float default);
bool        get_config_bool(const std::string&, const std::string&, bool default);
std::string get_config_string(const std::string&, const std::string&, const std::string& default);
```

---

### 📝 Write Functions

#### C-style
```cpp
void set_config_int(const char* section, const char* key, int value);
void set_config_float(const char* section, const char* key, float value);
void set_config_bool(const char* section, const char* key, bool value);
void set_config_string(const char* section, const char* key, const char* value);
```

#### `std::string` overloads
```cpp
void set_config_int(const std::string&, const std::string&, int value);
void set_config_float(const std::string&, const std::string&, float value);
void set_config_bool(const std::string&, const std::string&, bool value);
void set_config_string(const std::string&, const std::string&, const std::string& value);
```

---

## 📄 Format Example

```ini
[Window]
Width=1280

[Display]
Gamma=2.2
Fullscreen=True

[Player]
Name=Guest
```

---

## 📌 Notes

- Automatically preserves comments and formatting
- Rewrites only updated keys
- `get_config_string(...)` returns a heap-allocated buffer in the C-style version — don't forget to `delete[]` it!
- Written to be cross-platform and dependency-free

---

## 🆓 License

This code is released into the public domain via [The Unlicense](https://unlicense.org/). Use it freely in any project.
