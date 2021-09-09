#pragma once

#include "defines.hpp"
#include "types.hpp"
#include "logger.hpp"

#include <shaderc/shaderc.hpp>
#include <string>
#include <vector>

std::string load_shader(const char* path);

std::vector<uint32_t> compile_shader(
    const std::string& name,
    const std::string& source,
    shaderc_shader_kind kind,
    const std::vector<std::pair<std::string, std::string>>& definitions = {}
);