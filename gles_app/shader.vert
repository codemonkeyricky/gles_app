/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 - 2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

attribute vec2 mPosition;
attribute vec2 mTexCoords;
attribute vec3 av3colour;

uniform mat4 mvp;

varying vec3 vv3colour;
varying vec2 texCoords; 

void main() 
{
    // Set vertex output. 
	gl_Position = mvp * vec4(mPosition, 0.0, 1.0);

    // Set varyings. 
	vv3colour 	= av3colour;
    texCoords   = vec2(mTexCoords.x, mTexCoords.y); 
}

