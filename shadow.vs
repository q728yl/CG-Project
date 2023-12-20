#version 330 core
layout (location = 0) in vec3 aPos;
// ... ��������

out vec4 FragPosLightSpace;  // �Ӷ�����ɫ�����ݵ�Ƭ����ɫ���Ĺ�ռ�λ��

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
    FragPosLightSpace = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
