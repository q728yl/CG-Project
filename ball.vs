#version 330 core
layout (location = 0) in vec3 aPos;     // λ������
layout (location = 1) in vec2 aTexCoord; // ��������

out vec2 TexCoord; // ���ݸ�Ƭ����ɫ������������
out vec3 ourColor; // ���ݸ�Ƭ����ɫ������ɫ
flat out int textureId; // ���ݸ�Ƭ����ɫ��������id
flat out int shouldDraw1; // ���ݸ�Ƭ����ɫ�����Ƿ����id
uniform mat4 projection;
uniform mat4 view;

uniform mat4 modelMatrixs[30];
uniform vec3 colors[30];
uniform int textureIds[30];
uniform int shouldDraw[30];

void main()
{
	
    mat4 model = modelMatrixs[gl_InstanceID];
    ourColor = colors[gl_InstanceID];
    textureId = textureIds[gl_InstanceID];
    shouldDraw1 = shouldDraw[gl_InstanceID];
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord; // ���������괫�ݸ�Ƭ����ɫ��

}
