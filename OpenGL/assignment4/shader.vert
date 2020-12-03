//201721070 ��ȿ�� ��ǻ�ͱ׷��Ƚ� ���� 3. Lighting and Shadowing

#version 410 core

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;

uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 modelMat = mat4(1);

out vec3 worldPos_frag;
out vec3 normal;

void main(void) {
          vec4 worldPos = modelMat*vec4(in_Position, 1.0);
          worldPos_frag = worldPos.xyz;
          normal = normalize((modelMat*vec4(in_Normal,0)).xyz); //translation �����ϱ� ���� 0
          gl_Position= projMat*viewMat*worldPos;
}
