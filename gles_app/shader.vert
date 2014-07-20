
// Uniforms.
uniform mediump mat4 mvp;

// Attributes.
attribute mediump vec2 mPosition;
attribute mediump vec2 mTexCoords;
attribute mediump vec3 av3colour;

// Varyings.
varying mediump vec3 vv3colour;
varying mediump vec2 texCoords; 

void main() 
{
    // Set vertex output. 
	gl_Position = mvp * vec4(mPosition, 0.0, 1.0);

    // Set varyings. 
	vv3colour 	= av3colour;
    texCoords   = mTexCoords; // vec2(mTexCoords.x, mTexCoords.y); 
}

