precision mediump float; 
      	 								
uniform vec4 u_Color;      	   								

varying float v_diffuse; 

void main()                    		
{                              	
    gl_FragColor = u_Color * v_diffuse;
}