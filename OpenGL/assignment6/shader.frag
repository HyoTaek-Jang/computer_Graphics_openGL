//201721070 장효택 컴퓨터그래픽스 과제 6. Shadow Mapping


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

const vec2 poissonDisk64[64] = vec2[](

vec2(-0.613392,  0.617481),vec2( 0.170019, -0.040254),vec2(-0.299417,  0.791925),vec2( 0.645680,  0.493210),

vec2(-0.651784,  0.717887),vec2( 0.421003,  0.027070),vec2(-0.817194, -0.271096),vec2(-0.705374, -0.668203),

vec2( 0.977050, -0.108615),vec2( 0.063326,  0.142369),vec2( 0.203528,  0.214331),vec2(-0.667531,  0.326090),

vec2(-0.098422, -0.295755),vec2(-0.885922,  0.215369),vec2( 0.566637,  0.605213),vec2( 0.039766, -0.396100),

vec2( 0.751946,  0.453352),vec2( 0.078707, -0.715323),vec2(-0.075838, -0.529344),vec2( 0.724479, -0.580798),

vec2( 0.222999, -0.215125),vec2(-0.467574, -0.405438),vec2(-0.248268, -0.814753),vec2( 0.354411, -0.887570),

vec2( 0.175817,  0.382366),vec2( 0.487472, -0.063082),vec2(-0.084078,  0.898312),vec2( 0.488876, -0.783441),

vec2( 0.470016,  0.217933),vec2(-0.696890, -0.549791),vec2(-0.149693,  0.605762),vec2( 0.034211,  0.979980),

vec2( 0.503098, -0.308878),vec2(-0.016205, -0.872921),vec2( 0.385784, -0.393902),vec2(-0.146886, -0.859249),

vec2( 0.643361,  0.164098),vec2( 0.634388, -0.049471),vec2(-0.688894,  0.007843),vec2( 0.464034, -0.188818),

vec2(-0.440840,  0.137486),vec2( 0.364483,  0.511704),vec2( 0.034028,  0.325968),vec2( 0.099094, -0.308023),

vec2( 0.693960, -0.366253),vec2( 0.678884, -0.204688),vec2( 0.001801,  0.780328),vec2( 0.145177, -0.898984),

vec2( 0.062655, -0.611866),vec2( 0.315226, -0.604297),vec2(-0.780145,  0.486251),vec2(-0.371868,  0.882138),

vec2( 0.200476,  0.494430),vec2(-0.494552, -0.711051),vec2( 0.612476,  0.705252),vec2(-0.578845, -0.768792),

vec2(-0.772454, -0.090976),vec2( 0.504440,  0.372295),vec2( 0.155736,  0.065157),vec2( 0.391522,  0.849605),

vec2(-0.620106, -0.328104),vec2( 0.789239, -0.419965),vec2(-0.545396,  0.538133),vec2(-0.178564, -0.596057));



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

    float visibility = 1.;
    vec3 sCoord = shadowCoord.xyz/shadowCoord.w;

    float bias = 0.005;

    for (int i=0;i<64;i++){
    if ( texture( shadowMap, sCoord.xy + poissonDisk64[i]/600.0 ).r < sCoord.z-bias )
        visibility-=1./64.;
        }

    vec3 lColor = lightColor*visibility;

    lighting += diffuseTexMaterial.rgb * diffuseFactor * lColor;
    lighting += specularMaterial.rgb * specularColor*lColor;
    lighting += diffuseTexMaterial.rgb * ambientLightColor;

    out_Color = vec4(lighting,diffusecolor.a);


}
