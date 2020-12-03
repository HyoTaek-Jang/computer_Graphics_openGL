//201721070 장효택 컴퓨터그래픽스 과제 2. Rotating Bunny

#version 410 core
uniform vec3 cameraPos;
uniform vec3 lightColor = vec3(1,1,1);
uniform vec3 ambientLightColor = vec3(0.1,0.09,0.03);
uniform vec3 color = vec3(0.45,1,0.7);
uniform vec3 lightPos = vec3(10,10,10); //광원의 위치
uniform float shineness = 100.;

in vec3 normal;
in vec3 worldPos_frag;

out vec4 out_Color;
void main(void) {

    // 위에꺼 유니폼으로 뺴서 cpp에서 입력.

    vec3 L = normalize(lightPos-worldPos_frag);
    vec3 N = normalize(normal);
    vec3 V = normalize(cameraPos - worldPos_frag);
    vec3 R = N*dot(N,L)*2.-L;

    vec3 specularColor = pow(clamp(dot(R,V),0.,1.),shineness)*lightColor; // 금속이 아니면 조명 색임.
    vec3 lighting = ambientLightColor*color + color*max(0.,dot(N,L))*lightColor + specularColor;
    out_Color = vec4(lighting,1);

}
