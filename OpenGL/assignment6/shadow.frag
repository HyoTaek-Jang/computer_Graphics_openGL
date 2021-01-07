//201721070 장효택 컴퓨터그래픽스 과제 6. Shadow Mapping

#version 330 core 
layout(location = 0) out vec3 fragmentdepth;
void main(){
	fragmentdepth = vec3(gl_FragCoord.z); 
	}
