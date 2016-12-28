#version 330 core
uniform vec2 iResolution;
uniform sampler2D iChannel0;
uniform float iGlobalTime;


void main(void) {
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    float intensity = texture2D(iChannel0, vec2(uv.x, 0.0)).r; // Get intesity of this 'column'
    float i = float(intensity > uv.y); // Is this pixel lower (screenwise) than intensity?
    gl_FragColor = vec4(i);
}