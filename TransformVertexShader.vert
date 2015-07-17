#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in float x;

// Output data ; will be interpolated for each fragment.
out vec3 fragmentColor;
// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform int object;

void main(){	

	// Output position of the vertex, in clip space : MVP * position
	if(object == 0)
		gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);
	else if(object == 1)
		gl_Position = MVP * vec4(-3.0, vertexPosition_modelspace.x / 32768 * 2, x * 8, 1);
	else
		gl_Position = MVP * vec4( 3.0, vertexPosition_modelspace.x / 32768 * 2, x * 8, 1);

	// The color of each vertex will be interpolated
	// to produce the color of each fragment
	if(object == 0) fragmentColor = vertexColor;
	else if(object == 1) fragmentColor = vec3(1.0, 0.0, 0.0);
	else fragmentColor = vec3(0.0, 0.0, 1.0);
}

