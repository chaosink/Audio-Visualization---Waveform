#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in float z;

out vec3 fragmentColor;

uniform mat4 MVP;
uniform int object;

int color_factor = 8;
float size_factor = 0.01;
float size_factor1 = 0.1;

bool zhuangbi = true;
#define ZHUANGBI

void main() {
	mat4 scale = mat4(
		 size_factor, 0.0, 0.0, 0.0,
		 0.0, 1.0, 0.0, 0.0,
		 0.0, 0.0, size_factor, 0.0,
		 0.0, 0.0, 0.0, 1.0);
	mat4 scale1 = mat4(
		 size_factor1, 0.0, 0.0, 0.0,
		 0.0, 1.0, 0.0, 0.0,
		 0.0, 0.0, size_factor1, 0.0,
		 0.0, 0.0, 0.0, 1.0);
	switch(object) {
		case 1:
			gl_Position = MVP * vec4(-5.0, vertexPosition_modelspace.x / 32768 * 8, z, 1);
			break;
		case 2:
			gl_Position = MVP * vec4( 5.0, vertexPosition_modelspace.x / 32768 * 8, z, 1);
			break;
		#ifdef ZHUANGBI
		case 0:
			gl_Position = MVP * scale * vec4(vertexPosition_modelspace, 1);
			break;
		case 5:
			gl_Position = MVP * scale * vec4(vertexPosition_modelspace, 1);
			break;
		#endif
		default:
			gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
			break;
	}

	switch(object) {
		case 0:
			fragmentColor = zhuangbi ? vec3(1.0, gl_Position.y / color_factor, gl_Position.y / color_factor) : vertexColor;
			break;
		case 5:
			fragmentColor = zhuangbi ? vec3(gl_Position.y / color_factor, gl_Position.y / color_factor, 1.0) : vertexColor;
			break;
		case 1:
			fragmentColor = zhuangbi ? vec3(vertexPosition_modelspace.x / 32768, z / 5.0, 0.5) : fragmentColor = vec3(1.0, 0.5, 0.5);;
			break;
		case 2:
			fragmentColor = zhuangbi ? vec3(0.5, z / 5.0, vertexPosition_modelspace.x / 32768) : fragmentColor = vec3(0.5, 0.5, 1.0);;
			break;
		case 3:
			fragmentColor = vec3(1.0, gl_Position.y / color_factor, gl_Position.y / color_factor);
			break;
		case 4:
			fragmentColor = vec3(gl_Position.y / color_factor, gl_Position.y / color_factor, 1.0);
			break;
	}
}
