uniform mat4 u_MvpMatrix;
uniform vec3 u_VectorToLight; 

attribute vec4 a_Position;  
attribute vec3 a_Normal;  

varying float v_diffuse; 

void main()                    
{                                	  	  
    v_diffuse = max(dot(a_Normal, u_VectorToLight), 0.0); 
	
    gl_Position = u_MvpMatrix * a_Position;
}          