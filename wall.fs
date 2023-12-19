#version 330 core
in vec2 TexCoord;  // 接收纹理坐标
out vec4 FragColor;
uniform bool hasTexture; // 是否有纹理
uniform vec3 wallColor; // 从顶点着色器传递过来的颜色
uniform sampler2D textureSampler;  // 纹理采样器

void main()
{
    if (hasTexture) {
        FragColor = texture(textureSampler, TexCoord) * vec4(wallColor, 1.0);
    } else {
        FragColor = vec4(wallColor, 1.0);
    }
}