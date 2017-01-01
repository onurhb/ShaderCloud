#version 450 core

// --------------------------------- DO NOT TOUCH

uniform vec2 iResolution;
uniform sampler2D iChannel0;
uniform float iGlobalTime;

// - Other
in vec4 out_color;
in vec4 out_position;

// --------------------------------- USER DEFINED SHADER

#ifdef GL_ES
precision highp float;
#endif

#define PI  3.14159265359
#define EPS .01

#define CIRCLE_RADIUS   0.25
#define CIRCLE_GLOW     0.75
#define SAMPLE_SCALE    0.25

float hash(vec2 p)
{
    return fract(sin(dot(p,vec2(127.1,311.7))) * 43758.5453123);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2  uv = fragCoord.xy / iResolution.xy * 2. - 1.;

    float x = atan(-uv.x, -uv.y) / PI;
    x = (x < 0. ? 1. + x : 1. - x);
    vec3  s = texture2D(iChannel0, vec2(x, .25)).xyz * SAMPLE_SCALE;
    vec2  cv = uv * vec2(iResolution.x / iResolution.y, 1.);
    float ds = length(cv);

    float sr = (ds - s.x) / CIRCLE_RADIUS;
    float cl = (1. - sqrt(abs(1. - sr))) / sr + CIRCLE_GLOW;

    vec3 col = cl * vec3(abs(cos(iGlobalTime)), .5 + uv.x * uv.y, ds - uv.y);
    float grain = hash(hash(uv) * cv * iGlobalTime) * .05;
    float fade  = smoothstep(EPS, 2., iGlobalTime);

    fragColor = vec4((col + grain) * fade, 1);
}
// ---------------------------------

void main(){

     mainImage(gl_FragColor, gl_FragCoord.xy);

}