#version 330

//layout(lines) in;
//layout(line_strip, max_vertices = 2) out;
layout(triangles) in;
layout(triangle_strip, max_vertices = 300) out;

uniform mat4 MVP;

in vec3 fragmentColor[];
out vec3 Color;

float size = 4.0;
int num = 5;

void main() {
	for(int k = -5; k < num; k++)
		for(int j = -5; j < num; j++) {
			for(int i = 0; i < gl_in.length(); ++i) {
				gl_Position = gl_in[i].gl_Position + vec4( size * j + size / 2, size * k + size / 2, 0.0, 0.0);
				Color = fragmentColor[i];
				EmitVertex();
			}
			EndPrimitive();
		}
}
