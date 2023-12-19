#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;  // 传递给片段着色器的纹理坐标
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoord = aTexCoord;  // 传递纹理坐标
    gl_Position = projection * view * model * vec4(aPos, 1.0);
  
}
