#version 330 core

in vec4 outColor;  // �Ӷ�����ɫ��������ɫ
out vec4 FragColor;

void main()
{
    // ʹ������������ģ�����Ч���������Ϊ����y�������ɫ�仯
    //vec3 flameColor = mix(vec3(1.0, 0.5, 0.0), vec3(1.0, 0.0, 0.0), TexCoord.y);
    
    // �����ɫ�仯��ģ�����
    //vec3 finalColor = ourColor.rgb * flameColor;
    vec2 center = vec2(0.5, 0.5);
    vec2 toCenter = gl_PointCoord - center;
    float distanceToCenter = length(toCenter);
    
    if (distanceToCenter > 0.5) {
        // �����뾶��Χ�Ĳ��ֲ�����
        discard;
    }
    FragColor = vec4(outColor);
}