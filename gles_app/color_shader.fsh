precision mediump float; 
      	 								
uniform vec4 u_Color;      	   								

varying vec3 v_diffuse; 

void main()                    		
{                              	
    gl_FragColor = vec4(v_diffuse, 1.0);
}