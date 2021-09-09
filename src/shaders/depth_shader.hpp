#pragma once

#include <string>

const std::string depth_frag_shader{
    "#version 450\n"
    "layout(binding = 1) uniform sampler2D texSampler;\n"
    "layout(location = 0) in vec3 fragColor;\n"
    "layout(location = 1) in vec2 fragTexCoord;\n"
    "layout(location = 0) out vec4 outColor;\n"
    "void main() {\n"
    "    outColor = texture(texSampler, fragTexCoord);\n"
    "}"
};

const std::string depth_vert_shader{
    "#version 450\n"
    "layout(binding = 0) uniform UniformBufferObject {\n"
    "    mat4 model;\n"
    "    mat4 view;\n"
    "    mat4 proj;\n"
    "} ubo;\n"
    "layout(location = 0) in vec3 inPosition;\n"
    "layout(location = 1) in vec3 inColor;\n"
    "layout(location = 2) in vec2 inTexCoord;\n"
    "layout(location = 0) out vec3 fragColor;\n"
    "layout(location = 1) out vec2 fragTexCoord;\n"
    "void main() {\n"
    "    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);\n"
    "    fragColor = inColor;\n"
    "    fragTexCoord = inTexCoord;\n"
    "}"
};