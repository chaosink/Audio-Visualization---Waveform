#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 511) out;

uniform mat4 MVP;

in vec3 fragmentColor[];
out vec3 Color;

float length = 0.05;

float pi = 3.141592653589793238462643383279502884197169399;

mat4 rotate = mat4(
	 1.0, 0.0, 0.0, 0.0,
	 0.0, 1.0, 0.0, 0.0,
	 0.0, 0.0, 1.0, 0.0,
	 0.0, 0.0, 0.0, 1.0);

void main() {
for(int i = 0; i < gl_in.length(); i++) {
	for(float j = 0; j < 2 * pi; j += pi / 12) {
		rotate = mat4(
			 cos(j), -sin(j), 0.0, 0.0,
			 sin(j), cos(j), 0.0, 0.0,
			 0.0, 0.0, 1.0, 0.0,
			 0.0, 0.0, 0.0, 1.0);
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4(-length,-length,-length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4(-length,-length, length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4(-length, length,-length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4(-length, length, length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4( length, length,-length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4( length, length, length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4( length,-length,-length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4( length,-length, length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4(-length,-length,-length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4(-length,-length, length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		EndPrimitive();

		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4(-length,-length,-length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4(-length, length,-length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4( length,-length,-length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4( length, length,-length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		EndPrimitive();

		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4(-length,-length, length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4(-length, length, length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4( length, length, length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * rotate * (gl_in[i].gl_Position + vec4( length,-length, length, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		EndPrimitive();
	}
}
}
