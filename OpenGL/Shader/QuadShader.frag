#version 400 core

in vec2 qCoords[8];
out vec4 outColor;

uniform sampler2D lookUpTexture;
uniform float PMsz;
uniform float tanPhi;

void main()
{ 
	vec2 qScreenCoords[8];
	float m, z, c;
	vec3 cMax = vec3(-1, -1, -1);
	for (int i=0; i<8; ++i) {
		qScreenCoords[i] = texture(lookUpTexture, qCoords[0]).rg;
		z = texture(lookUpTexture, gl_FragCoord.xy).b;
		m = -(PMsz + z)/tanPhi;
		c = z - m*sqrt(pow(gl_FragCoord.x-qScreenCoords[i].x,2)+pow(gl_FragCoord.y-qScreenCoords[i].y,2));
		if (c>cMax.z) {
			cMax.z = c;
			cMax.xy = qScreenCoords[i];
		}
	}
	vec3 outColor = cMax;
    
}