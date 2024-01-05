#version 330 core

uniform vec3 vertices[500];  // 100�����ӵ�λ�úʹ�С
uniform vec4 colors[500];    // 100�����ӵ���ɫ
uniform float sizes[500];         // ʱ��
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec4 outColor;

void main() {
  
    vec4 p =projection * view *model* vec4(vertices[gl_InstanceID], 1.0);
    gl_Position = p;
    gl_PointSize = sizes[gl_InstanceID];
    outColor = colors[gl_InstanceID];
}
