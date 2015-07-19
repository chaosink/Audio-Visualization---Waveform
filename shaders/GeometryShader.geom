#version 330

layout(points) in;
layout(line_strip, max_vertices = 511) out;

uniform mat4 MVP;

in vec3 fragmentColor[];
out vec3 Color;

float length = 0.05;

void main() {
	for(int i = 0; i < gl_in.length(); i++) {
		for(float j = -10; j < 10; j += 20 / 20) {
			gl_Position = MVP * vec4(j, gl_in[i].gl_Position.y * sin((gl_in[i].gl_Position.z + 10.0) / 20 * 22050 * (j + 10.0)), gl_in[i].gl_Position.zw);
			Color = fragmentColor[i];
			EmitVertex();
		}
		EndPrimitive();
	}
	EndPrimitive();
}
