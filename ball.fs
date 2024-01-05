#version 330 core
out vec4 FragColor; // �����ɫ��֡����

in vec2 TexCoord;   // �������������
in vec3 ourColor; // �������ɫ
flat in int textureId; // ���������id
flat in int shouldDraw1;

// �����������������
uniform sampler2D ballTextureSampler;
uniform sampler2D floorTextureSampler;
uniform sampler2D ashTextureSampler;

void main()
{
    if (shouldDraw1 == 0) discard;

    vec3 color = ourColor;
    vec4 texColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
     if (textureId == 1) {
        texColor = texture(ballTextureSampler, TexCoord);
    } else if(textureId == 2){
        texColor = texture(floorTextureSampler, TexCoord);
    }
    else if(textureId == 3){
        texColor = texture(ashTextureSampler, TexCoord);
        //��ɫ
        color = vec3(0.5,0.5,0.5);
    }
    // ʹ��������ɫ�����ǵ���ɫ���
    FragColor = vec4(color, 1.0) * texColor;
}
