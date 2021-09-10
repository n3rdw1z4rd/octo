#pragma once

#include <string>

const std::string BASE_VERT_SHADER{
    "#version 450\n"
    "layout(location = 0) out vec3 fragColor;\n"
    "vec2 positions[3] = vec2[](\n"
    "    vec2(0.0, -0.5),\n"
    "    vec2(0.5, 0.5),\n"
    "    vec2(-0.5, 0.5)\n"
    ");\n"
    "vec3 colors[3] = vec3[](\n"
    "    vec3(1.0, 0.0, 0.0),\n"
    "    vec3(0.0, 1.0, 0.0),\n"
    "    vec3(0.0, 0.0, 1.0)\n"
    ");\n"
    "void main() {\n"
    "    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);\n"
    "    fragColor = colors[gl_VertexIndex];\n"
    "}"
};

const std::string DEPTH_VERT_SHADER{
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

const std::string BASE_FRAG_SHADER{
    "#version 450\n"
    "layout(location = 0) in vec3 fragColor;\n"
    "layout(location = 0) out vec4 outColor;\n"
    "void main() {\n"
    "    outColor = vec4(fragColor, 1.0);\n"
    "}"
};

const std::string DEPTH_FRAG_SHADER{
    "#version 450\n"
    "layout(binding = 1) uniform sampler2D texSampler;\n"
    "layout(location = 0) in vec3 fragColor;\n"
    "layout(location = 1) in vec2 fragTexCoord;\n"
    "layout(location = 0) out vec4 outColor;\n"
    "void main() {\n"
    "    outColor = texture(texSampler, fragTexCoord);\n"
    "}"
};
