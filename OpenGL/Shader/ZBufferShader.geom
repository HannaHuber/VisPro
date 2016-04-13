// GEOMETRY SHADER calculate world space positions (view from light) for each cube map face
#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 proj;
uniform mat4 view[6];

out vec4 world_pos;

void main() {
	for (int f=0; f<6; ++f) {
		gl_Layer = f;
		for (int v=0; v<3; ++v) {
			world_pos = gl_in[v].gl_Position;
			gl_Position = proj * view[f] * world_pos;
			EmitVertex();
		}
		EndPrimitive();
	}
}