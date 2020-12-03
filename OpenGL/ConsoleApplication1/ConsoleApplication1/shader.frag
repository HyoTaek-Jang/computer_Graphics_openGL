//201721070 장효택 컴퓨터그래픽스 과제 3. Lighting and Shadowing

#version 410 core
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 ambientLightColor;
uniform vec4 diffusecolor;
uniform vec3 lightPos;
uniform float shineness;
uniform vec4 specularMaterial;


in vec3 normal;
in vec3 worldPos_frag;

out vec4 out_Color;
void main(void) {

    vec3 L = normalize(lightPos-worldPos_frag);
    vec3 N = normalize(normal);
    vec3 V = normalize(cameraPos - worldPos_frag);
    vec3 R = N*dot(N,L)*2.-L;

    float diffuseFactor = clamp( dot(N,L), 0, 1);
    float specularColor = pow(clamp(dot(R,V),0.,1.),shineness); // 금속이 아니면 조명 색.

    vec3 lighting = vec3(0);

    lighting += diffusecolor.rgb * diffuseFactor * lightColor;
    lighting += specularMaterial.rgb * specularColor*lightColor;
    lighting += diffusecolor.rgb * ambientLightColor;

    out_Color = vec4(lighting,diffusecolor.a);

}
