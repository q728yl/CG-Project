#version 330 core
out vec4 FragColor; // 输出颜色到帧缓冲

in vec2 TexCoord;   // 输入的纹理坐标
in vec3 ourColor; // 输入的颜色
flat in int textureId; // 输入的纹理id
flat in int shouldDraw1;

// 定义两个纹理采样器
uniform sampler2D ballTextureSampler;
uniform sampler2D floorTextureSampler;
uniform sampler2D ashTextureSampler;

void main()
{
    if (shouldDraw1 == 0) discard;

    vec3 color = ourColor;
    vec4 texColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
     if (textureId == 1) {
        texColor = texture(ballTextureSampler, TexCoord);
    } else if(textureId == 2){
        texColor = texture(floorTextureSampler, TexCoord);
    }
    else if(textureId == 3){
        texColor = texture(ashTextureSampler, TexCoord);
        //灰色
        color = vec3(0.5,0.5,0.5);
    }
    // 使用纹理颜色和我们的颜色混合
    FragColor = vec4(color, 1.0) * texColor;
}
