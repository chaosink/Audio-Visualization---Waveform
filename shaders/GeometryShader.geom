#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 256) out;

uniform mat4 MVP;

in vec3 fragmentColor[];
out vec3 Color;

float length = 0.05;
float depth = 0.1;

void main() {
	for(int i = 0; i < gl_in.length(); i++) {
		gl_Position = MVP * (gl_in[i].gl_Position + vec4(-length, 0.0, 0.0, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * (gl_in[i].gl_Position + vec4( length, 0.0, 0.0, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * vec4(gl_in[i].gl_Position.x - length, 0, gl_in[i].gl_Position.zw);
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * vec4(gl_in[i].gl_Position.x + length, 0, gl_in[i].gl_Position.zw);
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * vec4(gl_in[i].gl_Position.x - length, 0, gl_in[i].gl_Position.z + depth, gl_in[i].gl_Position.w);
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * vec4(gl_in[i].gl_Position.x + length, 0, gl_in[i].gl_Position.z + depth, gl_in[i].gl_Position.w);
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * vec4(gl_in[i].gl_Position.x - length, gl_in[i].gl_Position.y, gl_in[i].gl_Position.z + depth, gl_in[i].gl_Position.w);
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * vec4(gl_in[i].gl_Position.x + length, gl_in[i].gl_Position.y, gl_in[i].gl_Position.z + depth, gl_in[i].gl_Position.w);
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * (gl_in[i].gl_Position + vec4(-length, 0.0, 0.0, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * (gl_in[i].gl_Position + vec4( length, 0.0, 0.0, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		EndPrimitive();

		gl_Position = MVP * (gl_in[i].gl_Position + vec4(-length, 0.0, 0.0, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * vec4(gl_in[i].gl_Position.x - length, 0, gl_in[i].gl_Position.zw);
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * vec4(gl_in[i].gl_Position.x - length, gl_in[i].gl_Position.y, gl_in[i].gl_Position.z + depth, gl_in[i].gl_Position.w);
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * vec4(gl_in[i].gl_Position.x - length, 0, gl_in[i].gl_Position.z + depth, gl_in[i].gl_Position.w);
		Color = fragmentColor[i];
		EmitVertex();
		EndPrimitive();

		gl_Position = MVP * (gl_in[i].gl_Position + vec4( length, 0.0, 0.0, 0.0));
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * vec4(gl_in[i].gl_Position.x + length, 0, gl_in[i].gl_Position.zw);
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * vec4(gl_in[i].gl_Position.x + length, gl_in[i].gl_Position.y, gl_in[i].gl_Position.z + depth, gl_in[i].gl_Position.w);
		Color = fragmentColor[i];
		EmitVertex();
		gl_Position = MVP * vec4(gl_in[i].gl_Position.x + length, 0, gl_in[i].gl_Position.z + depth, gl_in[i].gl_Position.w);
		Color = fragmentColor[i];
		EmitVertex();
		EndPrimitive();
	}
	EndPrimitive();
}
