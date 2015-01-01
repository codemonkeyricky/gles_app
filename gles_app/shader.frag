
precision lowp float;

uniform sampler2D tex;

varying vec2 texCoords; 
varying vec4 colors;

void main() 
{
	gl_FragColor = texture2D(tex, texCoords) * colors; 
}
