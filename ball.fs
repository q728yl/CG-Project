#version 330 core
out vec4 FragColor; // �����ɫ��֡����

in vec2 TexCoord;   // �������������
in vec3 ourColor; // �������ɫ
flat in int textureId; // ���������id

// �����������������
uniform sampler2D ballTextureSampler;
uniform sampler2D floorTextureSampler;

void main()
{
    vec4 texColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
     if (textureId == 0) {
        texColor = texture(ballTextureSampler, TexCoord);
    } else if(textureId == 1){
        texColor = texture(floorTextureSampler, TexCoord);
    }
    // ʹ��������ɫ�����ǵ���ɫ���
    FragColor = vec4(ourColor, 1.0) * texColor;
}
