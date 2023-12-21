#version 330 core
out vec4 FragColor; // �����ɫ��֡����

in vec2 TexCoord;   // �������������

uniform vec3 ourColor; // С�����ɫ

// �������Ѿ�����һ������Sampler2D uniform����
uniform sampler2D ourTexture; // ���������

void main()
{
    // �������в�����ɫ
    vec4 texColor = texture(ourTexture, TexCoord);
    
    // ʹ��������ɫ�����ǵ���ɫ���
    FragColor = vec4(ourColor, 1.0) * texColor;
}
