# ShaderCloud
ShaderCloud is a minimal audio visualizer inspired by shadertoy. <br/>
The visualizer can be programmed using the OpenGL shading language. <br/>
> [![VIDEO](http://img.youtube.com/vi/gT2zvqcUlKk/0.jpg)](https://youtu.be/gT2zvqcUlKk)  <br/>
> [Video Demo](https://youtu.be/gT2zvqcUlKk)

## Usage

Create a file with .frag extension.  <br/>
Copy the template below and write your shader in `mainImage()` <br/>

```
#version 450 core

// --------------------------------- DO NOT TOUCH

uniform vec2 iResolution;
uniform sampler2D iChannel0;
uniform float iGlobalTime;

// --------------------------------- USER DEFINED SHADER

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = fragCoord.xy / iResolution.xy;
	fragColor = vec4(uv,0.5+0.5*sin(iGlobalTime),1.0);
}
// ---------------------------------

void main(){
     mainImage(gl_FragColor, gl_FragCoord.xy);

}
```

Open the file in ShaderCloud with your favorite song and watch it whip nae nae!
