
precision lowp float;

varying vec3 vv3colour;
varying vec2 texCoords; 

uniform sampler2D tex;

void main() 
{
	gl_FragColor = texture2D(tex, texCoords) + vec4(vv3colour, 1.0) * 0.01;
}
