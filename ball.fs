#version 330 core
out vec4 FragColor; // 输出颜色到帧缓冲

in vec2 TexCoord;   // 输入的纹理坐标
in vec3 ourColor; // 输入的颜色
flat in int textureId; // 输入的纹理id

// 定义两个纹理采样器
uniform sampler2D ballTextureSampler;
uniform sampler2D floorTextureSampler;

void main()
{
    vec4 texColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
     if (textureId == 0) {
        texColor = texture(ballTextureSampler, TexCoord);
    } else if(textureId == 1){
        texColor = texture(floorTextureSampler, TexCoord);
    }
    // 使用纹理颜色和我们的颜色混合
    FragColor = vec4(ourColor, 1.0) * texColor;
}
