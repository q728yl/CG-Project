#version 330 core
layout (location = 0) in vec3 aPos;     // 位置坐标
layout (location = 1) in vec2 aTexCoord; // 纹理坐标

out vec2 TexCoord; // 传递给片段着色器的纹理坐标
out vec3 ourColor; // 传递给片段着色器的颜色
flat out int textureId; // 传递给片段着色器的纹理id
flat out int shouldDraw1; // 传递给片段着色器的是否绘制id
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
    TexCoord = aTexCoord; // 将纹理坐标传递给片段着色器

}
