#version 330 core
// ... ���������uniforms

in vec4 FragPosLightSpace;  
uniform sampler2D depthMap;
uniform vec3 lightPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // ������� [0, 1] ת���� [-1, 1] ��Χ��
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    // ��ȡ��ǰ���ֵ
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    
    // ����Ƿ�����Ӱ��
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    // ... ��Ĺ��ռ���

    // ������Ӱ
    float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);

    // ... Ӧ�õ���ɫ
}
