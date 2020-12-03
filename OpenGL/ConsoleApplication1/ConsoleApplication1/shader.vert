//201721070 장효택 컴퓨터그래픽스 과제 2. Rotating Bunny

#version 410 core

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;

uniform mat4 viewMat = mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,-3,1);
uniform mat4 projMat = mat4(1.299038, 0, 0, 0, 0, 1.732051, 0, 0, 0, 0, -1.002002, -1.0, 0, 0, -0.2002, 0);
uniform mat4 modelMat = mat4(1);

out vec3 worldPos_frag;
out vec3 normal;

void main(void) {
          vec4 worldPos = modelMat*vec4(in_Position, 1.0);
          worldPos_frag = worldPos.xyz;
          normal = normalize((modelMat*vec4(in_Normal,0)).xyz); //translation 무시하기 위해 0
          gl_Position= projMat*viewMat*worldPos;
}
