//201721070 장효택 컴퓨터그래픽스 과제 6. Shadow Mapping

#version 330 core
layout(location = 0) in vec3 in_Position;
uniform mat4 shadowMVP;
void main(){
    gl_Position = shadowMVP * vec4(in_Position,1);
}
