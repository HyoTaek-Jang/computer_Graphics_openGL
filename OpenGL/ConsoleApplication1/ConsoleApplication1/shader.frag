//201721070 장효택 컴퓨터그래픽스 과제 5. Texture Mapping + Bump Mapping


#version 410 core
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 ambientLightColor;
uniform vec4 diffusecolor;
uniform vec3 lightPos;
uniform float shineness;
uniform vec4 specularMaterial;

uniform sampler2D diffTex;
uniform sampler2D bumpTex;
uniform sampler2D shadowMap;


in vec3 normal;
in vec3 worldPos_frag;
in vec2 texCoord;
in vec4 shadowCoord;

out vec4 out_Color;

mat3 getTBN( vec3 N ) {
    vec3 Q1 = dFdx(worldPos_frag), Q2 = dFdy(worldPos_frag);
    vec2 st1 = dFdx(texCoord), st2 = dFdy(texCoord);
    float D = st1.s*st2.t-st2.s*st1.t;
    return mat3(normalize(( Q1*st2.t - Q2*st1.t )*D),
                        normalize((-Q1*st2.s + Q2*st1.s )*D), N);
}

#define TEX_DELTA 0.0001

void main(void) {


    vec3 L = normalize(lightPos-worldPos_frag);
    vec3 N = normalize(normal);

    mat3 TBN = getTBN( N );
    float Bu = texture( bumpTex, texCoord+vec2(TEX_DELTA,0) ).r
             - texture( bumpTex, texCoord-vec2(TEX_DELTA,0) ).r;
    float Bv = texture( bumpTex, texCoord+vec2(0,TEX_DELTA) ).r
             - texture( bumpTex, texCoord-vec2(0,TEX_DELTA) ).r;
    vec3 bumpVec = vec3(-Bu*15., -Bv*15., 1 );
    N = normalize( TBN* bumpVec );

    vec3 V = normalize(cameraPos - worldPos_frag);
    vec3 R = N*dot(N,L)*2.-L;

    vec4 diffuseTexMaterial = texture(diffTex, texCoord);

    float diffuseFactor = clamp( dot(N,L), 0, 1);
    float specularColor = pow(clamp(dot(R,V),0.,1.),shineness); // 금속이 아니면 조명 색.

    vec3 lighting = vec3(0);

    float visibility = 1.0;
    vec3 sCoord = shadowCoord.xyz/shadowCoord.w;
    if(texture(shadowMap, shadowCoord.xy).r <sCoord.z) visibility = 0.;

    vec3 lColor = lightColor*visibility;
    lighting += diffuseTexMaterial.rgb * diffuseFactor * lColor;
    lighting += specularMaterial.rgb * specularColor*lColor;
    lighting += diffuseTexMaterial.rgb * ambientLightColor;

    out_Color = vec4(lighting,diffusecolor.a);

}
