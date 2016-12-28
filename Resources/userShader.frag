#version 450 core

// --------------------------------- DO NOT TOUCH

uniform vec2 iResolution;
uniform sampler2D iChannel0;
uniform float iGlobalTime;

// - Other
in vec4 out_color;
in vec4 out_position;

// --------------------------------- USER DEFINED SHADER
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = fragCoord.xy / iResolution.xy;
	fragColor = vec4(0.15, 0.15, 0.15,1.0);
}
// ---------------------------------
void main(){


   mainImage(gl_FragColor, gl_FragCoord.xy);

}