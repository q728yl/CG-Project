#version 330 core
in vec2 TexCoord;  // ������������
out vec4 FragColor;
uniform bool hasTexture; // �Ƿ�������
uniform vec3 wallColor; // �Ӷ�����ɫ�����ݹ�������ɫ
uniform sampler2D textureSampler;  // ���������

void main()
{
    if (hasTexture) {
        FragColor = texture(textureSampler, TexCoord) * vec4(wallColor, 1.0);
    } else {
        FragColor = vec4(wallColor, 1.0);
    }
}