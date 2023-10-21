#pragma once

#include "defines.hpp"

#include <shaderc/shaderc.hpp>
#include <string>
#include <vector>

const std::string BASE_VERT_SHADER{R"(
    #version 450

    layout(location = 0) in vec2 inPosition;
    layout(location = 1) in vec3 inColor;
    layout(location = 0) out vec3 fragColor;

    void main() {
        gl_Position = vec4(inPosition, 0.0, 1.0);
        fragColor = inColor;
    }
)"};

const std::string BASE_FRAG_SHADER{R"(
    #version 450

    layout(location = 0) in vec3 fragColor;
    layout(location = 0) out vec4 outColor;

    void main() {
        outColor = vec4(fragColor, 1.0);
    }
)"};

const std::string DEPTH_VERT_SHADER{R"(
    #version 450

    layout(binding = 0) uniform UniformBufferObject {
        mat4 model;
        mat4 view;
        mat4 proj;
    } ubo;

    layout(location = 0) in vec3 inPosition;
    layout(location = 1) in vec3 inColor;
    layout(location = 2) in vec2 inTexCoord;
    layout(location = 0) out vec3 fragColor;
    layout(location = 1) out vec2 fragTexCoord;

    void main() {
        gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
        fragColor = inColor;
        fragTexCoord = inTexCoord;
    }
)"};

const std::string DEPTH_FRAG_SHADER{R"(
    #version 450

    layout(binding = 1) uniform sampler2D texSampler;
    layout(location = 0) in vec3 fragColor;
    layout(location = 1) in vec2 fragTexCoord;
    layout(location = 0) out vec4 outColor;
    
    void main() {
        outColor = texture(texSampler, fragTexCoord);
    }
)"};

std::vector<uint32_t> CompileShaderSource(
    const std::string& name,
    const std::string& source,
    shaderc_shader_kind kind,
    const std::vector<std::pair<std::string, std::string>>& definitions = {}
);