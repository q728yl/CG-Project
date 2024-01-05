#version 330 core

in vec4 outColor;  // 从顶点着色器接收颜色
out vec4 FragColor;

void main()
{
    // 使用纹理坐标来模拟火焰效果，这里简化为基于y坐标的颜色变化
    //vec3 flameColor = mix(vec3(1.0, 0.5, 0.0), vec3(1.0, 0.0, 0.0), TexCoord.y);
    
    // 添加颜色变化以模拟火焰
    //vec3 finalColor = ourColor.rgb * flameColor;
    vec2 center = vec2(0.5, 0.5);
    vec2 toCenter = gl_PointCoord - center;
    float distanceToCenter = length(toCenter);
    
    if (distanceToCenter > 0.5) {
        // 超出半径范围的部分不绘制
        discard;
    }
    FragColor = vec4(outColor);
}