//201721070 장효택 컴퓨터그래픽스 과제 5. Texture Mapping + Bump Mapping


#version 410 core

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_TexCoord;


uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 modelMat = mat4(1);
uniform mat4 shadowBiasMVP;

out vec3 worldPos_frag;
out vec3 normal;
out vec2 texCoord;
out vec4 shadowCoord;

void main(void) {
          vec4 worldPos = modelMat*vec4(in_Position, 1.0);
          worldPos_frag = worldPos.xyz;
          normal = normalize((modelMat*vec4(in_Normal,0)).xyz); //translation 무시하기 위해 0
          texCoord = vec2(in_TexCoord.x, 1-in_TexCoord.y);
          shadowCoord = shadowBiasMVP*vec4(in_Position,1.0);
          gl_Position= projMat*viewMat*worldPos;
}