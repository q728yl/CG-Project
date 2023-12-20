#version 330 core
layout (location = 0) in vec3 aPos;
// ... 其他属性

out vec4 FragPosLightSpace;  // 从顶点着色器传递到片段着色器的光空间位置

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
    FragPosLightSpace = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
