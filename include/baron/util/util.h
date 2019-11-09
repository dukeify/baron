#pragma once

#include <string>

namespace Baron {
 std::string deserializeClassModifiers(uint32_t modifiers);
 std::string deserializeFieldModifiers(uint32_t modifiers);
 std::string deserializeMethodModifiers(uint32_t modifiers);
}