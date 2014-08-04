
// Uniforms.
uniform mediump mat4 mvp;

// Attributes.
attribute mediump vec2 mPosition;
attribute mediump vec2 mTexCoords;
attribute mediump vec4 mColor;

// Varyings.
varying mediump vec2 texCoords; 
varying mediump vec4 colors;

void main() 
{
    // Set vertex output. 
	gl_Position = mvp * vec4(mPosition, 0.0, 1.0);

    // Set varyings. 
	colors    = mColor;
    texCoords = mTexCoords; 
}
