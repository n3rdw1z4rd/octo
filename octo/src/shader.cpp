#include "../include/core/logger.hpp"
#include "../include/core/shader.hpp"

std::vector<uint32_t> CompileShaderSource(
    const std::string& name,
    const std::string& source,
    shaderc_shader_kind kind,
    const std::vector<std::pair<std::string, std::string>>& definitions
) {
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;

    options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_0);
    options.SetSourceLanguage(shaderc_source_language_glsl);
    options.SetOptimizationLevel(shaderc_optimization_level_performance);

    std::vector<std::pair<std::string, std::string>> defs = definitions;
    defs.push_back(std::make_pair("__VK_GLSL__", "1"));

    for (auto& defPair : defs)
        options.AddMacroDefinition(defPair.first, defPair.second);

    auto module = compiler.CompileGlslToSpv(source, kind, name.c_str(), options);

    if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
        LogError(module.GetErrorMessage());
        return std::vector<uint32_t>();
    }

    return { module.cbegin(), module.cend() };
}