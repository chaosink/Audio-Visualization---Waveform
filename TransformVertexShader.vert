#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in float z;

out vec3 fragmentColor;

uniform mat4 MVP;
uniform int object;

void main() {
	if(object != 1 && object != 2)
		gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);
	else if(object == 1)
		gl_Position = MVP * vec4(-5.0, vertexPosition_modelspace.x / 32768 * 8, z, 1);
	else if(object == 2)
		gl_Position = MVP * vec4( 5.0, vertexPosition_modelspace.x / 32768 * 8, z, 1);

	if(object == 0) fragmentColor = vertexColor;
	else if(object == 1) fragmentColor = vec3(1.0, 0.5, 0.5);
	else if(object == 2) fragmentColor = vec3(0.5, 0.5, 1.0);
	else if(object == 3) fragmentColor = vec3(1.0, 0.8, 0.8);
	else if(object == 4) fragmentColor = vec3(0.8, 0.8, 1.0);
}
