#version 330 core
layout (location = 0) in vec3 aPos;     // 位置坐标
layout (location = 1) in vec2 aTexCoord; // 纹理坐标

out vec2 TexCoord; // 传递给片段着色器的纹理坐标

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord; // 将纹理坐标传递给片段着色器
}
