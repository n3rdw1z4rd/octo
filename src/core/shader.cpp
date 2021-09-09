#include "shader.hpp"

#include <filesystem>
#include <fstream>

// https://github.com/jbikker/lighthouse2/blob/master/lib/rendercore_vulkan_rt/vulkan_shader.cpp

// std::string _get_cwd() {
//     return std::filesystem::current_path().string();
// }

// std::vector<char> _read_spv(const std::string_view& path) {
//     std::ifstream path(path.data(), std::ios::binary | std::ios::in | std::ios::ate);

//     if (!fileStream.is_open())
//         FATALERROR("Could not open file.");

//     const size_t size = fileStream.tellg();
//     fileStream.seekg(0, std::ios::beg);
//     std::vector<char> data(size);
//     fileStream.read(data.data(), size);
//     fileStream.close();
//     return data;
// }

// shaderc_shader_kind _get_shader_kind(const std::string& extension) {
//     shaderc_shader_kind kind;

//     if (extension == "comp")
//         kind = shaderc_shader_kind::shaderc_glsl_default_compute_shader;
//     else if (extension == "vert")
//         kind = shaderc_shader_kind::shaderc_glsl_default_vertex_shader;
//     else if (extension == "frag")
//         kind = shaderc_shader_kind::shaderc_glsl_default_fragment_shader;
//     else if (extension == "rchit")
//         kind = shaderc_shader_kind::shaderc_glsl_default_closesthit_shader;
//     else if (extension == "rgen")
//         kind = shaderc_shader_kind::shaderc_glsl_default_raygen_shader;
//     else if (extension == "rmiss")
//         kind = shaderc_shader_kind::shaderc_glsl_default_miss_shader;
//     else if (extension == "rahit")
//         kind = shaderc_shader_kind::shaderc_glsl_default_anyhit_shader;
//     else if (extension == "geom")
//         kind = shaderc_shader_kind::shaderc_glsl_default_geometry_shader;
//     else if (extension == "mesh")
//         kind = shaderc_shader_kind::shaderc_glsl_default_mesh_shader;
//     else if (extension == "rcall")
//         kind = shaderc_shader_kind::shaderc_glsl_default_callable_shader;
//     else
//         kind = shaderc_shader_kind::shaderc_glsl_infer_from_source;

//     return kind;
// }

// std::string load_shader(const std::string& path) {
//     std::filesystem::path _path{ path };

//     if (file_ext == "spv") {

//     } else {

//     }
// }

std::vector<uint32_t> compile_shader(
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
        log_error(module.GetErrorMessage());
        return std::vector<uint32_t>();
    }

    return { module.cbegin(), module.cend() };
}