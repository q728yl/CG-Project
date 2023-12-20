#version 330 core
// ... 其他定义和uniforms

in vec4 FragPosLightSpace;  
uniform sampler2D depthMap;
uniform vec3 lightPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // 把坐标从 [0, 1] 转换到 [-1, 1] 范围内
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    // 获取当前深度值
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    
    // 检查是否在阴影中
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    // ... 你的光照计算

    // 计算阴影
    float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);

    // ... 应用到颜色
}
