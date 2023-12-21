#version 330 core
layout (location = 0) in vec3 aPos;     // λ������
layout (location = 1) in vec2 aTexCoord; // ��������

out vec2 TexCoord; // ���ݸ�Ƭ����ɫ������������

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord; // ���������괫�ݸ�Ƭ����ɫ��
}
