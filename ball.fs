#version 330 core
out vec4 FragColor; // 输出颜色到帧缓冲

in vec2 TexCoord;   // 输入的纹理坐标

uniform vec3 ourColor; // 小球的颜色

// 假设你已经有了一个纹理Sampler2D uniform变量
uniform sampler2D ourTexture; // 纹理采样器

void main()
{
    // 从纹理中采样颜色
    vec4 texColor = texture(ourTexture, TexCoord);
    
    // 使用纹理颜色和我们的颜色混合
    FragColor = vec4(ourColor, 1.0) * texColor;
}
